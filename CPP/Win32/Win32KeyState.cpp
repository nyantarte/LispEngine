#include"Win32Keystate.h"
void CWin32Keystate::update() {

	m_targetIdx = calcNextKeyBufIndex();
	m_state[m_targetIdx] =( GetAsyncKeyState(m_targetKey) & 0x01);

}
bool CWin32Keystate::isTriggered() {
	bool prevState = m_state[calcNextKeyBufIndex()];
	bool curState = m_state[m_targetKey];
	return prevState && !curState;
}
bool CWin32Keystate::isPressed() {
	//unsigned char prevState = m_state[calcNextKeyBufIndex()];
	bool curState = m_state[calcNextKeyBufIndex()];
	
	return curState;

}
int CWin32Keystate::calcNextKeyBufIndex() {
	return (m_targetIdx + 1) & 1;
}
