#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>

//包含platform驱动相关的头文件
#include <linux/ioport.h>
#include <linux/kobject.h>
#include <linux/klist.h>
#include <linux/lockdep.h>
#include <linux/compiler.h>
#include <linux/pm.h>
#include <asm/atomic.h>
#include <asm/device.h>
#include <linux/platform_device.h>
#include <linux/pm.h>

#include <mach/map.h>
#include <mach/gpio.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>

static struct resource key_resource[] = {
	[0] = {
		.start = EXYNOS4_GPX3(2),
		.end   = EXYNOS4_GPX3(5),
		.flags = IORESOURCE_IRQ,
	}
};

static void platform_buzzer_release(struct device * dev)
{
    return ;
}

/*定义一个platform_dervice变量*/
static struct platform_device my_device={
		.name =	"my_device2",
		.num_resources	  = ARRAY_SIZE(key_resource),
		.resource	  = key_resource,
		.dev = {
       	 	.release = platform_buzzer_release,
    }
};

static int __init my_dev_init(void)
{
	/*注册platform_device*/
	platform_device_register(&my_device);                                         
	return 0;
}

static void __exit my_dev_exit(void)
{
	/*注销platform_device*/
	platform_device_unregister(&my_device);                                 
}

module_init(my_dev_init);
module_exit(my_dev_exit);
MODULE_LICENSE("GPL");

