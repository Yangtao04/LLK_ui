#include "settingwidget.h"
#include "./ui_settingwidget.h"


SettingWidget::SettingWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::SettingWidget){
    ui->setupUi(this);
    this->setWindowTitle("欢乐连连看——设置");
    this->setWindowIcon(QIcon(":/lianliankan/LLK1.png"));
    ui->bgSlider->setRange(0,100);
    ui->seSlider->setRange(0,100);
    ui->bgSlider->setValue(50);
    ui->seSlider->setValue(50);
    connect(ui->bgSlider,&QSlider::valueChanged,this,&SettingWidget::backgroundVolumeChanged1);
    connect(ui->seSlider,&QSlider::valueChanged, this,&SettingWidget::soundEffectVolumeChanged1);

}

SettingWidget::~SettingWidget() {
    delete ui;
}
void SettingWidget::backgroundVolumeChanged1(int volume) {
    emit backgroundVolumeChanged(volume);
}
void SettingWidget::soundEffectVolumeChanged1(int volume) {
    emit soundEffectVolumeChanged(volume);
}