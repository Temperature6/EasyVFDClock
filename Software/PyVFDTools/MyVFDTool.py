import serial
import serial.tools.list_ports
import serial.serialutil
import time

VFD_BAUDRATE = 9600
CMD_TIME_FMT = "TIME+{0}\n"     # 时间设置*
CMD_STR_FMT = "STR+{0}\n"       # 设置开机字符串*
CMD_LED_FMT = "LED+{0}\n"       # 设置LED状态*
CMD_DFMT_FMT = "DFMT+{0}\n"     # 设置日期格式*
CMD_SSD_FMT = "SSD+%02d%02d\n"  # 设置亮屏时间*
CMD_REBOOT = "REBOOT\n"         # 重启*
CMD_BR_FMT = "BR+%d\n"          # 设置亮度
CMD_CON = "CON\n"               # 通信确认
CONFIRM_STR = "Hello VFD Clock - Powered By AQin\n"


class MyVFDTool:
    def __init__(self):
        self.use_port = None
        self.use_port_opened = False
        self.portList = list(serial.tools.list_ports.comports())

    def get_ports(self):
        return self.portList

    def set_vfd_port(self, port_name: str):
        try:
            if self.use_port_opened:
                self.use_port.close()
            self.use_port = serial.Serial(port=port_name,
                                          baudrate=VFD_BAUDRATE,
                                          bytesize=serial.EIGHTBITS,
                                          stopbits=serial.STOPBITS_ONE,
                                          timeout=0.5)

        except serial.serialutil.SerialException:
            return False
        self.use_port_opened = self.use_port.isOpen()

    def vfd_send(self, context: str):
        if self.use_port.isOpen():
            self.use_port.write(context.encode('utf-8'))
            print("[CMD]>>>" + context.replace('\n', ''))

    def sync_vfd_time(self):
        self.vfd_send(CMD_TIME_FMT.format(int(time.time())))

    def set_vfd_led(self, led1: int, led2: int, led3: int):
        led_val = led3 | (led2 << 1) | (led1 << 2)
        self.vfd_send(CMD_LED_FMT.format(led_val))

    def set_vfd_startup_str(self, ss: str):
        self.vfd_send(CMD_STR_FMT.format(ss if len(ss) <= 8 else ss[0:8]))

    def set_vfd_date_fmt(self, nfmt: int):
        self.vfd_send(CMD_DFMT_FMT.format(nfmt))

    def set_vfd_screen_on_period(self, beg_hour: int, end_hour: int):
        self.vfd_send(CMD_SSD_FMT % (beg_hour, end_hour))

    def set_vfd_brightness(self, brightness: int):
        self.vfd_send(CMD_BR_FMT % brightness)

    def set_vfd_reboot(self):
        self.vfd_send(CMD_REBOOT)

    def vfd_confirm(self) -> bool:
        self.vfd_send(CMD_CON)
        ret_str = self.use_port.readall().decode('utf-8')

        return ret_str == CONFIRM_STR

    def is_opened(self) -> bool:
        if self.use_port is None:
            return False
        return self.use_port.isOpen()

    def rescan_ports(self):
        self.portList = list(serial.tools.list_ports.comports())

    def close_port(self):
        if self.use_port is not None:
            self.use_port.close()
        self.use_port_opened = False


if __name__ == "__main__":
    myVFD = MyVFDTool()
    myVFD.set_vfd_port("COM6")
    # myVFD.sync_vfd_time()
    # myVFD.set_vfd_led(0, 1, 1)
    # myVFD.set_vfd_startup_str("XinNian")
    # myVFD.set_vfd_screen_on_period(7, 23)
    # print(myVFD.vfd_confirm())
