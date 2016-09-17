#include "AI.h"
#include "ChessRule.h"
#include "Game.h"
#include <algorithm>
#include <ctime>
#include <cstring>
#include <iostream>
#include "process.h"
#define WEIGHT	3

int nPlayer;
volatile int g_nBackState;	//注意加上volatile关键字告诉编译器变量可能会被意想不到地改变（如其它线程），确保指令不会因为优化而省略
clock_t tTime;
DWORD dwZobristKey, dwZobristLock1, dwZobristLock2;
bool g_bStop;
//int f = 1;

int anMove[5][8] = 
{
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -16, -1, 1, 16, 0, 0, 0, 0},
	{ -17, -15, 15, 17, 0, 0, 0, 0},
	{ -34, -30, 30, 34, 0, 0, 0, 0},
	{ -33, -31, -18, 14, -14, 18, 31, 33 } 
};

int anSoldierMove[2][3] =
{
	{ 16, -1, 1 },
	{ -16, -1, 1 },
};
int anVehicleAndGunDelta[4] = { -1, 1, -16, 16 };

int LenOFMove[5] = { 0, 4, 4, 4, 8 };

int anLim[5][8] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -17, -15, 15, 17, 0, 0, 0, 0 },
	{ -16, -16, -1, -1, 1, 1, 16, 16 }
};

