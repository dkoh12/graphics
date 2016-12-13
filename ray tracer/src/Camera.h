#ifndef CAMERA_H
#define CAMERA_H

struct Camera {
	// vectors: 
	// 	- eye: vector direction
	// 	- ll: lower left
	// 	- lr: lower right
	// 	- ul: upper left
	// 	- ur: upper right
	Vector3 eye, ll, lr, ul, ur;

	Camera() {}

	Camera(Vector3 &eye_, Vector3 &ll_, Vector3 &lr_, Vector3 &ul_, Vector3 &ur_): 
		eye(eye_), ll(ll_), lr(lr_), ul(ul_), ur(ur_) {}
	
};

#endif
