#pragma once
#include"GraphFramework.h"
class CNode {
	std::string m_name;
public:
	std::string name() {
		return m_name;
	}

	virtual void reset() {}
	virtual void Paint(image_render_target_ptr target, CDWTextFormat& f) {}
	virtual void update() {}
	virtual void exec() {}
	virtual void onActivate(bool flag) {}
};
typedef std::shared_ptr<CNode> node_ptr;
class CRougeNode :public CNode {
	CRouge m_rouge;
	

	
	chara_ptr m_pcChara;
public:
	CRougeNode() {
		m_rouge.reset();
		//m_rouge.GetCharaPositionInfo(0).chara.reset(new CCharactor(""));
	}
	CRect& border() {
		return m_rouge.RougeSize();
	}
	
	void setPCChara(chara_ptr p);
	void reset();
	virtual void Paint(image_render_target_ptr target, CDWTextFormat& f);
	virtual void update();
};
class CBattleNode:public CNode{

};
class CScreenNode :public CNode {
	CSize m_size;
public:
	typedef std::vector<node_ptr> NodeTbl;
private:
	NodeTbl m_children;
public:
	CSize& size() {
		return m_size;
	}
	NodeTbl& children() {
		return m_children;
	}
	virtual void Paint(image_render_target_ptr target, CDWTextFormat& f) {
		for (node_ptr c : m_children) {
			c->Paint(target, f);
		}
	
	}
	virtual void update() {
		for (node_ptr c : m_children) {
			c->update();
		}
	}
	virtual void onActivate(bool flag) {
		for (node_ptr c : m_children) {
			c->onActivate(flag);
		}
	}
};
template<class T>
class CConstNode {
	T m_value;
public:
	T& value() {
		return m_value;
	}
};
typedef std::shared_ptr<CConstNode<int>> int_const_node_ptr;

template<class T>
class CVariableNode:public CNode {
	T m_init;
	T m_value;
public:
	T& value() {
		return m_value;
	}
	T& init() {
		return m_init;
	}
	virtual void reset() {
		m_value = m_init;
	}


};
typedef CVariableNode<int> IntVarNode;
typedef std::shared_ptr<IntVarNode> int_var_node_ptr;
class CCounterNode :public CNode {

	std::string m_target;
	int m_inc_value;
public:

	std::string target() {
		return m_target;
	}
	int& incValue() {
		return m_inc_value;
	}
};
class CNextSceneNode :public CNode {
public:
	enum MOVE_TYPE {
		PUSH,
		POP,
		RETURN,
	};
private:
	MOVE_TYPE m_moveType;
	std::string m_target;
public:
	CNextSceneNode()
		:m_moveType(MOVE_TYPE::POP)
	{}
	MOVE_TYPE& moveType() {
		return m_moveType;
	}
	void setTarget(std::string n) {
		m_target = n;
	}
	std::string getTarget() {
		return m_target;
	}

	virtual void exec();
};
class CImageNode :public CNode, IOnIdleCallback {

	CRect m_border;
	image_ptr m_image;
	std::string m_imageSrc;
	std::string m_onClick;
	
public:
	CImageNode();
	~CImageNode();
	CRect& border() {
		return m_border;
	}

	std::string onClick() {
		return m_onClick;
	}

	std::string imageSrc() {
		return m_imageSrc;
	}
	virtual bool OnIdle();
	virtual void onActivate(bool flag);
};
class CTextNode :public CNode, IOnIdleCallback {
	std::wstring m_caption;
	CRect m_border;
	std::string m_onClick;
public:
	CTextNode();
	~CTextNode();
	CRect& border() {
		return m_border;
	}

	std::string onClick() {
		return m_onClick;
	}

	std::wstring getCaption() {
		return m_caption;
	}
	void setCaption(std::wstring t) {
		m_caption = t;
	}

	virtual bool OnIdle();
	virtual void Paint(image_render_target_ptr target, CDWTextFormat& f);
	virtual void onActivate(bool flag);
};