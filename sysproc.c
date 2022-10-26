#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
/*int thread_create(void (*fn) (void*), void* stack, void* arg);
int thread_join(void);
int thread_exit(void);
int lock_init(lock_t*);
int lock_acquire(lock_t*);
int lock_release(lock_t*);*/

int
sys_thread_create(void) {
  int fn, arg1, arg2, stack;
  if(argint(0, &fn)<0 || argint(1, &arg1)<0 || argint(2, &arg2)<0 || argint(3, &stack)<0)
    return -1;
  return thread_create((void *)fn, (void *)arg1, (void *)arg2, (void *)stack);
}

int 
sys_thread_join(void) {
    void* stack;
    if (argptr(0, (void*)&stack, sizeof(*stack)) < 0) {
        return -1;
    }
    return thread_join(stack);
}
int 
sys_thread_exit(void) {
    return thread_exit();
}
int 
sys_lock_init(void) {
    lock_t* lock;
    if (argptr(0, (void*)&lock, sizeof(*lock)) < 0) {
        return -1;
    }

    return lock_init(lock);
}
int 
sys_lock_acquire(void) {
    lock_t* lock;
    if (argptr(0, (void*)&lock, sizeof(*lock)) < 0) {
        return -1;
    }
    return lock_acquire(lock);

}
int 
sys_lock_release(void) {
    lock_t* lock;
    if (argptr(0, (void*)&lock, sizeof(*lock)) < 0) {
        return -1;
    }
    return lock_release(lock);

}