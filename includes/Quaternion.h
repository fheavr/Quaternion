#pragma once
#include "Vector.h"

class Quaternion
{
public:
	Quaternion(float w, float x, float y, float z);
	Quaternion(const Vector &vector, float angle);
	Quaternion(float pitch, float roll, float yaw);
	Quaternion(const float * const matrix, int dimension);
	Quaternion(const Quaternion& q);
	~Quaternion(void);

	Quaternion operator*( const Quaternion &q ) const;
	Vector operator*( const Vector &v ) const;
	void toMatrix( float * const matrix ) const;
	void toVectorAngle( float * const vector, float &angle ) const;
	void toEulerAngles( float &pitch, float &roll, float &yaw );

private:
	void normalize();
	void reverse();

	float w;
	float x;
	float y;
	float z;
};

