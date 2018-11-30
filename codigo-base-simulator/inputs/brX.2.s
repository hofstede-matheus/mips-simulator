        # Basic branch test
	.text

main:   
        addiu $v0, $zero, 0xa
        beq $zero, $zero, b
        addiu $v0, $zero, 0xa
b:
        addiu $7, $zero, 0xd00d
        addiu $v0, $zero, 0xa
        addiu $7, $zero, 0xd00d

        syscall
        
         
        
