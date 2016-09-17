#include "Zobrist.h"

DWORD rand32()
{
	return rand()|(rand() << 15)|(rand() << 30);
}

void xor(SZobrist &dst, const SZobrist &src)
{
	dst.dwKey ^= src.dwKey;
	dst.dwLock1 ^= src.dwLock1;
	dst.dwLock2 ^= src.dwLock2;
}

bool equl(const SZobrist &a, const SZobrist &b)
{
	return a.dwKey == b.dwKey && a.dwLock1 == b.dwLock1 && a.dwLock2 == b.dwLock2;
}