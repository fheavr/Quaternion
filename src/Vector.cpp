#include "Vector.h"
#include <cmath>

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector( const Vector &v )
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

void Vector::normalize()
{
	float mag = x * x + y * y + z * z;
	if( mag != 1 )
	{
	    mag = sqrt( mag );
	    x /= mag;
	    y /= mag;
	    z /= mag;
	}
}

double Vector::dot( const Vector &v )
{
	return x*v.x + y*v.y + z*v.z;
}

Vector Vector::cross( const Vector &v )
{
	return Vector( y*v.z - v.y*z,
		           z*v.x - x*v.z,
				   x*v.y - v.x*y );
}

Vector Vector::operator +(const Vector &v) const
{
	return Vector( x + v.x, y + v.y, z + v.z );
}

Vector Vector::operator *(float f) const
{
	return Vector( f*x, f*y, f*z );
}

Vector::~Vector()
{}