#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");

int Tick_init(void){

 	printk("Module inserted into kernell & ready for request\n");
 	return 0;
}
module_init(Tick_init);

 void Tock_exit(void)
{
 	printk("Goodbye TickTock Module!\n");
}
module_exit(Tock_exit);




