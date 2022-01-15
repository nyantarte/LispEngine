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
	*	@brief �����̓N���X
	*/
	DEF_CLASS(CLispLexer) {
		STRING m_src;		/*!��͑ΏۂƂȂ�\�[�X������*/

#ifdef USE_CLR
		int m_pos;
#else
		std::string::iterator m_pos;	/*!���̎���ւ̎Q�ƈʒu*/
#endif

	STRING m_currentToken;	/*!���ݎQ�Ƃ��Ă��鎚��*/
	public:
		CLispLexer()
#ifdef	USE_CLR
			:m_pos(0)
#else
			:m_pos(m_src.end())
#endif
		{}
		
		/**
		*	@param src	��͂��s���\�[�X������
		*/
		CLispLexer(STRING src) {
			reset(src);
		}
		/**
		*	@brief ���g���w�肳�ꂽ�\�[�X������ŏ��������s��
		*	@param src ��͂��s���\�[�X������
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
		*	@return ���ݎQ�Ƃ��Ă��鎚���Ԃ�
		*/
		STRING getCurrent() {
			return m_currentToken;
		}

		/**
		*	@return ���̎���擪�ւ̎Q�ƈʒu
		*/
#ifdef	USE_CLR
		int getPos(){
#else
		std::string::iterator getPos() {
#endif
			return m_pos;
		}

		/**
		*	@brief ���̎����Ԃ��A�Q�ƈʒu��Ԃ�������̎��̎���֐ݒ肷��
		*	@return ���̎����Ԃ�
		*/
		STRING getNext();
		/**
		*	@return �����񖖒[���Q�Ƃ��Ă�����true�A����ȊO��false��Ԃ�
		*/
		bool isEnd();


		/**
		*	@brief ����ւ̎Q�ƈʒu��ݒ肷��
		*	@param pos �ݒ肷��Q�ƈʒu
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
		*	@brief ���ݎQ�Ƃ��Ă���ʒu�Ŏ����͂��s���B��͌�A�Q�ƈʒu�����̎���̐擪�֐i�߂�
		*	@return ��͌��ʂƂȂ鎚���Ԃ�
		*/
		STRING parse();


	};
};