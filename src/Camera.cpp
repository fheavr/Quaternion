/*
 * Implementation of a quaternion based camera to allow full rotation
 * without the threat of gimbal lock
 */
#include "Camera.h"
#include <cstdio>

Camera::Camera( double x, double y, double z )
	:pos(x, y, z), rot(1, 0, 0, 0), 
	 focus(0, 0, 0), focusOn(true)
{}

Camera::~Camera(void)
{}

void Camera::rotate(float pitch, float roll, float yaw)
{
	rot = rot * Quaternion(pitch, roll, yaw);
}

void Camera::orbit(const Vector &centroid, const Vector &axisOfRotation, const Quaternion &q)
{
	printf("Camera.cpp: FIXME: (%p)->(%p, %p, %p)stub!\n", centroid, axisOfRotation, q);
}

void Camera::orbit(const Vector &centroid, const Vector &p1, const Vector &p2)
{}

/* Focuses the camera to always be looking at the given point
 * regardless of position */
void Camera::lookAt(const Vector &p)
{
	focus = Vector(p);
	focusOn = true;
}

void Camera::disableFocus( const Quaternion &q )
{
	rot = Quaternion(q);
	focusOn = false;
}

/* relocates the camera at the given point */
void Camera::move(const Vector &newLocation)
{
	pos = Vector(newLocation);
}

/* Moves along the given vector by the given amount from the current position */
void Camera::move(const Vector &dir, const float distance)
{
	Vector v(dir);
	v.normalize();
	v = v * distance;
	pos = pos + v;
}