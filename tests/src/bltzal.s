# 1

ori $2, $0, 0x1
lui $3, 0xF000
bltzal $3, end
nop
ori $2, $0, 0x00dd
end: jr $0
nop

# check if branch on less than
