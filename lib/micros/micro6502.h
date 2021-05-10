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
  void reset(MEMORY &memory);
  //====================================================

  Byte fetchByte(MEMORY &memory, u32 &cycles);
  Byte readByte(MEMORY &memory, u32 &cycles, Byte address);
  Byte readByte(MEMORY &memory, u32 &cycles, Word address);

  Word fetchWord(MEMORY &memory, u32 &cycles);

  /**
   * @name Load instructions
   * @brief Set the status of the Z and N registers
   *        when calling an Load Instruction
   * 
   */
  ///@{
  void LDAsetStatus(void);
  void LDXsetStatus(void);
  void LDYsetStatus(void);
  ///@}

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
      INS_LDA_ZPX = 0XB5,
      INS_LDA_ABS = 0XAD,
      INS_LDA_ABSX = 0xBD,
      INS_LDA_ABSY = 0xB9,
      INS_LDA_INDX = 0xA1,
      INS_LDA_INDY = 0xB1,

      //LDY:  Loads a byte of memory into the Y register setting
      //      the zero and negative flags as appropriate.
      INS_LDY_IM = 0xA0,
      INS_LDY_ZP = 0xA4,
      INS_LDY_ZPX = 0xB4,
      INS_LDY_ABS = 0xAC,
      INS_LDY_ABSX = 0xBC,

      

      //JSR:  pushes the address (minus one) of the return point
      //      on to the stack and then sets the program counter to
      //      the target memory address.
      INS_JSR = 0x20;
  //====================================================
  /**
   * @brief 
   * 
   * @param memory wich mem you are using.
   * @param cycles  amount of clock cycles to execute
   */
  void exec(MEMORY &memory, u32 &cycles);
};

#endif //if EMULATOR