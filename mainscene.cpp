#include "mainscene.h"
#include "./ui_mainscene.h"
#include <QIcon>
#include <QPainter>
#include <QLabel>
#include <QTimer>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    //设置固定大小，标题，窗口图标
    this->setFixedSize(800,600);
    this->setWindowTitle("欢乐连连看");
    this->setWindowIcon(QIcon(":/lianliankan/LLK1.png"));

    //切换到帮助窗口
    connect(ui->helpButton,&QPushButton::clicked,this,[=](){
        help = new helpScene();
        help->show();
    });

    //点击基本模式按钮，切换窗口
    connect(ui->baseButton,&QPushButton::clicked,this,[=](){
        basegame = new gameScene(this);
        basegame->show();
        //监听基本模式返回按钮
        connect(basegame,&gameScene::back,this,[=](){
            basegame->hide();
        });
    });





}

void MainScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/lianliankan/bg_m0ain.bmp");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}




MainScene::~MainScene()
{
    delete ui;
}
