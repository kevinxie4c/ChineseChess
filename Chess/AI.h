#ifndef AI_H
#define AI_H
#include "stdafx.h"
#include "Game.h"
#include "Zobrist.h"
#define MOVEMENT WORD
#define SRC(x) ((x) & 0xff)
#define DST(x) ((x) >> 8)
#define MOV(dst, src) ((dst)<<8 | (src))
#define LEN_OF_HASHTABLE	0x800000
#define LEN_MASK			0x7fffff
#define VALUNKNOWN			0xffffff
#define M_INFINITY			0xffff
#define HASH_EXACT			0x00
#define HASH_ALPHA			0x01
#define HASH_BETA			0x02
#define READY				0
#define RUNNING				1
#define FINISHED			2
#define BACK_MAX_DEPTH		12
#define WM_FINISHED (WM_USER + 0)

extern SChessPiece asPiece[32];
extern int anBoard[256];
extern int anIndex[256];
extern SChessPiece asPiece[32];
extern SZobrist ZobristTable[16][256];

typedef struct
{
	MOVEMENT mv;
	BYTE mvtype;
	DWORD dwKey;
}SRecord;

typedef struct
{
	DWORD dwKey,dwLock1, dwLock2;
	int flag,depth;
	long value;
}SHash;

typedef struct
{
	int maxdepth, maxtime;
	MOVEMENT res;
	int state;
	HWND hwnd;
}PARAMS,*PPARAMS;

void Search(PVOID pVoid);
void backsearch(PVOID pVoid);
MOVEMENT rootsearch(int depth);
void Move(const MOVEMENT mv, int &tmp, int &tmpIndex);
void InitHash();

inline BOOL Home(int Pos, int Player) {
	return (Pos & 0x80) == (Player << 7);
}

inline BOOL NotHome(int Pos, int Player) {
	return (Pos & 0x80) != (Player << 7);
}


#endif