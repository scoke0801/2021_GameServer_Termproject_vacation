// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <iostream>
#include <chrono>
#include <cassert>
#include <atlimage.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <WS2tcpip.h>
#include <array>
using namespace std;
#pragma comment(lib, "Ws2_32.lib") 

#include "Vector2D.h"
#include "../../Server/TP_Server/protocol.h"   

constexpr auto TILE_WIDTH = 48;
constexpr auto WINDOW_WIDTH = TILE_WIDTH * SCREEN_WIDTH;   // size of window
constexpr auto WINDOW_HEIGHT = TILE_WIDTH * SCREEN_WIDTH;
constexpr auto BUF_SIZE = 1024;

#define	WM_SOCKET				WM_USER + 1

const Vector2f MAP_TILE_SIZE{ TILE_WIDTH, TILE_WIDTH };

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


void display_error(const char* msg, int error_no);
void error_display(const char* msg);

#define MAX_PLAYER_NUM 10
#pragma pack (push, 1)
typedef struct move_packet
{
	short id;
	short playersNum;
	short size;
	short type;
	float x[MAX_PLAYER_NUM], y[MAX_PLAYER_NUM];
};
#pragma pack (pop)
enum class MoveType : char
{
	Up,
	Down,
	Left,
	Right
};