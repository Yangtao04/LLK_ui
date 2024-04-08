#include "CGameMap.h"
#include "Global.h"
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QImage>
#include <iostream>
using namespace std;
int** InitMap(){
    //获取地图大小和花色
    int Rows = 10;    //行
    int Cols = 16;    //列
    int PicNum = 20;  //花色数
    //开辟内存空间
    int **GameMap = new(nothrow) int*[Rows];
    if(GameMap == nullptr){
        cout<<"内存操作异常!"<<endl;
    }else{
        for(int i=0;i<Rows;i++){
            GameMap[i] = new(nothrow) int[Cols];
            if(GameMap[i] == nullptr){
                cout<<"内存操作异常!"<<endl;
            }
            memset(GameMap[i],0,sizeof(int)*Cols);
        }
    }
    if((Rows*Cols)%(PicNum*2)!=0){
        ReleaseMap(GameMap);
        cout<<"游戏花色与地图大小不匹配"<<endl;
    }
    int RepeatNum = Rows*Cols/PicNum;
    int Count = 0;
    for(int i=0;i<PicNum;i++){
        for(int j=0;j<RepeatNum;j++){
            GameMap[Count/Cols][Count%Cols] = i;
            Count++;
        }
    }
    //设置种子
    srand((int) time(nullptr));
    //随机交换两个数字
    int VertexNum = Rows * Cols;
    for(int i=0;i<VertexNum;i++){
        //随机得到两个坐标
        int index1 = rand()%VertexNum;
        int index2 = rand()%VertexNum;
        //交换两个数值
        int temp = GameMap[index1/Cols][index1%Cols];
        GameMap[index1/Cols][index1%Cols] = GameMap[index2/Cols][index2%Cols];
        GameMap[index2/Cols][index2%Cols] = temp;
    }
    return GameMap;
}//初始化地图

void ReleaseMap(int **GameMap){
    for (int i = 0; i <10;i++) {
        delete[]GameMap[i];
    }
    delete[]GameMap;
}//释放地图空间

QImage picture(const QString& string){
    QImage image(string);
    image = image.convertToFormat(QImage::Format_ARGB32);
    //遍历每个像素点
    for (int i = 0; i < image.height(); i++){
        for (int j = 0; j < image.width(); j++){
            QRgb rgb = image.pixel(j, i);
            if (rgb <= 0xFFFFFFFF&&rgb >=0xFFFF00F0)  //如果符合背景色
            {
                image.setPixel(j, i, qRgba(0, 0, 0, 0));//此像素设置为透明
            }
        }
    }
    return image;
}//图片处理

bool IsLink(int **Map, Vertex v1, Vertex v2) {
    if((v1.row==v2.row)&&(v1.col==v2.col))return false;
    //X直连方式
    if(v1.row==v2.row){
        if(v1.row==0||v1.row==9)return true;
        return LinkRow(Map,v1,v2)|| TwoCorner(Map,v1,v2);
    }
    //Y直连方式
    if(v1.col==v2.col){
        if(v1.col==0||v1.col==15)return true;
        return LinkCol(Map,v1,v2)|| TwoCorner(Map,v1,v2);
    }
    if(v1.row!=v2.row&&v1.col!=v2.col){
        return OneCorner(Map,v1,v2)|| TwoCorner(Map,v1,v2);
    }
    return false;
}//判断是否连接

bool LinkRow(int **Map, Vertex v1, Vertex v2) {
    int Col1 = v1.col;
    int Col2 = v2.col;
    int Row = v1.row;
    //保证Col1的值小于Col2
    if(Col1>Col2){
        int temp = Col1;
        Col1 = Col2;
        Col2 = temp;
    }
    //直通
    for(int i=Col1+1;i<=Col2;i++){
        if(i==Col2)return true;
        if(Map[Row][i]!=-1)break;
    }
    return false;
}//直线判断行

bool LinkCol(int **Map, Vertex v1, Vertex v2) {
    int Row1 = v1.row;
    int Row2 = v2.row;
    int Col = v1.col;
    //保证Row1的值小于Row2
    if(Row1>Row2){
        int temp = Row1;
        Row1 = Row2;
        Row2 = temp;
    }
    //直通
    for(int i=Row1+1;i<=Row2;i++){
        if(i==Row2)return true;
        if(Map[i][Col]!=-1)break;
    }
    return false;
}//直线判断列

