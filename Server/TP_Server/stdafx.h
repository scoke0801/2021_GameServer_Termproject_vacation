// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
    
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
//
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <thread>
#include <map>
#include <vector>
#include <mutex>
#include <array>
#include <queue>
#include <unordered_set>
#include <string>
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "MSWSock.lib") 

using namespace std;
#include "protocol.h"

#include "astar.h"
// C로 사용해라 라고 비쥬얼 스튜디오에게 알려줌
extern "C" {
#include "../include/lua.h"
#include "../include/lauxlib.h"
#include "../include/lualib.h"
} 
#pragma comment(lib, "lua54.lib") 
   
constexpr int PLAYER_START_POS_X = 1899;
constexpr int PLAYER_START_POS_Y = 1899;
constexpr int MAX_BUFFER = 1024;
constexpr int MON_EXP_BASE = 50;
 
constexpr int SERVER_ID = 0;

// 섹터의 크기는 시야의 2배, 시야 반지름의 2배
constexpr int SECTOR_SIZE = VIEW_RADIUS * 4;
constexpr int SECTOR_WIDTH = (WORLD_WIDTH / SECTOR_SIZE);
constexpr int SECTOR_HEIGHT = (WORLD_HEIGHT / SECTOR_SIZE);
 
// 명령 목록 정리
enum OP_TYPE {
	OP_RECV,				// 수신
	OP_SEND,				// 송신
	OP_ACCEPT,				// 클라이언트 접속
	OP_PLAYER_MOVE,			// 플레이어 이동
	OP_MONSTER_MOVE,		// 몬스터 이동
	OP_ATTACK,				// 공격 명령
	OP_PLAYER_APPROCH,		// 플레이어가 몬스터에게 접근
	OP_RUN_AWAY,			// 몬스터 도망감
	OP_RESPONE,				// 리스폰
	OP_HEAL,				// 체력 회복
	OP_POTION_HEAL,			// 아이템을 통한 체력 회복
	OP_FULL_HEAL			// 풀 피 회 복
};

// NPC의 이동 행동 타입 :: 랜덤한 방향으로, 스크립트를 통해(한 방향으로 도망침)
enum NPC_MOVE_TYPE { MOVE_RANDOM, MOVE_SCRIPT };

struct EX_OVER
{
	WSAOVERLAPPED	m_over;
	WSABUF			m_wsabuf[1];
	unsigned char	m_packetbuf[MAX_BUFFER];
	OP_TYPE			m_op;
	SOCKET			m_csocket;					// OP_ACCEPT에서만 사용
};

// 플레이어의 상태 :: 연결 가능, 연결 중, 인게임 중
enum PL_STATE { PLST_FREE, PLST_CONNECTED, PLST_INGAME };
struct TIMER_EVENT {
	// 필수정보들 , 언제 누가 무엇을..
	int objcet;				// 누가
	OP_TYPE e_type;			// 무엇을
	chrono::system_clock::time_point start_t;		// 언제
	int target_id;

	// 우선순위 큐를 이용하기 위한 연산자 오버로딩!
	constexpr bool operator<(const TIMER_EVENT& other) const {
		return (start_t > other.start_t);
	}
	constexpr bool operator==(const TIMER_EVENT& other) const {
		return (objcet == other.objcet);
	}
};

// DB 타이머를 사용할 경우 이를 확장
struct DB_EVENT {
	int id;
	OP_TYPE type;
};

// 통신에 사용하는 기본 객체
class ServerObject {
public:
	mutex				m_slock;		// 상태를 변경할 때는 다른 쓰레드와 겹치지 않도록
	atomic<PL_STATE>	m_state;		// 상태, 작업시 atomic해야 한다

	int					id;				// 배열의 인덱스
	char				m_name[200];	// 객체의 이름	


	// 게임에서 사용되는 주요 정보들
	short				x, y, start_x, start_y;
	int					HP, LEVEL, EXP, GOLD;
	int					startHP;
	short				power;// 데미지
	ObjectType			type;// 객체 종류 

	int					itemCount;
	vector<ITEM_TYPE>	Items;

	// 게임 로직 계산을 위하여 사용되는 변수들
	DIRECTION			direction; 
	int					targetId; // 공격 or 추격 대상
	bool				isOnRunaway;	

	NPC_MOVE_TYPE		npc_move_type;

	// 길찾기에 사용 되는 변수들
	mutex				pathLock;
	vector<Point2i>		path;// 목표지점까지의 경로

	// NPC 객체가 현재 동작 중 인지를 구분하기 위한 변수
	atomic<bool>		is_active;

	// 스트레스 테스트의 딜레이 측정에 사용되는 변수
	// 송수신에 걸린 시간을 측정하기 위함
	int					move_time;		

	// 스크립트 동작을 위한 변수
	lua_State*			L;		
	mutex				m_sl;	// 스크립트 접근은 고유하게
};

// 시야 처리, 통신에 필요한 데이터가 확장된 게임 객체
class SESSION : public ServerObject {
public: 
	SOCKET m_socket;

	EX_OVER m_recv_over;
	int m_prev_size;

	unordered_set<int> m_view_list;
	mutex m_vl;
};