/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>

#include "efm32gg.h"

static dev_t device_number;
struct cdev driver_cdev;
struct class *cl;
struct fasync_struct *async_queue;

#define DRIVER_NAME "gamepad"

static int __init gamepad_init(void);
static void __exit gamepad_cleanup(void);
static irqreturn_t interrupt_handler(int irq, void *dev_id, struct pt_regs *regs);

/* user program opens the driver */
static int gamepad_open(struct inode *inode, struct file *filp);

/* user program closes the driver */
static int gamepad_release(struct inode *inode, struct file *filp);

/* user program reads from the driver */
static ssize_t gamepad_read(struct file *filp, char __user *buff,
        size_t count, loff_t *offp);

/* user program writes to the driver */
static ssize_t gamepad_write(struct file *filp, const char __user *buff,
        size_t count, loff_t *offp);

/* fasync */
static int gamepad_fasync(int fd, struct file *filp, int mode);

module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Driver for gamepad");
MODULE_LICENSE("GPL");

static struct file_operations driver_fops = {
    .owner = THIS_MODULE,
    .read = gamepad_read,
    .write = gamepad_write,
    .open = gamepad_open,
    .release = gamepad_release,
    .fasync = gamepad_fasync
};

/*
 * driver_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init gamepad_init(void)
{
    printk(KERN_INFO "Loading the driver module...\n");

    int err;

    /* Dynamically allocate device numbers */
    err = alloc_chrdev_region(&device_number, 0, 1, DRIVER_NAME);

    if (err < 0) {
        printk(KERN_ALERT "Failed to allocate device numbers\n");
        goto fail_alloc_chrdev_region;
    }

    cdev_init(&driver_cdev, &driver_fops);
    driver_cdev.owner = THIS_MODULE;
    cdev_add(&driver_cdev, device_number, 1);
    cl = class_create(THIS_MODULE, DRIVER_NAME);
    device_create(cl, NULL, device_number, NULL, DRIVER_NAME);

    err = request_mem_region(GPIO_PC_DIN, 1, DRIVER_NAME);
    if (err == NULL) {
        printk(KERN_ALERT "Region not available\n");
        goto fail_request_mem_region;
    }

    iowrite32(0x33333333, GPIO_PC_MODEL);
    iowrite32(0xff, GPIO_PC_DOUT);
    iowrite32(0x22222222, GPIO_EXTIPSELL);

    iowrite32(0xff, GPIO_IEN);
    iowrite32(0xff, GPIO_IFC);

    request_irq(17, (irq_handler_t)interrupt_handler, 0,
            DRIVER_NAME, &driver_cdev);
    request_irq(18, (irq_handler_t)interrupt_handler, 0,
            DRIVER_NAME, &driver_cdev);

    iowrite32(0xFF, GPIO_EXTIFALL);

    printk(KERN_INFO "Gamepad driver loaded.\n");
    return 0;

fail_request_mem_region:
    device_destroy(cl, device_number);
    class_destroy(cl);
    cdev_del(&driver_cdev);

    unregister_chrdev_region(device_number, 1);
fail_alloc_chrdev_region:
    return err;
}

/*
 * module_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit gamepad_cleanup(void)
{
    printk(KERN_INFO "Unloading driver module...\n");

    // Disable interrupts
    iowrite32(0x0000, GPIO_IEN);
    free_irq(17, &driver_cdev);
    free_irq(18, &driver_cdev);

    release_mem_region(GPIO_PC_DIN, 1);

    /* Remove device */
    device_destroy(cl, device_number);
    class_destroy(cl);
    cdev_del(&driver_cdev);

    unregister_chrdev_region(device_number, 1);
}

static irqreturn_t interrupt_handler(int irq, void *dev_id, struct pt_regs *regs) {
    printk(KERN_INFO "Handle interrupt\n");
    iowrite32(0xff, GPIO_IFC);

    if(async_queue) {
        kill_fasync(&async_queue, SIGIO, POLL_IN);
    }

    return IRQ_HANDLED;
}

static int gamepad_open(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "Opening gamepad driver.\n");
    return 0;
}

static int gamepad_release(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "Closing gamepad driver.\n");
    return 0;
}

static ssize_t gamepad_read(struct file *filp, char __user *buff,
        size_t count, loff_t *offp)
{
    uint32_t data = ioread32(GPIO_PC_DIN);
    copy_to_user(buff, &data, 1);
    return 1;
}

static ssize_t gamepad_write(struct file *filp, const char __user *buff,
        size_t count, loff_t *offp)
{
    return 0;
}

static int gamepad_fasync(int fd, struct file *filp, int mode) {
    return fasync_helper(fd, filp, mode, &async_queue);
}
