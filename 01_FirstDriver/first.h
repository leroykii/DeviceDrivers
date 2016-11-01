#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/moduleparam.h>
#include <asm/errno.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <asm/ioctl.h>

typedef struct private_fd_data {
	int iminor;
	int imajor;
} private_fd_data; 


int scull_open(struct inode *pinode, struct file *pfile);
int scull_release(struct inode *pinode, struct file *pfile);
ssize_t scull_read(struct file* pfile, char* __user buffer, size_t length, loff_t* offset);
ssize_t scull_write(struct file* pfile, const char* __user buffer, size_t length, loff_t* offset);

loff_t scull_llseek(struct file *pfile, loff_t offset, int whence);
long scull_unlocked_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg);

int scull_flush(struct file *pfile, fl_owner_t id);

#define IOC_GET_BUFFERSIZE 1
#define IOC_GET_MAJOR 4
#define IOC_GET_MINOR 3
#define IOC_SET_MEM_SIZE 5
#define IOC_GET_MEM_SIZE 6
