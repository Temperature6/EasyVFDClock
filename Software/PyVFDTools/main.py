import sys
import argparse
from MyVFDTool import MyVFDTool
import os
import time

VERSION = 1.0

myVFD = MyVFDTool()
parser = argparse.ArgumentParser(description="Factory Mode")
parser.add_argument("-p",
                    type=str,
                    nargs="+",
                    help="[需要参数]目标设备的串口号，例如 COM1 或者 /dev/ttyUSB1",
                    default="AUTO")
parser.add_argument("--st",
                    nargs="?",
                    help="[需要参数]是否同步时间，值为 0 或 1",
                    default=1)
parser.add_argument("--led",
                    type=int,
                    nargs="+",
                    help="[需要参数]LED状态")
parser.add_argument("--dfmt",
                    type=int,
                    nargs="+",
                    help="[需要参数]日期格式，值为 1 或 2")
parser.add_argument("--ssd",
                    type=int,
                    nargs="+",
                    help="[需要参数]亮屏时间，例如0723")
parser.add_argument("--br",
                    type=int,
                    nargs="+",
                    help="[需要参数]亮度，范围 0 ~ 255")
parser.add_argument("--str",
                    type=str,
                    nargs="+",
                    help="[需要参数]开机字符串")


def AutoSelPort() -> str:
    port_list = myVFD.get_ports()
    for com in port_list:
        myVFD.set_vfd_port(list(com)[0])
        if myVFD.is_opened() and myVFD.vfd_confirm():
            myVFD.close_port()
            port = list(com)[0]
            return port
    return ""


def ClearScreen():
    os.system("cls")


def ShowMenu():
    # ClearScreen()
    print("------------------------------------------")
    print(f"VFD Helper V{VERSION} - Powered By AQin")
    print("输入数字，选择操作")
    print(f"1.选择串口: {myVFD.use_port.name if myVFD.is_opened() else 'None'}")
    print("2.同步电脑时间")
    print("3.设置开机字符串")
    print("4.设置LED状态")
    print("5.设置日期格式")
    print("6.设置亮屏时间")
    print("7.设置亮度")
    print("8.重启")
    print("9.退出程序")


def SelectCOM():
    # ClearScreen()
    port_list = myVFD.get_ports()
    print("0 - 自动选择")
    for i in range(len(port_list)):
        print(i + 1, " - ", list(port_list[i])[0], list(port_list[i])[1])
    ch = int(input("输入选择项的序号，回车确认："))

    if ch == 0:
        port = AutoSelPort()
    else:
        port = list(port_list[ch - 1])[0]

    if port == "":
        print("没有找到可用的 VFD Clock 串口设备")
    else:
        myVFD.set_vfd_port(port)


def SyncTime():
    if myVFD.is_opened():
        myVFD.sync_vfd_time()
        print("完成时间同步")
    else:
        print("错误：还没有可用的 VFD Clock 串口设备")


def SetStr():
    if myVFD.is_opened():
        ss = input("输入字符串(使用英文，且长度不大于8个字符)，回车确认:")
        if len(ss) <= 8:
            myVFD.set_vfd_startup_str(ss)
            print("开机字符串设置完成")
        else:
            print("错误：输入的字符串长度大于 8")
    else:
        print("错误：还没有可用的 VFD Clock 串口设备")


def SetLed():
    if myVFD.is_opened():
        print("依次输入三个数字，使用空格隔开，代表LED1，LED2，LED3的状态，1为亮，0为灭")
        in_str = input("输入（例如：1 0 1），回车确认：").split(" ")
        myVFD.set_vfd_led(int(in_str[0]), int(in_str[1]), int(in_str[2]))
        print("LED状态设置完成 - 需要重启")
    else:
        print("错误：还没有可用的 VFD Clock 串口设备")


def SetDateFmt():
    if myVFD.is_opened():
        print("设置日期显示格式:")
        print("1.格式1：%%y/%%m/%%d，例如 23/11/16:")
        print("2.格式2：%%m/%%d  %%u，例如 11/16  4:")
        ch = int(input("输入选择项的序号，回车确认："))
        myVFD.set_vfd_date_fmt(ch)
        print("日期格式设置完毕")
    else:
        print("错误：还没有可用的 VFD Clock 串口设备")


def SetPeriod():
    if myVFD.is_opened():
        print("设置亮屏时间：输入两个数，使用空格隔开\n"
              "分别代表显示屏开启的时间（小时）和显示屏关闭的时间时间（小时）\n"
              "使用24小时制")
        in_str = input("输入数据（例如：7 23），回车确认：").split(" ")
        myVFD.set_vfd_screen_on_period(int(in_str[0]), int(in_str[1]))
        print("亮屏时间设置完毕")

    else:
        print("还没有可用的 VFD Clock 串口设备")


def SetBrightness():
    if myVFD.is_opened():
        print("设置亮度，范围 0 ~ 255：")
        in_str = input("输入数据，回车确认：")
        myVFD.set_vfd_brightness(int(in_str))
        print("亮度设置完毕")

    else:
        print("还没有可用的 VFD Clock 串口设备")


def SetReboot():
    if myVFD.is_opened():
        myVFD.set_vfd_reboot()
        print("已发送重启命令")
    else:
        print("还没有可用的 VFD Clock 串口设备")


def Exit():
    myVFD.close_port()
    sys.exit(0)


if __name__ == "__main__":
    if len(sys.argv) == 1:
        while True:
            ShowMenu()
            ch_ = int(input("输入选择项的序号，回车确认："))
            if ch_ == 1:
                SelectCOM()
            elif ch_ == 2:
                SyncTime()
            elif ch_ == 3:
                SetStr()
            elif ch_ == 4:
                SetLed()
            elif ch_ == 5:
                SetDateFmt()
            elif ch_ == 6:
                SetPeriod()
            elif ch_ == 7:
                SetBrightness()
            elif ch_ == 8:
                SetReboot()
            elif ch_ == 9:
                Exit()
    else:
        # print("Argprase")
        args = parser.parse_args()
        # print(args)
        # 1.设置串口
        if args.p == "AUTO":
            port_name = AutoSelPort()
            if port_name == "":
                print("没有寻找到设备")
                Exit()
        else:
            port_name = args.p
        myVFD.set_vfd_port(port_name)

        # 2.是否需要同步时间？
        if args.st is not None:
            myVFD.sync_vfd_time()
            time.sleep(0.2)

        # 3.是否需要设置LED
        if args.led is not None:
            val = args.led[0]
            myVFD.set_vfd_led((val >> 2) & 1, (val >> 1) & 1, val & 1)
            time.sleep(0.2)

        # 4.是否需要设置日期格式
        if args.dfmt is not None:
            myVFD.set_vfd_date_fmt(args.dfmt[0])
            time.sleep(0.2)

        # 5.是否需要设置开关屏幕时间
        if args.ssd is not None:
            val = args.ssd[0]
            myVFD.set_vfd_screen_on_period(int(val / 100), val % 100)
            time.sleep(0.2)

        # 6.是否需要设置开关屏幕时间
        if args.br is not None:
            val = args.br[0]
            myVFD.set_vfd_brightness(val)
            time.sleep(0.2)

        # 7.是否需要设置开机字符串
        if args.str is not None:
            val = args.str[0]
            myVFD.set_vfd_startup_str(val)
            time.sleep(0.2)
        print("操作完成")
