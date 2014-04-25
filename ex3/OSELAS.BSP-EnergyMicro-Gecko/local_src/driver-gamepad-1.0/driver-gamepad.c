/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#include "efm32gg.h"

static void interrupt_handler(int irq, void *dev_id, struct pt_regs *regs);


/*
 * module_init - function to insert this module into kernel space
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

    err = request_mem_region(GPIO_PA_BASE, num_ports, "gamepad driver");
    if (err) return -1;
    ioremap_nocache(GPIO_PA_BASE, num_ports);

    iowrite32(0x33333333, GPIO_PC_MODEL);
    iowrite32(0xff, GPIO_PC_DOUT);
    iowrite32(0x22222222, GPIO_EXTIPSELL);

    iowrite32(0xff, GPIO_IEN);
    iowrite32(0xff, GPIO_IFC);

    request_irq(17, interrupt_handler, 0, "gamepad driver", NULL);
    request_irq(18, interrupt_handler, 0, "gamepad driver", NULL);

    iowrite32(0xFF, GPIO_EXTIFALL);
    iowrite32(0x00FF, GPIO_IEN);
    iowrite32(0xFF, GPIO_IFC);

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

    // Disable interrupts
    iowrite32(0x0000, GPIO_IEN);
    free_irq(17);
    free_irq(18);

    release_mem_region(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE);
}

static void interrupt_handler(int irq, void *dev_id, struct pt_regs *regs) {

}

module_init(driver_init);
module_exit(driver_cleanup);

MODULE_DESCRIPTION("Driver for gamepad");
MODULE_LICENSE("GPL");

