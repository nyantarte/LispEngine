#pragma once

#include"Config.h"
#ifdef USE_D2D_EXPORTS
	#ifdef D2D_EXPORTS
		#define LISP_SCRIPT_API __declspec(dllexport)
	#else

		#define LISP_SCRIPT_API __declspec(dllimport)
	#endif
#endif