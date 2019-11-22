# 9

lui $3, 0xffff
ori $3, $3, 0xfff9
ori $4, $0, 0x000a
divu $3, $4
mfhi $2 
jr $0
nop

# testing negative/positive remainder
