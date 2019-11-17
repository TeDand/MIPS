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


void do_rType(const uint32_t &instr, uint32_t reg[], uint32_t &pc, bool& isDelay);
void do_iType(const uint32_t &instr, uint32_t reg[], uint32_t &pc, std::vector<uint32_t> &instr_mem, std::vector<uint32_t> &data_mem, bool& isDelay);
void do_jType(const uint32_t &instr, uint32_t reg[], uint32_t &pc);

void decode_rType(const uint32_t &instr, int &r1, int &r2, int &dest, int &shft);
void decode_iType(const uint32_t &instr, int &r1, int &r2, int &imm);

int sll(const int &r2, const int &shft);
int srl(const int &r2, const int &shft);
int sra(const int &r2, const int &shft);
int sllv(const int &r1, const int &r2);
int srlv(const int &r1, const int &r2);
int srav(const int &r1, const int &r2);
int jr(const uint32_t &r1,uint32_t reg[]);
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


int j(const uint32_t &instr, const int& pc, uint32_t reg[]);
int jal(const uint32_t &instr, const int& pc, uint32_t reg[]);
int jalr(const int &r1,const int &dest,const int&pc,  uint32_t reg[]);

uint32_t lw(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem);
void sw(const int &r1, const int&r2,const uint32_t &offset, std::vector<uint32_t>&dataMem);

int addI(const int &r1, const int& imm);
int orI(const int &r1, const int& imm);

int beq(int r1, int r2, int imm);
int bgez(int r1, int imm);
int bgezal(int r1, int imm, uint32_t reg[], const int pc);
int bgtz(int r1, int imm);
int blez(int r1, int imm);
int bltz(int r1, int imm);
int bltzal(int r1, int imm, uint32_t reg[], const int pc);
int bne(int r1, int r2, int imm);

