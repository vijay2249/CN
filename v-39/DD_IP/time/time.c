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

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}

char *b;

int atoi(const char * str)
{
    int res = 0,i=0; // Initialize result
  
    // Iterate through all characters of input string and
    // update result
    for (i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
  
    // return result.
    return res;
}


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
     char *b=kmalloc(40*sizeof(char),GFP_KERNEL);
   struct timeval t1;
   do_gettimeofday(&t1);
   sprintf(b,"%ld",t1.tv_sec);
   copy_to_user(c,b,strlen(b));
   printk("Reading\n");  
   return t;
   // return 0;
}
ssize_t write_TickTock(struct file *f, const char *c, size_t t, loff_t *o){
    char *b=kmalloc(30*sizeof(char),GFP_KERNEL);
  copy_from_user(b,c,t);
  long int i=atoi(b);
  struct timespec ab;
  ab.tv_sec=i;
  ab.tv_nsec=0;
 // printk("%s",buff);
  do_settimeofday(&ab);
 
    printk("Write inside Ticktock ");
    return t;
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


