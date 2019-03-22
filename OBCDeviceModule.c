#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>


#define MAJOR_NUMBER 61/* forward declaration */


int onebyte_open(struct inode *inode, struct file *filep);
int onebyte_release(struct inode *inode, struct file *filep);
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);
static void onebyte_exit(void);


/* definition of file_operation structure */
struct file_operations onebyte_fops = {
	read: onebyte_read,
	write: onebyte_write,
	open: onebyte_open,
	release: onebyte_release
};

char *onebyte_data = NULL;

int onebyte_open(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}
int onebyte_release(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{
	int bytes_read = 0;
	
	/* Check if the buffer has been written */
	if(*buf != 0){
		return 0;	
	}
	copy_to_user(buf, onebyte_data, sizeof(char));
	
	bytes_read ++;
	return bytes_read;
}
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
	int bytes_write = 0;	
	copy_from_user(onebyte_data, buf, sizeof(char));

	/* Check the length of the bytes that have been written*/
	if(count > sizeof(char))
	{
		printk(KERN_ALERT "No space left on device\n");

		/*Return Linux System Error<28>: No space left on device */
		return -ENOSPC; 
	}

	bytes_write ++;
	return bytes_write;
}
