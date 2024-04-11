#include "CGameGraph.h"
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QImage>
AMGraph *InitMap(){
    AMGraph *GraphMap = new AMGraph;
    //获取地图大小和花色
    int Rows = 10;    //行
    int Cols = 16;    //列
    int PicNum = 20;  //花色数
    if((Rows*Cols)%(PicNum*2)!=0){
        //ReleaseMap(GameMap);
        cout<<"游戏花色与地图大小不匹配"<<endl;
    }
    int RepeatNum = Rows*Cols/PicNum;
    int Count = 0;
    for(int i=0;i<PicNum;i++){
        for(int j=0;j<RepeatNum;j++){
            //为图顶点的行、列、信息赋值
            GraphMap->vex[Count].row = Count/Cols;//顶点的行
            GraphMap->vex[Count].col = Count%Cols;//顶点的列
            GraphMap->vex[Count].info = i;        //顶点的信息
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
        //交换图的顶点信息
        int Temp = GraphMap->vex[index1].info;
        GraphMap->vex[index1].info = GraphMap->vex[index2].info;
        GraphMap->vex[index2].info = Temp;
    }
    return GraphMap;
}//初始化地图

QImage picture1(const QString& string){
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

bool IsLink(AMGraph* GraphMap, Vertex v1, Vertex v2) {
    if((v1.row==v2.row)&&(v1.col==v2.col))return false;
    //X直连方式
    if(v1.row==v2.row){
        if(v1.row==0||v1.row==9)return true;
        return LinkRow(GraphMap,v1,v2)|| TwoCorner(GraphMap,v1,v2);
    }
    //Y直连方式
    if(v1.col==v2.col){
        if(v1.col==0||v1.col==15)return true;
        return LinkCol(GraphMap,v1,v2)|| TwoCorner(GraphMap,v1,v2);
    }
    if(v1.row!=v2.row&&v1.col!=v2.col){
        return OneCorner(GraphMap,v1,v2)|| TwoCorner(GraphMap,v1,v2);
    }
    return false;
}//判断是否连接

bool LinkRow(AMGraph* GraphMap, Vertex v1, Vertex v2) {
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
        if(GraphMap->vex[Row*16+i].info!=-1)break;
    }
    return false;
}//直线判断行

bool LinkCol(AMGraph* GraphMap, Vertex v1, Vertex v2) {
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
        if(GraphMap->vex[i*16+Col].info!=-1)break;
    }
    return false;
}//直线判断列

bool LineX(AMGraph* GraphMap, int Row, int Col1, int Col2) {
    //保证Col1的值小于Col2
    if(Col1>Col2){
        int temp = Col1;
        Col1 = Col2;
        Col2 = temp;
    }
    //直通
    for(int i=Col1+1;i<=Col2;i++){
        if(i==Col2)return true;
        if(GraphMap->vex[Row*16+i].info!=-1)break;
    }
    return false;
}//判断横向是否可以连接

bool LineY(AMGraph* GraphMap, int Col, int Row1, int Row2) {
    //保证Row1的值小于Row2
    if(Row1>Row2){
        int temp = Row1;
        Row1 = Row2;
        Row2 = temp;
    }
    //直通
    for(int i=Row1+1;i<=Row2;i++){
        if(i==Row2)return true;
        if(GraphMap->vex[i*16+Col].info!=-1)break;
    }
    return false;
}//判断纵向是否可以连接

bool OneCorner(AMGraph* GraphMap, Vertex v1, Vertex v2) {
    if(GraphMap->vex[v1.row*16+v2.col].info==-1){
        if(LineY(GraphMap,v2.col,v1.row,v2.row)&& LineX(GraphMap,v1.row,v1.col,v2.col)){
            return true;
        }
        if((v1.row==0||v1.row==9)&& LineY(GraphMap,v2.col,v1.row,v2.row)){
            return true;
        }
        if((v2.col==0||v2.col==15)&& LineX(GraphMap,v1.row,v1.col,v2.col)){
            return true;
        }
    }
    if(GraphMap->vex[v2.row*16+v1.col].info==-1){
        if(LineY(GraphMap,v1.col,v1.row,v2.row)&& LineX(GraphMap,v2.row,v1.col,v2.col)){
            return true;
        }
        if((v2.row==0||v2.row==9)&& LineY(GraphMap,v1.col,v1.row,v2.row)){
            return true;
        }
        if((v1.col==0||v1.col==15)&& LineX(GraphMap,v2.row,v1.col,v2.col)){
            return true;
        }
    }
    return false;
}//判断一个拐点连接

bool TwoCorner(AMGraph* GraphMap, Vertex v1, Vertex v2) {
    for(int Col=0;Col<16;Col++){
        if(GraphMap->vex[v1.row*16+Col].info==-1&&GraphMap->vex[v2.row*16+Col].info==-1){
            if(LineY(GraphMap,Col,v1.row,v2.row)||Col==0||Col==15){
                if(LineX(GraphMap,v1.row,v1.col,Col)&& LineX(GraphMap,v2.row,v2.col,Col)){
                    return true;
                }
            }
        }
    }
    for(int Row=0;Row<10;Row++){
        if(GraphMap->vex[Row*16+v1.col].info==-1&&GraphMap->vex[Row*16+v2.col].info==-1){
            if(LineX(GraphMap,Row,v1.col,v2.col)||Row==0||Row==9){
                if(LineY(GraphMap,v1.col,v1.row,Row)&&LineY(GraphMap,v2.col,v2.row,Row)){
                    return true;
                }
            }
        }
    }
    return false;
}//判断两个拐点连接

AMGraph *reset(AMGraph* GraphMap) {
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
        if(GraphMap->vex[index1].info!=-1&&GraphMap->vex[index2].info!=-1){
            int temp = GraphMap->vex[index1].info;
            GraphMap->vex[index1].info = GraphMap->vex[index2].info;
            GraphMap->vex[index2].info = temp;
        }
    }
    return GraphMap;
}//重新设置地图

bool gameOver(AMGraph* GraphMap) {
    //获取地图大小和花色
    int Rows = 10;    //行
    int Cols = 16;    //列
    for(int i=0;i<Rows;i++){
        for(int j=0;j<Cols;j++){
            if(GraphMap->vex[i*16+j].info!=-1){return false;}
        }
    }
    return true;
}

Vertex *vertex(AMGraph* GraphMap) {
    Vertex *vertex1;
    for(int i=0;i<10;i++){
        for(int j=0;j<16;j++){
            if(GraphMap->vex[i*16+j].info!=-1){
                vertex1[0].row=i;vertex1[0].col=j;vertex1[0].info=GraphMap->vex[i*16+j].info;
                for(int n=i;n<10;n++){
                    for(int m=j;m<16;m++){
                        if(GraphMap->vex[n*16+m].info==vertex1[0].info){
                            vertex1[1].row=n;vertex1[1].col=m;vertex1[1].info=GraphMap->vex[n*16+m].info;
                            if(IsLink(GraphMap,vertex1[0],vertex1[1])){
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

AMGraph *GraphLink(AMGraph *GraphMap) {
    for(int i=0;i<160;i++){
        for(int j=0;j<160;j++){
            if(IsLink(GraphMap,GraphMap->vex[i],GraphMap->vex[j])){
                GraphMap->GraphMap[i][j] =1;
            } else{
                GraphMap->GraphMap[i][j] =0;
            }
        }
    }
    return GraphMap;
}
