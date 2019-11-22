# 12
#
#
# Freddie

ori $2, $0, 0x0
bltzal $2, add
nop
add $2, $31, $0
jr $0
nop
add: addi $2, $2, 0x0003
jr $31

# check link reg when 0
