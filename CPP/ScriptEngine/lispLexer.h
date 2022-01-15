#pragma once
#include"ScriptEngineConfig.h"
#ifdef USE_CLR
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
#else
#include<string>
#endif
namespace LispScriptEngine {
	/**
	*	@brief 字句解析クラス
	*/
	DEF_CLASS(CLispLexer) {
		STRING m_src;		/*!解析対象となるソース文字列*/

#ifdef USE_CLR
		int m_pos;
#else
		std::string::iterator m_pos;	/*!次の字句への参照位置*/
#endif

	STRING m_currentToken;	/*!現在参照している字句*/
	public:
		CLispLexer()
#ifdef	USE_CLR
			:m_pos(0)
#else
			:m_pos(m_src.end())
#endif
		{}
		
		/**
		*	@param src	解析を行うソース文字列
		*/
		CLispLexer(STRING src) {
			reset(src);
		}
		/**
		*	@brief 自身を指定されたソース文字列で初期化を行う
		*	@param src 解析を行うソース文字列
		*/
		void reset(STRING src) {
			m_src = src;
#ifdef USE_CLR
			m_pos = 0;
#else
			m_pos = m_src.begin();
#endif
			getNext();

		}

		/**
		*	@return 現在参照している字句を返す
		*/
		STRING getCurrent() {
			return m_currentToken;
		}

		/**
		*	@return 次の字句先頭への参照位置
		*/
#ifdef	USE_CLR
		int getPos(){
#else
		std::string::iterator getPos() {
#endif
			return m_pos;
		}

		/**
		*	@brief 次の字句を返し、参照位置を返した字句の次の字句へ設定する
		*	@return 次の字句を返す
		*/
		STRING getNext();
		/**
		*	@return 文字列末端を参照していたらtrue、それ以外はfalseを返す
		*/
		bool isEnd();


		/**
		*	@brief 字句への参照位置を設定する
		*	@param pos 設定する参照位置
		*/
#ifdef	USE_CLR
		void setPos(int pos) {
#else
		void setPos(std::string::iterator pos) {
#endif
			m_pos = pos;
			getNext();
		}
	private:
		/**
		*	@brief 現在参照している位置で字句解析を行う。解析後、参照位置を次の字句の先頭へ進める
		*	@return 解析結果となる字句を返す
		*/
		STRING parse();


	};
};