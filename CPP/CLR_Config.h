#pragma once
#ifdef USE_CLR
#define STRING String^
#define _SIZE_T unsigned
#define REF(T) T^
#define CLASS(T,N) T^ N=gcnew T()
#define _CLASS(T,N,P) T^ N=gcnew T(P)
#define PTR(T) T^
#define NEW gcnew
#define MAP Dictionary
#define LIST LinkedList
#define ARRAY List
#define STACK Stack
#define REF_ACCESS(I,M) I->M
#define DEF_CLASS(T) public ref class T
#define DERIVE_CLASS(T,S) public ref class T:public S

namespace CLR_Utils{
generic<class F,class S>
public ref class Tuple{
public:
	F first;
	S second;
public:
	Tuple(F f,S s)
		:first(f),
		 second(s)
	{}
};
};


#define ASSERT Debug::Assert

#else
#define STRING std::string
#define _SIZE_T std::size_t
#define REF(T) T&
#define CLASS(T,N) T N;
#define PTR(T) T*
#define MAP std::map
#define ARRAY std::vector
#define STACK std::stack
#define LIST std::list
#define REF_ACCESS(I,M) I.M
#define NEW new
#define DEF_CLASS(T) class T
#define DERIVE_CLASS(T,S) class T:public S
#define TUPLE std::pair
#define ASSERT assert


#endif
