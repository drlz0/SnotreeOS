.global outb

outb:
    movw 4(%esp), %dx    // move the data to be sent into the al register
    movb 8(%esp), %al    // move the address of the I/O port into the dx register
    outb %al, (%dx)      // send the data to the I/O port
    ret                  // return to the calling function

.global inb

inb:
    movl 4(%esp), %edx     // move the address of the I/O port to the edx register
    inb (%dx), %al         // read a byte from the I/O port and store it in the al register
    ret                    // return the read byte so it matches the style of .global outb
