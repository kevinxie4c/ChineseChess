#ifndef GAME_H
#define GAME_H
#include "Windows.h"
#define STATE_INIT		0
#define STATE_RUNNING	1
#define STATE_END		2

extern HWND hWnd;
extern volatile int g_nBackState;
extern bool g_bStop;

const int MASK_COLOR = RGB(0, 255, 0);
const int SQUARE_SIZE = 56;
const int BOARD_EDGE = 8;
const int BOARD_WIDTH = BOARD_EDGE + SQUARE_SIZE * 9 + BOARD_EDGE;
const int BOARD_HEIGHT = BOARD_EDGE + SQUARE_SIZE * 10 + BOARD_EDGE;

typedef struct
{
	int nType;
	int nPriority;
	int nPos;
}SChessPiece;

void GameInit(HINSTANCE hInstance);
void GameMain(HDC hdc);
void DrawBmp(HDC hdc, HDC hdcMem, int x, int y, HBITMAP hBitmap);
void DrawBoard(HDC hdc);
void print();

#endif
