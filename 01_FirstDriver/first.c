#include "first.h"

#define DEVICE_NAME "scull"
// #define FIRST_MAJOR 4
#define N_DEVICES 2
#define BARES_MAYOR 0 // Por defecto asignación dinámica

static int argv1;
module_param(argv1, int, S_IRUGO);

dev_t my_dev;
struct cdev *my_cdev;

ssize_t scull_read(
	struct file* pfile, 
	char* __user buffer, 
	size_t length, 
	loff_t* offset) 
{

	printk("Entered in scull_read()\n");

	return 0;
}
  
ssize_t scull_write(
	struct file* pfile, 
	const char* __user buffer, 
	size_t length, 
	loff_t* offset) 
{

	printk("Entered in scull_write()\n");
	return 0;
}


int scull_release(struct inode *pinode, struct file *pfile) {


	printk("Entered in closed\n");

	return 0;
}

int scull_open(struct inode *pinode, struct file *pfile) {
	
	int imajor = MAJOR(pinode->i_rdev);
	int iminor = MINOR(pinode->i_rdev);

	printk("Entered in open\n");
	printk("[open] MAJOR: %d\n", imajor);
	printk("[open] MINOR: %d\n", iminor);
	
	switch(iminor) {
		case 0: /* Device 0 */

		

		pfile->private_data = &imajor;






		
		break;

		
		case 1: /* Device 1 */
		
		break;
		
		default: /* Not a valid device */
		printk("The device whose minor number is %d does not exist.\n", iminor);
		return -ENODEV;
		break;
	}

	return 0;
}


struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	// .llseek = scull_llseek,
	.read = scull_read,
	.write = scull_write,
	// .ioctl = scull_ioctl,
	.open = scull_open,

	.release = scull_release
};


static int __init hello_init(void)
{
	

	printk(KERN_ALERT "Hello, world\n");
	printk(KERN_ALERT "Argv: %d\n", argv1);

	//if ((register_chrdev_region(FIRST_MAJOR, N_DEVICES, "scull")) < 0)
	if (alloc_chrdev_region(&my_dev, BARES_MAYOR, N_DEVICES, "gradiant"))
	{
		printk(KERN_ALERT "Error: register_chrdev_region\n");
		return -EAGAIN;
	}
	else printk(KERN_INFO "Region registered. M:%d, m:%d\n", MAJOR(my_dev), MINOR(my_dev));
	
	my_cdev = cdev_alloc();
	if (my_cdev == NULL) {
		printk(KERN_ALERT "Error: cdev_alloc()\n");
		return -EAGAIN;
	}
	kobject_set_name(&my_cdev->kobj, "my_cdev%d", N_DEVICES);

	cdev_init(my_cdev, &scull_fops);

	if (cdev_add(my_cdev, my_dev, N_DEVICES) < 0) 
	{
		printk(KERN_ALERT "Error: cdev_add()\n");
		return -EAGAIN;
	}

	return 0;
}
static void __exit hello_exit(void)
{
	cdev_del(my_cdev);
	unregister_chrdev_region(my_dev, N_DEVICES);

	printk(KERN_ALERT "Goodbye, cruel world\n");
}



module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("bares@gradiant.org");
MODULE_DESCRIPTION("Getting to know Linux Kernel");
MODULE_VERSION("0.0");