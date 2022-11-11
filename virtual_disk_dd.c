#include <init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/kfifo.h>

#define TEST_DEV_NAME "VirtualDisk"
#define TEST_DEV_MAJOR 246
#define BUFFER_SIZE 128

char *internel_buf;

int device_open(struct inode *inode, struct file *filep)
{
    int number = MINOR(inode->i_rdev);
    pritnk("[CHR_DEV] Virtual Character Device open : Minor Number: %d\n", number);

    return 0;
}

ssize_t chr_write(struct file *filep, const char *buf, size_t count, loff_t *f_ops)
{
    copy_from_user(internel_buf, buf, count);
    internel_buf[count] = 0;

    pritnk("[CHR_DEV] write data: %s\n", internel_buf);

    return count;
}

ssize_t chr_read(struct file *filep, const char *buf, size_t count, loff_t *f_ops)
{
    copy_to_user(buf, internel_buf, count);
    pritnk("[CHR_DEV] read data: %s\n", internel_buf);
    return count;
}
// device 2
long chr_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
    case 0:
        printk("[CHR_DEV] cmd value is %d\n", cmd);
        break;
    case 1:
        printk("[CHR_DEV] cmd value is %d\n", cmd);
        break;
    case 2:
        printk("[CHR_DEV] cmd value is %d\n", cmd);
        break;
    case 3:
        printk("[CHR_DEV] cmd value is %d\n", cmd);
        break;
    case 4:
        printk("[CHR_DEV] cmd value is %d\n", cmd);
        break;
    default:
        break;
    }
    return 0;

}

int chr_release(struct inode *inode, struct file *filep)
{
    printk("[CHR_DEV] Virtual Char Device Release\n");
    return 0; 
}

struct file_operations chr_fops =
{
    .owner = THIS_MOULE,
    .write = chr_write,
    .read = chr_read,
    .open = chr_open,
    .release = chr_release,
    .unlocked_ioctl = chr_ioctl,
};




int chr_init(void)
{
    int registration;

    internal_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);

    printk("[CHR_DEV] Registration Virtual Character Device to Kernel\n");
    registration = register_chrdev(CHR_DEV_MAJOR, CHR_DEV_NAME, &chr_fops);
    if (registration < 0)
    {
        return registeration;
    }
    printk("[CHR_DEV] Major number : %d\n", registeration);

    return 0;
    
}

void chr_exit(void)
{
    printk("[CHR_DEV] UnRegistration Virtual Character Device from Kernel\n");
    unregister_chrdev(CHR_DEV_MAJOR, CHR_DEV_NAME);
}

module_init(chr_init);
module_exit(chr_exit);

MODULE_LICENSE("GPL");