//new stuff
int addIU(const int &r1, const int& imm);
int andI(const int &r1, const int& imm);
uint32_t lb(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem);
uint32_t lbu(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem);
uint32_t lh(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem);
uint32_t lhu(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem);
int lui( const int& imm);
uint32_t lwl(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem);
uint32_t lwr(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem);
void sb(const int &r1, const int&r2, const uint32_t &offset, std::vector<uint32_t>&dataMem);
void sh(const int &r1, const int&r2, const uint32_t &offset, std::vector<uint32_t>&dataMem);
int slti(int r1, int imm);
int sltiu(uint32_t r1, uint32_t imm);
int xorI(const int &r1, const int& imm);

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

  std::ifstream binStream;

  char* binmen = new char[IMEM_LENGTH];

  try {
    binStream.open(argv[1], std::ios_base::binary | std::ios_base::in | std::ios_base::ate);
    if( !binStream.good()){
      //std::cerr <<"Not a file"<<std::endl;
      exit(-12);
    }
    std::streampos size = binStream.tellg();
    sizeI = size;
    if (sizeI > IMEM_LENGTH){
      //file too big
      exit(-11);
    }
    binStream.seekg (0, std::ios::beg);
    binStream.read(binmen, size);
    binStream.close();
    binStream.clear();
  }
  catch (...) {
    exit(-21); //no input file - exited with error
    //std::cerr << "Input file does not exist" << std::endl;
  }

  for (int i = 0; i < sizeI; i++){
    uint32_t tmp = static_cast<uint8_t>(binmen[i]) << (8 * (3-(i%4)));
    instr_mem[i / 4] += tmp;
  }
 
  delete(binmen);



  // bool isDelay = 0;
  // registers[1] = DATA_OFFSET;
  // data_mem[0] = 0xFF;

  // find_instr(0b10001100001000100000000000000000, registers, pc, isDelay, instr_mem, data_mem);

  // //std::cerr << std::bitset<32>(registers[2]) << std::endl;

   //BINARY TESTING SUITE


  //Print out all instructions
  /*for (int i = 0; i < sizeI/4; i++) {
    std::cerr << std::bitset<32>(instr_mem[i]) << std::endl;
  }*/

  bool isDelay = false; //true if executing next
  uint32_t dealySlot = 0;
  bool tuped = false;
  /*if(tuped){
 
    registers[1] = 4;
    registers[2] = addI(registers[1],0x2);
    std::cout<< "not signed"<<registers[2]<<std::endl;
    registers[2] = addIU(registers[1],0x2);
    std::cout<< "signed"<<registers[2]<<std::endl;
    jr(registers[0], registers);
  }*/




  while (1) {
    //This is for TESTING ----- REMOVE IT
     /*std::cerr << "3:" << std::bitset<32>(registers[3]) << std::endl;
     std::cerr << "4: " << std::bitset<32>(registers[4]) << std::endl;
     std::cerr << "2: " << std::bitset<32>(registers[2]) << std::endl;
     std::string tBlock;
     std::cout << std::endl;
     std::cout << "Program counter = "<<std::hex << pc*4 + IMEM_OFFSET<<std::endl;
     std::cout << "Vector index = " << std::hex << pc << std::endl;
     std::cout << "And instruction is: " << std::bitset<32>(instr_mem[pc])<<std::endl;
     std::cout << std::endl;
     std::cin >> tBlock;*/

    registers[0] = 0;

    if(isDelay && (pc +1 < sizeI/4)){
      isDelay = false;
      find_instr(instr_mem[pc], registers,pc,isDelay, instr_mem, data_mem);
      if(isDelay){
         dealySlot = instr_mem[pc];
         pc+=1;
       }


    }
    else if(dealySlot !=0){
        isDelay = true;
        find_instr(dealySlot, registers,pc, isDelay, instr_mem, data_mem);
        isDelay = false;
        dealySlot = 0;
    }
     else if (pc +1 < IMEM_LENGTH/4) {
       find_instr(instr_mem[pc], registers,pc, isDelay, instr_mem, data_mem);
       if(isDelay){
         dealySlot = instr_mem[pc];
       }
       pc += 1;
     }
     else {
       //std::cerr << "Accessing out of range memory" << std::endl;
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
    do_rType(instr, reg,pc,isDelay);
  }
  else if ((opcode == 0b1100000000000000000000000000) || (opcode == 0b1000000000000000000000000000)) {
    if(isDelay){
      do_jType(instr,reg,pc);
    }
    else{
      isDelay = true;
    }
  }
  else{
    //std::cerr<<"itype"<<std::endl;
    do_iType(instr, reg, pc, instr_mem, data_mem,isDelay);
  }
}

void do_jType(const uint32_t &instr, uint32_t reg[],uint32_t &pc) {
  //std::cerr << "Do J Type" << std::endl;
  int opcode = instr & 0xFC000000 >> 26;
  if(opcode == 0b10){
    pc = j(instr, pc, reg);
  }
  else if(opcode == 11){
    pc = jal(instr, pc, reg);
  }

}

