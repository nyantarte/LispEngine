#include"charactor.h"
std::shared_ptr<CCharaManager> CCharaManager::s_Self(new CCharaManager());
chara_ptr  CCharaManager::getChara(std::string n) {
	NameCharaTbl::iterator it = m_nameCharaTbl.find(n);
	if (it != m_nameCharaTbl.end()) {
		return it->second;
	}
	return nullptr;

}