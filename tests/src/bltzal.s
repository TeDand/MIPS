# 10

ori $2, $0, 0x0007
sub $3, $0, $2
bltzal $3, and
nop
jr $0
nop
and: addi $2, 0x0003
jr $31

# basic functionality
