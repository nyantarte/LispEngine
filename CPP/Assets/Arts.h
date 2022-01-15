#pragma once
class CArts {
public:
	enum TYPE {
		ASTEROID,
		HOUND,
	};

private:
	TYPE m_type;
	
	int m_range;
	int m_rad;
public:
	CArts()
		:m_type(TYPE::ASTEROID),
		 m_range(0),
		 m_rad(0)
	{}

	TYPE& type() {
		return m_type;
	}
	
	int& range() {
		return m_range;
	}

	int& rad() {
		return m_rad;
	}

};