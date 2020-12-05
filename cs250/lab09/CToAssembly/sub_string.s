.data
	out_string: .skip 100	//allocates 100 bytes for out_string
.text
.global sub_string
#######################################################################
# This function performs a substring operation on a string.	      #
# The string that it will use is in register 0, the start index	      #
# is in register 1, and the end index is in register 2. The substring #
# will end up being a string starting at the start index and ending   #
# at the end index.						      #
#######################################################################
sub_string:
	push {r4-r10, fp, lr}	//pushes registers 4-10 onto the stack
	mov r4, r0		//moves the string into register 4
	mov r5, r1		//moves the beginning index into register 5
	mov r6, r2		//moves the end index into register 6
	mov r8, #0		//r8 contains 0 to keep track of the index of the substring array

	ldr r7, =out_string	//moves the array into register 7
	mov r8,r7		//moves the pointer to the beginning of out_string into register 8
	add r4, r4, r5		//moves the index of the string to the beginning index
	sub r4, #1		//adds 1 to the index of r4 to correct everything

	for:
	cmp r5, r6
	bgt endfor  		//if start index is greater than end index, branch to endfor
	ldrb r9, [r4]		//loads current index pointer from r4 into r9
	str r9, [r7]		//loads the current index pointer from r9 into that same index from r7
	add r4, #1		//increments r4 (current string pointer)
	add r5, #1		//increments the beginning index to continue the for loop
	add r7, #1		//increments r7 (out_string index)
	ble for			//branches back to the beginning of the for loop

	endfor:
	mov r10, #0		//null character for end of string
	str r10, [r7]		//loads that null character from register 10 into the current index pointer in register 7
	mov r0, r8		//moves the pointer to the beginning of out_string into r0 in order to return it
	pop {r4-r10, fp, pc}	//pops register 4-10 off of the stack
