/* Thanks to the following websites for their help:
 * for efficient forms of all of the algorithms:
 * http://www.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
 *
 * for understanding all of the concepts:
 * http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/geometric/axisAngle/index.htm
 * http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
 */

#include "Quaternion.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>

/* Build a quaternion from quaternion components */
Quaternion::Quaternion( const float w, const float x,
					    const float y, const float z )
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

/* Build a quaternion from a vector and an angle (in radians) */
Quaternion::Quaternion( const Vector &v, const float ang )
{
	float mag     = sqrt( v.x*v.x + v.y*v.y + v.z*v.z );
	float axis[3] = {v.x/mag, v.y/mag, v.z/mag};
	float sinAng  = sin( ang * (float)0.5 );

	w = cos(ang);
	x = axis[0]*sinAng;
	y = axis[1]*sinAng;
	z = axis[2]*sinAng;
}

/* Construct a quaternion from yaw pitch and roll in degrees */
Quaternion::Quaternion( float pitch, float roll, float yaw )
{
	/* Using the above formula for vecotrs and angles
	 * multiply the three quaternions generated from each angle and
	 * their respective axis and simplify */

	float p = pitch * M_PI_2 / 180.0;
	float y = yaw   * M_PI_2 / 180.0;
	float r = roll  * M_PI_2 / 180.0;
 
	float sinp = sin(p);
	float siny = sin(y);
	float sinr = sin(r);
	float cosp = cos(p);
	float cosy = cos(y);
	float cosr = cos(r);

	this->w = cosr * cosp * cosy + sinr * sinp * siny;
	this->x = sinr * cosp * cosy - cosr * sinp * siny;
	this->y = cosr * sinp * cosy + sinr * cosp * siny;
	this->z = cosr * cosp * siny - sinr * sinp * cosy;
 
	normalize();
}

/* Construct a quaternion from a rotation matrix of size
 * size (dimension x dimension) represented by rows 
 *
 * Note: while a quaternion represents only rotations, the dimension
 *       will allow the extraction of the rotation matrix from a 
 *       more complicated transformation matrix
 */
Quaternion::Quaternion( const float * const matrix, int dimension )
{
    #define MAX( A, B ) ((B) > (A)) ? (B) : (A)
	switch( dimension )
	{
	case 3:
	    this->w = sqrt( MAX( 0, 1 + matrix[0] + matrix[4] + matrix[8] ) ) / 2;
	    this->x = sqrt( MAX( 0, 1 + matrix[0] - matrix[4] - matrix[8] ) ) / 2;
	    this->y = sqrt( MAX( 0, 1 - matrix[0] + matrix[4] - matrix[8] ) ) / 2;
		this->z = sqrt( MAX( 0, 1 - matrix[0] - matrix[4] + matrix[8] ) ) / 2;

        this->x = _copysign( x, matrix[7] - matrix[5] ); // m21 - m12
        this->y = _copysign( y, matrix[2] - matrix[6] ); // m02 - m20
        this->z = _copysign( z, matrix[3] - matrix[1] ); // m10 - m01
		break;
	case 4: // TODO: This isnt right:
	/*    this->w = sqrt( MAX( 0, 1 + matrix[0] + matrix[5] + matrix[10] + matrix[15] ) ) / 2;
	    this->x = sqrt( MAX( 0, 1 + matrix[0] - matrix[5] - matrix[10] - matrix[15] ) ) / 2;
	    this->y = sqrt( MAX( 0, 1 - matrix[0] + matrix[5] - matrix[10] - matrix[15] ) ) / 2;
		this->z = sqrt( MAX( 0, 1 - matrix[0] - matrix[5] + matrix[10] - matrix[15] ) ) / 2;
		
        this->x = _copysign( x, matrix[7] - matrix[5] ); // m21 - m12
        this->y = _copysign( y, matrix[2] - matrix[6] ); // m02 - m20
        this->z = _copysign( z, matrix[3] - matrix[1] ); // m10 - m01
     */
	default:
		printf("Invalid matrix dimension\n");
		throw "Invalid dimension";
	}
    #undef MAX
}

/* Copy constructor from another quaternion */
Quaternion::Quaternion( const Quaternion &q )
{
    w = q.w;
    x = q.x;
	y = q.y;
	z = q.z;
}

/* Quaternion Multiplication
 * Multiplying q1 = c1 + v1 and q2 = c2 + v2 gives qf = q1 * q2
 * where the results qf = c + v and
 * c = c1*c2 - dot(v1, v2)
 * v = c1*v2 + c2*v1 + cross(v1, v2)
 */
Quaternion Quaternion::operator* ( const Quaternion& q ) const
{
	return Quaternion( w * q.w - x * q.x - y * q.y - z * q.z,
		               w * q.x + x * q.w + y * q.z - z * q.y,
	                   w * q.y + y * q.w + z * q.x - x * q.z,
	                   w * q.z + z * q.w + x * q.y - y * q.x );
}

/* Based on the following code from 
 * http://www.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
 * I simplified a lot in order to maximize performance
 * Vector3 vn(vec);
 * vn.normalise();
 *
 * Quaternion vecQuat, resQuat;
 * vecQuat.x = vn.x;
 * vecQuat.y = vn.y;
 * vecQuat.z = vn.z;
 * vecQuat.w = 0.0f;
 *
 * resQuat = vecQuat * getConjugate();
 * resQuat = *this * resQuat;
 *
 * return (Vector3(resQuat.x, resQuat.y, resQuat.z));
 */
Vector Quaternion::operator* ( const Vector& v ) const
{
	Vector vnorm(v);
	vnorm.normalize();

    Vector vRotated(
		-2*vnorm.x*(w - x + y + z) + 2*vnorm.y*x*y             + 2*vnorm.z*x*z,
		 2*vnorm.x*x*y             - 2*vnorm.y*(w + x - y + z) + 2*vnorm.z*y*z,
		 2*vnorm.x*x*z             + 2*vnorm.y*y*z             + 2*vnorm.z*(w + x + y - z)
		);
 
	return vRotated;
}

void Quaternion::normalize()
{
	float mag = w * w + x * x + y * y + z * z;
	if( abs(mag - 1) < 0.000001 && abs(mag) < 0.000001 )
	{
		mag = sqrt(mag);
		w /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
	}
}

void Quaternion::reverse()
{
	//(*this)*Quaternion(
}

Quaternion::~Quaternion( void )
{
}
