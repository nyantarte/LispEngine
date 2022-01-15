#pragma once
#include"Win32Framework.h"
enum KEY{
	KEY_SPACE=VK_SPACE,
	KEY_RETURN=VK_RETURN,
	KEY_LEFT=VK_LEFT,
	KEY_RIGHT=VK_RIGHT,
	KEY_UP=VK_UP,
	KEY_DOWN=VK_DOWN,
	KEY_X= 0x53,
	KEY_Y= 0x5A,
	KEY_A= 0x43,
	KEY_B= 0x58,
	MOUSE_L_BTN=VK_LBUTTON,


};
class CWin32Keystate
{
	KEY m_targetKey;
	bool m_state[2];
	int  m_targetIdx;

public:
	CWin32Keystate(KEY k)
		:m_targetKey(k),
		m_targetIdx(0)
	{}
	CWin32Keystate()
		:m_targetIdx(0)
	{}

	void update();

	bool isTriggered();
	bool isPressed();


	void setKey(KEY k){
		m_targetKey = k;
	}
private:
	int calcNextKeyBufIndex();

};

