#pragma once

#include "Vector.h"
#include "Matrix.h"
#include <armadillo>

using namespace std;
using namespace arma;

mat getPseudoInv(vector<Vector>& jVectors) {
    int n = jVectors.size();
    mat J(3, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) {
            J(j, i) = jVectors[i][j];
        }
    }

    mat U;
    vec d;
    mat V;

    svd(U, d, V, J);

    int num = d.n_elem;
    for (int i = 0; i < num; i++) {
        if (d[i] < 0.0001) {
            d[i] = 0;
        } else {
            d[i] = 1.0 / d[i];
        }
    }

    mat Dprime = join_vert(diagmat(d), zeros(n-3, 3));
    mat jPrime = V * Dprime * U.t();

    return jPrime;
}

void drawLink(Vector, Vector);

struct Link {
    const double d; //length
    Vector r; //rotation

    shared_ptr<Link> child;

    mat rotateMatrix();
    Matrix translateMatrix();

    void getVector(Matrix F = I);
    Vector getEndpoint(Matrix F = I);
    vector<Link*> getLinks();
    vector<Vector> calcJacobianVectors(double e);
    vec getCostDiff(Vector goal);
    vec getParams();
    void setParams(vec params);
    void updateParams(Vector goal, const double step=1.0);
};

// rotational matrix
inline mat Link::rotateMatrix() {
    mat I(3, 3);
    I.eye();

    double theta = r.mag();

    if (theta < 0.0001) {
        return I;
    }

    Vector rnorm = r.norm();
    vec r = {rnorm[0], rnorm[1], rnorm[2]};

    mat rcross(3, 3);
    rcross(0, 0) = 0; rcross(0, 1) = -r[2]; rcross(0, 2) =  r[1];
    rcross(1, 0) = r[2]; rcross(1, 1) = 0; rcross(1, 2) = -r[0];
    rcross(2, 0) = -r[1]; rcross(2, 1) = r[0]; rcross(2, 2) = 0;

    return rcross * sin(theta) + I + rcross * rcross * (1 - cos(theta));
}

// translational matrix
inline Matrix Link::translateMatrix() {
    mat R = rotateMatrix();
    vec p = {d, 0, 0};
    vec Rp = R * p;

    return {{
        {R(0, 0), R(0, 1), R(0, 2), Rp[0]},
        {R(1, 0), R(1, 1), R(1, 2), Rp[1]},
        {R(2, 0), R(2, 1), R(2, 2), Rp[2]},
        {0, 0, 0, 1}
    }};
}

// get the whole chain
inline void Link::getVector(Matrix F) {
    mat R = rotateMatrix();
    vec p = {d, 0, 0};
    vec Rp = R * p;
    Matrix T = translateMatrix();

    Vector origin = {0, 0, 0, 1};
    Vector endpoint = {Rp[0], Rp[1], Rp[2], 1};

    drawLink(F * origin, F * endpoint);
    if (this->child) {
        this->child->getVector(F * T);
    }
}

// get endpoint of chain
inline Vector Link::getEndpoint(Matrix F) {
    mat R = rotateMatrix();
    vec p = {d, 0, 0};
    vec Rp = R * p;
    Matrix T = translateMatrix();

    Vector endpoint = {Rp[0], Rp[1], Rp[2], 1};
    
    if (child) {
        return child->getEndpoint(F * T);
    } else {
        return F * endpoint;
    }
}

// get vector of links
inline vector<Link*> Link::getLinks() {
    vector<Link*> links;
    Link* head = this;
    while (head) {
        links.push_back(head);
        head = head->child.get();
    }
    return links;
}


inline vector<Vector> Link::calcJacobianVectors(double e) {
    vector<Link*> links = getLinks();
    vector<Vector> jVectors;
    for (Link* link : links) {
        for (int i = 0; i < 3; i++) {
            double orig = (*link).r[i];

            (*link).r[i] = orig + e;
            Vector vPlus = getEndpoint();
            (*link).r[i] = orig - e;
            Vector vMinus = getEndpoint();

            jVectors.push_back((vPlus - vMinus) / (2.0 * e));
            (*link).r[i] = orig;
        }
    }
    return jVectors;
}

inline vec Link::getCostDiff(Vector goal) {
    Vector endpoint = getEndpoint();
    Vector diff = endpoint - goal;
    vec cost = {diff[0], diff[1], diff[2]};
    return cost;
}

inline vec Link::getParams() {
    vector<Link*> links = getLinks();
    vec params(3 * links.size());
    for (int i = 0; i < links.size(); i++) {
        params[3*i] = links[i]->r[0];
        params[3*i + 1] = links[i]->r[1];
        params[3*i + 2] = links[i]->r[2];
    }
    return params;
}

inline void Link::setParams(vec params) {
    vector<Link*> links = getLinks();
    for (int i = 0; i < links.size(); i++) {
        links[i]->r[0] = params[3*i];
        links[i]->r[1] = params[3*i + 1];
        links[i]->r[2] = params[3*i + 2];
    }
}

inline void Link::updateParams(Vector goal, const double step) {
    vec currParams = getParams();
    vec currCost = getCostDiff(goal);
    double currError = norm(currCost, 2);

    if (currError < 0.001) {
        return;
    }

    vector<Vector> jVectors = calcJacobianVectors(0.001);
    mat pInv = getPseudoInv(jVectors);
    
    vec nextParams = currParams - (pInv * currCost) * step;
    setParams(nextParams);
    vec nextCost = getCostDiff(goal);
    double nextError = norm(nextCost, 2);

    setParams(currParams);

    if (nextError > currError) {
        if (step > 0.01) {
            updateParams(goal, step / 2.0);
        }
    } else {
        setParams(nextParams); 
        updateParams(goal);
    }
}