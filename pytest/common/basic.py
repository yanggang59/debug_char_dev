import ctypes

class my_data(ctypes.Structure):
    _fields_ = [
        ('field1', ctypes.c_int),
        ('field2', ctypes.c_int)
    ]


def _IOC(dir_, type_, nr, size):
    return (dir_ << 30) | (ord(type_) << 8) | (nr << 0) | (size << 16)

def _IOW(type_, nr, size):
    return _IOC(1, type_, nr, size)

def _IOR(type_, nr, size):
    return _IOC(2, type_, nr, size)

def _IOWR(type_, nr, size):
    return _IOC(3, type_, nr, size)

__all__ = ['_IOW', '_IOR', '_IOWR', "my_data"]