/* (c) Simon Toth 2016 */

#ifndef SYNACOR_CHALLENGE_MEMORY_H
#define SYNACOR_CHALLENGE_MEMORY_H

#include <string>
#include <cstdint>
#include <array>
#include <stack>

class Memory
  {
public:
  Memory(const std::string& filename);
  ~Memory();

  uint16_t operator[](uint16_t index) const;
  uint16_t& operator[](uint16_t index);

  uint16_t reg(uint16_t index) const;
  uint16_t& reg(uint16_t index);

  uint16_t translate_operand(uint16_t value) const;

  static constexpr uint16_t max_operand_value = 32775;
  static constexpr uint16_t max_memory_addr = 32767;
  static constexpr uint16_t max_register_index = 7;

  void stack_push(uint16_t value);
  uint16_t stack_pop();
  bool stack_empty();

private:
  uint16_t *p_code;
  uint16_t *p_memory;
  size_t p_code_size;
  int p_fd;

  std::array<uint16_t,8> p_registers;
  std::stack<uint16_t> p_stack;
  };

#endif //SYNACOR_CHALLENGE_CODE_H
