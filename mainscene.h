#ifndef MAINSCENE_H
#define MAINSCENE_H
#include "helpscene.h"
#include <QMainWindow>
#include "gamescene.h"
#include <QPainter>
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainScene;
}
QT_END_NAMESPACE

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();
    //音乐
    void updateBackgroundVolume(int volume);

    //绘图事件
    void paintEvent(QPaintEvent *);

private:
    Ui::MainScene *ui;
    helpScene *help = nullptr;
    gameScene *basegame = nullptr;
    QMediaPlayer *backgroundPlayer;
    QAudioOutput *bgAudioOutput;
    SettingWidget *settingWidget = nullptr;
};
#endif // MAINSCENE_H
