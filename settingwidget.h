#ifndef LLK_UI_SETTINGWIDGET_H
#define LLK_UI_SETTINGWIDGET_H

#include <QWidget>
#include <QSlider>
QT_BEGIN_NAMESPACE
namespace Ui { class SettingWidget; }
QT_END_NAMESPACE

class SettingWidget : public QWidget {
Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = nullptr);

    ~SettingWidget() override;
signals:
    void backgroundVolumeChanged(int volume);
    void soundEffectVolumeChanged(int volume);
private slots:
    void backgroundVolumeChanged1(int volume);
    void soundEffectVolumeChanged1(int volume);

private:
    Ui::SettingWidget *ui;
};


#endif //LLK_UI_SETTINGWIDGET_H
