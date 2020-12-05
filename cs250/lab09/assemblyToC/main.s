.data
in_string: .asciz "%[^\n]"					//stores input string
start_index: .string "%d"					//stores index to start sub_string
end_index: .string "%d"						//stores index to stop sub_string

q1: .asciz "Enter a string: "					//in_string string printed before scanf
q2: .asciz "Enter the start index: "				//start_index string printed before scanf
q3: .asciz "Enter the end index: "				//end_index string printed before scanf
output: .asciz "The substring of the given string is '%s'"	//final output string

input: .skip 100						//allocates 100 bytes for 'input', which is the word that substring will be performed on

.text
.global main
#########################################################################
# This assembly function takes in user input for a string, start index,  #
# and end index. Using those, it calls a function called sub_string,	 #
# which returns a substring from start index to end index. This function #
# then prints that substring out.					 #
##########################################################################
main:
push {r4-r10, fp, lr}	//pushes registers 4-10 to the stack
mov fp, sp		//moves the stack pointer to the same position as the frame pointer
sub sp, #8		//allocate space on the stack for 2 variables (start_index, end_index)
//1st scanf section
ldr r0, =q1		//loads r0 for printf to output the first question
bl printf		//prints the first question
ldr r0, =in_string	//loads r0 with the in_string scanf format
ldr r1, =input		//makes sure the in_string is put into variable 'input'
bl scanf		//asks the user for in_string

//2nd scanf section
ldr r0, =q2		//loads r0 for printf to output the second question
bl printf		//prints the second question
ldr r0, =start_index	//loads r0 with the start_index scanf format
sub r4, fp, #4		//moves fp into correct position to put the next thing scanned in onto the stack
mov r1, r4		//start_index will be stored in r4
bl scanf		//asks the user for start_index

//3rd scanf section
ldr r0, =q3		//loads r0 for printf to output the third question
bl printf		//prints the third question
ldr r0, =end_index	//loads r0 with the end_index scanf format
sub r5, fp, #8		//move fp into the correct position to put the next thing scanned in onto the stack
mov r1, r5		//end_index will be stored in r5
bl scanf		//asks the user for end_index


//sub_string
ldr r0, =input		//loads r0 with the in_string
ldr r1, [r4]		//loads r1 with the start_index
ldr r2, [r5]		//loads r2 with the end_index
bl sub_string		//call sub_string (c code)
mov r6, r0		//loads r6 with the output of sub_string

//final print output
ldr r0, =output		//loads r0 with final format string
mov r1, r6		//loads r1 with the sub_string (will replace %s in output string)
bl printf		//prints the output string

mov r0, #10		//moves #10 to register 0, which will be used to put a newline onto the end of the final string
bl putchar		//prints the newline

mov sp, fp		//moves the frame pointer to the same position as the stack pointer
pop {r4-r10, fp, pc}	//pops registers 4-10 off the stack
