#pragma once
class CWin32Error {
};
class CWin32RegisterClassFailed :public CWin32Error {};

class CQueryInterfaceFailed :public CWin32Error {};
class CCoCreateInstanceFailed :public CWin32Error {};
