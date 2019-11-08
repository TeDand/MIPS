#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>

const uint32_t IMEM_OFFSET = 0x10000000;
const uint32_t IMEM_LENGTH = 0x1000000;
const uint32_t DATA_OFFSET = 0x20000000;
const uint32_t DATA_LENGTH =  0x4000000;
const int WORD_LENGTH = 32;

void find_instr(const uint32_t &instr, uint32_t reg[], uint32_t &pc, bool& isDelay, std::vector<uint32_t> &instr_mem, std::vector<uint32_t> &data_mem);


void do_rType(const uint32_t &instr, uint32_t reg[], uint32_t &pc);
void do_iType(const uint32_t &instr, uint32_t reg[], uint32_t &pc, std::vector<uint32_t> &instr_mem, std::vector<uint32_t> &data_mem);
void do_jType(const uint32_t &instr, uint32_t reg[], uint32_t &pc);

void decode_rType(const uint32_t &instr, int &r1, int &r2, int &dest, int &shft);
void decode_iType(const uint32_t &instr, int &r1, int &r2, int &imm);

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
int andM(const int &r1, const int &r2);
int orM(const int &r1, const int &r2);
int xorM(const int &r1, const int &r2);
int nor(const int &r1, const int &r2);
int slt(const int &r1, const int &r2);
int sltu(const int &r1, const int &r2);

uint32_t lw(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem);
void sw(const int &r1, const int&r2,const uint32_t &offset, std::vector<uint32_t>&dataMem);

