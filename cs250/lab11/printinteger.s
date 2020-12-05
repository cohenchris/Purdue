.text
.global printx
.global printd

############################################################
#                          PRINTX                          #
############################################################
printx:
push {r4-r10, fp, lr}
mov fp, sp

mov r5, #8 		//counter for loop (do this 8 times)
mov r4, r0		//stores the first argument into r4 for safekeeping
mov r6, r1		//stores the second argument into r6 for safekeeping
mov r7, #0		//r7 will be the boolean register to help keep track of what to do with leading zeros

loop:
sub r5, #1
cmp r5, #1		//if you're on the last nibble to print, just print it
mov r0, r4, lsr #28	//moves the result of shifting contents of r4 7 nibbles to the left, which isolates the lsb, into r0
lsl r4, #4		//shifts the actual contents of r4(the argument to be converted) over 1 nibble, since we already delt with the lsb
blt continuePrintx

cmp r0, #0
beq leadingZeroCheck	//if r0 is 0, branch to leadingZeroCheck to see if we need to print it
bgt continuePrintx

leadingZeroCheck:
cmp r7, #0		//if the 'boolean' value in r7 is 0, keep going
beq loop

continuePrintx:
mov r7, #1		//if you ever get to print anything, you must print the rest of it, so change the 'boolean' vaue in r7 to 1
add r0, #48		//adjusts to make sure the character is printable

cmp r0, #57
ble isNum		//if it IS less than or equal to 57, that means r0 is a number 

isChar:
add r0, #7		//if the value of r0 is greater than 57 (not a number), add 7 to make it within the valid range of letters
cmp r6, #1		//checks to see if the second argument (r1) is 1 or not. 1 means it should be printed as a capital letter, so branch
beq isNum		//if second argument is less than 1 (would be 0), branch straight to printing. Otherwise, continue
add r0, #32		//adds 32 to r0 to make it a lowercase letter

isNum:
bl putchar

cmp r5, #0
bgt loop

mov sp, fp
pop {r4-r10, fp, pc}

############################################################
#                          PRINTD                          #
############################################################
printd:
push {r4-r10, fp, lr}
mov fp, sp

mov r4, r0		//moves the 32 bit integer to convert into r4

zero:
cmp r4, #0
bne negativeCheck	//if r4 is anything besides zero, branch to where we check for negative/positive
mov r0, #48		//if it's zero, move the ascii representation of zero to r0 and print it
bl putchar
bl end

//////////NEGATIVE SIGN CHECK//////////
negativeCheck:
mov r5, r4, lsr #31	//isolates the first bit to check whether or not it's a negative number
cmp r5, #1		//if r5 is a 1, that means it's a negative number
beq negative		//branch to the part of the asm that pushes a negative sign onto the stack
bl positive

negative:
mov r0, #45		//moves the ascii value 45 into r0 (45 in ascii is '-')
//mov r5, #45		//stores the ascii value 45 onto the stack (45 in ascii is '-')
//str r5, [fp, #-4]	//stores the negative sign on the stack
//ldr r0, [fp, #-4]	//moves the negative sign into r0
bl putchar		//prints the negative sign

rsb r4, r4, #0		//stores the positive version of r4 into r4 (if this executes, r4 is originally negative)
////////////////////////////////////

positive:
mov r7, r4 		//r7 will now be a copy of the value we need to work with

mov r9, #0		//beginning of numDigits counter
bl numDigits

numDigitsDone:
sub r9, #1		//subtracts 1 from the numDigits to make the next loop work properly

printLoop:
mov r8, r4		//moves the number to print into r8 to make this loop easier
mov r6, r9		//counter for the upcoming loop
cmp r6, #0		//if r6 is 0, don't divide at all
ble afterfor

for:
mov r0, r8
bl u_divide_by_10	//divide the number by 10
mov r8, r0
sub r6, #1
cmp r6, #0
bgt for

mov r0, r8
add r0, #48
bl putchar

afterfor:
cmp r9, #0		//if the remaining number in r9 is a single digit, print it
ble printChar
mov r6, #0		//counter for the upcoming loop
mov r5, r8 		//beginning value for the number to subtract from the original number

mov r10, #10		//value for the multiplication loop2

loop2:
mul r5, r5, r10 	//multiply r5 by 10
add r6, #1		//increment counter
cmp r6, r9
blt loop2		//if r8 is less than r9, we still need to multiply more

label:
sub r4, r4, r5		//takes the number we're extracting digits from and removes the digit that we just pushed to the stack
sub r9, #1		//numDigit counter is decreased to help extract the next digit
cmp r9, #0
bge printLoop

printChar:
mov r0, r4
add r0, #48
bl putchar

bl end
############################################################
#                     HELPER FUNCTIONS                     #
############################################################

numDigits:
add r9, #1		//increments numDigits (r9)
mov r0, r7		//moves the number who we need to count the digits for into r0
bl u_divide_by_10	//divides the number by 10
mov r7, r0
cmp r7, #0
bgt numDigits		//if not yet 0, we can still divide more and find more digits
ble numDigitsDone	//returns if r0 is 0 (done dividing)

u_divide_by_10:
   /* r0 contains the argument to be divided by 10 */
   ldr r1, .Lu_magic_number_10 /* r1 ← magic_number */
   umull r1, r2, r1, r0   /* r1 ← Lower32Bits(r1*r0). r2 ← Upper32Bits(r1*r0) */
   mov r0, r2, LSR #3     /* r0 ← r2 >> 3 */
   bx lr                  /* leave function */
   .align 4
   .Lu_magic_number_10: .word 0xcccccccd
############################################################
end:
mov sp, fp
pop {r4-r10, fp, pc}
