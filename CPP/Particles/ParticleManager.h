#pragma once
#include"ParticlesFrameworks.h"
class CParticle {
	int m_life;
	CPoint m_pos;
	CPoint m_dir;
	int m_rad;
public:
	CParticle()
		:m_life(0)
	{}
	int& life() {
		return m_life;
	}

	CPoint& pos() {
		return m_pos;
	}
	CPoint& dir() {
		return m_dir;
	}
	int& rad() {
		return m_rad;
	}

};

template<class T>
class CParticleManager {
	typedef std::vector<T> ParticleTbl;
	ParticleTbl m_particleTbl;

public:
	ParticleTbl& particleTbl() {
		return m_particleTbl;
	}
	T& add(CPoint& pos, CPoint& dir, int  life, int r) {

		for (T& p : m_particleTbl) {
			if (0 >= p.life()) {
				p.pos() = pos;
				p.dir() = dir;
				p.life() = life;
				p.rad() = r;
				return p;

			}

		}
	}

	T& add(CVector& pos, CVector& dir, int  life, int r) {

		for (T& p : m_particleTbl) {
			if (0 >= p.life()) {
				p.pos() = pos;
				p.dir() = dir;
				p.life() = life;
				p.rad() = r;
				return p;;

			}

		}
	}
	void update() {
		for (T& p : m_particleTbl) {
			if (0 < p.life()) {
				p.pos() += p.dir();
				--p.life();
				
			}
		}
	
	}
	void reset() {
		for (T& p : m_particleTbl) {
			p.life() = 0;
		}
	}
};