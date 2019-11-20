# 100

lui $3, 0x2000
ori $4, $0, 0x64
sb $4, 3($3)
lb $2, 3($3)
jr $0

# basic functionality
