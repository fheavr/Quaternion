/*
 * Camera class with orientation w + xi + yj + zk
 * and position
 */
#pragma once
#include "Quaternion.h"
#include "Vector.h"

class Camera {
public:
	Camera(double x, double y, double z);
	~Camera();
	void rotate(float pitch, float roll, float yaw);
	void orbit(const Vector &centroid, const Vector &axisOfRotation, const Quaternion &q);
	void orbit(const Vector &centroid, const Vector &p1, const Vector &p2);
	void lookAt(const Vector &p);
	void disableFocus(const Quaternion &q);
	void move(const Vector &newLocation);
	void move(const Vector &dir, const float distance);

private:
	Quaternion rot;
    Vector pos;
	Vector focus;
	bool focusOn;
};