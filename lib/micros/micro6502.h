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

  /**
   * @brief Writes one byte of given data on set addres
   * 
   * @param cycles 
   * @param addres 
   * @param data 
   */
  void writeByte(u32 &cycles, u32 addres, Byte data);
};

struct CPU
{
  Word PC; //Progam Counter
  Byte SP; //Stack Pointer

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
  //: Is not necesary to do this overload,
  //: if we send a Byte the MSB become cero.
  // Byte readByte(MEMORY &memory, u32 &cycles, Byte address);
  Byte readByteMemory(MEMORY &memory, u32 &cycles, Word address);
  Byte fetchByteMemory(MEMORY &memory, u32 &cycles);

  Word readWordMemory(MEMORY &memory, u32 &cycles, Word addres);
  Word fetchWordMemory(MEMORY &memory, u32 &cycles);

  Byte fetchRegister(u32 &cycles, Byte &reg);
  Byte readRegister(Byte &reg);

  void writeRegister(Byte &reg, Byte data);

  /**
   *  @brief Set the status of the Z and N registers when 
   *         calling a Load Instruction 
   */
  void loadSetStatus(Byte reg);

  /**
   *  @brief Set the status of the Z and N registers 
   *         when calling an increment instruction
   */
  void incrementSetStatus(Byte reg);

  //====================================================
  /**
   * @brief OPCODE definitions
   * @{
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

      //LDX:  Loads a byte of memory into the X register setting
      //      the zero and negative flags as appropriate.
      INS_LDX_IM = 0xA2,
      INS_LDX_ZP = 0xA6,
      INS_LDX_ZPY = 0xB6,
      INS_LDX_ABS = 0xAE,
      INS_LDX_ABSY = 0xBE,

      //INX:  Adds one to the X register setting the
      //      zero and negative flags as appropriate.
      INS_INX = 0xE8,

      //INY:  Adds one to the Y register setting the
      //      zero and negative flags as appropriate.
      INS_INY = 0xC8,

      //INC:  Adds one to the value held at a specified memory
      //      location setting the zero and negative flags as appropriate.
      INS_INC_ZP = 0xE6,
      INS_INC_ZPX = 0xF6,
      INS_INC_ABS = 0xEE,
      INS_INC_ABSX = 0xFE,

      //STA:  Stores the contents of the accumulator into memory.
      INS_STA_ZP = 0x85,
      INS_STA_ZPX = 0x95,
      INS_STA_ABS = 0x8D,
      INS_STA_ABSX = 0x9D,
      INS_STA_ABSY = 0x99,
      INS_STA_INDX = 0x81,
      INS_STA_INDY = 0x91,

      //STX:  Stores the contents of the X register into memory.
      INS_STX_ZP = 0x86,
      INS_STX_ZPY = 0x96,
      INS_STX_ABS = 0x8E,

      //STY:  Stores the contents of the Y register into memory.
      INS_STY_ZP = 0x84,
      INS_STY_ZPX = 0x94,
      INS_STY_ABS = 0x8C,
      
      //JSR:  pushes the address (minus one) of the return point
      //      on to the stack and then sets the program counter to
      //      the target memory address.
      INS_JSR = 0x20;
  /** @} */
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