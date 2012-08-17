/*
 * Copyright © 2012 Fabian Schuiki
 */

#pragma once

#include <cmath>
#include <sstream>


template <typename T>
class Vector
{
public:
	T x;
	T y;
	T z;
	
	//Constructors
	Vector()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vector(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	//Operators
	Vector<T>& operator= (Vector<T> v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	Vector<T> operator- ()	{ return Vector<T>(-x, -y, -z); }
	
	//CVector-CVector
	Vector<T> operator+ (Vector<T> v)	{ return Vector<T>(x+v.x, y+v.y, z+v.z); }
	Vector<T> operator- (Vector<T> v)	{ return Vector<T>(x-v.x, y-v.y, z-v.z); }
	Vector<T> operator* (Vector<T> v)	{ return Vector<T>(x*v.x, y*v.y, z*v.z); }
	Vector<T> operator/ (Vector<T> v)	{ return Vector<T>(x/v.x, y/v.y, z/v.z); }
	//CVector-Scalar
	Vector<T> operator+ (T v)	{ return Vector<T>(x+v, y+v, z+v); }
	Vector<T> operator- (T v)	{ return Vector<T>(x-v, y-v, z-v); }
	Vector<T> operator* (T v)	{ return Vector<T>(x*v, y*v, z*v); }
	Vector<T> operator/ (T v)	{ return Vector<T>(x/v, y/v, z/v); }
	
	//CVector-CVector
	Vector<T>& operator+= (Vector<T> v)	{ x += v.x; y += v.y; z += v.z; return *this; }
	Vector<T>& operator-= (Vector<T> v)	{ x -= v.x; y -= v.y; z -= v.z; return *this;	}
	Vector<T>& operator*= (Vector<T> v)	{ x *= v.x; y *= v.y; z *= v.z; return *this; }
	Vector<T>& operator/= (Vector<T> v)	{ x /= v.x; y /= v.y; z /= v.z; return *this; }
	
	//CVector-Scalar
	Vector<T>& operator+= (T v)	{ x += v; y += v; z += v; return *this; }
	Vector<T>& operator-= (T v)	{ x -= v; y -= v; z -= v; return *this; }
	Vector<T>& operator*= (T v)	{ x *= v; y *= v; z *= v; return *this; }
	Vector<T>& operator/= (T v)	{ x /= v; y /= v; z /= v; return *this; }
	
	//Operations
	inline T length()
	{
		return sqrt(x*x + y*y + z*z);
	}
	
	void normalize()
	{
		T length = this->length();
		
		x /= length;
		y /= length;
		z /= length;
	}
	
	inline T dot(Vector<T> v)
	{
		return (x*v.x + y*v.y + z*v.z);
	}
	
	inline Vector<T> cross(Vector<T> v)
	{
		return Vector<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	
	//Description
	std::string toString() {
		std::stringstream s(std::stringstream::out);
		s << "{" << x << ", " << y << ", " << z << "}";
		return s.str();
	}
};

typedef Vector<int> ivec3;
typedef Vector<float> vec3;
