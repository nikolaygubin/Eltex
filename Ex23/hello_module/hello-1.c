/* 
 * hello-1.c - The simplest kernel module. 
*/

#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void) 
{ 
    pr_info("Hello world 1.\n"); 
 
    /* A non 0 return means init_module failed; module can't be loaded. */ 
    return 0; 
} 
 
void cleanup_module(void) 
{ 
    pr_info("Goodbye world 1.\n"); 
} 
 
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello module");
MODULE_AUTHOR("Nikolay");
MODULE_VERSION("1.0");
