#include "assert.h"
#include "error.h"
#include "syscall.h"
#include "trap.h"

static inline int32_t do_syscall0(int syscallno) {
  int32_t ret;
  asm volatile("int %1\n"
    : "=a" (ret)
    : "i" (T_SYSCALL),
      "a" (syscallno)
    : "cc", "memory");
  return ret;
}
static inline int32_t do_syscall1(int syscallno, uint32_t a1) {
  int32_t ret;
  asm volatile("int %1\n"
    : "=a" (ret)
    : "i" (T_SYSCALL),
      "a" (syscallno),
      "d" (a1)
    : "cc", "memory");
  return ret;
}
static inline int32_t do_syscall2(int syscallno, uint32_t a1, uint32_t a2) {
  int32_t ret;
  asm volatile("int %1\n"
    : "=a" (ret)
    : "i" (T_SYSCALL),
      "a" (syscallno),
      "d" (a1),
      "c" (a2)
    : "cc", "memory");
  return ret;
}
static inline int32_t do_syscall3(int syscallno, uint32_t a1, uint32_t a2, uint32_t a3) {
  int32_t ret;
  asm volatile("int %1\n"
    : "=a" (ret)
    : "i" (T_SYSCALL),
      "a" (syscallno),
      "d" (a1),
      "c" (a2),
      "b" (a3)
    : "cc", "memory");
  return ret;
}
static inline int32_t do_syscall4(int syscallno, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4) {
  int32_t ret;
  asm volatile("int %1\n"
    : "=a" (ret)
    : "i" (T_SYSCALL),
      "a" (syscallno),
      "d" (a1),
      "c" (a2),
      "b" (a3),
      "D" (a4)
    : "cc", "memory");
  return ret;
}
static inline int32_t do_syscall5(int syscallno, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5) {
  int32_t ret;
  asm volatile("int %1\n"
    : "=a" (ret)
    : "i" (T_SYSCALL),
      "a" (syscallno),
      "d" (a1),
      "c" (a2),
      "b" (a3),
      "D" (a4),
      "S" (a5)
    : "cc", "memory");
  return ret;
}


__attribute__((noreturn)) void sys_exit(int code) {
  do_syscall1(SYS_exit, (uint32_t) code);
  panic("SYS_exit returned");
}

void sys_printk(const char *out, size_t size) {
  assert(do_syscall2(SYS_printk, (uint32_t) out, (uint32_t) size) == E_SUCCESS);
}

void sys_sleep(int ticks) {
  do_syscall1(SYS_sleep, (uint32_t) ticks); // this call won't return success
}

KeyboardListener sys_listenKeyboard(KeyboardListener handler) {
  return (KeyboardListener) do_syscall1(SYS_listenKeyboard, (uint32_t) handler);
}

ClockListener sys_listenClock(ClockListener handler) {
  return (ClockListener) do_syscall1(SYS_listenClock, (uint32_t) handler);
}

uint8_t sys_drawPoint(uint16_t x, uint16_t y, uint8_t color) {
  int32_t result = do_syscall3(SYS_drawPoint, x, y, color);
  assert(result >= E_SUCCESS);
  return (uint8_t) result;
}

__attribute__((noreturn)) void sys_crash() {
  for (;;) do_syscall0(SYS_crash);
}

int sys_getpid() {
  return do_syscall0(SYS_getpid);
}

int sys_fork() {
  return do_syscall0(SYS_fork);
}

int sys_sem_open(int sem) {
  int result = do_syscall1(SYS_sem_open, (uint32_t) sem);
  assert(result >= E_SUCCESS);
  return result;
}
int sys_sem_close(int sem) {
  assert(do_syscall1(SYS_sem_close, (uint32_t) sem) == E_SUCCESS);
  return E_SUCCESS;
}
int sys_sem_wait(int sem) {
  int result = do_syscall1(SYS_sem_wait, (uint32_t) sem);
  assert(result >= E_SUCCESS);
  return result;
}
int sys_sem_post(int sem) {
  assert(do_syscall1(SYS_sem_post, (uint32_t) sem) == E_SUCCESS);
  return E_SUCCESS;
}
int sys_mmap(void *addr, int id) {
  assert(do_syscall2(SYS_mmap, (uint32_t) addr, (uint32_t) id) == E_SUCCESS);
  return E_SUCCESS;
}

int fs_open(const char *pathname, int flags) {
  return do_syscall2(SYS_fs_open, (uint32_t) pathname, (uint32_t) flags);
}
int fs_read(int fd, void *buf, int len) {
  return do_syscall3(SYS_fs_read, (uint32_t) fd, (uint32_t) buf, (uint32_t) len);
}
int fs_write(int fd, const void *buf, int len) {
  return do_syscall3(SYS_fs_write, (uint32_t) fd, (uint32_t) buf, (uint32_t) len);
}
int fs_lseek(int fd, int offset, int whence) {
  return do_syscall3(SYS_fs_lseek, (uint32_t) fd, (uint32_t) offset, (uint32_t) whence);
}
int fs_close(int fd) {
  return do_syscall1(SYS_fs_close, (uint32_t) fd);
}