int main(int argc, char *argv[]) {

  std::vector<uint32_t> instr_mem;
  instr_mem.resize(0x1000000 / 4);

  std::vector<uint32_t> data_mem;
  data_mem.resize(0x4000000 / 4);

  std::fill(instr_mem.begin(), instr_mem.end(), 0);
  std::fill(data_mem.begin(), data_mem.end(), 0);

  uint32_t registers[34] = {0}; // 32 general purpose registers followed by; HI, LO
  uint32_t pc = 0;

  //char memblock[100];
  int sizeI = 0;


  //putting testing here as not reading from file
  // data_mem[0]= 0xF0F0F0F0;
  // data_mem[1] = 0xFFFF0000;
  // data_mem[2] = 0xFFFF;
  // registers[1] = 0x20000000;
  // registers[2] = 0x20000004;
  // registers[3] = 0xF;
  // std::cerr<< std::endl;
 /* std::cerr << "Start by storing first word of data addressed by r1 into r4"<<std::endl;
  registers[4] = lw(registers[1],0, instr_mem,data_mem);
  std::cerr << "r4 is now: "<<std::bitset<32>(registers[4])<<std::endl;
  std::cerr<<"r4 should be: "<<std::bitset<32>(data_mem[0])<<std::endl;
  std::cerr << "Now load second word of data addressed by r2 into r4"<<std::endl;
  registers[4] = lw(registers[2],0,instr_mem,data_mem);
  std::cerr << "r4 is now: "<<std::bitset<32>(registers[4])<<std::endl;
  std::cerr<<"r4 should be: "<<std::bitset<32>(data_mem[1])<<std::endl;
  std::cerr<<"Now load third word using an ofset of 8 bytes"<<std::endl;
  registers[4] = lw(registers[1],8,instr_mem,data_mem);
  std::cerr << "r4 is now: "<<std::bitset<32>(registers[4])<<std::endl;
  std::cerr<<"r4 should be: "<<std::bitset<32>(data_mem[2])<<std::endl;
  std::cerr<<std::endl;
  std::cerr << "Now storing r3 into first data word pointed at by r1"<<std::endl;
  sw(registers[1],registers[3], 0,data_mem);
  std::cerr << "data 0 is now: "<<std::bitset<32>(data_mem[0])<<std::endl;
  std::cerr<<"data 0 should be: "<<std::bitset<32>(registers[3])<<std::endl;
  std::cerr << "Now storing r3 into second data word pointed at by r2"<<std::endl;
  sw(registers[2],registers[3], 0,data_mem);
  std::cerr << "data 0 is now: "<<std::bitset<32>(data_mem[1])<<std::endl;
  std::cerr<<"data 0 should be: "<<std::bitset<32>(registers[3])<<std::endl;
  std::cerr<<"Now store into third word using an ofset of 8 bytes"<<std::endl;
  sw(registers[1],registers[3],8,data_mem);
  std::cerr << "data 0 is now: "<<std::bitset<32>(data_mem[2])<<std::endl;
  std::cerr<<"data 0 should be: "<<std::bitset<32>(registers[3])<<std::endl;
  std::cerr<< std::endl;*/


  std::ifstream binStream;

  char* binmen = new char[IMEM_LENGTH];

  try {
    binStream.open(argv[1], std::ios_base::binary | std::ios_base::in | std::ios_base::ate);
    std::streampos size = binStream.tellg();
    sizeI = size;
    binStream.seekg (0, std::ios::beg);
    binStream.read(binmen, size);
    binStream.close();
    binStream.clear();
  }
  catch (...) {
    exit(-21); //no input file - exited with error
    std::cerr << "Input file does not exist" << std::endl;
  }

  for (int i = 0; i < sizeI; i++){
    uint32_t tmp = static_cast<uint32_t>(binmen[i]) << 8 * (3-(i%4));
    instr_mem[i / 4] += tmp;
  }

  delete(binmen);

  // bool isDelay = 0;
  // registers[1] = DATA_OFFSET;
  // data_mem[0] = 0xFF;

  // find_instr(0b10001100001000100000000000000000, registers, pc, isDelay, instr_mem, data_mem);

  // std::cerr << std::bitset<32>(registers[2]) << std::endl;

   //BINARY TESTING SUITE


  //Print out all instructions
  //for (int i = 0; i < sizeI/4; i++) {
  //  std::cerr << std::bitset<32>(instr_mem[i]) << std::endl;
  //}

  bool isDelay = false; //true if executing next
  uint32_t dealySlot = 0;

  while (1) {
    //This is for TESTING ----- REMOVE IT
    // std::string tBlock;
    // std::cout << std::endl;
    // std::cout << "Program counter = "<<std::hex << pc*4 + IMEM_OFFSET<<std::endl;
    // std::cout << "Vector index = "<<std::hex << pc<<std::endl;
    // std::cout << "And instruction is: " << std::bitset<32>(instr_mem[pc])<<std::endl;
    // std::cout << std::endl;
    // std::cin >> tBlock;
    registers[0] = 0;
    if(isDelay && (pc +1 < sizeI/4)){
      isDelay = false;
      find_instr(instr_mem[pc], registers,pc,isDelay, instr_mem, data_mem);
      if(isDelay){
         dealySlot = instr_mem[pc];
       }
      pc+=1;

    }
    else if(dealySlot !=0){
        find_instr(dealySlot, registers,pc, isDelay, instr_mem, data_mem);
        dealySlot = 0;
        pc +=1;
    }
     else if (pc +1 < IMEM_LENGTH/4) {
       find_instr(instr_mem[pc], registers,pc, isDelay, instr_mem, data_mem);
       if(isDelay){
         dealySlot = instr_mem[pc];
       }
       pc += 1;
     }
     else if(pc == 0){
      std::cerr << "Program completed successfully" << std::endl;
      exit(static_cast<uint8_t>(registers[2])); //program executed properly and exited
     }
     else {
       std::cerr << "Accessing out of range memory" << std::endl;
       exit(-11); //memory out of address range - exited with error
     }
   }

  return 0;
}

