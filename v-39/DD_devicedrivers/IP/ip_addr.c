#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>


#include<linux/kernel.h>
#include<linux/version.h>
#include<linux/string.h>
#include<linux/uaccess.h>
#include<linux/time.h>
#include<linux/slab.h>


 

MODULE_LICENSE("GPL");

int open_TickTock(struct inode *i, struct file *f);
int close_TickTock(struct inode *i, struct file *f);
ssize_t read_TickTock(struct file *f, char *c, size_t t, loff_t *o);
ssize_t write_TickTock(struct file *f, const char *c, size_t t, loff_t *o);
int Major;  


int open_TickTock(struct inode *i, struct file *f){
    printk("Opened inside Ticktock ");
    return 0;
}
int close_TickTock(struct inode *i, struct file *f){
    printk("Closed inside Ticktock ");
    return 0;
}
ssize_t read_TickTock(struct file *f, char *c, size_t t, loff_t *o){
    printk("Read from Ticktock ");
     int n;
    struct ifreq ifr;
    char array[] = "eth0";
 
    n = socket(AF_INET, SOCK_DGRAM, 0);
    //Type of address to retrieve - IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;
    //Copy the interface name in the ifreq structure
    strncpy(ifr.ifr_name , array , IFNAMSIZ - 1);
    ioctl(n, SIOCGIFADDR, &ifr);
    close(n);
    //display result
    printf("IP Address is %s - %s\n" , array , inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr) );
  
    return 0;
}
ssize_t write_TickTock(struct file *f, const char *c, size_t t, loff_t *o){
    printk("Write inside Ticktock ");
    return 0;
}

static struct file_operations fileops = {
  .read = read_TickTock, 
  .write = write_TickTock,
  .open = open_TickTock,
  .release = close_TickTock
};


int Tick_init(void){
  	Major = register_chrdev(0, "mydevdri3", &fileops);
 		if(Major>0){
 			printk("Module inserted into kernell & ready for request %d\n",Major);
 			return 0;
 		}else{
 			printk("Module not inserted properly !!\n"); 
            return -1;
 		}
}

 void Tock_exit(void)
{
 	unregister_chrdev(Major, "mydevdri3");
 	printk("Goodbye TickTock Module!\n");
}

module_init(Tick_init);

module_exit(Tock_exit);


