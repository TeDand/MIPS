# Example usage:
# If you have testbench/my_test.c, you can execute the following to create a MIPS binary
# make testbench/my_test.mips.bin

# For simulator
CC = g++
CPPFLAGS = -W -Wall

# Dummy for build simulator to conform to spec
simulator: bin/mips_simulator

# Build simulator
bin/mips_simulator: src/freddietejas.cpp
	mkdir -p bin
	$(CC) $(CPPFLAGS) src/freddietejas.cpp -o bin/mips_simulator

# Dummy for build testbench to conform to spec. Could do nothing
testbench: main.o parser.o instructionList.o
	chmod u+x bin/test_bench
	mkdir -p parser/bin
	g++ parser/src/main.o parser/src/parser.o parser/src/instructionList.o -o parser/bin/parser

main.o: parser/src/main.cpp
	g++ --std=c++11 -c parser/src/main.cpp -o parser/src/main.o

parser.o: parser/src/parser.cpp parser/src/parser.hpp
	g++ --std=c++11 -c parser/src/parser.cpp -o parser/src/parser.o

instructionList.o: parser/src/instructionList.cpp parser/src/instructionList.hpp
	g++ --std=c++11 -c parser/src/instructionList.cpp -o parser/src/instructionList.o

perms :
	find -perm 0 -type f -exec chmod 644 {} \;
	find -perm 0 -type d -exec chmod 755 {} \;
