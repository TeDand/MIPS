# 1

lui $2, 0x00ff
sub $3, $0, $2
bltzal $3, and
ori $2, 0x0001
jr $0
nop
and: andi $2, 0x0003
jr $31

# basic functionality
