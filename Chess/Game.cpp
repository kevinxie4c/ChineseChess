#include "Game.h"
#include "ChessRule.h"
#include "Windows.h"
#include "resource.h"
#include "AI.h"
#include "Transform.h"
#include "Zobrist.h"
#include <consoleapi.h>
#include <cstdio>
#include <algorithm>
#include "process.h"

#pragma comment(lib,"msimg32.lib")

bool g_bLeftDown, g_bPlayer;
int g_nGameState;
int g_xMouse, g_yMouse, g_xSelected, g_ySelected;
HANDLE hback;
extern FILE *pf;

PARAMS params;

HBITMAP hBoard, hSelected, ahBitmap[16];

int anBoard[256] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 5, 4, 3, 2, 1, 2, 3, 4, 5, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0,
	0, 0, 0, 7, 0, 7, 0, 7, 0, 7, 0, 7, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0xf, 0, 0xf, 0, 0xf, 0, 0xf, 0, 0xf, 0, 0, 0, 0,
	0, 0, 0, 0, 0xe, 0, 0, 0, 0, 0, 0xe, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0xd, 0xc, 0xb, 0xa, 0x9, 0xa, 0xb, 0xc, 0xd, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
int anBoard[256] =
{
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 4, 3, 2, 1, 2, 3, 4, 5, 0, 0, 0, 0,
 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 7, 0, 7, 0, 7, 0, 7, 0, 7, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0,15, 0,15, 0,15, 0,15, 0,15, 0, 0, 0, 0,
 0, 0, 0, 0, 0,12, 0,14, 0, 0,14,12, 0, 0, 0, 0,
 0, 0, 0, 0,13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,11,10, 9,10,11, 0,13, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};*/


const int maxrow = 16;
const int maxcol = 16;
int anStd[10][9];

int anIndex[256];
SChessPiece asPiece[32];

int anPriority[16] = {
	0, 0x00, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0001,
	0, 0x00, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0001,
};

SZobrist ZobristTable[16][256];
/*
void print()
{
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
			fprintf(pf, "%2d,", anBoard[i * 16 + j]);
		fprintf(pf, "\n");
	}
	fprintf(pf, "\n");
}*/

bool comp(const SChessPiece &a, const SChessPiece &b)
{
	return a.nPriority > b.nPriority;
}

void SortByPriority()
{
	std::sort(asPiece, asPiece + 16, comp);
	std::sort(asPiece + 16, asPiece + 32, comp);
	for (int i = 0; i < 256; ++i)
		anIndex[i] = -1;
	for (int i = 0; i < 32; ++i)
	if (asPiece[i].nPos)
		anIndex[asPiece[i].nPos] = i;
}

void GameInit(HINSTANCE hInstance)
{
	if (g_nGameState != STATE_INIT) return;
	g_bLeftDown = false;
	//g_bPlayer = false;
	g_xSelected = g_ySelected = -1;
	hBoard = LoadBitmap(hInstance, MAKEINTRESOURCE(BOARD));
	hSelected = LoadBitmap(hInstance, MAKEINTRESOURCE(SELECTED));
	ahBitmap[BLACK | GENERAL ] = LoadBitmap(hInstance, MAKEINTRESOURCE(BGENERAL));
	ahBitmap[BLACK | GUARDER ] = LoadBitmap(hInstance, MAKEINTRESOURCE(BGUARDER));
	ahBitmap[BLACK | ELEPHANT] = LoadBitmap(hInstance, MAKEINTRESOURCE(BELEPHANT));
	ahBitmap[BLACK | HORSE   ] = LoadBitmap(hInstance, MAKEINTRESOURCE(BHORSE));
	ahBitmap[BLACK | VEHICLE ] = LoadBitmap(hInstance, MAKEINTRESOURCE(BVEHICLE));
	ahBitmap[BLACK | GUN     ] = LoadBitmap(hInstance, MAKEINTRESOURCE(BGUN));
	ahBitmap[BLACK | SOLDIER ] = LoadBitmap(hInstance, MAKEINTRESOURCE(BSOLDIER));
	ahBitmap[RED   | GENERAL ] = LoadBitmap(hInstance, MAKEINTRESOURCE(RGENERAL));
	ahBitmap[RED   | GUARDER ] = LoadBitmap(hInstance, MAKEINTRESOURCE(RGUARDER));
	ahBitmap[RED   | ELEPHANT] = LoadBitmap(hInstance, MAKEINTRESOURCE(RELEPHANT));
	ahBitmap[RED   | HORSE   ] = LoadBitmap(hInstance, MAKEINTRESOURCE(RHORSE));
	ahBitmap[RED   | VEHICLE ] = LoadBitmap(hInstance, MAKEINTRESOURCE(RVEHICLE));
	ahBitmap[RED   | GUN     ] = LoadBitmap(hInstance, MAKEINTRESOURCE(RGUN));
	ahBitmap[RED   | SOLDIER ] = LoadBitmap(hInstance, MAKEINTRESOURCE(RSOLDIER));
	for (int i = 0; i < 256; ++i)
		anIndex[i] = -1;
	int bcnt = 0, rcnt = 16;
	for (int i = 0; i < maxrow; ++i)
	for (int j = 0; j < maxcol; ++j)
	if (anBoard[i*maxcol + j])
	{
		if ((anBoard[i*maxcol + j] & COLORMASK) == BLACK)
		{
			asPiece[bcnt].nType = anBoard[i*maxcol + j];
			asPiece[bcnt].nPos = i*maxcol + j;
			anIndex[i*maxcol + j] = bcnt++;
		}
		else
		{
			asPiece[rcnt].nType = anBoard[i*maxcol + j];
			asPiece[rcnt].nPos = i*maxcol + j;
			anIndex[i*maxcol + j] = rcnt++;
		}
	}
	for (int i = 0; i < 32; ++i)
		asPiece[i].nPriority = anPriority[asPiece[i].nType];
	SortByPriority();
	TransIndexStd(anStd, anBoard);
	srand((unsigned)time(NULL));
	for (int i = 0; i < 16; ++i)
	for (int j = 0; j < 256; ++j)
	{
		ZobristTable[i][j].dwKey = rand32();
		ZobristTable[i][j].dwLock1 = rand32();
		ZobristTable[i][j].dwLock2 = rand32();
	}
	for (int j = 0; j < 256; ++j)
	{
		ZobristTable[0][j].dwKey = 0;
		ZobristTable[0][j].dwLock1 = 0;
		ZobristTable[0][j].dwLock2 = 0;
	}
	InitHash();
	params.state = READY;
	params.hwnd = hWnd;
	params.maxdepth = 12;
	params.maxtime = 20;
	g_nBackState = READY;
}

