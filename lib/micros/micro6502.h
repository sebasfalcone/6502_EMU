#ifndef EMULATOR
#define EMULATOR

#include <inttypes.h> //Gave us uint8_t uint16_t etc definitions

using Byte = uint8_t;
using Word = uint16_t;

using u32 = uint32_t;

struct MEMORY
{
  //USING 64 MB of memory
  static constexpr u32 MAX_MEM = 1024 * 64;
  Byte data[MAX_MEM];

  /**
   * @brief Memory cleanup
   */
  void init();

  /**
   * @brief Read 1 byte from memory
   * 
   * @param addr Memory address to read from -  
   * @return Byte 
   */
  Byte &operator[](u32 addr);

  /**
  * @brief Writes two bytes of given data on set addres
  * 
  * @param cycles 
  * @param addres 
  * @param data 
  */
  void writeWord(u32 &cycles, u32 addres, Word data);
};

struct CPU
{
  Word PC; //Progam Counter
  Word SP; //Stack Pointer

  Byte A, X, Y; //Registers

  //Status flags
  Byte C : 1; //Carry
  Byte Z : 2; //Zero
  Byte I : 3; //Interrupt disable
  Byte D : 4; //Decimal mode
  Byte B : 5; //Break command
  Byte V : 6; //Overflow
  Byte N : 7; //Negative

  /**
   * @brief Initialize SP and PC. Clear flags and registers
   * 
   */
  void reset(MEMORY &);
  //====================================================

  Byte fetchByte(MEMORY &, u32 &);
  Byte readByte(MEMORY &, u32 &, Byte);

  Word fetchWord(MEMORY &, u32 &);

  /**
   * @brief Set the status of the Z and N registers
   *        when calling an LDA instruction
   * 
   */
  void LDAsetStatus(void);
  //====================================================
  /**
   * @brief OPCODE definitions
   * 
   */
  static constexpr Byte
      //LDA:  Loads a byte of memory into the accumulator setting
      //      the zero and negative flags as appropriate.
      INS_LDA_IM = 0xA9,
      INS_LDA_ZP = 0xA5,
      INS_LDA_ZPX = 0XAD,

      //JSR:  pushes the address (minus one) of the return point
      //      on to the stack and then sets the program counter to
      //      the target memory address.
      INS_JSR = 0x20;
  //====================================================
  /**
   * @brief 
   * 
   * @param memory wich mem you are using.
   * @param ticks  amount of clock cycles to execute
   */
  void exec(MEMORY &, u32);
};

#endif //if EMULATOR