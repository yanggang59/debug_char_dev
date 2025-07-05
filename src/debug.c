#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/pci.h>
#include <linux/pci_regs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include "debug.h"


struct debug_cdev* g_debug;


static int debug_open(struct inode *inode, struct file *file)
{ 
    struct debug_cdev *debug;
	printk(KERN_INFO "\n%s: opened device", NAME);
    debug = container_of(inode->i_cdev, struct debug_cdev, cdev);
    file->private_data = debug;
	return 0;
}

static int debug_close(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "\n%s: device close", NAME);	
	return 0;
}


static ssize_t debug_read(struct file *file, char *dst, size_t count, loff_t *f_offset) 
{
	struct debug_cdev *debug = (struct debug_cdev *)file->private_data;
	LAB_DEBUG("debug_read: count = %ld, offset = %lld \r\n", count, *f_offset);
	if(*f_offset + count > debug->buf_size) {
		LAB_ERROR("over read debug file\r\n");
		return -EINVAL;
	}
	*f_offset += count;
	return count;
}

static ssize_t debug_write(struct file *file, const char *src, size_t count, loff_t *f_offset) 
{
	struct debug_cdev *debug = (struct debug_cdev *)file->private_data;
	LAB_DEBUG("debug_write: count = %ld, offset = %lld \r\n", count, *f_offset);
	if(*f_offset + count > debug->buf_size) {
		LAB_ERROR("over write debug file\r\n");
		return -EINVAL;
	}
	*f_offset += count;
	return count;
}

static long debug_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	struct my_data data_from_user = {0, 0};
	LAB_DEBUG("debug_ioctl , cmd = %#x, arg = %#lx\r\n", cmd, arg);
	
	switch(cmd)
	{
		case IOCTL_RAW_WRITE_INT:
			LAB_DEBUG("debug_ioctl raw write test, arg = %ld \r\n", arg);
			break;
		
		case IOCTL_RAW_WRITE_DATA:
			LAB_DEBUG("debug_ioctl raw write data test, \r\n");
			if (copy_from_user(&data_from_user, (void __user *)arg, sizeof(struct my_data)))
            	return -EFAULT;
			LAB_DEBUG("field1 = %d, field2 = %d\r\n", data_from_user.field1, data_from_user.field2);
			break;

		case IOCTL_RAW_READ:
			LAB_DEBUG("debug_ioctl raw read test \r\n");
			break;


		default:
			break;
	}
	return 0;
}

#if DEBUG_USE_MMAP
static int debug_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct debug_cdev *debug;
	unsigned long start, size;
	start = (unsigned long)vma->vm_start;
    size = (unsigned long)(vma->vm_end - vma->vm_start);
	debug = (struct debug_cdev *)filp->private_data;
	LAB_DEBUG("debug_mmap , size = %#lx \r\n", size);
	if(size >= debug->buf_size) {
		size = debug->buf_size;
	}
	if(remap_pfn_range(vma, start, virt_to_phys(debug->buf) >> PAGE_SHIFT, size, PAGE_SHARED)) {
		LAB_ERROR("debug_mmap failed\r\n");
		return -1;
	}
	return 0;
}
#endif

static struct file_operations debug_fops = {
	.owner = THIS_MODULE,
	.open = debug_open,
#if DEBUG_USE_MMAP
	.mmap = debug_mmap,
#endif
	.release = debug_close,
	.write = debug_write,
	.read = debug_read,
	.unlocked_ioctl = debug_ioctl,
};

void delete_debug_cdev(struct debug_cdev* debug)
{
	printk(KERN_INFO "\n%s: free module", NAME);
}

static int __init debug_init(void)
{
	struct debug_cdev* debug;
	g_debug = debug = kmalloc(sizeof(struct debug_cdev), GFP_KERNEL);
	if(!g_debug) {
		printk(KERN_ALERT "\n%s: failed to allocate debug_dev", NAME);
		return ENOMEM;
	}

    if (alloc_chrdev_region(&debug->cdevno, 0, 1, NAME) < 0) {
		printk(KERN_ALERT "\n%s: failed to allocate a major number", NAME);
		return -ENOMEM;
	}

    // allocate a device class
	if ((debug->class = class_create(THIS_MODULE, NAME)) == NULL) {
		printk(KERN_ALERT "\n%s: failed to allocate class", NAME);
		return -ENOMEM;
	}
	// allocate a device file
	if (device_create(debug->class, NULL, debug->cdevno, NULL, NAME) == NULL) {
		printk(KERN_ALERT "\n%s: failed to allocate device file", NAME);
		return -ENOMEM;
	}	

	cdev_init(&debug->cdev, &debug_fops);
	debug->cdev.owner = THIS_MODULE;

	debug->buf_size = BUF_LENGTH;
	// allocates a buffer of size BUF_LENGTH
	if ((debug->buf = kcalloc(BUF_LENGTH, sizeof(char), GFP_KERNEL)) == NULL) {
		printk(KERN_ALERT "\n%s: failed to allocate buffer", NAME);
		return -ENOMEM;
	}

	// add device to the kernel 
	if (cdev_add(&debug->cdev, debug->cdevno, 1)) {
		printk(KERN_ALERT "\n%s: unable to add char device", NAME);
		return -ENOMEM;
	}

	printk(KERN_INFO "\n%s: loaded module", NAME);
	return 0;

}
 
static void __exit debug_exit(void)
{
	if(!g_debug) return;
	if (g_debug->buf)
		kfree(g_debug->buf);	
	if (g_debug->class && g_debug->cdevno) {
		device_destroy(g_debug->class, g_debug->cdevno);
		class_destroy(g_debug->class);
		unregister_chrdev_region(g_debug->cdevno, 1);
	}
	kfree(g_debug);
	g_debug = NULL;
}
 
 module_init(debug_init);
 module_exit(debug_exit);

MODULE_LICENSE("Dual BSD/GPL");