        # Basic branch test
	.text

main:   
        addiu $v0, $zero, 0xa
        beq $zero, $zero, b
        addiu $v0, $zero, 0xa
        addiu $v0, $zero, 0xb
        addiu $v0, $zero, 0xc
        addiu $v0, $zero, 0xd
        addiu $v0, $zero, 0xf
b:
        addiu $v0, $zero, 0x1f
        addiu $v0, $zero, 0xa
        addiu $7, $zero, 0xd00d

        syscall
        
         
        
