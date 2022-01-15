// header.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// またはプロジェクト専用のインクルード ファイル
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include <Windows.h>
#include <d2d1_1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib" )
#include <wincodec.h>
#include <wincodecsdk.h>

#include<memory>
#include<cassert>
#include<string>


#include"../Win32/win32_error.h"