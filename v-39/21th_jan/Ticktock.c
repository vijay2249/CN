#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>


MODULE_LICENSE("GPL");

int open_TickTock(struct inode *i, struct file *f);
int close_TickTock(struct inode *i, struct file *f);
ssize_t read_TickTock(struct file *f, char *c, size_t t, loff_t *o);
ssize_t write_TickTock(struct file *f, const char *c, size_t t, loff_t *o);
int Major;  

static struct file_operations fileops = {
  .read = read_TickTock, 
  .write = write_TickTock,
  .open = open_TickTock,
  .release = close_TickTock
};


int Tick_init(void){
  	Major = register_chrdev(0,"TickTock CDD", &fileops);
 		if(Major>0){
 			printk("Module inserted into kernell & ready for request\n");
 			return 0;
 		}else{
 			printk("Module not inserted properly !!\n"); 
           return -1;
 		}
}
module_init(Tick_init);

 void Tock_exit(void)
{
 	unregister_chrdev(0, "TickTock CDD");
 	printk("Goodbye TickTock Module!\n");
}
module_exit(Tock_exit);




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
    return 0;
}
ssize_t write_TickTock(struct file *f, const char *c, size_t t, loff_t *o){
    printk("Write inside Ticktock ");
    return 0;
}