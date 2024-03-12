#ifndef __DEBUG_CDEV__
#define __DEBUG_CDEV__

#include <linux/cdev.h>
#include <linux/ioctl.h>



#undef DEBUG_THIS_MODULE
#define DEBUG_THIS_MODULE                 1
#if DEBUG_THIS_MODULE
#define NUPA_DEBUG(fmt,...)               printk("[NUPA DEBUG] "fmt, ##__VA_ARGS__)
#define NUPA_ERROR(fmt,...)               printk("[NUPA ERROR] "fmt, ##__VA_ARGS__)
#else
#define NUPA_DEBUG(fmt,...)
#define NUPA_ERROR(fmt,...)
#endif


//CMD         31:30       29:16          15:8            7:0
//meaning     dir         data size      device type     function code
//bit         2           14             8               8
#define IOCTL_MAGIC                      ('D')
#define IOCTL_RAW_WRITE                  _IO(IOCTL_MAGIC, 1)
#define IOCTL_RAW_READ                   _IO(IOCTL_MAGIC, 2)
#define IOCTL_DUMP_MSG_ON                _IO(IOCTL_MAGIC, 3)
#define IOCTL_DUMP_MSG_OFF               _IO(IOCTL_MAGIC, 4)
#define IOCTL_HACK_CTL_ON                _IO(IOCTL_MAGIC, 5)
#define IOCTL_HACK_CTL_OFF               _IO(IOCTL_MAGIC, 6)
#define IOCTL_HOLD_IC_ON                 _IO(IOCTL_MAGIC, 7)
#define IOCTL_HOLD_IC_OFF                _IO(IOCTL_MAGIC, 8)

#define NAME                            "debug"
#define BUF_LENGTH                      (8 << 12)

#define DEBUG_USE_MMAP                1


struct debug_cdev{
    dev_t cdevno;
    struct class *class;
    char *buf;
    int buf_size;
    struct cdev cdev;
};

#endif
