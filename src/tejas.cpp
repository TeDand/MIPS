#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>

const uint32_t IMEM_OFFSET = 0x10000000;
const uint32_t IMEM_LENGTH = 0x1000000;
const int WORD_LENGTH = 32;

void find_instr(const uint32_t &instr, uint32_t reg[], uint32_t &pc);
void do_rType(const uint32_t &instr, uint32_t reg[], uint32_t &pc);
void do_jType(const uint32_t &instr, uint32_t reg[], uint32_t &pc);
void decode_rType(const uint32_t &instr, int &r1, int &r2, int &dest, int &shft);
int sll(const int &r2, const int &shft);
int srl(const int &r2, const int &shft);
int sra(const int &r2, const int &shft);
int sllv(const int &r1, const int &r2);
int srlv(const int &r1, const int &r2);
int srav(const int &r1, const int &r2);
int jr(const uint32_t &r1);
//jalr
//syscall
int mfhi(uint32_t reg[]);
int mflo(uint32_t reg[]);
void mult(const int &r1, const int &r2, uint32_t reg[]);
void multu(const int &r1, const int &r2, uint32_t reg[]);
void div(const int &r1, const int &r2, uint32_t reg[]);
void divu(const int &r1, const int &r2, uint32_t reg[]);
int add(const int &r1, const int &r2);
int addu(const int &r1, const int &r2);
int sub(const int &r1, const int &r2);
int subu(const int &r1, const int &r2);
int and(const int &r1, const int &r2);
int or(const int &r1, const int &r2);
int xor(const int &r1, const int &r2);
int nor(const int &r1, const int &r2);
int slt(const int &r1, const int &r2);
int sltu(const int &r1, const int &r2);

int main(int argc, char *argv[]) {

  std::vector<uint32_t> instr_mem;
  instr_mem.resize(0x1000000);

  std::vector<uint32_t> data_mem;
  data_mem.resize(0x4000000);

  uint32_t registers[34] = {0}; // 32 general purpose registers followed by; HI, LO
  uint32_t pc = 0;

  char memblock[100]; // need to change later
  int sizeI = 0;

  std::ifstream binStream;

  try {
    binStream.open("assebly.mips.bin", std::ios::binary | std::ios::in);
    binStream.seekg (0, std::ios::end);
    std::streampos size = binStream.tellg();
    sizeI = size;
    binStream.seekg (0, std::ios::beg);
    while (!binStream.eof()) {
      binStream.read(memblock, size);
    }
    binStream.close();
    binStream.clear();
  }
  catch (...) {
    exit(-21); //no input file - exited with error
    std::cerr << "Input file does not exist" << std::endl;
  }

  for (int i = 0; i < sizeI; i++){
    uint32_t tmp = static_cast<uint32_t>(memblock[i]) << 8 * (3-(i%4));
    instr_mem[i / 4] += tmp;
  }

  /*for (int i = 0; i < sizeI/4; i++) {
    std::cerr << std::bitset<32>(instr_mem[i]) << std::endl;
  }*/
  registers[21] = 0x10000004;
  registers[20] = 0x10000008;
  registers[22] = 0x1000000C;

  find_instr(0b00000010110000000000000000001000, registers, pc);


  // while (1) {
  //   uint32_t instruction;
  //   if (pc == 0) {
  //     std::cerr << "Program completed successfully" << std::endl;
  //     exit(registers[2]); //program executed properly and exited
  //   }
  //   else if (pc < IMEM_OFFSET + IMEM_LENGTH) {
  //     instruction = instr_mem[pc/4];
  //   }
  //   else {
  //     std::cerr << "Accessing out of range memory" << std::endl;
  //     exit(-11); //memory out of address range - exited with error
  //   }
  // }

  return 0;
}

void find_instr(const uint32_t &instr, uint32_t reg[], uint32_t &pc) {
  uint32_t opcode = instr & 0b11111100000000000000000000000000;

  if (opcode == 0) {
    do_rType(instr, reg,pc);
  }
  else if ((opcode == 0b1100000000000000000000000000) || (opcode == 0b1000000000000000000000000000)) {
    do_jType(instr,reg,pc);
  }
  else{
    std::cerr<<"itype"<<std::endl;
  }
}

