#pragma once
template<class T>
void SAFE_RELEASE(T*& pRef) {
	if (nullptr != pRef) {
		pRef->Release();
		pRef = nullptr;
	}
}