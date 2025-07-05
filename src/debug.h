#ifndef __DEBUG_CDEV__
#define __DEBUG_CDEV__

#include <linux/cdev.h>
#include <linux/ioctl.h>



#undef DEBUG_THIS_MODULE
#define DEBUG_THIS_MODULE                 1
#if DEBUG_THIS_MODULE
#define LAB_DEBUG(fmt,...)               printk("[KERNEL DEBUG] "fmt, ##__VA_ARGS__)
#define LAB_ERROR(fmt,...)               printk("[KERNEL ERROR] "fmt, ##__VA_ARGS__)
#else
#define LAB_DEBUG(fmt,...)
#define LAB_ERROR(fmt,...)
#endif

struct my_data {
    int field1;
    int field2;
};

//CMD         31:30       29:16          15:8            7:0
//meaning     dir         data size      device type     function code
//bit         2           14             8               8
#define IOCTL_MAGIC                           ('D')
#define IOCTL_RAW_WRITE_INT                   _IOW(IOCTL_MAGIC, 1, int)
#define IOCTL_RAW_WRITE_DATA                  _IOW(IOCTL_MAGIC, 2, struct my_data)
#define IOCTL_RAW_READ                        _IO(IOCTL_MAGIC, 3)



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