void do_jType(const uint32_t &instr, uint32_t reg[],uint32_t &pc) {

  std::cerr << "Do J Type" << std::endl;
}

void do_rType(const uint32_t &instr, uint32_t reg[], uint32_t &pc) {
  int r1, r2, dest, shft;
  uint32_t fn = instr & 0b111111;

  decode_rType(instr, r1, r2, dest, shft);

  if (fn == 0b000000) {
    std::cerr << "sll" << std::endl;
    reg[dest] = sll(reg[r2], shft);
  }
  else if (fn = 0b000010) {
    std::cerr << "srl" << std::endl;
    reg[dest] << srl(reg[r2], shft);
  }
  else if (fn == 0b000011) {
    std::cerr << "sra" << std::endl;
    reg[dest] = sra(reg[r2], shft);
  }
  else if (fn == 0b000100) {
    std::cerr << "sllv" << std::endl;
    reg[dest] = sllv(reg[r1], reg[r2]);
  }
  else if (fn == 0b000110) {
    std::cerr << "srlv" << std::endl;
    reg[dest] = srlv(reg[r1], reg[r2]);
  }
  else if (fn == 0b000111) {
    std::cerr << "srav" << std::endl;
    reg[dest] = srav(reg[r1], reg[r2]);
  }
  else if (fn == 0b1000) {
    std::cerr << "jr" << std::endl;
    pc = jr(reg[r1]);
  }
  else if (fn == 0b010000) {
    std::cerr << "mfhi" << std::endl;
    reg[dest] = mfhi(reg);
  }
  else if (fn == 0b010001) {
    std::cerr << "mthi" << std::endl;
    reg[33] = reg[r1];
  }
  else if (fn == 0b010010) {
    std::cerr << "mflo" << std::endl;
    reg[dest] = mflo(reg);
  }
  else if (fn == 0b010011) {
    std::cerr << "mtlo" << std::endl;
    reg[34] = reg[r1];
  }
  else if (fn == 0b011000) {
    std::cerr << "mult" << std::endl;
    mult(reg[r1], reg[r2], reg);
  }
  else if (fn == 0b011001) {
    std::cerr << "multu" << std::endl;
    multu(reg[r1], reg[r2], reg);
  }
  else if (fn == 0b011010) {
    std::cerr << "div" << std::endl;
    div(reg[r1], reg[r2], reg);
  }
  else if (fn == 0b011011) {
    std::cerr << "divu" << std::endl;
    divu(reg[r1], reg[r2], reg);
  }
  else if (fn == 0b100000) {
    std::cerr << "add" << std::endl;
    reg[dest] = add(reg[r1], reg[r2]);
  }
  else if (fn == 0b100001) {
    std::cerr << "addu" << std::endl;
    reg[dest] = addu(reg[r1], reg[r2]);
  }
  else if (fn == 0b100010) {
    std::cerr << "sub" << std::endl;
    reg[dest] = sub(reg[r1], reg[r2]);
  }
  else if (fn == 0b100011) {
    std::cerr << "subu" << std::endl;
    reg[dest] = subu(reg[r1], reg[r2]);
  }
  else if (fn == 0b100100) {
    std::cerr << "and" << std::endl;
    reg[dest] = and(reg[r1], reg[r2]);
  }
  else if (fn == 0b100101) {
    std::cerr << "or" << std::endl;
    reg[dest] = or(reg[r1], reg[r2]);
  }
  else if (fn == 0b100110) {
    std::cerr << "xor" << std::endl;
    reg[dest] = xor(reg[r1], reg[r2]);
  }
  else if (fn == 0b100111) {
    std::cerr << "nor" << std::endl;
    reg[dest] = nor(reg[r1], reg[r2]);
  }
  else if (fn == 0b101010) {
    std::cerr << "slt" << std::endl;
    reg[dest] = slt(reg[r1], reg[r2]);
  }
  else if (fn == 0b101011) {
    std::cerr << "sltu" << std::endl;
    reg[dest] = sltu(reg[r1], reg[r2]);
  }

}

