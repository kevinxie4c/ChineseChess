#include "Transform.h"

void StdToIndex(int &n, const int i, const int j)
{
	n = ((i +3) << 4) + j + 3;
}

void IndexToStd(int &i, int &j, const int n)
{
	i = (n >> 4) - 3;
	j = (n & 0x0f) - 3;
}

void TransStdIndex(int dst[256], const int src[10][9])
{
	for (int i = 0; i < 10; ++i)
	for (int j = 0; j < 9; ++j)
	{
		int index;
		StdToIndex(index, i, j);
		dst[index] = src[i][j];
	}
}

void TransIndexStd(int dst[10][9], const int src[256])
{
	for (int i = 0; i < 10; ++i)
	for (int j = 0; j < 9; ++j)
	{
		int index;
		StdToIndex(index, i, j);
		dst[i][j] = src[index];
	}
}