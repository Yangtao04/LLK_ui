#ifndef LLK_UI_CGAMEGRAPH_H
#define LLK_UI_CGAMEGRAPH_H

#include <QMainWindow>
#include <QImage>
#include <QString>
#include <QGridLayout>
#include "Global.h"
//邻接矩阵
typedef struct {
    Vertex vex[160];//顶点表
    int GraphMap[160][160];//邻接矩阵
    int vexNum;//顶点数目
    int arcNum;//边的数目
}AMGraph;
AMGraph *InitMap();//初始化地图
//void ReleaseMap(int **GameMap);//释放地图空间
QImage picture1(const QString& string);//图片处理

AMGraph *GraphLink(AMGraph *GraphMap);//更新邻接矩阵

bool IsLink(AMGraph *GraphMap,Vertex v1,Vertex v2);//判断是否连接

bool LinkRow(AMGraph *GraphMap,Vertex v1,Vertex v2);//直线判断行
bool LinkCol(AMGraph *GraphMap,Vertex v1,Vertex v2);//直线判断列

bool LineX(AMGraph *GraphMap,int Row,int Col1,int Col2);//判断横向是否可以连接
bool LineY(AMGraph *GraphMap,int Col,int Row1,int Row2);//判断纵向是否可以连接
bool OneCorner(AMGraph *GraphMap,Vertex v1,Vertex v2);//判断一个拐点连接

bool TwoCorner(AMGraph *GraphMap,Vertex v1,Vertex v2);//判断两个拐点连接

AMGraph *reset(AMGraph* GraphMap);//重新设置地图

bool gameOver(AMGraph* GraphMap);//判断游戏胜利

Vertex *vertex(AMGraph* GraphMap);//提示，寻找两个可消元素


#endif //LLK_UI_CGAMEGRAPH_H