void GameMain(HDC hdc)
{
	if (g_nGameState != STATE_RUNNING) return;
	if (g_bPlayer)
	{
		if (g_nBackState == READY)
		{
			g_bStop = false;
			_beginthread(backsearch, 0, NULL);
		}
		if (g_bLeftDown )
		{
			int x = (g_xMouse - BOARD_EDGE) / SQUARE_SIZE;
			int y = (g_yMouse - BOARD_EDGE) / SQUARE_SIZE;
			if (x < 0 || x > 8 || y < 0 || y > 9)
				g_xSelected = g_ySelected = -1;
			else
			if (!(anStd[y][x] & RED))	//目标位置无红方
			{
				if (g_xSelected != -1 && IsLegal(anStd[g_ySelected][g_xSelected], anStd, g_xSelected, g_ySelected, x, y)
					&& (anStd[g_ySelected][g_xSelected] & RED))	//有选中的位置，走法合法且选中的为红方
				{
					g_bStop = true;
					while (g_nBackState != FINISHED);	//等待后台思考线程结束
					anStd[y][x] = anStd[g_ySelected][g_xSelected];
					anStd[g_ySelected][g_xSelected] = 0;
					//TransStdIndex(anBoard, anStd);
					int dst, src;
					StdToIndex(dst, y, x);
					StdToIndex(src, g_ySelected, g_xSelected);
					MOVEMENT mv;
					int tmp;
					mv = MOV(dst, src);
					Move(mv, tmp, tmp);
					g_xSelected = x;
					g_ySelected = y;
					g_bPlayer = false;
					//print();
				}
				else
					g_xSelected = g_ySelected = -1;
			}
			else
			{
				g_xSelected = x;
				g_ySelected = y;
			}
			DrawBoard(hdc);
		}
	}
	if (!g_bPlayer)
	{
		if (params.state == READY)
			_beginthread(Search, 0, &params);
		else
		if (params.state == FINISHED)
		{
			MOVEMENT mv;
			int tmp;
			mv = params.res;
			Move(mv, tmp, tmp);
			TransIndexStd(anStd, anBoard);
			IndexToStd(g_ySelected, g_xSelected, DST(mv));
			g_bPlayer = true;
			DrawBoard(hdc);
			params.state = READY;
			if (g_nBackState==FINISHED) g_nBackState = READY;
			if (g_nBackState == READY && g_bPlayer)
			{
				g_bStop = false;
				_beginthread(backsearch, 0, NULL);
			}
			//print();
		}
	}
}

void DrawBmp(HDC hdc, HDC hdcMem, int x, int y, HBITMAP hBitmap)
{
	SelectObject(hdcMem, hBitmap);
	TransparentBlt(hdc, x, y, SQUARE_SIZE, SQUARE_SIZE, hdcMem, 0, 0, SQUARE_SIZE, SQUARE_SIZE, MASK_COLOR);
}

void DrawBoard(HDC hdc)
{
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBoard);
	BitBlt(hdc, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, hdcMem, 0, 0, SRCCOPY);
	for (int i = 0; i < 10; ++i)
	for (int j = 0; j <  9; ++j)
	if (anStd[i][j])
		DrawBmp(hdc, hdcMem, BOARD_EDGE + j*SQUARE_SIZE, BOARD_EDGE + i*SQUARE_SIZE, ahBitmap[anStd[i][j]]);
	if (g_xSelected != -1)
		DrawBmp(hdc, hdcMem, BOARD_EDGE + g_xSelected*SQUARE_SIZE, BOARD_EDGE + g_ySelected*SQUARE_SIZE, hSelected);
	DeleteDC(hdcMem);
}