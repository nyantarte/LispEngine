#pragma once
#include"Win32Framework.h"
class CPoint :public POINT {
public:
	CPoint(int _x, int _y) {
		x = _x;
		y = _y;
	}
	CPoint() {
		x = 0;
		y = 0;
	}
	CPoint(POINT& po) {
		x = po.x;
		y = po.y;
	}

	bool equals(int _x, int _y) {
		return x == _x && y == _y;
	}

	CPoint operator +(CPoint& po) {

		return CPoint(this->x + po.x, this->y + po.y);
	}

	CPoint operator -(CPoint& po) {

		return CPoint(this->x - po.x, this->y - po.y);
	}

	CPoint& operator+=(CPoint& po) {
		this->x += po.x;
		this->y += po.y;
		return *this;
	}
	CPoint& operator*=(int s) {
		this->x *= s;
		this->y *= s;
		return *this;
	}
	CPoint& operator/=(int s) {
		this->x /= s;
		this->y /= s;
		return *this;
	}


	bool operator==(CPoint& po) {
		return equals(po.x, po.y);
	}
	bool operator!=(CPoint& po) {
		return !equals(po.x, po.y);
	}
	float GetDist(CPoint& po) {
		int xLen = po.x - this->x;
		int yLen = po.y - this->y;
		return (float)sqrt(xLen * xLen + yLen * yLen);
	
	}

	float GetDist(int _x,int _y) {
		int xLen = _x - this->x;
		int yLen = _y - this->y;
		return (float)sqrt(xLen * xLen + yLen * yLen);

	}
	std::string toString() {
		std::stringstream ss;
		ss << this->x << "," << this->y;
		return ss.str();
	
	}
};
class CSize :public SIZE {
public:
	CSize(int x, int y) {
		cx = x;
		cy = y;
	}
	CSize() {}
};
class CRect :public RECT {

public:
	CRect() {}
	CRect(int l, int t, int w, int h)
	{
		left = l;
		top = t;
		right = left + w;
		bottom = top + h;
	}
	CRect(CPoint& p,int w,int h) {
		left = p.x - w / 2;
		top = p.y - h / 2;
		right = p.x + w / 2;
		bottom = p.y + h / 2;
	}

	CRect(CSize s) {
		left = 0;
		top = 0;
		right = left + s.cx;
		bottom = top + s.cy;
	}
	int GetWidth() {
		return right - left;
	}
	int GetHeight() {
		return bottom - top;
	}

	void GetCenterPos(CPoint& pos) {
		pos.x = left + GetWidth() / 2;
		pos.y = top + GetHeight() / 2;
	}
	bool IsInside(CPoint& pos) {
		return left <= pos.x && right > pos.x && top <= pos.y && bottom > pos.y;
	}
	bool IsInside(int x, int y) {
		return left <= x && right > x && top <= y && bottom > y;
	}

};
