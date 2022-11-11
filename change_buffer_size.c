#include <init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/kfifo.h>

#define TEST_DEV_NAME "VirtualDisk"
#define TEST_DEV_MAJOR 246

char *internel_buf;

int device_open(struct inode *inode, struct file *filep)
{
    int number = MINOR(inode->i_rdev);
    pritnk("CHR_DEV")
}