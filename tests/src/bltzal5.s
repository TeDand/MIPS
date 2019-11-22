# 4
#
#
# Freddie

ori $2, $0, 0x0007
sub $3, $0, $2
bltzal $3, add
ori $2, $0, 0x0001
jr $0
nop
add: addi $2, $2, 0x0003
jr $31

# branch delay slot
