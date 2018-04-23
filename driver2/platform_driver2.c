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
#include <linux/gpio.h>
#include <linux/irq.h>




char a[4][2]={"0","1","2","3"};

static irqreturn_t btn_irq_handler(int irq, void *dev)
{
	char *p = (char *)dev;
	
	printk("%s\n",dev);
	
	return IRQ_HANDLED;
}

/* 编写一个探索函数（probe），函数只打印一些调试信息，然后返回0 */
static int my_probe(struct platform_device *pdev)
{
	printk(":: probe : device driver match success !\n");
	
	struct resource *key_resource=platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	printk("%x\n",key_resource);
	int irq;
	int i=0;
	while(i!=(key_resource->end-key_resource->start+1))
	{
		irq = gpio_to_irq(key_resource->start+i);
		request_irq(irq,btn_irq_handler,IRQ_TYPE_EDGE_FALLING, 
				a[i], a[i]);
		i++;
	}
	
	return 0;
}

/*编写一个移除函数(remove), 函数只打印一些调试信息，然后返回0*/
static int my_remove(struct platform_device *pdev)
{
	printk("the device kernel module is remove\n");
	
	struct resource *key_resource=platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	
	printk("%x\n",key_resource);
	int i=0;
	int irq;
	while(i!=(key_resource->end-key_resource->start+1))
	{
		irq = gpio_to_irq(key_resource->start+i);
		free_irq(irq, a[i]);
		i++;
	}
	
	return 0;
}

/*定义一个platform_driver变量*/
struct platform_driver my_drv={
		.probe = my_probe ,
		.remove = my_remove ,
		.driver = {
				.owner = THIS_MODULE,
				.name =  "my_device2",
		}
};

static int my_drv_init(void)
{
	/*注册platform_driver*/
	platform_driver_register(&my_drv);                           
	return 0;
}

static void my_drv_exit(void)
{
	/*注销platform_driver*/
	platform_driver_unregister(&my_drv);                           
}

module_init(my_drv_init);
module_exit(my_drv_exit);
MODULE_LICENSE("GPL");
