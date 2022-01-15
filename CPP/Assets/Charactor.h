#pragma once

#include"AssetsFramework.h"
#include"AssetsConfig.h"
#include"Arts.h"
class CCharaStatus {
public:
	int life;
	int atk;
};

class CCharactor;
typedef std::shared_ptr<CCharactor> chara_ptr;

class CCharactor {
	CCharaStatus m_baseStatus;
	CCharaStatus m_curStatus;

	std::string m_name;
	CArts m_arts;
public:
	enum FLAG {
		PLAYER_USE_ONLY=1,
		FIELD_OBJECT=2,
		PLAYER_USE=4,
	};
	typedef std::vector<std::string> DropTbl;
private:
	int m_flags;

	DropTbl m_dropCharaTbl;

public:
	CCharactor(std::string name)
		:m_name(name),
		m_flags(0)
	{}

	CCharactor()
	: m_flags(0)
	{}
	CCharactor(CCharactor& c) 
		:m_name(c.m_name),
		 m_baseStatus(c.m_baseStatus),
		 m_curStatus(c.m_curStatus),
		 m_arts(c.m_arts),
		 m_flags(c.m_flags)
	{
	
	}
	CCharaStatus& getCurrentStatus() {
		return m_curStatus;
	}

	std::string getName() {
		return m_name;
	}
	void setName(std::string n) {
		m_name = n;
	}
	int& flags() {
		return m_flags;
	}
	CArts& arts() {
		return m_arts;
	}
	DropTbl& dropCharaTbl() {
		return m_dropCharaTbl;
	}

	chara_ptr clone() {
		return chara_ptr(new CCharactor(*this));
	}
};
class CParty {
	chara_ptr m_members[PARTY_CHARA_MAX];
	
};
typedef std::shared_ptr<CParty> party_ptr;
class CCharaManager {
public:
	static std::shared_ptr<CCharaManager> s_Self;
private:
	typedef std::map<std::string, chara_ptr> NameCharaTbl;
	NameCharaTbl m_nameCharaTbl;
	typedef std::vector<chara_ptr> CharaTbl;
	CharaTbl m_charaTbl;
public:
	chara_ptr getChara(std::string n);
	void add(chara_ptr p) {
		m_charaTbl.push_back(p);
		m_nameCharaTbl.insert(NameCharaTbl::value_type(p->getName(), p));
	}
	CharaTbl& charaTbl() {
		return m_charaTbl;
	}

};