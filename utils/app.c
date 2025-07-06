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
#define IOCTL_MAGIC                           ('D')
#define IOCTL_RAW_WRITE_INT                   _IOW(IOCTL_MAGIC, 1, int)
#define IOCTL_RAW_WRITE_DATA                  _IOW(IOCTL_MAGIC, 2, struct my_data)
#define IOCTL_RAW_READ_INT                    _IOR(IOCTL_MAGIC, 3, int)
#define IOCTL_RAW_READ_DATA                   _IOR(IOCTL_MAGIC, 4, struct my_data)



#define DBG_CHAR_DEV                            "/dev/debug"
#define BUF_LENGTH                      (8 << 12)

static const char short_options[] = "w:rspm";
static const struct option long_options[] = {
	{"write", required_argument, NULL, 'w'},
    {"read", no_argument, NULL, 'r' },
    {"write_data", no_argument, NULL, 's' },
    {"read_data", no_argument, NULL, 'p' },
    {"mmap", no_argument, NULL, 'm' },
	{ 0, 0, 0, 0 }
};

void dump_buf(char* buf, int len)
{
  printf("**************************************************************************************\r\n");
  printf("     ");
  for(int i = 0; i < 16; i++) 
    printf("%4X ", i);

  for(int j = 0; j < len; j++) {
    if(j % 16 == 0) {
      printf("\n%4X ", j);
    }
    printf("%4X ", buf[j]);
  }

  printf("\n**************************************************************************************\r\n");
}


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
    int read_val = 0;
    printf("[Info] %s : %d \r\n", __func__, __LINE__);
    fd = open(DBG_CHAR_DEV, O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        exit(-1);
    }
    
    ret = ioctl(fd, IOCTL_RAW_READ_INT, &read_val);
    
    if(ret < 0) {
        fprintf(stderr, "ioctl: %s\n", strerror(errno));
        exit(-1);
    }
    printf("[Info] read_val = %d\r\n", read_val);
    close(fd);
}

static void debug_ioctl_raw_read_data(void)
{
    int fd, ret;
    struct my_data data;
    printf("[Info] %s : %d \r\n", __func__, __LINE__);
    fd = open(DBG_CHAR_DEV, O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        exit(-1);
    }
    
    ret = ioctl(fd, IOCTL_RAW_READ_DATA, &data);
    
    if(ret < 0) {
        fprintf(stderr, "ioctl: %s\n", strerror(errno));
        exit(-1);
    }
    printf("[Info] field1 = %d, field2 = %d\r\n", data.field1, data.field2);
    close(fd);
}

static void debug_mmap(void)
{
    int fd, ret;
    int read_val = 0;
    char* m_buf, *l_buf;
    printf("[Info] %s : %d \r\n", __func__, __LINE__);
    fd = open(DBG_CHAR_DEV, O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        exit(-1);
    }

    m_buf = mmap(NULL, BUF_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (m_buf == (void*) -1) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        exit(-1);
    }

    l_buf = (char* )malloc(BUF_LENGTH);
    if(!l_buf) {
        fprintf(stderr, "malloc: %s\n", strerror(errno));
        exit(-1);
    }

    memcpy(l_buf, m_buf, BUF_LENGTH);
    dump_buf(l_buf, BUF_LENGTH);
}

static void usage(void)
{
	printf( "usage: app [--write | -w ] \n"
		"       [--read | -r \n"
		"       [--write_data | -s \n"
		"       [--read_data | -p \n"
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
            case 'p': 
                debug_ioctl_raw_read_data();
                break;
            case 'm': 
                debug_mmap();
                break;
            default:
                usage();
                break;
        }
    }
    return 0;
}

