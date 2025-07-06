import fcntl
import struct
import os
import ctypes
import mmap
import sys

sys.path.append("../")
from common.basic import *


def write_data(filed1, filed2):
    print("[Info] write data")
    data = my_data()
    data.field1 = filed1
    data.field2 = filed2
    size = ctypes.sizeof(my_data)
    IOCTL_MAGIC = 'D'
    IOCTL_WRITE_DATA_NR = 2
    cmd_write_data = _IOW(IOCTL_MAGIC, IOCTL_WRITE_DATA_NR, size)
    fd = open('/dev/debug', 'wb')
    try:
        fcntl.ioctl(fd, cmd_write_data, data)
    except OSError as e:
        print(f"Write ioctl failed: {e}")
    fd.close()

def read_data():
    print("[Info] read data")
    data_in = my_data()
    size = ctypes.sizeof(my_data)
    IOCTL_MAGIC = 'D'
    IOCTL_READ_DATA_NR = 4
    cmd_read_data = _IOR(IOCTL_MAGIC, IOCTL_READ_DATA_NR, size)
    fd = open('/dev/debug', 'wb')
    try:
        fcntl.ioctl(fd, cmd_read_data, data_in, True)
    except OSError as e:
        print(f"Write ioctl failed: {e}")
    fd.close()
    print("[Info] filed1 = " + str(data_in.field1))
    print("[Info] filed2 = " + str(data_in.field2))


if __name__ == '__main__':
    write_data(222, 444)
    read_data()