bool LineX(int **Map, int Row, int Col1, int Col2) {
    //保证Col1的值小于Col2
    if(Col1>Col2){
        int temp = Col1;
        Col1 = Col2;
        Col2 = temp;
    }
    //直通
    for(int i=Col1+1;i<=Col2;i++){
        if(i==Col2)return true;
        if(Map[Row][i]!=-1)break;
    }
    return false;
}//判断横向是否可以连接

bool LineY(int **Map, int Col, int Row1, int Row2) {
    //保证Row1的值小于Row2
    if(Row1>Row2){
        int temp = Row1;
        Row1 = Row2;
        Row2 = temp;
    }
    //直通
    for(int i=Row1+1;i<=Row2;i++){
        if(i==Row2)return true;
        if(Map[i][Col]!=-1)break;
    }
    return false;
}//判断纵向是否可以连接

bool OneCorner(int **Map, Vertex v1, Vertex v2) {
    if(Map[v1.row][v2.col]==-1){
        if(LineY(Map,v2.col,v1.row,v2.row)&& LineX(Map,v1.row,v1.col,v2.col)){
            return true;
        }
        if((v1.row==0||v1.row==9)&& LineY(Map,v2.col,v1.row,v2.row)){
            return true;
        }
        if((v2.col==0||v2.col==15)&& LineX(Map,v1.row,v1.col,v2.col)){
            return true;
        }
    }
    if(Map[v2.row][v1.col]==-1){
        if(LineY(Map,v1.col,v1.row,v2.row)&& LineX(Map,v2.row,v1.col,v2.col)){
            return true;
        }
        if((v2.row==0||v2.row==9)&& LineY(Map,v1.col,v1.row,v2.row)){
            return true;
        }
        if((v1.col==0||v1.col==15)&& LineX(Map,v2.row,v1.col,v2.col)){
            return true;
        }
    }
    return false;
}//判断一个拐点连接

bool TwoCorner(int **Map, Vertex v1, Vertex v2) {
    for(int Col=0;Col<16;Col++){
        if(Map[v1.row][Col]==-1&&Map[v2.row][Col]==-1){
            if(LineY(Map,Col,v1.row,v2.row)||Col==0||Col==15){
                if(LineX(Map,v1.row,v1.col,Col)&& LineX(Map,v2.row,v2.col,Col)){
                    return true;
                }
            }
        }
    }
    for(int Row=0;Row<10;Row++){
        if(Map[Row][v1.col]==-1&&Map[Row][v2.col]==-1){
            if(LineX(Map,Row,v1.col,v2.col)||Row==0||Row==9){
                if(LineY(Map,v1.col,v1.row,Row)&&LineY(Map,v2.col,v2.row,Row)){
                    return true;
                }
            }
        }
    }
    return false;
}//判断两个拐点连接

int **reset(int **Map) {
    //获取地图大小和花色
    int Rows = 10;    //行
    int Cols = 16;    //列
    //设置种子
    srand((int) time(nullptr));
    //随机交换两个数字
    int VertexNum = Rows * Cols;
    for(int i=0;i<VertexNum;i++){
        //随机得到两个坐标
        int index1 = rand()%VertexNum;
        int index2 = rand()%VertexNum;
        //交换两个数值
        if(Map[index1/Cols][index1%Cols]!=-1&&Map[index2/Cols][index2%Cols]!=-1){
            int temp = Map[index1/Cols][index1%Cols];
            Map[index1/Cols][index1%Cols] = Map[index2/Cols][index2%Cols];
            Map[index2/Cols][index2%Cols] = temp;
        }
    }
    return Map;
}//重新设置地图

bool gameOver(int **Map) {
    //获取地图大小和花色
    int Rows = 10;    //行
    int Cols = 16;    //列
    for(int i=0;i<Rows;i++){
        for(int j=0;j<Cols;j++){
            if(Map[i][j]!=-1){return false;}
        }
    }
    return true;
}

Vertex *vertex(int **Map) {
    Vertex *vertex1;
    for(int i=0;i<10;i++){
        for(int j=0;j<16;j++){
            if(Map[i][j]!=-1){
                vertex1[0].row=i;vertex1[0].col=j;vertex1[0].info=Map[i][j];
                for(int n=i;n<10;n++){
                    for(int m=j;m<16;m++){
                        if(Map[n][m]==vertex1[0].info){
                            vertex1[1].row=n;vertex1[1].col=m;vertex1[1].info=Map[n][m];
                            if(IsLink(Map,vertex1[0],vertex1[1])){
                                return vertex1;
                            }
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}







