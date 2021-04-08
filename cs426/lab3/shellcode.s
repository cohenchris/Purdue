.text

.global _start
_start:
  movq  $0x68732f6e69622f, %rax # "hs/nib/" --> %rax
  pushq %rax                    # push the sh command onto the stack

  movq  $0x3b, %rax             # execve (
  movq  %rsp, %rdi              #         /bin/sh ,
  xor   %rsi, %rsi              #         NULL ,
  xor   %rdx, %rdx              #         NULL
                                #        )
  syscall
