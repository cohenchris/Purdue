.data

print_string: .asciz "Hello World\n"

.text

.global main
main:
push {r4-r9, fp, lr}
ldr r0, =print_string

bl printf
pop {r4-r9, fp, pc}
