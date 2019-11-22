# 2

lui $3, 0x1000
ori $2, $0, 0x01
ori $3, $3, 0x18
jr	$3
addi $2, $2, 0x1
addi $2, $2, 0x1
jr $0

# test branch delay slot
