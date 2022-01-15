#pragma once
class CGraphError {
public:


};

class CJSONUnexcpectedEOF :public CGraphError {


};
class CJSONInvalidToken :public CGraphError {};
class CJSONInvalidType :public CGraphError {};