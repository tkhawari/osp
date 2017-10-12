#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h> /* for put_user */


int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "s3422192Device"
#define BUF_LEN 80
#define MAX_LENGTH 200
/*
 * Global variables
 */
static short size_of_msg;
static int Major;
static int Device_Open = 0;
static char msg[BUF_LEN];
static char *msg_Ptr; 

static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};

/*
 * Module is loaded when this function is called.
 */
int init_module(void)
{
  Major = register_chrdev(0, DEVICE_NAME, &fops);

  if (Major < 0) {
    printk(KERN_ALERT "Registering char device failed with %d\n", Major);
    unregister_chrdev(Major, DEVICE_NAME);
    return Major;
  }

  printk(KERN_INFO "Device has been registered as %d\n", Major);

  return SUCCESS;
}

/*** module is unloaded with this function ***/

void cleanup_module(void)
{
  if (Major < 0) {  
   unregister_chrdev(Major, DEVICE_NAME);
  }
}


/*
 * Called when a process tries to open the device file, like
 * "cat /dev/s3422192Device"
 */
static int device_open(struct inode *inode, struct file *filp)
{

  if (Device_Open)
    return -EBUSY;

  Device_Open++;

  return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *filp)
{
  Device_Open--;

  module_put(THIS_MODULE);
  return SUCCESS;
}

/*
 * Called when a process, which already opened the dev file, attempts to read
 * from it.
 */
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h   */
                           char *buffer,      /* buffer to fill with data */
                           size_t length,     /* length of the buffer     */
                           loff_t *offset)
{
	int error_count = 0;
	error_count = copy_to_user(buffer,msg,size_of_msg);
	if (error_count==0){            // if true then have success
      printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", size_of_msg);
      return (size_of_msg=0);  // clear the position to the start and return 0
   }
   else {
      printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;   
	}
}

/*
 * Called when a process writes to device file: This is assignment 2 message etc cat > /dev/s3422192Device
 */
static ssize_t
device_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
  /* int numBytes;
  int writeBytes;
  int maxBuffBytes;

  maxBuffBytes = MAX_LENGTH - *off;
  if(maxBuffBytes > len) writeBytes = len;
  else writeBytes = maxBuffBytes;

 if(writeBytes == 0){
	return -ENOSPC;
	}
  numBytes = writeBytes - copy_from_user(msg + writeBytes,buf,writeBytes);
  *off += numBytes;
  sprintf(msg,"message: %s",msg);
  return numBytes; */
 sprintf(msg, "%s", buf);
 size_of_msg = strlen(msg);
 printk(KERN_INFO "works");
 return len;
 
} 
