#include "memory.h"

#include <iostream>
#include <cstdint>
#include <functional>

using namespace std;

constexpr bool debug_trace = false;

int32_t op00_halt(Memory&, int32_t position)
  {
  if (debug_trace)
    cerr << "op00_halt()" << endl;

  cout << endl << "Execution halted." << endl;
  return -1;
  }

int32_t op01_set(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op01_set(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "]" << ")" << endl;

  mem.reg(mem[position+1]) = mem.translate_operand(mem[position+2]);
  return position+3;
  }

int32_t op02_push(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op02_push(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "])" << endl;

  mem.stack_push(mem.translate_operand(mem[position+1]));
  return position+2;
  }

int32_t op03_pop(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op03_pop(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "])" << endl;

  mem.reg(mem[position+1]) = mem.stack_pop();
  return position+2;
  }

int32_t op04_eq(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op04_eq(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "]" << "," << mem[position+3] << "[" << mem.translate_operand(mem[position+3]) << "]" << ")" << endl;

  uint16_t& result = mem.reg(mem[position+1]);
  uint16_t lhs = mem.translate_operand(mem[position+2]);
  uint16_t rhs = mem.translate_operand(mem[position+3]);
  if (lhs == rhs)
    result = 1;
  else
    result = 0;

  return position+4;
  }

int32_t op05_gt(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op05_gt(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "]" << "," << mem[position+3] << "[" << mem.translate_operand(mem[position+3]) << "]" << ")" << endl;

  uint16_t& result = mem.reg(mem[position+1]);
  uint16_t lhs = mem.translate_operand(mem[position+2]);
  uint16_t rhs = mem.translate_operand(mem[position+3]);
  if (lhs > rhs)
    result = 1;
  else
    result = 0;

  return position+4;
  }

int32_t op06_jmp(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op06_jmp(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << ")" << endl;
  return mem.translate_operand(mem[position+1]);
  }

int32_t op07_jt(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op07_jt(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "]" << ")" << endl;

  uint16_t operand = mem.translate_operand(mem[position+1]);
  if (operand != 0)
    return mem.translate_operand(mem[position+2]);

  return position+3;
  }

int32_t op08_jf(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op08_jf(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "]" << ")" << endl;

  uint16_t operand = mem.translate_operand(mem[position+1]);
  if (operand == 0)
    return mem.translate_operand(mem[position+2]);

  return position+3;
  }

int32_t op09_add(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op09_add(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "]" << "," << mem[position+3] << "[" << mem.translate_operand(mem[position+3]) << "]" << ")" << endl;

  uint16_t& result = mem.reg(mem[position+1]);
  uint16_t lhs = mem.translate_operand(mem[position+2]);
  uint16_t rhs = mem.translate_operand(mem[position+3]);
  result = (lhs + rhs) % (mem.max_memory_addr+1);

  return position+4;
  }

int32_t op10_mult(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op10_mult(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "]" << "," << mem[position+3] << "[" << mem.translate_operand(mem[position+3]) << "]" << ")" << endl;

  uint16_t& result = mem.reg(mem[position+1]);
  uint16_t lhs = mem.translate_operand(mem[position+2]);
  uint16_t rhs = mem.translate_operand(mem[position+3]);
  result = (lhs * rhs) % (mem.max_memory_addr+1);

  return position+4;
  }

int32_t op11_mod(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op11_mod(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "]" << "," << mem[position+3] << "[" << mem.translate_operand(mem[position+3]) << "]" << ")" << endl;

  uint16_t& result = mem.reg(mem[position+1]);
  uint16_t lhs = mem.translate_operand(mem[position+2]);
  uint16_t rhs = mem.translate_operand(mem[position+3]);
  result = lhs % rhs;

  return position+4;
  }

int32_t op12_and(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op12_and(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "]" << "," << mem[position+3] << "[" << mem.translate_operand(mem[position+3]) << "]" << ")" << endl;

  uint16_t& result = mem.reg(mem[position+1]);
  uint16_t lhs = mem.translate_operand(mem[position+2]);
  uint16_t rhs = mem.translate_operand(mem[position+3]);
  result = lhs & rhs;

  return position+4;
  }

int32_t op13_or(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op13_or(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "]" << "," << mem[position+3] << "[" << mem.translate_operand(mem[position+3]) << "]" << ")" << endl;

  uint16_t& result = mem.reg(mem[position+1]);
  uint16_t lhs = mem.translate_operand(mem[position+2]);
  uint16_t rhs = mem.translate_operand(mem[position+3]);
  result = lhs | rhs;

  return position+4;
  }

int32_t op14_not(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op14_not(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "])" << endl;

  mem.reg(mem[position+1]) = (~mem.translate_operand(mem[position+2])) & 0x7FFF;

  return position+3;
  }

int32_t op15_rmem(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op15_rmem(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "])" << endl;

  mem.reg(mem[position+1]) = mem[mem.translate_operand(mem[position+2])];
  return position+3;
  }

int32_t op16_wmem(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op16_wmem(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "]" << "," << mem[position+2] << "[" << mem.translate_operand(mem[position+2]) << "])" << endl;

  mem[mem.translate_operand(mem[position+1])] = mem.translate_operand(mem[position+2]);
  return position+3;
  }

int32_t op17_call(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op17_call(" << mem[position+1] << "[" << mem.translate_operand(mem[position+1]) << "])" << endl;

  mem.stack_push(position+2);
  return mem.translate_operand(mem[position+1]);
  }

int32_t op18_ret(Memory& mem, int32_t position)
  {
  if (debug_trace)
    cerr << "op18_ret()" << endl;

  if (mem.stack_empty())
    return -1;

  return mem.stack_pop();
  }

int32_t op19_out(Memory& mem, int32_t position)
  {
  cout << static_cast<char>(mem.translate_operand(mem[position+1]));
  flush(cout);
  return position+2;
  }

int32_t op20_in(Memory& mem, int32_t position)
  {
  char character;
  cin >> noskipws >> character;
  mem.reg(mem[position+1]) = character;
  return position+2;
  }


int32_t op21_noop(Memory&, int32_t position)
  {
  if (debug_trace)
    cerr << "op21_noop()" << endl;
  return position+1;
  }

int32_t unknown(Memory&, int32_t position)
  {
  abort();
  return position+1;
  }

std::function<int32_t(Memory&, int32_t)> ops[] = {
  op00_halt,
  op01_set,
  op02_push,
  op03_pop,
  op04_eq,
  op05_gt,
  op06_jmp,
  op07_jt,
  op08_jf,
  op09_add,
  op10_mult,
  op11_mod,
  op12_and,
  op13_or,
  op14_not,
  op15_rmem,
  op16_wmem,
  op17_call,
  op18_ret,
  op19_out,
  op20_in,
  op21_noop
};

int main()
  {
  Memory test("challenge.bin");

  int32_t position = 0;
  while (position >= 0)
    {
    uint16_t op = test[position];
    position = ops[op](test,position);
    }

  return 0;
  }