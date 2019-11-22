# 4

lui $3, 0x1000
addi $3, 0x0020
jalr add, $4
addi $2, 0x0001
end: jr $0
nop
add: addi $2, 0x0003
jr $4

# basic functionality
