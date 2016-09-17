#ifndef ZOBRIST_H
#define ZOBRIST_H
#include <cstdlib>
#include <ctime>
#include "stdafx.h"

typedef struct
{
	DWORD dwKey, dwLock1, dwLock2;
}SZobrist;

DWORD rand32();
void xor(SZobrist &dst, const SZobrist &src);

#endif