void find_instr(const uint32_t &instr, uint32_t reg[], uint32_t &pc, bool& isDelay, std::vector<uint32_t> &instr_mem, std::vector<uint32_t> &data_mem) {
  uint32_t opcode = instr & 0b11111100000000000000000000000000;
  if (instr == 0) {
    return;
  }
  else if (opcode == 0) {
    do_rType(instr, reg,pc);
  }
  else if ((opcode == 0b1100000000000000000000000000) || (opcode == 0b1000000000000000000000000000)) {
    do_jType(instr,reg,pc);
  }
  else{
    std::cerr<<"itype"<<std::endl;
    do_iType(instr, reg, pc, instr_mem, data_mem);
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
  else if (fn == 0b000010) {
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
    reg[dest] = andM(reg[r1], reg[r2]);
  }
  else if (fn == 0b100101) {
    std::cerr << "or" << std::endl;
    reg[dest] = orM(reg[r1], reg[r2]);
  }
  else if (fn == 0b100110) {
    std::cerr << "xor" << std::endl;
    reg[dest] = xorM(reg[r1], reg[r2]);
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

void do_iType(const uint32_t &instr, uint32_t reg[], uint32_t &pc, std::vector<uint32_t> &instr_mem, std::vector<uint32_t> &data_mem) {
  int r1, r2, imm;
  uint32_t op = (instr >> 26) & 0b111111;

  decode_iType(instr, r1, r2, imm);

  if (op == 0b100011) {
    std::cerr << "lw" << std::endl;
    reg[r2] = lw(reg[r1], imm, instr_mem, data_mem);
  }
  else if (op == 0b101011) {
    std::cerr << "sw" << std::endl;
    sw(reg[r1], reg[r2], imm, data_mem);
  }
}

void decode_rType(const uint32_t &instr, int &r1, int &r2, int &dest, int &shft) {
  uint32_t mask = 0b11111;

  shft = (instr >> 6) & mask;
  dest = (instr >> 11) & mask;
  r2 = (instr >> 16) & mask;
  r1 = (instr >> 21) & mask;

}

void decode_iType(const uint32_t &instr, int &r1, int &r2, int &imm) {
  uint32_t mask = 0b11111;
  uint32_t mask2 = 0b1111111111111111;

  imm = instr & mask2;
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
  reg[33] = r1 / r2;
  reg[34] = r1 % r2;
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

int andM(const int &r1, const int &r2) {
  return (r1 & r2);
}

int orM(const int &r1, const int &r2) {
  return (r1 | r2);
}

int xorM(const int &r1, const int &r2) {
  return (r1 ^ r2);
}

int nor(const int &r1, const int &r2) {
  return ~(r1 | r2);
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


uint32_t lw(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem){

  uint32_t address = r1 + offset;
  if(address & 0b11 != 0){
    std::cout << "got here"<<std::endl;
    exit(-11);
  }

  if((address >= IMEM_OFFSET) && (address < IMEM_OFFSET + IMEM_LENGTH - 3)){
    return instMem[(address-IMEM_OFFSET)/4];
  }
  else if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH - 3)){
    return dataMem[(address-DATA_OFFSET)/4];
  }
  else if(address == 0x30000000){
    char response;
    std::cin>>response;
    return response;
  }
  else{
    std::exit(-11);
  }

}

void sw(const int &r1, const int&r2, const uint32_t &offset, std::vector<uint32_t>&dataMem){

  uint32_t address = r1 + offset;
  if(address & 0b11 != 0){
    exit(-11);
  }
  if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH - 3)){
    dataMem[(address-DATA_OFFSET)/4] = r2;
  }
  else if(address == 0x30000004){
    char response = r2;
    std::cout<< response;
  }
  else{
    std::exit(-11);
  }

}


/* TO DOkj
Test lw sw - TICK
CHECK divisible by 4 sorry forgot to do this - tick
Find the real exceptions
Test getchar putchar
Deal with the 0 register

// change memblock size
have we initialiased all memory?


Test jr, lw, sw with binaries proper
Edge cases especially lw sw
Clean it all up a bit
Add More instructions -> repeat some of the above
TEST BENCH!!
*/
