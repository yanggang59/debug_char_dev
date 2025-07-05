#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <ctype.h>


struct my_data {
    int field1;
    int field2;
};

//CMD         31:30       29:16          15:8            7:0
//meaning     dir         data size      device type     function code
//bit         2           14             8               8
#define IOCTL_MAGIC                          ('D')
#define IOCTL_RAW_WRITE_INT                  _IOW(IOCTL_MAGIC, 1, int)
#define IOCTL_RAW_WRITE_DATA                 _IOW(IOCTL_MAGIC, 2, struct my_data)
#define IOCTL_RAW_READ                       _IO(IOCTL_MAGIC, 3)




#define DBG_CHAR_DEV                            "/dev/debug"


static const char short_options[] = "w:rs";
static const struct option long_options[] = {
	{"write", required_argument, NULL, 'w'},
    {"read", no_argument, NULL, 'r' },
    {"write_struct", no_argument, NULL, 's' },
	{ 0, 0, 0, 0 }
};


static void debug_ioctl_raw_write(int param)
{
    int fd, ret;
    printf("[Info] %s : %d \r\n", __func__, __LINE__);
    fd = open(DBG_CHAR_DEV, O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        exit(-1);
    }
    
    ret = ioctl(fd, IOCTL_RAW_WRITE_INT, param);
    
    if(ret < 0) {
        fprintf(stderr, "ioctl: %s\n", strerror(errno));
        exit(-1);
    }
    close(fd);
}

static void debug_ioctl_raw_write_data(void)
{
    int fd, ret;
    printf("[Info] %s : %d \r\n", __func__, __LINE__);
    struct my_data data = {.field1 = 10, .field2 = 20};
    fd = open(DBG_CHAR_DEV, O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        exit(-1);
    }
    
    ret = ioctl(fd, IOCTL_RAW_WRITE_DATA, &data);
    
    if(ret < 0) {
        fprintf(stderr, "ioctl: %s\n", strerror(errno));
        exit(-1);
    }
    close(fd);
}


static void debug_ioctl_raw_read(void)
{
    int fd, ret;
    printf("[Info] %s : %d \r\n", __func__, __LINE__);
    fd = open(DBG_CHAR_DEV, O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        exit(-1);
    }
    
    ret = ioctl(fd, IOCTL_RAW_READ, NULL);
    
    if(ret < 0) {
        fprintf(stderr, "ioctl: %s\n", strerror(errno));
        exit(-1);
    }
    close(fd);
}

static void usage(void)
{
	printf( "usage: app [--write | -w] \n"
		"       [--read | -r \n"
    );
	exit(1);
}


int main(int argc, char** argv)
{
    int c;
    int param = 0;
    char *endptr;
    while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (c) {
            case 'w':
                param = strtol(optarg, &endptr, 10);
                printf("[Info] param = %d\r\n", param);
                debug_ioctl_raw_write(param);
                break;
            case 's': 
                debug_ioctl_raw_write_data();
                break;
            case 'r': 
                debug_ioctl_raw_read();
                break;
            default:
                usage();
                break;
        }
    }
    return 0;
}

