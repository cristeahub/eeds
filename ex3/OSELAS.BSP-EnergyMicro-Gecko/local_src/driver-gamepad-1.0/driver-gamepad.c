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

#include "efm32gg.h"

static dev_t device_number;
struct cdev driver_cdev;

#define DRIVER_NAME "gamepad"

static int __init driver_init(void);
static int __exit driver_cleanup(void);
static irqreturn_t interrupt_handler(int irq, void *dev_id, struct pt_regs *regs);

/* user program opens the driver */
static int driver_open(struct inode *inode, struct file *filp);

/* user program closes the driver */
static int driver_release(struct inode *inode, struct file *filp);

/* user program reads from the driver */
static ssize_t driver_read(struct file *filp, char __user *buff,
        size_t count, loff_t *offp);

/* user program writes to the driver */
static ssize_t driver_write(struct file *filp, const char __user *buff,
        size_t count, loff_t *offp);

module_init(driver_init);
module_exit(driver_cleanup);

MODULE_DESCRIPTION("Driver for gamepad");
MODULE_LICENSE("GPL");

static struct file_operations driver_fops = {
    .owner = THIS_MODULE,
    .read = driver_read,
    .write = driver_write,
    .open = driver_open,
    .release = driver_release
};

/*
 * driver_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init driver_init(void)
{
    printk(KERN_INFO "Loading driver module...\n");

    unsigned long num_ports = GPIO_IFC - GPIO_PA_BASE; //GPIO_IFC is the last used address in GPIO

    cdev_init(&driver_cdev, &driver_fops);

    err = request_mem_region(GPIO_PA_BASE, num_ports, DRIVER_NAME);
    if (err) return -1;
    ioremap_nocache(GPIO_PA_BASE, num_ports);

    iowrite32(0x33333333, GPIO_PC_MODEL);
    iowrite32(0xff, GPIO_PC_DOUT);
    iowrite32(0x22222222, GPIO_EXTIPSELL);

    iowrite32(0xff, GPIO_IEN);
    iowrite32(0xff, GPIO_IFC);

    request_irq(17, interrupt_handler, 0, DRIVER_NAME, &driver_cdev);
    request_irq(18, interrupt_handler, 0, DRIVER_NAME, &driver_cdev);

    iowrite32(0xFF, GPIO_EXTIFALL);
    iowrite32(0x00FF, GPIO_IEN);
    iowrite32(0xFF, GPIO_IFC);

    /* Dynamically allocate device numbers */
    int result = alloc_chrdev_region(&device_number, 0, 1, DRIVER_NAME);

    if (result < 0) {
        printk(KERN_ALERT "Failed to allocate device numbers\n");
        return -1;
    }

    printk(KERN_INFO "Gamepad driver loaded.\n");
    return 0;
}

/*
 * module_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit driver_cleanup(void)
{
    printk(KERN_INFO "Unloading driver module...\n");

    unregister_chrdev_region(device_number, 1);

    // Disable interrupts
    iowrite32(0x0000, GPIO_IEN);
    free_irq(17, &driver_cdev);
    free_irq(18, &driver_cdev);

    release_mem_region(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE);
}

static void interrupt_handler(int irq, void *dev_id, struct pt_regs *regs) {

}



