#include "first.h"

#define N_DEVICES 2
#define BARES_MAYOR 0 // Por defecto asignación dinámica


#define BLOCKS_MEMORY 8
#define BLOCKTOTALBUFFER_SIZE 128
int TOTALBUFFER_SIZE = BLOCKS_MEMORY * BLOCKTOTALBUFFER_SIZE;

char* grd_memory;

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

// TODO ver si offset es el mismo que está en pfile

	unsigned long not_copied = 0;
	char* pmem = grd_memory;
	int bytes_to_read = length; 

	printk("[m:] Entered in scull_read() [offset: %lld]\n", *offset);

	if (*offset >= TOTALBUFFER_SIZE) {
		printk(KERN_ALERT "Error: Offset is beyond the allocated memory range.\n");
		return -EFAULT;
	}

	if (*offset + bytes_to_read >= TOTALBUFFER_SIZE) {
		bytes_to_read = TOTALBUFFER_SIZE - *offset;
		printk(KERN_INFO "Warning: Read would go over memory range so it has been reduced.\n");
	}

	not_copied = copy_to_user(__user buffer, pmem + *offset, bytes_to_read);
	
	if (not_copied > 0) { 
		printk(KERN_ALERT "Error: %lu bytes could not be copied. \n", not_copied);
		return -EFAULT;
	}

	printk(KERN_INFO "Bytes read: %ld\n", bytes_to_read - not_copied);

	*offset += bytes_to_read;
	return bytes_to_read;
}

ssize_t scull_write(
	struct file* pfile, 
	const char* __user buffer, 
	size_t length, 
	loff_t* offset) 
{

	unsigned long not_copied = 0;
	char* pmem = grd_memory;
	int bytes_to_write = length; 

	printk("[m:] Entered in scull_write() [offset: %lld]\n", *offset);

	if (*offset >= TOTALBUFFER_SIZE) {
		printk(KERN_ALERT "Error: Offset is beyond the allocated memory range.\n");
		return -EFAULT;
	}

	if (*offset + bytes_to_write >= TOTALBUFFER_SIZE) {
		bytes_to_write = TOTALBUFFER_SIZE - *offset;
		printk(KERN_INFO "Warning: Read would go over memory range so it has been reduced.\n");
	}

	not_copied = copy_from_user(pmem + *offset, __user buffer, bytes_to_write);

	if (not_copied > 0) { 
		printk(KERN_ALERT "Error: %lu bytes could not be copied. \n", not_copied);
		return -EFAULT;
	}
	
	*offset += bytes_to_write;
	return bytes_to_write;
}


int scull_release(struct inode *pinode, struct file *pfile) {

	printk("Entered in release()\n");

	kfree(pfile->private_data);

	return 0;
}

int scull_open(struct inode *pinode, struct file *pfile) {
	
	int imajor = MAJOR(pinode->i_rdev);
	int iminor = MINOR(pinode->i_rdev);
	private_fd_data* fd_data = kmalloc(sizeof(private_fd_data), GFP_KERNEL);

	printk("Entered in open()\n");
	printk("[open] MAJOR: %d\n", imajor);
	printk("[open] MINOR: %d\n", iminor);

	fd_data->imajor = imajor;
	fd_data->iminor = iminor;
	
	switch(iminor) {
		case 0: /* Device 0 */
		pfile->private_data = (void*)fd_data;
		break;
		
		case 1: /* Device 1 */
		pfile->private_data = (void*)fd_data;
		break;
		
		default: /* Not a valid device */
		printk("The device whose minor number is %d does not exist.\n", iminor);
		return -ENODEV;
		break;
	}

	return 0;
} 


loff_t scull_llseek(struct file *pfile, loff_t offset, int whence) {

	printk(KERN_INFO "Entered in llseek(): offset %lld, whence %d", offset, whence);

	/* Check offset is within the allocated memory region */
	if (offset >= BLOCKTOTALBUFFER_SIZE * BLOCKS_MEMORY) 
		return -EFAULT; 

	pfile->f_pos = offset;

	return 0;
}

//int scull_ioctl (struct inode *pinode, struct file *pfile, 
//	unsigned int cmd, unsigned long arg) 


long scull_unlocked_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg) {

	private_fd_data* fd_data = (private_fd_data*) pfile->private_data;
	
	printk(KERN_INFO "IOCTL M:%d, m:%d\n", fd_data->imajor, fd_data->iminor);

	switch (cmd) {	

		case IOC_GET_BUFFERSIZE:
		printk(KERN_INFO "IOCTL (IOC_GET_BUFFERSIZE)\n");
		*((int*)arg) = TOTALBUFFER_SIZE;
		return 0;
		break;

		case IOC_GET_MAJOR:
		printk(KERN_INFO "IOCTL (IOC_GET_MAJOR)\n");
		copy_to_user((int*)arg, &fd_data->imajor, sizeof(int));
		return 0;
		break;

		case IOC_GET_MINOR:
		printk(KERN_INFO "IOCTL (IOC_GET_MINOR)\n");
		copy_to_user((int*)arg, &fd_data->iminor, sizeof(int));
		return 0;
		break;

		case IOC_SET_MEM_SIZE:
		printk(KERN_INFO "IOCTL (IOC_SET_MEM_SIZE)\n");
		copy_from_user(&TOTALBUFFER_SIZE , (int*)arg, sizeof(int));
		kfree(grd_memory);
		grd_memory = NULL;
		grd_memory = kmalloc(TOTALBUFFER_SIZE, GFP_KERNEL);
		if (grd_memory == NULL) {
			printk(KERN_INFO "Warning: Memory havent' been allocated.\n");
			return -EAGAIN;
		}
		printk(KERN_INFO "[ioctl] Memory resized to %d bytes\n", TOTALBUFFER_SIZE);
		pfile->f_pos = 0;
		
		return 0;

		case IOC_GET_MEM_SIZE:
		printk(KERN_INFO "IOCTL (IOC_GET_MEM_SIZE)\n");
		copy_to_user((int*)arg, &TOTALBUFFER_SIZE, sizeof(int));
		return 0;

		default:
		printk(KERN_INFO "Warning: Unknown IOCTL command. Try again\n");
		return -EAGAIN;
	}
	

	return -EFAULT;
}


int scull_flush(struct file *pfile, fl_owner_t id) {

	return 0;
}



struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.llseek = scull_llseek,
	.read = scull_read,
	.write = scull_write,
	.unlocked_ioctl = scull_unlocked_ioctl,
	.open = scull_open,
	// .mmap = scull_mmap, 
	.flush = scull_flush, 
	// .fallocate
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

	grd_memory = kmalloc(BLOCKS_MEMORY * BLOCKTOTALBUFFER_SIZE, GFP_KERNEL);
	if (grd_memory == NULL) {
		printk(KERN_ALERT "Error: kmalloc()\n");
		return -EAGAIN;
	}
	else printk(KERN_INFO "grd_memory allocated (%d bytes)\n", BLOCKS_MEMORY * BLOCKTOTALBUFFER_SIZE);
	memset(grd_memory, 0x0, BLOCKS_MEMORY * BLOCKTOTALBUFFER_SIZE);

	return 0;
}
static void __exit hello_exit(void)
{
	kfree(grd_memory);
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