void do_rType(const uint32_t &instr, uint32_t reg[], uint32_t &pc, bool& isDelay) {
  int r1, r2, dest, shft;
  uint32_t fn = instr & 0b111111;

  decode_rType(instr, r1, r2, dest, shft);

  if (fn == 0b000000) {
    if (r1 == 0b0) {
      //std::cerr << "sll" << std::endl;
      reg[dest] = sll(reg[r2], shft);
    }
    else {
      //std::cerr << "invalid sll" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b000010) {
    if (r1 == 0b0) {
      //std::cerr << "srl" << std::endl;
      reg[dest] << srl(reg[r2], shft);
    }
    else {
      //std::cerr << "invalid srl" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b000011) {
    if (r1 == 0b0) {
      //std::cerr << "sra" << std::endl;
      reg[dest] = sra(reg[r2], shft);
    }
    else {
      //std::cerr << "invalid sra" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b000100) {
    if (shft == 0b0) {
      //std::cerr << "sllv" << std::endl;
      reg[dest] = sllv(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid sllv" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b000110) {
    if (shft == 0b0) {
      //std::cerr << "srlv" << std::endl;
      reg[dest] = srlv(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid srlv" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b000111) {
    if (shft == 0b0) {
      //std::cerr << "srav" << std::endl;
      reg[dest] = srav(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid srav" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b1000) {
    if (r2 == 0b0) {
      if(isDelay){
        //std::cerr << "jr" << std::endl;
        pc  = jr(r1,reg);
      }
      else{
        isDelay = true;
      }
    }
    else {
      //std::cerr << "invalid jr" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b010000) {
    if ((r1 == 0b0) && (r2 == 0b0) && (shft == 0b0)) {
      //std::cerr << "mfhi" << std::endl;
      reg[dest] = mfhi(reg);
    }
    else {
      //std::cerr << "invalid mfhi" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b010001) {
    if ((dest == 0b0) && (r2 == 0b0) && (shft == 0b0)) {
      //std::cerr << "mthi" << std::endl;
      reg[33] = reg[r1];
    }
    else {
      //std::cerr << "invalid mthi" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b010010) {
    if ((r1 == 0b0) && (r2 == 0b0) && (shft == 0b0)) {
      //std::cerr << "mflo" << std::endl;
      reg[dest] = mflo(reg);
    }
    else {
      //std::cerr << "invalid mflo" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b010011) {
    if ((dest == 0b0) && (r2 == 0b0) && (shft == 0b0)) {
      //std::cerr << "mtlo" << std::endl;
      reg[34] = reg[r1];
    }
    else {
      //std::cerr << "invalid mtlo" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b011000) {
    if ((shft == 0b0) && (dest == 0b0)) {
      //std::cerr << "mult" << std::endl;
      mult(reg[r1], reg[r2], reg);
    }
    else {
      //std::cerr << "invalid mult" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b011001) {
    if ((shft == 0b0) && (dest == 0b0)) {
      //std::cerr << "multu" << std::endl;
      multu(reg[r1], reg[r2], reg);
    }
    else {
      //std::cerr << "invalid multu" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b011010) {
    if ((dest == 0b0) && (shft == 0b0)) {
      //std::cerr << "div" << std::endl;
      div(reg[r1], reg[r2], reg);
    }
    else {
      //std::cerr << "invalid div" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b011011) {
    if ((dest == 0b0) && (shft == 0b0)) {
      //std::cerr << "divu" << std::endl;
      divu(reg[r1], reg[r2], reg);
    }
    else {
      //std::cerr << "invalid divu" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b100000) {
    if (shft == 0b0) {
      //std::cerr << "add" << std::endl;
      reg[dest] = add(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid add" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b100001) {
    if (shft == 0b0) {
      //std::cerr << "addu" << std::endl;
      reg[dest] = addu(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid addu" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b100010) {
    if (shft == 0b0) {
      //std::cerr << "sub" << std::endl;
      reg[dest] = sub(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid sub" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b100011) {
    if (shft == 0b0) {
      //std::cerr << "subu" << std::endl;
      reg[dest] = subu(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid subu" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b100100) {
    if (shft == 0b0) {
      //std::cerr << "and" << std::endl;
      reg[dest] = andM(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid and" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b100101) {
    if (shft == 0b0) {
      //std::cerr << "or" << std::endl;
      reg[dest] = orM(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid or" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b100110) {
    if (shft == 0b0) {
      //std::cerr << "xor" << std::endl;
      reg[dest] = xorM(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid xor" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b100111) {
    if (shft == 0b0) {
      //std::cerr << "nor" << std::endl;
      reg[dest] = nor(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid nor" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b101010) {
    if (shft == 0b0) {
      //std::cerr << "slt" << std::endl;
      reg[dest] = slt(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid slt" << std::endl;
      exit(-12);
    }
  }
  else if (fn == 0b101011) {
    if (shft == 0b0) {
      //std::cerr << "sltu" << std::endl;
      reg[dest] = sltu(reg[r1], reg[r2]);
    }
    else {
      //std::cerr << "invalid sltu" << std::endl;
      exit(-12);
    }
  }
  else if(fn == 0b1001){
    if (r2 == 0b0) {
      //std::cerr<< "jalr"<<std::endl;
      if(isDelay){
        pc = jalr(r1,dest, pc, reg);
      }
      else{
        isDelay = true;
      }
    }
    else {
      //std::cerr << "invalid jalr" << std::endl;
      exit(-12);
    }
  }
}


void do_iType(const uint32_t &instr, uint32_t reg[], uint32_t &pc, std::vector<uint32_t> &instr_mem, std::vector<uint32_t> &data_mem, bool& isDelay) {
  int r1, r2, imm;
  uint32_t op = (instr >> 26) & 0b111111;

  decode_iType(instr, r1, r2, imm);

  if (op == 0b100011) {
    //std::cerr << "lw" << std::endl;
    reg[r2] = lw(reg[r1], imm, instr_mem, data_mem);
  }
  else if (op == 0b101011) {
    //std::cerr << "sw" << std::endl;
    sw(reg[r1], reg[r2], imm, data_mem);
  }
  else if(op == 0b100){
    //std::cerr << "addI"<<std::endl;
    reg[r2] = addI(reg[r1], imm);
  }
  else if(op == 0b1101){
    //std::cerr << "orI"<<std::endl;
    reg[r2] = orI(reg[r1], imm);
  }
  else if(op == 0b1001){
    reg[r2] = addIU(reg[r1], imm);
  }
  else if(op == 0b1100){
    reg[r2] = andI(reg[r1], imm);
  }
  else if(op == 0b100000){
    reg[r2] = lb(reg[r1],imm,instr_mem,data_mem);
  }
  else if(op == 0b100100){
    reg[r2] = lbu(reg[r1],imm,instr_mem,data_mem);
  }
  else if(op == 0b100001){
    reg[r2] = lh(reg[r1],imm,instr_mem,data_mem);
  }
  else if(op == 0b100101){
    reg[r2] = lhu(reg[r1],imm,instr_mem,data_mem);
  }
  else if(op == 0b1111 && reg[r1] == 0){
    reg[r2] = lui(imm);
  }
  else if(op == 0b100010){
    reg[r2] = reg[r2] & lwl(reg[r1],imm,instr_mem,data_mem);
  }
  else if(op == 0b100110){
    reg[r2] = reg[r2] & lwr(reg[r1],imm,instr_mem,data_mem);
  }
  else if(op == 0b101000){
    sb(r1,reg[r2],imm,data_mem);
  }
  else if(op == 0b101001){
    sh(reg[r1],reg[r2],imm,data_mem);
  }
  else if(op == 0b1010){
    reg[r2] = slti(reg[r1],imm);
  }
  else if(op == 0b1011){
    reg[r2] = sltiu(reg[r1],imm);
  }
  else if(op == 0b1110){
    reg[r2] = xorI(reg[r1], imm);
  }
  else if(op == 0b100){
    if(isDelay){
      //std::cerr << "beq"<<std::endl;
      pc += beq(reg[r1],reg[r2], imm);
    }
    else{
      isDelay = true;
    }
  }
  else if(op == 1){
    if(r2 == 1){
      if(isDelay){
        //std::cerr << "bgez"<<std::endl;
        pc += bgez(reg[r1], imm);
      }
      else{
        isDelay = true;
      }
    }
    else if(r2 == 0b10001){
      if(isDelay){
        //std::cerr << "bgezal"<<std::endl;
        pc += bgezal(reg[r1], imm,reg,pc);
      }
      else{
        isDelay = true;
      }
    }
    else if(r2 == 0){
      if(isDelay){
        //std::cerr << "bltz"<<std::endl;
        pc += bltz(reg[r1], imm);
      }
      else{
        isDelay = true;
      }
    }
    else if(r2 == 0b10000){
      if(isDelay){
        //std::cerr << "bltzal"<<std::endl;
        pc += bltzal(reg[r1], imm,reg,pc);
      }
      else{
        isDelay = true;
      }
    }
    else {
      exit(-12);
    }
  }
  else if(op == 0b111){
    if(isDelay){
        //std::cerr << "bgtz"<<std::endl;
        pc += bgtz(reg[r1], imm);
      }
      else{
        isDelay = true;
      }
  }
  else if((op == 0b110) && (r2 == 0b0)){
    if(isDelay){
        //std::cerr << "blez"<<std::endl;
        pc += blez(reg[r1], imm);
      }
      else{
        isDelay = true;
      }
  }
  else if(op == 0b101){
    if(isDelay){
        //std::cerr << "bne"<<std::endl;
        pc += bne(reg[r1],reg[r2], imm);
      }
      else{
        isDelay = true;
      }
  }



  else{
    //invalid opcode
    exit(-12);
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
  uint32_t num = static_cast<uint32_t>(shft);
  return (r2 << num);
}

int srl(const int &r2, const int &shft) { // logical shift right
  int num = static_cast<uint32_t>(r2);
  return (num >> shft);
}

int sra(const int &r2, const int &shft) { // arithmetic shift right
  uint32_t num = static_cast<uint32_t>(shft);
  return (r2 >> num);
}

int sllv(const int &r1, const int &r2) { // logical shift left using r2
  return (r1 << r2);
}

int srlv(const int &r1, const int &r2) { // logical shift right using r2
  int num = static_cast<uint32_t>(r1);
  return (r2 >> num);
}

int srav(const int &r1, const int &r2) { // arithmetic shift right using r2
  uint32_t ur1 = static_cast<uint32_t>(r1);
  return (r2 >> ur1);
}

int jr(const uint32_t &r1,uint32_t reg[]){ // jump regeister
    if(reg[r1] == 0){
      //std::cerr << "Program completed successfully" << std::endl;
      exit(static_cast<uint8_t>(reg[2])); //program executed properly and exited
    }
    int target  = reg[r1] - IMEM_OFFSET;
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
  reg[34] = static_cast<uint32_t>(result); // LO
  reg[33] = static_cast<uint32_t>(result >> 32); // HI
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
  int result = r1 + r2;
  if ((r1 > 0) && (r2 > 0) && (result < 0)) { // checking for overflows
    exit(-10);
  }
  else if ((r1 < 0) && (r2 < 0) && (result > 0)) {
    exit(-10);
  }
  else {
    return result;
  }
}

int addu(const int &r1, const int &r2) { // unsigned addition
  return (r1 + r2);
}

int sub(const int &r1, const int &r2) { // signed subtraction
  int nr2 = -r2; // negating the second number
  int result = r1 + nr2;

  if ((r1 > 0) && (nr2 > 0) && (result < 0)) { // checking for overflows
    exit(-10);
  }
  else if ((r1 < 0) && (nr2 < 0) && (result > 0)) {
    exit(-10);
  }
  else {
    return result;
  }
}

int subu(const int &r1, const int &r2) { // unsigned subtraction
  uint32_t ur1 = static_cast<uint32_t>(r1);
  uint32_t ur2 = static_cast<uint32_t>(r2);

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

int sltu(const int &r1, const int &r2) { // set on less than unsigned
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

  if(address & 0b11 != 0){ // checking if the memory location is divisible by 4
    exit(-11);
  }
  if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH - 3)){ // checking if the memory location is within the data memory bounds
    dataMem[(address-DATA_OFFSET)/4] = r2;
  }
  else if(address == 0x30000004){
    char response = r2;
    std::cout << response;
  }
  else{ // exits with an error if attempting to write to locations other than data memory
    std::exit(-11);
  }

}

int j(const uint32_t &instr, const int& pc, uint32_t reg[]){
    uint32_t target = (instr&0x3FFFFFF)<<2;
    target += ((pc*4)+IMEM_OFFSET) & 0xF0000000;
    if(target == 0){
      //std::cerr << "Program completed successfully" << std::endl;
      exit(static_cast<uint8_t>(reg[2]));
    }
    else if(target >= IMEM_OFFSET + IMEM_LENGTH ){
      exit(-11);
    }
    else{
      return (target - IMEM_OFFSET)/4;
    }
}

int jal(const uint32_t &instr, const int& pc, uint32_t reg[]){
    reg[30] = (pc*4 + IMEM_OFFSET) + 4;
    uint32_t target = (instr&0x3FFFFFF)<<2;
    target += ((pc*4)+IMEM_OFFSET) & 0xF0000000;
    if(target == 0){
      //std::cerr << "Program completed successfully" << std::endl;
      exit(static_cast<uint8_t>(reg[2]));
    }
    else if(target >= IMEM_OFFSET + IMEM_LENGTH || target < IMEM_OFFSET){
      exit(-11);
    }
    else{
      return (target - IMEM_OFFSET)/4;
    }
}

int jalr(const int &r1,const int &dest,const int&pc,  uint32_t reg[]){
  reg[dest] = (pc*4 + IMEM_OFFSET) + 4;
  jr(r1,reg);
}

int addI(const int &r1, const int& imm){
  return (r1 + static_cast<int16_t>(imm));
}

int orI(const int &r1, const int& imm){
  return (r1 | imm);
}

int beq(int r1, int r2, int imm){
  if(r1 == r2){
    return imm;
  }
  else return 1;
}

int bgez(int r1, int imm){
  if(r1 >= 0){
    return imm;
  }
  else{
    return 1;
  }
}

int bgezal(int r1, int imm, uint32_t reg[], const int pc){
  //double check if next line should be in the if
  reg[31] = pc+1;
  if(r1 >= 0){
    return imm;
  }
  else{
    return 1;
  }
}

int bgtz(int r1, int imm){
  if(r1 > 0){
    return imm;
  }
  else{
    return 1;
  }
}

int blez(int r1, int imm){
  if(r1 <= 0){
    return imm;
  }
  else{
    return 1;
  }
}

int bltz(int r1, int imm){
  if(r1 < 0){
    return imm;
  }
  else{
    return 1;
  }
}

int bltzal(int r1, int imm, uint32_t reg[], const int pc){
  //double check if next line should be in the if
  reg[31] = pc+1;
  if(r1 < 0){
    return imm;
  }
  else{
    return 1;
  }
}

int bne(int r1, int r2, int imm){
  if(r1 != r2){
    return imm;
  }
  else return 1;
}

// New stuff


int addIU(const int &r1, const int& imm){

  return (r1 + imm);

}
int andI(const int &r1, const int& imm){

  return (r1 & imm);

}

uint32_t lb(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem){

  uint32_t address = r1 + offset;

  if((address >= IMEM_OFFSET) && (address < IMEM_OFFSET + IMEM_LENGTH)){
    uint32_t word = instMem[(address-IMEM_OFFSET)/4];
    uint8_t wmask =(word >>(8*(3-((address-IMEM_OFFSET)%4))));
    return(static_cast<uint32_t>(0xFF & wmask));
  }
  else if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH)){
    uint32_t word = dataMem[(address-DATA_OFFSET)/4];
    uint8_t wmask =(word >>(8*(3-((address-DATA_OFFSET)%4))));
    return(static_cast<uint32_t>(0xFF & wmask));
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
uint32_t lbu(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem){

  uint32_t address = r1 + offset;

  if((address >= IMEM_OFFSET) && (address < IMEM_OFFSET + IMEM_LENGTH)){
    uint32_t word = instMem[(address-IMEM_OFFSET)/4];
    uint32_t wmask =(word >>(8*(3-((address-IMEM_OFFSET)%4))));
    return(0xFF & wmask);
  }
  else if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH)){
    uint32_t word = dataMem[(address-DATA_OFFSET)/4];
    uint8_t wmask =(word >>(8*(3-((address-DATA_OFFSET)%4))));
    return(0xFF & wmask);
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

uint32_t lh(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem){

  uint32_t address = r1 + offset;
  if(address & 0b1 != 0){
    exit(-11);
  }
  if((address >= IMEM_OFFSET) && (address < IMEM_OFFSET + IMEM_LENGTH)){
    uint32_t word = instMem[(address-IMEM_OFFSET)/4];
    uint16_t wmask =(word >>(16*(1-((address-IMEM_OFFSET)%2))));
    return(static_cast<int16_t>(0xFFFF & wmask));
  }
  else if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH)){
    uint32_t word = dataMem[(address-DATA_OFFSET)/4];
    uint8_t wmask =(word >>(16*(1-((address-DATA_OFFSET)%2))));
    return(static_cast<int16_t>(0xFFFF & wmask));
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

uint32_t lhu(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem){

  uint32_t address = r1 + offset;
  if(address & 0b1 != 0){
    exit(-11);
  }
  if((address >= IMEM_OFFSET) && (address < IMEM_OFFSET + IMEM_LENGTH)){
    uint32_t word = instMem[(address-IMEM_OFFSET)/4];
    uint16_t wmask =(word >>(16*(1-((address-IMEM_OFFSET)%2))));
    return(0xFFFF & wmask);
  }
  else if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH)){
    uint32_t word = dataMem[(address-DATA_OFFSET)/4];
    uint8_t wmask =(word >>(16*(1-((address-DATA_OFFSET)%2))));
    return(0xFFFF & wmask);
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

int lui( const int& imm){
  return ((imm) << 16);

}

uint32_t lwr(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem){

  uint32_t address = r1 + offset;
  int align = address%4;
  uint32_t word;

  if((address >= IMEM_OFFSET) && (address < IMEM_OFFSET + IMEM_LENGTH)){
    word = instMem[(address-IMEM_OFFSET)/4];

  }
  else if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH)){
    word = dataMem[(address-DATA_OFFSET)/4];
  }
  else if(address == 0x30000000){
    char response;
    std::cin>>response;
    return response;
  }
  else{
    std::exit(-11);
  }

  uint32_t mask = 0xFF000000;
  uint32_t result = 0;
    for (int i = 0; i < align+1; i++){
      result = (mask & word) | result;
      mask = mask >> 8;
    }
  return(result);

}

uint32_t lwl(const int &r1, const uint32_t &offset, std::vector<uint32_t>& instMem, std::vector<uint32_t>&dataMem){

  uint32_t address = r1 + offset;
  int align = address%4;
  uint32_t word;

  if((address >= IMEM_OFFSET) && (address < IMEM_OFFSET + IMEM_LENGTH)){
    word = instMem[(address-IMEM_OFFSET)/4];

  }
  else if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH)){
    word = dataMem[(address-DATA_OFFSET)/4];
  }
  else if(address == 0x30000000){
    char response;
    std::cin>>response;
    return response;
  }
  else{
    std::exit(-11);
  }

  uint32_t mask = 0xFF;
  uint32_t result = 0;
    for (int i = 0; i < align+1; i++){
      result = (mask & word) | result;
      mask = mask << 8;
    }
  return(result);

}

void sb(const int &r1, const int&r2, const uint32_t &offset, std::vector<uint32_t>&dataMem){

  uint32_t address = r1 + offset;
  int align = 8*(3- (address%4));
  if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH - 1)){
    uint32_t target = dataMem[(address-DATA_OFFSET)/4] & ~(0xFF << align);
    dataMem[(address-DATA_OFFSET)/4] = target|((r2 & 0xFF)<<offset);
  }
  else if(address == 0x30000004){
    char response = r2;
    std::cout<< response;
  }
  else{
    std::exit(-11);
  }

}

void sh(const int &r1, const int&r2, const uint32_t &offset, std::vector<uint32_t>&dataMem){

  uint32_t address = r1 + offset;
  int align = 16* (2-(address%2));
  if((address >= DATA_OFFSET) && (address < DATA_OFFSET + DATA_LENGTH - 1)){
    uint32_t target = dataMem[(address-DATA_OFFSET)/4] & ~(0xFF << align);
    dataMem[(address-DATA_OFFSET)/4] = target|((r2 & 0xFF)<<offset);
  }
  else if(address == 0x30000004){
    char response = r2;
    std::cout<< response;
  }
  else{
    std::exit(-11);
  }

}

int slti(int r1, int imm){
  if(r1 < imm){
    return 1;
  }
  else{
    return 0;
  }
}
int sltiu(uint32_t r1, uint32_t imm){
  if(r1 < imm){
    return 1;
  }
  else{
    return 0;
  }
}
int xorI(const int &r1, const int& imm){
  return (r1 ^ imm);
}


//Qs : how do you jump to 0 if you add PC offset each time!!?
//After delay, after jump, do you add +4 to pc before finding next instruction
//if next instruction after jalr is a jalr, is the first destination still put into register

//lh, addI