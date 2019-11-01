#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>

const uint32_t IMEM_OFFSET = 0x10000000;
const uint32_t IMEM_LENGTH = 0x1000000;
const int WORD_LENGTH = 32;

void find_instr(const uint32_t &instr, uint32_t reg[]);
void do_rType(const uint32_t &instr, uint32_t reg[]);
void decode_rType(const uint32_t &instr, int &r1, int &r2, int &dest, int &shft);
int addu(const int &r1, const int &r2);
void subu(const std::string &instr, uint32_t reg[]);
void mul(const std::string &instr, uint32_t reg[]);

int main(int argc, char *argv[]) {

  std::vector<uint32_t> instr_mem;
  instr_mem.resize(0x1000000);

  std::vector<uint32_t> data_mem;
  data_mem.resize(0x4000000);

  uint32_t registers[32] = {0};
  uint32_t pc = IMEM_OFFSET;

  char memblock[100];
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

  for (int i = 0; i < sizeI/4; i++) {
    std::cerr << std::bitset<32>(instr_mem[i]) << std::endl;
  }

  find_instr(instr_mem[0], registers);

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

void find_instr(const uint32_t &instr, uint32_t reg[]) {
  uint32_t opcode = instr & 0b11111100000000000000000000000000;
  bool rType = 0, jType = 0;

  if (opcode == 0b00000000000000000000000000000000) {
    rType = 1;
  }
  else if ((opcode == 0b00001100000000000000000000000000) || (opcode == 0b00001000000000000000000000000000)) {
    jType = 1;
  }

  if (rType) {
    std::cerr << "R type" << std::endl;
    do_rType(instr, reg);
  }
  else if (jType) {
    std::cerr << "J type" << std::endl;
  }
  else { //if not r or j type then it must be a i type instruction
    std::cerr << "I type" << std::endl;
  }
}

void do_rType(const uint32_t &instr, uint32_t reg[]) {
  int r1, r2, dest, shft;
  uint32_t fn = instr & 0b00000000000000000000000000111111;

  decode_rType(instr, r1, r2, dest, shft);

  reg[dest] = 8;
  reg[r1] = 0;
  reg[r2] = 4294967295;

  if (fn == 0b00000000000000000000000000100000) {
    std::cerr << "addu" << std::endl;
    reg[dest] = addu(reg[r1], reg[r2]);
  }

  std::cerr << reg[dest] << " = " << reg[r1] << " + " << reg[r2] << std::endl;
}

void decode_rType(const uint32_t &instr, int &r1, int &r2, int &dest, int &shft) {
  uint32_t mask = 0b00000000000000000000000000011111;

  shft = (instr >> 6) & mask;
  std::cerr << shft << std::endl;
  std::cerr << std::bitset<32>(shft) << std::endl;
  dest = (instr >> 11) & mask;
  std::cerr << dest << std::endl;
  std::cerr << std::bitset<32>(dest) << std::endl;
  r2 = (instr >> 16) & mask;
  std::cerr << r2 << std::endl;
  std::cerr << std::bitset<32>(r2) << std::endl;
  r1 = (instr >> 21) & mask;
  std::cerr << r1 << std::endl;
  std::cerr << std::bitset<32>(r1) << std::endl;
}

int addu(const int &r1, const int &r2) {
  return r1 + r2;
}

void subu(const std::string &instr, uint32_t reg[]) {
  std::string dest = instr.substr(6, 5);
  std::string r1 = instr.substr(11, 5);
  std::string r2 = instr.substr(16, 5);
  // reg[10] = 8;
  // reg[11] = 0;
  // reg[9] = 4294967295;
  reg[std::stoi(dest, nullptr, 2)] = reg[std::stoi(r1, nullptr, 2)] - reg[std::stoi(r2, nullptr, 2)];
  // std::cerr << std::stoi(dest, nullptr, 2) << " = " << std::stoi(r1, nullptr, 2) << " - " << std::stoi(r2, nullptr, 2) << std::endl;
  // std::cerr << reg[10] << std::endl;
}

void mul(const std::string &instr, uint32_t reg[]) {
  std::string dest = instr.substr(6, 5);
  std::string r1 = instr.substr(11, 5);
  std::string r2 = instr.substr(16, 5);
  // reg[10] = 8;
  // reg[11] = 0;
  // reg[9] = 4294967295;
  reg[std::stoi(dest, nullptr, 2)] = reg[std::stoi(r1, nullptr, 2)] * reg[std::stoi(r2, nullptr, 2)];
  // std::cerr << std::stoi(dest, nullptr, 2) << " = " << std::stoi(r1, nullptr, 2) << " * " << std::stoi(r2, nullptr, 2) << std::endl;
  // std::cerr << reg[10] << std::endl;
}
