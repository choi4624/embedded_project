#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/kfifo.h>
#include <linux/slab.h> 
#include <linux/uaccess.h>

#define TEST_DEV_NAME "VirtualDisk"
#define TEST_DEV_MAJOR 246
#define BUFFER_SIZE 128


static int buffer_size = 16;
static int read_buffer_size = 2;
module_param(buffer_size, int, 0000);   
module_param(read_buffer_size, int, 0000);


struct kfifo fifo_buffer; // 내부 버퍼

#define CH_WRITE_BUFFER_SIZE _IOW(DEV_MAJOR_NUMBER,1,int)
#define CH_READ_BUFFER_SIZE _IOW(DEV_MAJOR_NUMBER,2,int)


int device_open(struct inode *inode, struct file *filep)
{
    int number = MINOR(inode->i_rdev);
    pritnk("[CHR_DEV] Virtual Character Device open : Minor Number: %d\n", number);

    return 0;
}

ssize_t chr_write(struct file *filep, const char *buf, size_t count, loff_t *f_ops)
{
    unsigned int val;
    char * buffer;
    int ret;
    int i =0; 
    char letter;
    printk(KERN_INFO "BufferedMem_write\n");
    buffer= kmalloc(count, GFP_KERNEL);

    val = count;
    copy_from_user(buffer, buf, count);

    for(i = 0; i<count;i++) // 한글자씩 채우기  
    {
        if(kfifo_is_full(&fifo_buffer)){
            kfifo_out(&fifo_buffer, &letter, sizeof(letter));
        }
        letter=write_buffer[i]; 
        kfifo_in(&fifo_buffer,&letter,sizeof(letter));  
        printk(KERN_INFO "%c", letter);
    }
    printk(KERN_INFO "\n", letter);

    pritnk("[CHR_DEV] write data: %s\n", buffer);
    printk(KERN_INFO "kfifo_in ok\n");
    return count;
}

ssize_t chr_read(struct file *filep, const char *buf, size_t count, loff_t *f_ops)
{
    unsigned int val;
    char * buffer;
    int ret;
    int i =0; 
    char letter;

    
    // kfifo 할당 

    buffer= kmalloc(read_buffer_size, GFP_KERNEL);

    printk(KERN_INFO "reuslt\n");
    for ( i = 0; i < read_buffer_size; i++)
    {
        if (!kfifo_is_empty(&fifo))
        {
        ret = kfifo_out(&fifo, &letter, sizeof(letter));
        buffer[i] = letter;
        printk(KERN_INFO "%c\n", letter);
        }    
    }
    
    printk(KERN_INFO "kfifo size: %d\n", kfifo_size(&fifo));
    printk(KERN_INFO "kfifo len: %d\n", kfifo_len(&fifo));
    printk(KERN_INFO "kfifo avail: %d\n", kfifo_avail(&fifo));
    
    val = read_buffer_size;
    copy_to_user(buf,buffer,read_buffer_size)

    printk(KERN_INFO "read ok\n");

    return 0;
}

// device 2 ES-10-device%20driver.pdf

long chr_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    int size;
    struct kfifo temp;
    char letter;

    switch (cmd)
    {
        
    case CH_WRITE_BUFFER_SIZE:
        printk("[CHR_DEV] cmd value is %d\n", cmd);
        copy_from_user(&size,(void __user *)arg,sizeof(size));
        kfifo_alloc(&temp, buffer_size, GFP_KERNEL);

        while (!kfifo_is_empty(&fifo_buffer)){  
                kfifo_out(&fifo_buffer, &letter, sizeof(letter));
                kfifo_in(&temp,&letter,sizeof(letter));
            }

        
        buffer_size = size;
        kfifo_free(&fifo_buffer);
        kfifo_alloc(&fifo_buffer, buffer_size, GFP_KERNEL);
        while (!kfifo_is_empty(&temp))
            {
                if(kfifo_is_full(&fifo_buffer)){ 
                    kfifo_out(&fifo_buffer, &letter, sizeof(letter));
                }
                kfifo_out(&temp, &letter, sizeof(letter));
                kfifo_in(&fifo_buffer,&letter,sizeof(letter));
            }
        kfifo_free(&temp);

        size = kfifo_size(&fifo_buffer);
        printk(KERN_INFO "kfifo size: %d\n", kfifo_size(&fifo));
        printk(KERN_INFO "kfifo len: %d\n", kfifo_len(&fifo));
        printk(KERN_INFO "kfifo avail: %d\n", kfifo_avail(&fifo));
        copy_to_user((void __user *)arg, &size, sizeof(size))
        break;

    case CH_READ_BUFFER_SIZE:
        read_buffer_size = size;
        printk("[CHR_DEV] cmd value is %d\n", cmd);
        copy_from_user(&size,(void __user *)arg,sizeof(size))
        printk(KERN_INFO "read buffer size changed : %d \n", read_buffer_size);
        break;

    default:
        read_buffer_size
        printk("[CHR_DEV] cmd value is %d default route\n", cmd);
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
    .open = chr_open, // NOT USED
    .release = chr_release, // NOT USED
    .unlocked_ioctl = chr_ioctl, // change size 
};




int chr_init(void)
{
    // 장치 등록 
    int registration, student;
    student = 201714038;
    printk(KERN_INFO "virutalDisk %d device driver registerd \n", student); 

//  HB-ES-101-4-Kernel API-1 > kfifo 새 큐 만드는 함수 
    if (kfifo_alloc(&fifo_buffer, buffer_size, GFP_KERNEL)){
        printk(KERN_ERR "error kfifo_alloc\n");
    }

    printk("[CHR_DEV] Registration Virtual Character Device to Kernel\n");
    registration = register_chrdev(CHR_DEV_MAJOR, CHR_DEV_NAME, &chr_fops);
    if (registration < 0)
    {
        return registration;
    }
    printk("[CHR_DEV] Major number : %d buffer size : %d\n", registration,kfifo_size(&fifo_buffer));

    return 0;
    
}

void chr_exit(void)
{
    int student = 201714038;
    printk("[CHR_DEV] UnRegistration Virtual Character Device from Kernel\n");
    // kfifo 해제, 장치 해제 

    kfifo_free(&fifo);
    unregister_chrdev(CHR_DEV_MAJOR, CHR_DEV_NAME);
    printk(KERN_INFO "virutalDisk %d device driver unregisterd \n", student); 
}

module_init(chr_init);
module_exit(chr_exit);

MODULE_LICENSE("GPL");