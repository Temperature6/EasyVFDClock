#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vfddriver.h"
#include <QTime>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*槽函数*/
    //VFD相关
    void SOLT_SyncTime();
    void SOLT_SetStr();
    void SOLT_SetLed();
    void SOLT_DateFmt();
    void SOLT_ScreenShutDown();
    void SOLT_SetBrightness();

    //界面相关
    void SOLT_ScanPorts();
    void SOLT_ComChanged();
    void SOLT_ShowTime();
    void SOLT_SwitchPortState();
    void SOLT_SlideValChanged();

    //通信相关
    void SOLT_ReceiveMsg();
    void CheckPort();


private:
    QSerialPort* serial;
    VFDDriver* VFD;
    Ui::MainWindow *ui;
    QDateTime curTime;
    QString curTimeStr;
    QString curTimeStamp;
    QTimer* timer = new QTimer();

    void LogOut(QString log, QString opName);

};
#endif // MAINWINDOW_H
