#include "ChessRule.h"
#include <cmath>

bool IsLegal(int nType,int board[10][9], int sx, int sy, int dx, int dy)
{
	switch (nType & TYPEMASK)
	{
	case GENERAL:
		if (dx < 3 || dx > 5 || dy < 7 || (dx != sx && dy != sy) || abs(dx - sx) > 1 || abs(dy - sy) > 1) return false;
		else return true;
	case GUARDER:
		if (dx < 3 || dx > 5 || dy < 7 ||  abs(dx - sx) != 1 || abs(dy - sy) != 1) return false;
		else return true;
	case ELEPHANT:
		if (dy < 5 || abs(dx - sx) != 2 || abs(dy - sy) != 2 || board[(sy + dy) / 2][(sx + dx) / 2]) return false;
		else return true;
	case HORSE:
		//利用位运算减少运算量
	{
				  int a = abs(dx - sx);
				  int b = abs(dy - sy);
				  if ((a | b) != 3 || (a & b) || board[AveAdjust(sy, dy)][AveAdjust(sx, dx)]) return false;
				  else return true;
	}
	case VEHICLE:
		if (dx == sx)
		{
			int tbegin, tend;
			if (dy > sy) { tbegin = sy; tend = dy; }
			else { tbegin = dy; tend = sy; }
			for (int i = tbegin + 1; i < tend; ++i)
			if (board[i][sx]) return false;
			return true;
		}
		else
		if (dy == sy)
		{
			int tbegin, tend;
			if (dx > sx) { tbegin = sx; tend = dx; }
			else { tbegin = dx; tend = sx; }
			for (int i = tbegin + 1; i < tend; ++i)
			if (board[sy][i]) return false;
			return true;
		}
		else return false;
	case GUN:
		if (dx == sx)
		{
			int tbegin, tend;
			if (dy > sy) { tbegin = sy; tend = dy; }
			else { tbegin = dy; tend = sy; }
			bool once = false, more = false;
			for (int i = tbegin + 1; i < tend; ++i)
			if (board[i][sx])
			{
				if (once) { more = true; break; }
				else once = true;
			}
			if ((board[dy][dx] && once && !more) || (!board[dy][dx] && !once)) return true;
			else return false;
		}
		else
		if (dy == sy)
		{
			int tbegin, tend;
			if (dx > sx) { tbegin = sx; tend = dx; }
			else { tbegin = dx; tend = sx; }
			bool once = false, more = false;			
			for (int i = tbegin + 1; i < tend; ++i)
			if (board[sy][i])
			{
				if (once) { more = true; break; }
				else once = true;
			}
			if ((board[dy][dx] && once && !more) || (!board[dy][dx] && !once)) return true;
			else return false;
		}
		else return false;
	case SOLDIER:
		if (dy < 5)
		{
			if ((dx != sx && dy != sy) || sy - dy > 1 || sy - dy < 0 || abs(dx - sx)>1) return false;
			else return true;
		}
		else
		if (dx != sx || sy - dy != 1) return false;
		else return true;
	default:
		return false;
	}
}

inline int AveAdjust(int src, int dest)
{
	if (abs(dest - src) == 2) return (dest + src)/2;
	else return src;
}