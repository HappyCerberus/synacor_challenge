#include "memory.h"

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>
using namespace std;



Memory::Memory(const string& filename)
  {
  p_fd = open(filename.c_str(), O_RDONLY); // open file
  if (p_fd == -1)
    throw runtime_error(string("Couldn't open file \"")+filename+string("\"."));

  struct stat fs;
  if (fstat(p_fd, &fs) == -1)
    {
    close(p_fd);
    throw runtime_error(string("Couldn't stat file \""+filename+string("\".")));
    }

  p_code = static_cast<uint16_t*>(mmap(0, static_cast<size_t>(fs.st_size), PROT_READ, MAP_SHARED, p_fd, 0));
  if (p_code == MAP_FAILED)
    {
    close(p_fd);
    throw runtime_error(string("Couldn't map file \""+filename+("\" to memory.")));
    }

  p_code_size = static_cast<size_t>(fs.st_size/2);
  p_memory = new uint16_t[p_code_size];
  for (size_t i = 0; i < p_code_size; i++)
    p_memory[i] = p_code[i];

  for (size_t i = 0; i < 8; i++)
    p_registers[i] = 0;

  }

Memory::~Memory()
  {
  munmap(p_code,p_code_size*2);
  close(p_fd);
  delete[] p_memory;
  }

uint16_t Memory::operator[](uint16_t index) const
  {
  if (index > max_memory_addr)
    throw out_of_range("Memory address out of bounds.");

  return p_memory[index];
  }

uint16_t& Memory::operator[](uint16_t index)
  {
  if (index > max_memory_addr)
    throw out_of_range("Memory address out of bounds.");

  return p_memory[index];
  }

uint16_t Memory::reg(uint16_t index) const
  {
  if (index > max_register_index+max_memory_addr+1 || index <= max_memory_addr)
    throw out_of_range("Register index out of bounds.");

  return p_registers[index-max_memory_addr-1];
  }

uint16_t& Memory::reg(uint16_t index)
  {
  if (index > max_register_index+max_memory_addr+1 || index <= max_memory_addr)
    throw out_of_range("Register index out of bounds.");

  return p_registers[index-max_memory_addr-1];
  }

uint16_t Memory::translate_operand(uint16_t value) const
  {
  if (value > max_operand_value)
    throw out_of_range("Operand value out of range.");

  if (value > max_memory_addr)
    return reg(value);

  return value;
  }

uint16_t Memory::stack_pop()
  {
  if (p_stack.size() == 0)
    throw runtime_error("Popping empty stack.");

  uint16_t value = p_stack.top();
  p_stack.pop();
  return value;
  }

void Memory::stack_push(uint16_t value)
  {
  p_stack.push(value);
  }

bool Memory::stack_empty()
  {
  return p_stack.size() == 0;
  }