#pragma once
#ifdef LISPSCRIPTENGINE_EXPORTS
#define LISP_SCRIPT_API __declspec(dllexport)
#else

#define LISP_SCRIPT_API __declspec(dllimport)
#endif
