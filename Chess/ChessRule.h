#ifndef CHESSRULE_H
#define CHESSRULE_H

#define GENERAL		0x01
#define GUARDER		0x02
#define ELEPHANT	0x03
#define HORSE		0x04
#define VEHICLE		0x05
#define GUN			0x06
#define SOLDIER		0x07
#define BLACK		0x00
#define RED			0x08
#define TYPEMASK	0x07
#define COLORMASK	0x08
#define XMAX		9
#define YMAX		10

bool IsLegal(int nType, int board[10][9], int sx, int sy, int dx, int dy);
inline int AveAdjust(int src, int dest);

#endif