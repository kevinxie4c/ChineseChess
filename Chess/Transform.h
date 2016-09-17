#ifndef TRANSFORM_H
#define TRANSFORM_H

void TransStdIndex(int dst[256], const int src[10][9]);
void TransIndexStd(int dst[10][9], const int src[256]);
void StdToIndex(int &n, const int i, const int j);
void IndexToStd(int &i, int &j, const int n);

#endif