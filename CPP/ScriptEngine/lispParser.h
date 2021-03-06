#pragma once
#include"ScriptEngineConfig.h"
#ifdef USE_CRL
using namespace System;
using namespace System::Diagnostic;
using namespace System::IO;
using namespace System::Text;
#else
#include<vector>
#include<string>
#include<memory>
#endif

#include"lispTypes.h"
#include"lispLexer.h"
namespace LispScriptEngine {
	/**
	*	@brief 構文解析クラス
	*/
	DEF_CLASS(CLispParser) {
		lisp_ptr m_root;	/*!	ルートチャンクオブジェクト*/
		typedef ARRAY<lisp_ptr> NameStack;
		CLASS(NameStack,m_nameStack);	/*!	スタックフレームを保持*/
#ifdef USE_CLR
		static lisp_ptr s_rootType=gcnew CLispType("root");/*!	デフォルトで作成されるルートチャンクオブジェクトのタイプ*/
#else
		static lisp_ptr s_rootType ;/*!	デフォルトで作成されるルートチャンクオブジェクトのタイプ*/
#endif

	public:
		CLispParser();

		/**
		*	@brief 構文解析を行う
		*	@param l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*/
		void parse(REF(CLispLexer) l);

		/**
		*	@return ルートチャンクオブジェクト
		*/
		lisp_ptr getRoot() {
			return m_root;
		}
		
		/**
		*	@return 現在の名前検索用スタック最上位オブジェクトを返す
		*/
		lisp_ptr getTop() {
#ifdef USE_CLR
			return m_nameStack[m_nameStack->Count - 1];
#else
			return *(m_nameStack.end() - 1);
#endif
		}

	private:
		/**
		*	@brief	値構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseValue(REF(CLispLexer) l);
		/**
		*	@brief	関数呼び出し構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseCall(REF(CLispLexer) l);
		/**
		*	@brief	引数構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseArgs(REF(CLispLexer) l);
		/**
		*	@brief	変数定義構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseLet(REF(CLispLexer) l);
		/**
		*	@brief	For構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseFor(REF(CLispLexer) l);
		/**
		*	@brief	While構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseWhile(REF(CLispLexer) l);
		/**
		*	@brief	If構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseIf(REF(CLispLexer) l);
		/**
		*	@brief	ElseIf構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseElseIf(REF(CLispLexer) l);
		/**
		*	@brief	Else構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseElse(REF(CLispLexer) l);
		/**
		*	@brief	関数定義構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseDefunc(REF(CLispLexer) l);
		/**
		*	@brief	include構文の解析を行う
		*	@param	l	構文解析を行うソース文字列を格納した字句解析オブジェクト
		*	@return 	解析した結果となるCLispObjectへの参照
		*/
		lisp_ptr parseInclude(REF(CLispLexer) l);

		/**
		*	@brief 名前検索スタックを用いて指定された名前のオブジェクトを検索する
		*	@param name	検索に使用する名前
		*	@param popNum	名前スタック上でポップを行った回数
		*	@return 検索結果となるオブジェクト、見つからなかったらnullを返す
		*/
		lisp_ptr find(STRING name,
#ifdef USE_CLR
			int% popNum
#else
			int& popNum
#endif
		);

		/**
		*	@brief オブジェクトを名前検索スタック上に登録する
		*	@param name	検索に使用する名前
		*	@param o	登録するオブジェクト
		*/
		void regist(STRING name, lisp_ptr p);

		/**
		*	@brief 名前検索スタック最上位にオブジェクト登録する
		*	@param p	登録するオブジェクト
		*/
		void push(lisp_ptr p) {
#ifdef USE_CLR
			m_nameStack->Add(p);
#else
			m_nameStack.push_back(p);
#endif
		}
		/**
		*	@brief	名前検索スタック最上位のオブジェクトを取り除く
		*/
		void pop() {
#ifdef USE_CLR
			m_nameStack->RemoveAt(m_nameStack->Count-1);
#else
			m_nameStack.pop_back();
#endif
		}

		/**
		*	@brief ファイルからソース文字列を読み込み、字句解析オブジェクトにその文字列で初期化する
		*	@param l	初期化対象となる字句解析オブジェクト
		*	@param fname	ソース文字列を読み込むファイルのファイルパス
		*/
		void genLexerFromFile(REF(CLispLexer) l,STRING fname);

	};
};