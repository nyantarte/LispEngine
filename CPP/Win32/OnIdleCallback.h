#pragma once
class IOnIdleCallback {
public:
	virtual bool OnIdle() { return false; }
};
typedef std::set<IOnIdleCallback*> OnIdleCallbackTbl;