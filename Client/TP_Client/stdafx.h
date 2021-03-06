// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include <imm.h>  

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
//
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <cassert>
#include <atlimage.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <WS2tcpip.h>
#include <array>
#include <queue>
using namespace std;
#pragma comment(lib, "Ws2_32.lib") 
#pragma comment(lib, "imm32.lib") 

#include "Vector2D.h"
#include "../../Server/TP_Server/protocol.h"  
 
constexpr auto TILE_WIDTH = 48;
constexpr auto WINDOW_WIDTH = TILE_WIDTH * SCREEN_WIDTH;   // size of window
constexpr auto WINDOW_HEIGHT = TILE_WIDTH * SCREEN_WIDTH;
constexpr auto BUF_SIZE = 1024;

#define	WM_SOCKET				WM_USER + 1

// 디버그 모드에서는 서버와 연결하지 않고 
// 클라이언트 기능을 테스트 해볼 수 있도록
#define ALWAYS_TEST 0 // 0 : debug 모드에서만, 1 : 상시

#ifdef _DEBUG
	#define TEST_MODE
#elif ALWAYS_TEST
	#define TEST_MODE
#endif

const Vector2f MAP_TILE_SIZE{ TILE_WIDTH, TILE_WIDTH };
// 디버그 모드인 경우에
// 클라이언트의 하위창으로 콘솔창을 사용하는 코드
//#ifdef _DEBUG
//	#ifdef UNICODE
//		#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//	#else
//		#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//	#endif
//#endif
#ifdef UNICODE
	#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
	#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

inline int wrap(int data, int low, int high)
{
	assert(low < high);
	const int n = (data - low) % (high - low);
	return (n >= 0) ? (n + low) : (n + high);
}
inline int wrap(int data, int MAX)
{
	return data % MAX;
}
inline float wrap(float data, float low, int high)
{
	assert(low < high);
	const float f = std::fmod((data - low), (high - low));
	return (f >= 0) ? (f + low) : (f + high);
}
inline float wrap(float data, float MAX)
{
	return std::fmod(data, MAX);
}
inline int clamp(int data, int low, int high)
{
	return min(max(data, low), high);
}
inline double clamp(double data, double low, double high)
{
	return min(max(data, low), high);
}

inline bool isZero(double data)
{
	return std::fabs(data) < std::numeric_limits<double>::epsilon();
}

inline Vector2i ToTileIndex(float x, float y)
{
	//x -= MAP_TILE_START_POS.x;
	//y -= MAP_TILE_START_POS.y;

	return {int( x / MAP_TILE_SIZE.x), int(y / MAP_TILE_SIZE.y) };
}

#define VK_0	0x30 
#define VK_1	0x31
#define VK_2	0x32
#define VK_3	0x33
#define VK_4	0x34
#define VK_5	0x35 
#define VK_6	0x36
#define VK_7	0x37
#define VK_8	0x38
#define VK_9	0x39
#define VK_DOT	0x2E

#define VK_W 0x57
#define VK_a 0x41
#define VK_A 0x61
#define VK_S 0x53
#define VK_D 0x44
#define VK_B 0x42
#define VK_C 0x43
#define VK_U 0x55
#define VK_I 0x49
#define VK_O 0x4F
#define VK_P 0x50
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C 

void display_error(const char* msg, int error_no);
void error_display(const char* msg);

//bool IsNpc(int id){ return id >= NPC_ID_START; }

TCHAR* StringToTCHAR(std::string& s);
string TCHARToString(const TCHAR* ptsz); 