#ifndef LLK_UI_CGAMEMAP_H
#define LLK_UI_CGAMEMAP_H
#include "Global.h"
#include <QMainWindow>
#include <QImage>
#include <QString>
#include <QGridLayout>
int** InitMap();//初始化地图

void ReleaseMap(int **GameMap);//释放地图空间

QImage picture(const QString& string);//图片处理

bool IsLink(int **Map,Vertex v1,Vertex v2);//判断是否连接

bool LinkRow(int **Map,Vertex v1,Vertex v2);//直线判断行
bool LinkCol(int **Map,Vertex v1,Vertex v2);//直线判断列

bool LineX(int **Map,int Row,int Col1,int Col2);//判断横向是否可以连接
bool LineY(int **Map,int Col,int Row1,int Row2);//判断纵向是否可以连接
bool OneCorner(int **Map,Vertex v1,Vertex v2);//判断一个拐点连接

bool TwoCorner(int **Map,Vertex v1,Vertex v2);//判断两个拐点连接

int** reset(int **Map);//重新设置地图

bool gameOver(int **Map);//判断游戏胜利

Vertex *vertex(int **Map);//提示，寻找两个可消元素
#endif //LLK_UI_CGAMEMAP_H
