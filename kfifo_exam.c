#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/kfifo.h>

struct kfifo fifo;

static int kfifo_exam_init(void){
    unsigned int val;
    int ret;
    int i =0; 
    printk(KERN_INFO"kfifo_exam_init!\n");
    ret = kfifo_alloc(&fifo, 64, GFP_KERNEL);
    if (ret)
    {
        printk(KERN_ERR "error kfifo_alloc\n");
    }
    printk(KERN_INFO "kfifo size: %d\n", kfifo_size(&fifo));
    printk(KERN_INFO "kfifo len: %d\n", kfifo_len(&fifo));
    printk(KERN_INFO "kfifo avail: %d\n", kfifo_avail(&fifo));
    while (!kfifo_is_full(&fifo))
    {
        kfifo_in(&fifo, &i, sizeof(i));
        i++;
    }
    printk(KERN_INFO "kfifo_in ok\n");

    printk(KERN_INFO "kfifo size: %d\n", kfifo_size(&fifo));
    printk(KERN_INFO "kfifo len: %d\n", kfifo_len(&fifo));
    printk(KERN_INFO "kfifo avail: %d\n", kfifo_avail(&fifo));

    printk(KERN_INFO "reuslt\n");
    while (!kfifo_is_empty(&fifo))
    {
        ret = kfifo_out(&fifo, &val, sizeof(val));
        printk(KERN_INFO "%u\n", val);
        
    }
    return 0;
}

static void kfifo_exam_exit(void){
    printk(KERN_INFO"kfifo_exam_exit!\n");
    kfifo_free(&fifo);

}

module_init(kfifo_exam_init);
module_exit(kfifo_exam_exit);
MODULE_LICENSE("GPL");