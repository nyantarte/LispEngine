#pragma once
#include"AssetsFramework.h"
#include"vector.h"
#include"Charactor.h"
class CBullet {

	int m_life;
	CVector m_pos;
	CVector m_dir;
	int m_rad;
	chara_ptr m_owner;
public:
	CBullet()
		:m_life(0)
	{}
	int& life() {
		return m_life;
	}

	CVector& pos() {
		return m_pos;
	}
	CVector& dir() {
		return m_dir;
	}
	int& rad() {
		return m_rad;
	}

	void setOwner(chara_ptr o) {
		m_owner = o;
	}
	chara_ptr getOwner() {
		return m_owner;
	}
};