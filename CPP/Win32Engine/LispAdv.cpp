#include"LispAdv.h"

LispScriptEngine::lisp_ptr CLispADV::s_selfType(new LispScriptEngine::CLispType("ADV"));

void CLispADV::loadImage(int idx, std::string txt) {
	m_adv.loadImage(idx, txt);
}
void CLispADV::clear(int idx) {
	m_adv.clear(idx);

}