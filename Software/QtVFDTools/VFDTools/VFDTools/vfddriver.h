#ifndef VFDDRIVER_H
#define VFDDRIVER_H
#include <QString>
#include <QTextBrowser>
#include <QSerialPort>
#include <QSerialPortInfo>


#define VFD_BAUDRATE    9600
#define CMD_TIME_FMT    "TIME+%u\n"         // 时间设置
#define CMD_STR_FMT     "STR+%s\n"          // 设置开机字符串
#define CMD_LED_FMT     "LED+%d\n"          // 设置LED状态
#define CMD_DFMT_FMT    "DFMT+%d\n"         // 设置日期格式
#define CMD_SSD_FMT     "SSD+%02d%02d\n"    // 设置亮屏时间
#define CMD_REBOOT      "REBOOT\n"          // 重启
#define CMD_SETBR       "BR+%d\n"           // 设置亮度
#define CMD_CON         "CON\n"             // 通信确认
#define CONFIRM_STR     "Hello VFD Clock - Powered By AQin\n"


class VFDDriver
{
private:
    QString ComName;
    QTextBrowser* MyLog;
    QSerialPort* serial;

    void LogOut(QString log, QString opName = "");
public:
    /* 通信操作 */
    VFDDriver(QSerialPort* ser, QTextBrowser* logOut);
    bool OpenCom();
    bool isComOpen();
    void SetComName(QString comName);
    void Close();
    /* 设备操作 */
    void SendCmd(QString cmd);
    void SyncTime();
    void SetStr(QString str);
    void SetLed(bool led1, bool led2, bool led3);
    void SetDateFmt(int nfmt);
    void SetOnOffTime(int onH, int offH);
    void SetBrightness(int brightness);
    void Reboot();
};

#endif // VFDDRIVER_H
