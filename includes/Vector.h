#pragma once

class Vector
{
public:
	Vector(float x, float y, float z);
	Vector(const Vector&);
	~Vector();

	void normalize();
	double dot( const Vector &v );
	Vector cross( const Vector &v );
	Vector operator +(const Vector &v) const;
	Vector operator *(float f) const;

	float x;
	float y;
	float z;

private:
	float mag;
};