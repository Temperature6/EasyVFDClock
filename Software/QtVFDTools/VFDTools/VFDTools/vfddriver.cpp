#include "vfddriver.h"
#include <QTime>
#include <QDebug>

VFDDriver::VFDDriver(QSerialPort* ser, QTextBrowser* logOut) : serial(ser), MyLog(logOut)
{
    serial->setBaudRate(VFD_BAUDRATE);          //波特率9600
    serial->setStopBits(QSerialPort::OneStop);  //1位停止位
    serial->setDataBits(QSerialPort::Data8);    //8位数据位
    serial->setParity(QSerialPort::NoParity);   //无校验位


}

bool VFDDriver::OpenCom()
{
    serial->open(QIODevice::ReadWrite);
    bool opened = isComOpen();
    if (opened)
    {
        //MyLog->insertPlainText("[INFO]设备串口设置为 “" + ComName + "”");
        LogOut("设置串口为 “" + ComName + "” ", "INFO");
    }
    else
    {
        //MyLog->insertPlainText("[ERROR]无法打开串口  “"+ ComName + "”");
        LogOut("无法打开串口 “" + ComName + "” ", "ERROR");
    }
    return opened;
}

bool VFDDriver::isComOpen()
{
    return serial->isOpen();
}

void VFDDriver::SetComName(QString comName)
{
    ComName = comName;
    serial->setPortName(comName);
}

void VFDDriver::Close()
{
    serial->close();
}

void VFDDriver::SendCmd(QString cmd)
{
    if (isComOpen())
    {
        LogOut(cmd, "SEND");
        //qDebug() << "[DEBUG]" << cmd;
        serial->write(cmd.toLatin1());
    }
    else
    {
        LogOut("没有打开的串口", "ERROR");
    }
}

void VFDDriver::SyncTime()
{
    uint curTimeStamp = QDateTime::currentDateTimeUtc().toTime_t();
    SendCmd(QString::asprintf(CMD_TIME_FMT, curTimeStamp));
}

void VFDDriver::SetStr(QString str)
{
    SendCmd(QString::asprintf(CMD_STR_FMT, str.toLatin1().data()));
}

void VFDDriver::SetLed(bool led1, bool led2, bool led3)
{
    SendCmd(QString::asprintf(CMD_LED_FMT, (led3 | (led2 << 1) | (led1 << 2))));
}

void VFDDriver::SetDateFmt(int nfmt)
{
    SendCmd(QString::asprintf(CMD_DFMT_FMT, nfmt));
}

void VFDDriver::SetOnOffTime(int onH, int offH)
{
    SendCmd(QString::asprintf(CMD_SSD_FMT, onH, offH));
}

void VFDDriver::SetBrightness(int brightness)
{
    SendCmd(QString::asprintf(CMD_SETBR, brightness));
}

void VFDDriver::Reboot()
{
    //UNUSED FUNCTION
}

void VFDDriver::LogOut(QString log, QString opName)
{
    if (opName.isEmpty())
    {
        MyLog->append(log);
    }
    else
    {
        MyLog->append(QString("[%1]%2").arg(opName, log).remove('\n'));
    }
}
