# 235

lui $3, 0xffff
ori $3, $3, 0xfff9
ori $4, $0, 0x0003
mult $3, $4
mflo $2 
jr $0
nop

# testing negative*positive LO
