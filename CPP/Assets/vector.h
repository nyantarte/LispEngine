#pragma once
#include"AssetsFramework.h"
class CVector {
public:
	union {
		float xyz[3];
		struct {
			float x, y, z;
		};
	};
	static CVector s_zero;
public:
	CVector(float _x = 0, float _y = 0, float _z = 0)
		:x(_x),
		y(_y),
		z(_z)
	{}
	CVector(CPoint& po)
		:x(po.x),
		y(po.y),
		z(0)
	{}

	CPoint ToPoint() {
		return CPoint((int)this->x, (int)this->y);
	}

	bool isInside(CVector& v, float rad) {
		CVector sub = *this - v;

		return sub.Length() < rad;
	}
	bool isInside(CPoint& po, float rad) {
		CVector tmp(po);
		return isInside(tmp, rad);
	}
	float Length() {
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}
	CVector operator+(CVector& v) {
		return CVector(this->x + v.x, this->y + v.y, this->z + z);
	}
	CVector operator+(CVector v) {
		return CVector(this->x + v.x, this->y + v.y, this->z + z);
	}

	CVector operator-(CVector& v){
		return CVector(this->x - v.x, this->y - v.y, this->z - z);
	}
	/*
	CVector operator-(CVector v) {
		return CVector(this->x - v.x, this->y - v.y, this->z - z);
	}*/
	CVector operator*(float s) {
		return CVector(this->x *s, this->y *s, this->z *s);
	}
	CVector operator/(float s) {
		return CVector(this->x / s, this->y / s, this->z / s);
	}

	
	CVector& operator+=(CVector& v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	
	}
	CVector& operator-=(CVector& v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;

	}

	CVector& operator*=(float s) {
		this->x *= s;
		this->y *= s;
		this->z *= s;
		return *this;

	}
	CVector& operator/=(float s) {
		this->x /= s;
		this->y /= s;
		this->z /= s;
		return *this;

	}

	CVector& operator=(CPoint& p) {
		return *this = CVector(p.x, p.y);
	}
};