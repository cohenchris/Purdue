/* clkdisp.s - clkdisp (x86) */

/*------------------------------------------------------------------------
 * clkdisp  -  Interrupt dispatcher for clock interrupts (x86 version)
 *------------------------------------------------------------------------
 */
#include <icu.s>

    .data
    retaddr: .byte 4
    oldeip: .byte 4
    codesegment: .byte 4
    flags: .byte 4


		.text
		.globl	clkdisp		# Clock interrupt dispatcher
clkdisp:
		pushal			# Save registers
		cli			# Disable further interrupts
		movb	$EOI,%al	# Reset interrupt
		outb	%al,$OCW1_2

		call	clkhandler	# Call high level handler

    /* Kernel is about to go back to userland. Once it is there, it should
     * handle the async IPC message (if there is one). This can be done by
     * modifying its runtime stack to detour to the callback function before
     * returning to the original EIP.
     */

    call timeslice_ipc    // copies prtmpbuf to prmbufptr, sets prtmpvalid = 0, returns fp
    movl %eax, retaddr
    // If timeslice_ipc returns NULL, there is no callback to worry about, so jump to 'end'
    // and execute as normal.
    cmp $0, retaddr
    jz end

    popal			# Restore registers

    // There IS a callback to return to first, so manipulate the stack to accomodate this
    // old EIP
    movl (%esp), %eax   // Move old EIP to the new top of the stack (saving for later)
    movl %eax, -4(%esp)

    // CS
    movl 4(%esp), %eax  // Shift CS down 1 to make room for old EIP above
    movl %eax, (%esp)

    // EFLAGS
    movl 8(%esp), %eax  // Shift EFLAGS down 1 to make room for old EIP above
    movl %eax, 4(%esp)

    // old EIP
    movl -4(%esp), %eax // Move old EIP to where EFLAGS was, callback will return to old EIP
    movl %eax, 8(%esp)

    // CALLBACK
    movl retaddr, %eax  // Move callback return address to the new top of the stack
    movl %eax, -4(%esp)

    subl $4, %esp       // Move ESP down 1 word to reflect the stack's growth by 1 word

    # Stack now looks like this:
    # ------------
    # | old EIP  |
    # ------------
    # |  EFLAGS  |
    # ------------
    # |    CS    |
    # ------------ <-- ESP (top of stack)
    # | CALLBACK |
    # ------------

    sti			  # Restore interrupt status
    iret      # Return from interrupt

    end:
      sti			  # Restore interrupt status
      popal			# Restore registers
      iret			# Return from interrupt
