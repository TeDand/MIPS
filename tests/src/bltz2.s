# 4
#
#
# Freddie

ori $2, $0, 0x0
bltz $2, end
nop
addi $2, $0, 0x4
end: jr $0
nop

# check if no branch on 0
