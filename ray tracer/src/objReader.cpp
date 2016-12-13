#include "Shape.h"
#include "Colors.h"
#include "Vectors.h"
#include "Camera.h"
#include "Light.h"

#include <vector>
#include <string>
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<Triangle> constructTriangle(vector<Vector3> vertexes, vector<Vector3> faces) {
	// initiate triangles
	vector<Triangle> tri;

	for (Vector3 f: faces) {
		Vector3 a = vertexes[f.x-1];
		Vector3 b = vertexes[f.y-1];
		Vector3 c = vertexes[f.z-1];
    	tri.push_back(Triangle(a, b, c));
	}

	return tri;
}

vector<Triangle> readObj (string filename){
	cout << "==== Reading Object File ====: " << filename << endl;
	string line; string delimiter = " ";
	vector<Vector3> vertexes; vector<Vector3> faces;

    ifstream infile;
    infile.open(filename, ios::in);

    if (infile.is_open()) {
        while (getline(infile, line)) {
        	// break line to tokens
        	vector<string> v; int pos = 0; string token;
            while ((pos = line.find(delimiter)) != string::npos) {
                token = line.substr(0, pos);
                v.push_back(token);
                line.erase(0, pos + delimiter.length());
            }
            v.push_back(line);

            // parse command
            if (v[0].compare("v") == 0) {
                Vector3 vectex(stof(v[1]), stof(v[2]), stof(v[3]));
				cout << "vectex: " << vectex << endl;
                vertexes.push_back(vectex);

            } else if (v[0].compare("f") == 0) {
                Vector3 face(stof(v[1]), stof(v[2]), stof(v[3]));
				cout << "face" << face << endl;
                faces.push_back(face);
            }
        }
        infile.close();
    } else {
        cout << "unable to open file";
    }
	cout << "==== Done Object File ====" << endl;
    return constructTriangle(vertexes, faces);
}

