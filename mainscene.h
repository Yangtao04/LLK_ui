#ifndef MAINSCENE_H
#define MAINSCENE_H
#include "helpscene.h"
#include <QMainWindow>
#include "gamescene.h"

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

    //绘图事件
    void paintEvent(QPaintEvent *);

private:
    Ui::MainScene *ui;
    helpScene *help = nullptr;
    gameScene *basegame = nullptr;
};
#endif // MAINSCENE_H
