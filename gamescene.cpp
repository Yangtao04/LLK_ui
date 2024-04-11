#include "gamescene.h"
#include "./ui_gamescene.h"
#include "CGameGraph.h"
#include "settingwidget.h"
#include "Global.h"
#include <QPainter>
#include <QTimer>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <QWidget>
#include <QGridLayout>
#include <QMediaPlayer>
#include <QAudioOutput>

gameScene::gameScene(QWidget *parent)
    : QMainWindow{parent}
    , ui(new Ui::gameScene)
{
    ui->setupUi(this);
    //设置固定大小，标题，窗口图标
    this->setFixedSize(800,600);
    this->setWindowTitle("欢乐连连看——基本模式");
    this->setWindowIcon(QIcon(":/lianliankan/LLK1.png"));

    //切换到帮助窗口
    help = new helpScene;
    connect(ui->helpButton,&QPushButton::clicked,this,[=](){
        help->show();
    });
    //返回主界面
    connect(ui->backButton,&QPushButton::clicked,this,[=](){
        backgroundPlayer->stop();
        emit this->back();
    });
    //倒计时的标签
    // 设置倒计时时间
    remainingTime = 300;
    ui->timeLabel->setText(QString::number(remainingTime));
    // 创建定时器
    timer = new QTimer(this);
    timer->setInterval(1000); // 1秒
    connect(ui->startButton,&QPushButton::clicked,this,[=](){
        enableLabelClicking(true); // 启用标签点击
        backgroundPlayer->play();
        timer->start();
    });
    connect(ui->stopButton,&QPushButton::clicked,this,[=](){
        enableLabelClicking(false); // 禁用标签点击
        backgroundPlayer->stop();
        timer->stop();
    });
    // 初始化进度条
    ui->timeprogressBar->setRange(0, 300); // 设置进度条范围为0到300
    ui->timeprogressBar->setValue(300);    // 设置进度条初始值为300
    // 定时器超时事件处理函数
    connect(timer, &QTimer::timeout, this, [&]() {
        // 更新剩余时间
        remainingTime--;
        // 更新标签,进度条显示
        ui->timeLabel->setText(QString::number(remainingTime));
        ui->timeprogressBar->setValue(remainingTime);
        // 如果倒计时完成，停止定时器
        if (remainingTime <= 0) {
            timer->stop();
            backgroundPlayer->stop();
            enableLabelClicking(false);
            failPlayer->play();
            QMessageBox::warning(this,"Game Over",tr("游戏挑战失败!"));
        }
    });

    //创建地图
    GraphMap = InitMap();
    //开始刷新游戏地图
    StartGame();
    //地图禁止点击
    enableLabelClicking(false);
    //重排按钮，重排地图
    connect(ui->resetButton,&QPushButton::clicked, this,&gameScene::reset_ui);
    //提示按钮
    connect(ui->tipButton,&QPushButton::clicked,this,[=](){
        Vertex *vertex1;
        vertex1 = vertex(GraphMap);
        getLabel(vertex1[0].row,vertex1[0].col);
        getLabel(vertex1[1].row,vertex1[1].col);
        QMessageBox::information(this,"提示","第"+QString::number(vertex1[0].row+1)+"行，"+QString::number(vertex1[0].col+1)+"列与第"+QString::number(vertex1[1].row+1)+"行，"+QString::number(vertex1[1].col+1)+"列可以相消");
    });
    //设置按钮
    settingWidget = new SettingWidget;
    connect(ui->settingButton,&QPushButton::clicked,this,[=](){
        settingWidget->show();
    });
    connect(settingWidget,&SettingWidget::soundEffectVolumeChanged,this,&gameScene::updateSoundEffectVolume);
    connect(settingWidget,&SettingWidget::backgroundVolumeChanged,this,&gameScene::updateBackgroundVolume);
    //消除音效
    // 创建 QMediaPlayer 对象并加载音效文件
    soundEffectPlayer = new QMediaPlayer(this);
    backgroundPlayer = new QMediaPlayer(this);
    victoryPlayer = new QMediaPlayer(this);
    failPlayer = new QMediaPlayer(this);
    seaudioOutput = new QAudioOutput(this);
    bgaudioOutput = new QAudioOutput(this);
    victoryAudioOutput = new QAudioOutput(this);
    failAudioOutput = new QAudioOutput(this);
    seaudioOutput->setVolume(0.5);
    bgaudioOutput->setVolume(0.5);
    victoryAudioOutput->setVolume(0.5);
    failAudioOutput->setVolume(0.5);
    soundEffectPlayer->setAudioOutput(seaudioOutput);
    backgroundPlayer->setAudioOutput(bgaudioOutput);
    victoryPlayer->setAudioOutput(victoryAudioOutput);
    failPlayer->setAudioOutput(failAudioOutput);
    soundEffectPlayer->setSource(QUrl("C:/Users/yangtao/Desktop/lianliankan/1.mp3"));
    backgroundPlayer->setSource(QUrl("C:/Users/yangtao/Desktop/lianliankan/2.mp3"));
    victoryPlayer->setSource(QUrl("C:/Users/yangtao/Desktop/lianliankan/v.mp3"));
    failPlayer->setSource(QUrl("C:/Users/yangtao/Desktop/lianliankan/f.mp3"));
    backgroundPlayer->play();
    backgroundPlayer->setLoops(-1);


}
//绘图
void gameScene::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/lianliankan/fruit_b0g.bmp");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}
//根据地图绘制图片
void gameScene::StartGame() {
    // 删除旧的布局和其中的所有控件
    QLayout *existingLayout = ui->gameArea->layout();
    if (existingLayout) {
        QLayoutItem *child;
        while ((child = existingLayout->takeAt(0)) != nullptr) {
            delete child->widget(); // 删除控件
            delete child;           // 释放布局项内存
        }
        delete existingLayout;     // 释放布局内存
    }
    layout = new QGridLayout(ui->gameArea);
    QString imagePath = ":/lianliankan/fruit";
    layout->setSpacing(0);
    // 设置标签的固定大小
    QSize labelSize(40, 40); // 假设标签大小为40x540
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 16; ++col) {
            ClickableLabel *label = new ClickableLabel(QString::number(GraphMap->vex[row*16+col].info + 1),row,col,ui->gameArea);
            QString finPath = imagePath + QString::number(GraphMap->vex[row*16+col].info + 1) + ".bmp";
            QImage image = picture1(finPath);
            QPixmap pixmap = QPixmap::fromImage(image);
            label->setPixmap(pixmap);
            label->setFixedSize(labelSize); // 设置标签的固定大小
            label->setScaledContents(true); // 图片自适应label大小
            layout->addWidget(label, row, col); // 将标签添加到布局中
            connect(label, &ClickableLabel::clicked, this,&gameScene::onLabelClicked);
        }
    }
    // 设置布局
    GraphMap = GraphLink(GraphMap);
    ui->gameArea->setLayout(layout);
}
//点击标签槽函数
void gameScene::onLabelClicked(int row, int col, const QString &text) {
    qDebug() << "Clicked on row:" << row << "column:" << col << "value:" << text;
    // 设置被点击的标签样式，为其添加边框
    if (auto clickedLabel = qobject_cast<ClickableLabel *>(sender())) {
        // 设置阴影效果
        QString shadowStyle = "border: 2px solid red;"; // 设置边框为红色，宽度为2px
        clickedLabel->setStyleSheet(shadowStyle);
    }

    // 根据点击的标签信息构造顶点结构体
    Vertex vertex;
    vertex.row = row;
    vertex.col = col;
    vertex.info = text.toInt();
    // 将顶点信息保存起来，可以使用 QVector 或者 std::pair 来保存，这里使用 QVector
    //static QVector<Vertex> selectedVertices;
    selectedVertices.append(vertex);
    // 当保存的顶点信息达到两个时，进行比较并处理
    if (selectedVertices.size() == 2) {
        compareLabels(selectedVertices[0], selectedVertices[1]);
        // 清空保存的顶点信息，以便下一次点击
        selectedVertices.clear();

    }
}
//比较顶点
void gameScene::compareLabels(Vertex &v1,Vertex &v2) {
    qDebug()<<v1.row<<" "<<v1.col<<" "<<v1.info;
    qDebug()<<v2.row<<" "<<v2.col<<" "<<v2.info;
    if(GraphMap->GraphMap[v1.row*16+v1.col][v2.row*16+v2.col]&&v1.info==v2.info){
        GraphMap->vex[v1.row*16+v1.col].info = -1;
        GraphMap->vex[v2.row*16+v2.col].info = -1;
        soundEffectPlayer->play();
    }
    StartGame();
    if(gameOver(GraphMap)){
        backgroundPlayer->stop();
        timer->stop();
        victoryPlayer->play();
        QMessageBox::warning(this,"Game Over","游戏挑战成功!");
    }
}
//重排函数
void gameScene::reset_ui() {
    GraphMap = reset(GraphMap);
    StartGame();
    enableLabelClicking(false);
}
//判断游戏是否开始和暂停
void gameScene::enableLabelClicking(bool enable) {
    layout = qobject_cast<QGridLayout*>(ui->gameArea->layout());
    if (!layout)
        return;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 16; ++col) {
            QWidget *widget = layout->itemAtPosition(row, col)->widget();
            if (auto label = qobject_cast<ClickableLabel*>(widget)) {
                label->setEnabled(enable);
            }
        }
    }
}
//获取标签
void gameScene::getLabel(const int row, const int col) {
    // 获取指定位置的布局项
    QLayoutItem *layoutItem = layout->itemAtPosition(row, col);
    if (layoutItem) {
        // 将布局项转换为 ClickableLabel 类型
        ClickableLabel *label = qobject_cast<ClickableLabel *>(layoutItem->widget());
        if (label) {
            // 对标签进行修改
            QString shadowStyle = "border: 2px solid green;"; // 设置边框为红色，宽度为2px
            label->setStyleSheet(shadowStyle);
        }
    }
}
//背景音乐
void gameScene::updateBackgroundVolume(int volume) {
    qDebug()<<volume;
    qreal volumeLevel = static_cast<qreal>(volume) / 100.0;
    bgaudioOutput->setVolume(volumeLevel);
}
//游戏音效
void gameScene::updateSoundEffectVolume(int volume) {
    qDebug()<<volume;
    qreal volumeLevel = static_cast<qreal>(volume) / 100.0;
    seaudioOutput->setVolume(volumeLevel);
    victoryAudioOutput->setVolume(volumeLevel);
    failAudioOutput->setVolume(volumeLevel);
}

//析构函数
gameScene::~gameScene() {
    delete ui;
}