int anInBoard[256]=
{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int anInFort[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

long anValue[16] = { 
	0, 0, 20, 20, 100, 200, 100, 20,
	0, 0, 20, 20, 100, 200, 100, 20
};

SHash HashTable[2][LEN_OF_HASHTABLE];

//int nHistory[0xffff];

void InitHash()
{
	dwZobristKey = dwZobristLock1 = dwZobristLock2 = 0;
	for (int i = 0; i<256; ++i)
	if (anBoard[i])
	{
		dwZobristKey ^= ZobristTable[anBoard[i]][i].dwKey;
		dwZobristLock1 ^= ZobristTable[anBoard[i]][i].dwLock1;
		dwZobristLock2 ^= ZobristTable[anBoard[i]][i].dwLock2;
	}
}

long ProbeHash(int depth, int alpha, int beta)
{
	SHash *phash = &HashTable[nPlayer][dwZobristKey & LEN_MASK];
	if (phash->dwKey == dwZobristKey  && phash->dwLock1 == dwZobristLock1 && phash->dwLock2 == dwZobristLock2
		&& phash->depth >= depth)
	{
		if (phash->flag == HASH_EXACT) 
			return phash->value;
		
		if ((phash->flag == HASH_ALPHA) && (phash->value <= alpha)) 
			return alpha;
		
		if ((phash->flag == HASH_BETA) && (phash->value >= beta)) 
			return beta;
	}
	return VALUNKNOWN;
}

void RecordHash(int depth, int val, int flag) 
{
	SHash *phash = &HashTable[nPlayer][dwZobristKey & LEN_MASK];
	phash->dwKey = dwZobristKey;
	phash->dwLock1 = dwZobristLock1;
	phash->dwLock2 = dwZobristLock2;
	phash->value = val;
	phash->depth = depth;
	phash->flag = flag;
}
/*
bool CmpHis(const MOVEMENT &a, const MOVEMENT &b)
{
	return nHistory[a] > nHistory[b];
}

inline void SortByHis(MOVEMENT *amv,int len)
{
	std::sort(amv, amv + len, CmpHis);
}
*/

void Move(const MOVEMENT mv, int &tmp, int &tmpIndex)
{
	int dst = DST(mv);
	int src = SRC(mv);
	dwZobristKey ^= ZobristTable[anBoard[src]][src].dwKey;
	dwZobristKey ^= ZobristTable[anBoard[dst]][dst].dwKey;
	dwZobristKey ^= ZobristTable[anBoard[src]][dst].dwKey;
	dwZobristLock1 ^= ZobristTable[anBoard[src]][src].dwLock1;
	dwZobristLock1 ^= ZobristTable[anBoard[dst]][dst].dwLock1;
	dwZobristLock1 ^= ZobristTable[anBoard[src]][dst].dwLock1;
	dwZobristLock2 ^= ZobristTable[anBoard[src]][src].dwLock2;
	dwZobristLock2 ^= ZobristTable[anBoard[dst]][dst].dwLock2;
	dwZobristLock2 ^= ZobristTable[anBoard[src]][dst].dwLock2;
	tmp = anBoard[dst];
	tmpIndex = anIndex[dst];
	if (anIndex[dst] != -1)
		asPiece[anIndex[dst]].nPos = 0;
	asPiece[anIndex[src]].nPos = dst;
	anIndex[dst] = anIndex[src];
	anIndex[src] = -1;
	anBoard[dst] = anBoard[src];
	anBoard[src] = 0;
	nPlayer = !nPlayer;
}

void Cancel(const MOVEMENT mv, int tmp, int tmpIndex)
{
	int dst = DST(mv);
	int src = SRC(mv);
	anIndex[src] = anIndex[dst];
	anIndex[dst] = tmpIndex;
	asPiece[anIndex[src]].nPos = src;
	if (anIndex[dst] != -1)
		asPiece[anIndex[dst]].nPos = dst;
	anBoard[src] = anBoard[dst];
	anBoard[dst] = tmp;
	nPlayer = !nPlayer;
	dwZobristKey ^= ZobristTable[anBoard[src]][src].dwKey;
	dwZobristKey ^= ZobristTable[anBoard[dst]][dst].dwKey;
	dwZobristKey ^= ZobristTable[anBoard[src]][dst].dwKey;
	dwZobristLock1 ^= ZobristTable[anBoard[src]][src].dwLock1;
	dwZobristLock1 ^= ZobristTable[anBoard[dst]][dst].dwLock1;
	dwZobristLock1 ^= ZobristTable[anBoard[src]][dst].dwLock1;
	dwZobristLock2 ^= ZobristTable[anBoard[src]][src].dwLock2;
	dwZobristLock2 ^= ZobristTable[anBoard[dst]][dst].dwLock2;
	dwZobristLock2 ^= ZobristTable[anBoard[src]][dst].dwLock2;
}

bool FaceToFace()
{
	for (int i = 0; i < 16; ++i)
	if (asPiece[i].nType == 1)
	for (int j = asPiece[i].nPos + 16; j < 208; j += 16)
	if (anBoard[j])
	{
		if (anBoard[j] == 0x9) return true;
		else return false;
	}
	return false;
}

inline void AddMove(MOVEMENT *(&pmv)[3], int(&aLen)[3], int CurPos, int NextPos)
{
	if (anIndex[NextPos] == -1)
		pmv[2][aLen[2]++] = MOV(NextPos, CurPos);
	else
	if ((anBoard[NextPos] & TYPEMASK) == GENERAL)
		pmv[0][aLen[0]++] = MOV(NextPos, CurPos);
	else
		pmv[1][aLen[1]++] = MOV(NextPos, CurPos);
}

void CreateMove(MOVEMENT *(&pmv)[3], int (&aLen)[3])
{
	int CurPos, NextPos, l, r, type, delta;
	aLen[0] = aLen[1] = aLen[2] = 0;
	l = 0 + 16 * nPlayer;
	r = l + 16;
	for (int i = l; i < r; ++i)
	if (asPiece[i].nPos)	//确认棋子在棋盘上
	{
		CurPos = asPiece[i].nPos;
		type = asPiece[i].nType & TYPEMASK;
		if ((type) > 0 && (type) < 5)
		{
			for (int j = 0; j < LenOFMove[type]; ++j)
			{
				NextPos = CurPos + anMove[type][j];
				//检查是否在棋盘内且落点无己方棋子
				if (anInBoard[NextPos] && !(anIndex[NextPos] >= l && anIndex[NextPos] < r))
				{
					switch (type)
					{
					case GENERAL:
					case GUARDER:
						if (anInFort[NextPos])
							AddMove(pmv, aLen, CurPos, NextPos);
						break;
					case ELEPHANT:
						if (NotHome(NextPos, nPlayer))
							break;
					case HORSE:
						if (!anBoard[CurPos + anLim[type][j]])
							AddMove(pmv, aLen, CurPos, NextPos);
						break;
					default:
						break;
					}
				}
			}
		}
		else
		{
			switch (type)
			{
			case VEHICLE:
				for (int j = 0; j < 4; ++j)
				{
					delta = anVehicleAndGunDelta[j];
					for (NextPos = asPiece[i].nPos + delta; anBoard[NextPos] == 0 && anInBoard[NextPos]; NextPos += delta)
						AddMove(pmv, aLen, CurPos, NextPos);
					if (anInBoard[NextPos] && !(anIndex[NextPos] >= l && anIndex[NextPos] < r))
						AddMove(pmv, aLen, CurPos, NextPos);
				}
				break;
			case GUN:
				for (int j = 0; j < 4; ++j)
				{
					delta = anVehicleAndGunDelta[j];
					for (NextPos = asPiece[i].nPos + delta; anBoard[NextPos] == 0 && anInBoard[NextPos]; NextPos += delta)
						AddMove(pmv, aLen, CurPos, NextPos);
					for (NextPos += delta; anBoard[NextPos] == 0 && anInBoard[NextPos]; NextPos += delta);	//跳过空格子
					//此处if不是for语句的子句
					if (anInBoard[NextPos] && !(anIndex[NextPos] >= l && anIndex[NextPos] < r))
						AddMove(pmv, aLen, CurPos, NextPos);
				}
				break;
			case SOLDIER:
				int len;
				if (Home(CurPos,nPlayer))
					len = 1;
				else
					len = 3;
				for (int j = 0; j < len; ++j)
				{
					NextPos = CurPos + anSoldierMove[nPlayer][j];
					if (anInBoard[NextPos] && !(anIndex[NextPos] >= l && anIndex[NextPos] < r))
						AddMove(pmv, aLen, CurPos, NextPos);
				}
				break;
			default:
				break;
			}
		}
	}
}

inline long AddValue(int NextPos)
{
	if (anIndex[NextPos] == -1)
		return 1;
	else
		return anValue[anBoard[NextPos]] >> WEIGHT;
}

long ValueSelf(int player)
{
	int CurPos, NextPos, l, r, type, delta;
	long res = 0;
	l = 0 + 16 * player;
	r = l + 16;
	for (int i = l; i < r; ++i)
	if (asPiece[i].nPos)	//确认棋子在棋盘上
	{
		res += anValue[asPiece[i].nType];
		CurPos = asPiece[i].nPos;
		type = asPiece[i].nType & TYPEMASK;
		if ((type) > 0 && (type) < 5)
		{
			for (int j = 0; j < LenOFMove[type]; ++j)
			{
				NextPos = CurPos + anMove[type][j];
				//检查是否在棋盘内且落点无己方棋子
				if (anInBoard[NextPos] && !(anIndex[NextPos] >= l && anIndex[NextPos] < r))
				{
					switch (type)
					{
					case GENERAL:
					case GUARDER:
						if (anInFort[NextPos])
							res += AddValue(NextPos);
						break;
					case ELEPHANT:
						if (NotHome(NextPos, nPlayer))
							break;
					case HORSE:
						if (!anBoard[CurPos + anLim[type][j]])
							res += AddValue(NextPos);
						break;
					default:
						break;
					}
				}
			}
		}
		else
		{
			switch (type)
			{
			case VEHICLE:
				for (int j = 0; j < 4; ++j)
				{
					delta = anVehicleAndGunDelta[j];
					for (NextPos = asPiece[i].nPos + delta; anBoard[NextPos] == 0 && anInBoard[NextPos]; NextPos += delta)
						++res;
					if (anInBoard[NextPos] && !(anIndex[NextPos] >= l && anIndex[NextPos] < r))
						res += (anValue[anBoard[NextPos]] >> WEIGHT);
				}
				break;
			case GUN:	//注意和生成着法的区别
				for (int j = 0; j < 4; ++j)
				{
					delta = anVehicleAndGunDelta[j];
					for (NextPos = asPiece[i].nPos + delta; anBoard[NextPos] == 0 && anInBoard[NextPos]; NextPos += delta);	//跳过空格子
					//此处for不是for语句的子句
					for (NextPos += delta; anBoard[NextPos] == 0 && anInBoard[NextPos]; NextPos += delta)
						++res;
					if (anInBoard[NextPos] && !(anIndex[NextPos] >= l && anIndex[NextPos] < r))
						res += (anValue[anBoard[NextPos]] >> WEIGHT);
				}
				break;
			case SOLDIER:
				int len;
				if (Home(CurPos, nPlayer))
					len = 1;
				else
					len = 3;
				for (int j = 0; j < len; ++j)
				{
					NextPos = CurPos + anSoldierMove[nPlayer][j];
					if (anInBoard[NextPos] && !(anIndex[NextPos] >= l && anIndex[NextPos] < r))
						res += AddValue(NextPos);
				}
				break;
			default:
				break;
			}
		}
	}
	return res;
}

inline long CalcValue()
{
	return ValueSelf(nPlayer) - ValueSelf(!nPlayer);
}

long long g_cnt;

long quiesce(int alpha, int beta)
{
	MOVEMENT killmv[16], capmv[64], normv[128];
	//MOVEMENT mvbest = 0;
	MOVEMENT *ptr[3] = { killmv, capmv, normv };
	int aLen[3], tmp, tmpIndex;
	//int flag = HASH_ALPHA;
	long val;
	++g_cnt;
	/*
	if ((val = ProbeHash(depth, alpha, beta)) != VALUNKNOWN)
		return val;
	
	if (depth == 0)
	{
		val = CalcValue();
		RecordHash(depth, val, HASH_EXACT);
		return val;
	}
	*/
	val = CalcValue();
	if (val >= beta)
		return beta;
	if (val > alpha)
		alpha = val;
	CreateMove(ptr, aLen);
	if (aLen[0] || FaceToFace())	//如果本方有杀将着法或将对脸则返回无穷大
	{
		//RecordHash(depth, M_INFINITY, HASH_EXACT);
		return M_INFINITY;
	}
	//SortByHis(amv, nLen);
	for (int j = 0; j < aLen[1]; ++j)
	{
		Move(ptr[1][j], tmp, tmpIndex);
		val = -quiesce(-beta, -alpha);
		//if (val == 15 && f) { print(); f = 0; }
		Cancel(ptr[1][j], tmp, tmpIndex);
		if (val >= beta)
		{
			//nHistory[amv[i]] += depth*depth;
			//RecordHash(depth, val, HASH_BETA);
			return beta;
		}
		if (val > alpha)
		{
			//mvbest = ptr[i][j];
			//flag = HASH_EXACT;
			alpha = val;
		}
	}
	//if (mvbest)
	//	nHistory[mvbest] += depth*depth;
	//RecordHash(depth, alpha, flag);
	return alpha;
}

long alphabeta(int depth, int alpha, int beta)
{
	MOVEMENT killmv[16], capmv[64], normv[128];
	//MOVEMENT mvbest = 0;
	MOVEMENT *ptr[3] = { killmv, capmv, normv };
	int aLen[3], tmp, tmpIndex, flag = HASH_ALPHA;
	long val;
	++g_cnt;
	if ((val = ProbeHash(depth, alpha, beta)) != VALUNKNOWN)
		return val;
	if (depth == 0)
	{
		val = quiesce(alpha, beta);	//因为本方未行棋，仍然是本方局面，所以不用取相反数，也不用颠倒alpha, beta
		RecordHash(depth, val, HASH_EXACT);
		return val;
	}
	CreateMove(ptr,aLen);
	if (aLen[0] || FaceToFace())	//如果本方有杀将着法或将对脸则返回无穷大
	{
		RecordHash(depth, M_INFINITY, HASH_EXACT);
		return M_INFINITY;
	}
	//SortByHis(amv, nLen);
	for (int i = 0; i < 3; ++i)
	for (int j = 0; j < aLen[i]; ++j)
	{
		Move(ptr[i][j], tmp, tmpIndex);
		val = -alphabeta(depth - 1, -beta, -alpha);
		//if (val == 15 && f) { print(); f = 0; }
		Cancel(ptr[i][j], tmp, tmpIndex);
		if (val >= beta)
		{
			//nHistory[amv[i]] += depth*depth;
			RecordHash(depth, val, HASH_BETA);
			return beta;
		}
		if (val > alpha)
		{
			//mvbest = ptr[i][j];
			flag = HASH_EXACT;
			alpha = val;
		}
	}
	//if (mvbest)
	//	nHistory[mvbest] += depth*depth;
	RecordHash(depth, alpha, flag);
	return alpha;
}

MOVEMENT rootsearch(int depth,long &lastmax,MOVEMENT lastmove,int maxtime)
{
	MOVEMENT res;
	MOVEMENT killmv[16], capmv[64], normv[128];
	MOVEMENT *ptr[3] = { killmv, capmv, normv };
	int aLen[3], tmp, tmpIndex;
	long val;
	long max = -M_INFINITY;
	nPlayer = 0;
	CreateMove(ptr,aLen);
	//SortByHis(amv, nLen);
	if (aLen[0] || FaceToFace()) //如果本方有杀将着法或将对脸则返回
	{
		RecordHash(depth, M_INFINITY, HASH_EXACT);
		return ptr[0][0];
	}
	for (int i = 0; i < 3; ++i)
	for (int j = 0; j <aLen[i]; ++j)
	{
		Move(ptr[i][j], tmp, tmpIndex);
		val = -alphabeta(depth - 1, -M_INFINITY, M_INFINITY);
		Cancel(ptr[i][j], tmp, tmpIndex);
		if (val > max)
		{
			max = val;
			res = ptr[i][j];
		}
		if (clock() - tTime > maxtime*CLOCKS_PER_SEC)
		{
			if (max > lastmax)
				return res;
			else
				return lastmove;
		}
	}
	//nHistory[res] += depth*depth;
	RecordHash(depth, max, HASH_EXACT);
	lastmax = max;
	return res;
}

void Search(PVOID pVoid)
{
	tTime = clock();
	PPARAMS pParams = (PPARAMS)pVoid;
	int deep;
	long lastmax= -M_INFINITY;
	MOVEMENT res=0;
	pParams->state = RUNNING;
	g_cnt = 0;
	//memset(nHistory, 0, sizeof nHistory);
	for (int i = 1; i <= pParams->maxdepth; ++i)
	{
		//g_cnt = 0;
		res = rootsearch(i, lastmax, res, pParams->maxtime);
		deep = i;
		
		std::cout << "Calc depth:"<<i<<' '<<g_cnt << std::endl;
		if (clock() - tTime > pParams->maxtime*CLOCKS_PER_SEC)
			break;
		
	}
	
	std::cout << "depth:" << deep << " time:" << (clock() - tTime) / CLOCKS_PER_SEC << " cnt:" << g_cnt << std::endl;
	pParams->res = res;
	pParams->state = FINISHED;
	SendMessage(pParams->hwnd, WM_FINISHED, 0, 0);
	_endthread();
}

long backalphabeta(int depth,int alpha, int beta)
{
	MOVEMENT killmv[16], capmv[64], normv[128];
	//MOVEMENT mvbest = 0;
	MOVEMENT *ptr[3] = { killmv, capmv, normv };
	int aLen[3], tmp, tmpIndex, flag = HASH_ALPHA;
	long val;
	if (g_bStop)
		return VALUNKNOWN;
	++g_cnt;
	if ((val = ProbeHash(depth, alpha, beta)) != VALUNKNOWN)
		return val;
	if (depth == 0)
	{
		val = quiesce(alpha, beta);	//因为本方未行棋，仍然是本方局面，所以不用取相反数，也不用颠倒alpha, beta
		RecordHash(depth, val, HASH_EXACT);
		return val;
	}
	CreateMove(ptr, aLen);
	if (aLen[0] || FaceToFace())	//如果本方有杀将着法或将对脸则返回无穷大
	{
		RecordHash(depth, M_INFINITY, HASH_EXACT);
		return M_INFINITY;
	}
	//SortByHis(amv, nLen);
	for (int i = 0; i < 3; ++i)
	for (int j = 0; j < aLen[i]; ++j)
	{
		Move(ptr[i][j], tmp, tmpIndex);
		val = -backalphabeta(depth - 1, -beta, -alpha);
		//if (val == 15 && f) { print(); f = 0; }
		Cancel(ptr[i][j], tmp, tmpIndex);
		if (val == VALUNKNOWN || g_bStop)
			return VALUNKNOWN;
		if (val >= beta)
		{
			//nHistory[amv[i]] += depth*depth;
			RecordHash(depth, val, HASH_BETA);
			return beta;
		}
		if (val > alpha)
		{
			//mvbest = ptr[i][j];
			flag = HASH_EXACT;
			alpha = val;
		}
	}
	//if (mvbest)
	//	nHistory[mvbest] += depth*depth;
	RecordHash(depth, alpha, flag);
	return alpha;
}

void backsearch(PVOID pVoid)
{
	g_nBackState = RUNNING;
	g_cnt = 0;
	nPlayer = 1;
	for (int i = 1; i <= BACK_MAX_DEPTH; ++i)
	{
		if (g_bStop)
			break;
		backalphabeta(i, -M_INFINITY, M_INFINITY);
		std::cout << "Calc (back) depth:" << i << ' ' << g_cnt << std::endl;
	}
	g_nBackState = FINISHED;
	_endthread();
}

