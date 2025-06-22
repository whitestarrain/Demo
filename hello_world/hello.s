#! as hello.s -o hello.o && ld -o hello hello.o && ./hello
#! gcc -c hello.s && ld hello.o && ./a.out

.section .rodata
msg:
  .string "Hello, world\n"

  .text
  .global _start
_start:
  movq $1, %rax # system call 1 is write
  movq $1, %rdi # file handle 1 is stdout
  leaq msg(%rip), %rsi # put the address of string to %rsi
                       # also can be written: movq $msg, %rsi
  movq $13, %rdx # number of bytes
  syscall

  movq $60, %rax # system call 60 is exit
  movq $0, %rdx  # we want return code 0
  syscall        # exit(0)