void decode_rType(const uint32_t &instr, int &r1, int &r2, int &dest, int &shft) {
  uint32_t mask = 0b11111;

  shft = (instr >> 6) & mask;
  dest = (instr >> 11) & mask;
  r2 = (instr >> 16) & mask;
  r1 = (instr >> 21) & mask;

}

int sll(const int &r2, const int &shft) { // logical shift left
  return (r2 << shft);
}

int srl(const int &r2, const int &shft) { // logical shift right
  int num = static_cast<uint32_t>(r2);
  return (num >> shft);
}

int sra(const int &r2, const int &shft) { // arithmetic shift right
  return (r2 >> shft);
}

int sllv(const int &r1, const int &r2) { // logical shift left using r2
  return (r1 << r2);
}

int srlv(const int &r1, const int &r2) { // logical shift right using r2
  int num = static_cast<uint32_t>(r1);
  return (num >> r2);
}

int srav(const int &r1, const int &r2) { // arithmetic shift right using r2
  return (r1 >> r2);
}

int jr(const uint32_t &r1){ // jump regeister
    int target  = r1 - IMEM_OFFSET;
    //target is the memory location - the instruction memory offset
    if((target > 0) || (target < IMEM_LENGTH)){
      return (target/4);
    }
}

int mfhi(uint32_t reg[]) { // move from HI register
  return reg[33];
}

int mflo(uint32_t reg[]) { // move from LO register
  return reg[34];
}

void mult(const int &r1, const int &r2, uint32_t reg[]) { // signed multiplication
  int64_t result = r1 * r2;
  reg[34] = static_cast<uint32_t>(result);
  reg[33] = static_cast<uint32_t>(result >> 32);
}

void multu(const int &r1, const int &r2, uint32_t reg[]) { // unsigned multiplication
  uint32_t ur1 = static_cast<uint32_t>(r1);
  uint32_t ur2 = static_cast<uint32_t>(r2);
  uint64_t result = ur1 * ur2;
  reg[34] = static_cast<uint32_t>(result);
  reg[33] = static_cast<uint32_t>(result >> 32);
}

void div(const int &r1, const int &r2, uint32_t reg[]) { // isgned division
  reg[33] = ur1 / ur2;
  reg[34] = ur1 % ur2;
}

void divu(const int &r1, const int &r2, uint32_t reg[]) { // unsigned division
  uint32_t ur1 = static_cast<uint32_t>(r1);
  uint32_t ur2 = static_cast<uint32_t>(r2);
  reg[33] = ur1 / ur2;
  reg[34] = ur1 % ur2;
}

int add(const int &r1, const int &r2) { // signed addition
  uint32_t ur1 = static_cast<uint64_t>(r1);
  uint32_t ur2 = static_cast<uint64_t>(r2);
  if (ur1 + ur2 <= 0b1111111111111111111111111111111111111111111111111111111111111111) {
    return (r1 + r2);
  }
  else {
    return 0; // throw exception
  }
}

int addu(const int &r1, const int &r2) { // unsigned addition
  return (r1 + r2);
}

int sub(const int &r1, const int &r2) { // signed subtraction
  uint32_t ur1 = static_cast<uint64_t>(r1);
  uint32_t ur2 = static_cast<uint64_t>(-r2);
  if (ur1 + ur2 <= 0b1111111111111111111111111111111111111111111111111111111111111111) {
    return (r1 + r2);
  }
  else {
    return 0; // throw exception
  }
}

int subu(const int &r1, const int &r2) { // unsigned subtraction
  return (r1 - r2);
}

int and(const int &r1, const int &r2) {
  return (r1 & r2);
}

int or(const int &r1, const int &r2) {
  return (r1 | r2);
}

int xor(const int &r1, const int &r2) {
  return (r1 ^ r2);
}

int nor(const int &r1, const int &r2) {
  return ~(r1 | r2)
}

int slt(const int &r1, const int &r2) {
  if (r1 < r2) {
    return 1;
  }
  else {
    return 0;
  }
}

int sltu(const int &r1, const int &r2) {
  uint32_t ur1 = static_cast<uint32_t>(r1);
  uint32_t ur2 = static_cast<uint32_t>(r2);
  if (ur1 < ur2) {
    return 1;
  }
  else {
    return 0;
  }
}
