# 7
# 
#
# Freddie

ori $2, $0, 0x0007
bgtz $2, end
nop
ori $2, $0, 0x00dd
end: jr $0
nop

# check branch when +ve
