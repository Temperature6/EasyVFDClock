#include "mainwindow.h"
#include "../ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*变量初始化*/
    serial = new QSerialPort();
    VFD = new VFDDriver(serial, ui->tb_log);

    /*进行需要的操作*/
    SOLT_ScanPorts();   //扫描所有串口
    timer->start(1000);
    SOLT_SlideValChanged();

    connect(ui->btn_sync_time, &QPushButton::clicked, this, &MainWindow::SOLT_SyncTime);
    connect(ui->btn_rescan, &QPushButton::clicked, this, &MainWindow::SOLT_ScanPorts);
    connect(ui->cb_port_list, &QComboBox::currentTextChanged, this, &MainWindow::SOLT_ComChanged);
    connect(ui->btn_open_port, &QPushButton::clicked, this, &MainWindow::SOLT_SwitchPortState);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::SOLT_ReceiveMsg);
    connect(ui->btn_set_dfmt, &QPushButton::clicked, this, &MainWindow::SOLT_DateFmt);
    connect(ui->btn_set_led, &QPushButton::clicked, this, &MainWindow::SOLT_SetLed);
    connect(ui->btn_set_ssd, &QPushButton::clicked, this, &MainWindow::SOLT_ScreenShutDown);
    connect(ui->btn_set_str, &QPushButton::clicked, this, &MainWindow::SOLT_SetStr);
    connect(timer, &QTimer::timeout, this, &MainWindow::SOLT_ShowTime);
    connect(ui->hs_bright, &QSlider::valueChanged, this, &MainWindow::SOLT_SlideValChanged);
    connect(ui->btn_bright_set, &QPushButton::clicked, this, &MainWindow::SOLT_SetBrightness);
}

MainWindow::~MainWindow()
{
    delete timer;
    delete VFD;
    delete ui;

}

void MainWindow::SOLT_SyncTime()
{
    if (VFD->isComOpen())
    {
        VFD->SyncTime();
    }
    else
    {
        CheckPort();
    }
}

void MainWindow::SOLT_ScanPorts()
{
    //disconnect(ui->cb_port_list, &QComboBox::currentTextChanged, this, &MainWindow::SOLT_ComChanged);
    ui->cb_port_list->clear();
    ui->cb_port_list->addItem("None");
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->cb_port_list->addItem(info.portName());
    }
    LogOut("串口扫描完毕", "INFO");
    //connect(ui->cb_port_list, &QComboBox::currentTextChanged, this, &MainWindow::SOLT_ComChanged);
}

void MainWindow::SOLT_ComChanged()
{
    QString curText = ui->cb_port_list->currentText();

    if (curText != "None" && curText != "")
    {
        VFD->Close();
        VFD->SetComName(curText);
        if (VFD->OpenCom())
        {
            ui->btn_open_port->setText("关闭");
        }
        else
        {
            ui->btn_open_port->setText("开启");
            LogOut("无法打开串口", "ERROR");
        }
    }
}

void MainWindow::SOLT_SetStr()
{
    if (VFD->isComOpen())
    {
        VFD->SetStr(ui->le_str_edit->text());
    }
    else
    {
        CheckPort();
    }
}

void MainWindow::SOLT_SetLed()
{
    if (VFD->isComOpen())
    {
        VFD->SetLed(ui->cb_led1->isChecked(),
                    ui->cb_led2->isChecked(),
                    ui->cb_led3->isChecked());
    }
    else
    {
        CheckPort();
    }

}

void MainWindow::SOLT_DateFmt()
{
    if (VFD->isComOpen())
    {
        int nfmt = 0;
        if (ui->rb_datefmt1->isChecked())
        {
            nfmt = 1;
        }
        else if (ui->rb_datefmt2->isChecked())
        {
            nfmt = 2;
        }
        VFD->SetDateFmt(nfmt);
    }
    else
    {
        CheckPort();
    }

}

void MainWindow::SOLT_ScreenShutDown()
{
    if (VFD->isComOpen())
    {
        VFD->SetOnOffTime(ui->sb_beg_hour->value(),
                          ui->sb_end_hour->value());
    }
    else
    {
        CheckPort();
    }
}

void MainWindow::SOLT_ReceiveMsg()
{
    if (serial->canReadLine())
    {
        LogOut((QString)serial->readAll(), "RECEIVE");
    }
}

void MainWindow::LogOut(QString log, QString opName)
{
    if (opName.isEmpty())
    {
        ui->tb_log->append(log);
    }
    else
    {
        ui->tb_log->append(QString("[%1]%2").arg(opName, log).remove('\n'));
    }
}

void MainWindow::SOLT_ShowTime()
{
    curTime = QDateTime::currentDateTime();
    curTimeStr = curTime.toString("日期：yyyy/MM/dd 时间：hh:mm:ss");
    ui->label_show_time->setText(curTimeStr);
    curTimeStamp = QString("时间戳: %1").arg(curTime.toSecsSinceEpoch());
    ui->label_show_ts->setText(curTimeStamp);
}

void MainWindow::SOLT_SwitchPortState()
{
    if (ui->cb_port_list->currentText() != "None" )
    {
        if (VFD->isComOpen())
        {
            VFD->Close();
            ui->btn_open_port->setText("打开");
        }
        else
        {
            if (VFD->OpenCom())
            {
                ui->btn_open_port->setText("关闭");
            }
            else
            {
                ui->btn_open_port->setText("打开");
            }
        }
    }

}

void MainWindow::SOLT_SetBrightness()
{
    if (VFD->isComOpen())
    {
        VFD->SetBrightness(ui->hs_bright->value());
    }
    else
    {
        CheckPort();
    }
}

void MainWindow::SOLT_SlideValChanged()
{
    ui->lb_hs_val->setText(QString::asprintf("亮度:%03d", ui->hs_bright->value()));
}

void MainWindow::CheckPort()
{
    if (VFD->isComOpen())
    {
        ui->btn_open_port->setText("关闭");
    }
    else
    {
        LogOut("没有打开的串口", "ERROR");
        ui->btn_open_port->setText("打开");
    }
}
