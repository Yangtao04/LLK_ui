#include "helpscene.h"
#include <QIcon>
#include <QPainter>
#include <QLabel>
#include <QScrollArea>

helpScene::helpScene(QWidget *parent)
    : QWidget{parent}
{
    //窗口标题
    this->setWindowTitle("帮助");
    this->setWindowIcon(QIcon(":lianliankan/LLK1.png"));
    //设置背景图片
    this->resize(450,360);
    auto *bkg = new QLabel(this);
    bkg->setPixmap(QPixmap(":lianliankan/basic_help.bmp"));
    bkg->resize(this->size());
    auto *scroll = new QScrollArea(this);
    scroll->setWidget(bkg);
    scroll->setWidgetResizable(true);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

