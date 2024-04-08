#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "helpscene.h"
#include "Global.h"
#include "settingwidget.h"
#include <QMainWindow>
#include <QImage>
#include <QString>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QGridLayout>
#include <QMediaPlayer>
#include <QAudioOutput>
QT_BEGIN_NAMESPACE
namespace Ui {
class gameScene;
}
QT_END_NAMESPACE
class ClickableLabel : public QLabel {
Q_OBJECT
public:
    explicit ClickableLabel(const QString &text, int row, int col, QWidget *parent = nullptr)
            : QLabel(text, parent), m_row(row), m_col(col), m_text(text){}

signals:
    void clicked(int row, int col, const QString &text);

protected:
    void mousePressEvent(QMouseEvent *event) override {
        emit clicked(m_row, m_col, m_text);
    }

public:
    int m_row;
    int m_col;
    QString m_text;
};

class gameScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit gameScene(QWidget *parent = nullptr);
    //绘图事件
    void paintEvent(QPaintEvent *) override;
    //开始游戏
    void StartGame();
    //比较标签
    void compareLabels(Vertex &v1,Vertex &v2);
    //获取标签
    void getLabel(const int row,const int col);



    ~gameScene();
private:
    Ui::gameScene *ui;
    helpScene *help = nullptr;
    SettingWidget *settingWidget = nullptr;
    int remainingTime;
    QTimer *timer;
    int **GameMap;
    QGridLayout *layout;
    QVector<Vertex> selectedVertices; // 保存选定的两个顶点信息
    QMediaPlayer *soundEffectPlayer;
    QMediaPlayer *backgroundPlayer;
    QAudioOutput *seaudioOutput;
    QAudioOutput *bgaudioOutput;


signals:
    void back();
public slots:
    void onLabelClicked(int row, int col, const QString &text);
    //重排函数
    void reset_ui();
    //判断游戏是否开始和暂停
    void enableLabelClicking(bool enable);
    //音乐
    void updateBackgroundVolume(int volume);
    void updateSoundEffectVolume(int volume);
};

#endif // GAMESCENE_H
