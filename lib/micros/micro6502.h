#ifndef M6502_EMULATOR_H
#define M6502_EMULATOR_H

#include <inttypes.h> //Gave us uint8_t uint16_t etc definitions

using Byte = uint8_t;
using Word = uint16_t;
using u32 = uint32_t;

//MASKS
#define MASK_FLAG_C 0b00000001
#define MASK_FLAG_Z 0b00000010
#define MASK_FLAG_I 0b00000100
#define MASK_FLAG_D 0b00001000
#define MASK_FLAG_B 0b00010000
#define MASK_FLAG_V 0b00100000
#define MASK_FLAG_N 0b01000000

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

  Byte readByte(u32 &cycles, Word address);
  Word readWord(u32 &cycles, Word addres);

  Byte fetchByte(u32 &cycles, Word &PC);
  Word fetchWord(u32 &cycles, Word &PC);
};

struct CPU
{
  /**
   * @brief 
   * 
   * @param memory Wich memory you are using.
   * @param cycles Amount of clock cycles to execute
   */
  void exec(MEMORY &memory, u32 &cycles);

  /**
   * @brief Initialize SP and PC 
   *        Clear flags and registers
   *        Initialize Memory
   */
  void reset(MEMORY &memory);
  //====================================================
  /**
   * @brief Operations with registers
   * @{
   */
  Byte fetchRegister(u32 &cycles, Byte &reg);
  Byte readRegister(Byte &reg);
  void writeRegister(Byte &reg, Byte data);
  /** @} */
  //====================================================
  /**
   *  @brief Set the status of the Z & N flags when 
   *         calling a Load Instruction 
   *  @param 
   */
  void loadSetStatus(const Byte reg);

  /**
   *  @brief Set the status of the Z & N flags 
   *         when calling an increment instruction
   *  @param reg: Destiny register
   */
  void incrementSetStatus(const Byte reg);

  /**
   * @brief Set the status of Z & N flags
   *        when calling a transfer instruction
   * @param reg: Destiny register
   */
  void transferSetStatus(const Byte reg);

  /**
   * @brief Set the status of Z, N & C flags
   *        when calling a compare instruction
   * @param select: result of comparison
   */
  void cmpSetStatus(const Byte reg);

  /**
   * @brief Set the status of Z & N flags
   *        when calling an AND instruction
   */
  void andSetStatus();

  /**
   * @brief Set the status of Z & N flags
   *        when calling an EOR instruction
   */
  void eorSetStatus();

  /**
   * @brief Set the status of Z & N flags
   *        when calling an ORA instruction
   */
  void oraSetStatus();

  /**
   * @brief Set the status of Z,N & V flags
   *        when calling an BIT instruction
   * 
   * @param value Data in memory
   */
  void bitSetStatus(Byte value);

  void adcSetStatus(const Byte reg);

  Word PC; //Progam Counter
  Byte SP; //Stack Pointer

  Byte A, X, Y; //Registers

  //Status flags
  struct statusFlags
  {
    Byte C : 1; //Carry
    Byte Z : 1; //Zero
    Byte I : 1; //Interrupt disable
    Byte D : 1; //Decimal mode
    Byte B : 1; //Break command
    Byte V : 1; //Overflow
    Byte N : 1; //Negative
  };

  union unionFlags
  {
    statusFlags one;
    Byte all;
  } flags;

  /**
   * @brief OPCODE definitions
   * @{
   */
  static constexpr Byte
      //!Increments & Decrements
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

      //LDX:  Loads a byte of memory into the X register setting
      //      the zero and negative flags as appropriate.
      INS_LDX_IM = 0xA2,
      INS_LDX_ZP = 0xA6,
      INS_LDX_ZPY = 0xB6,
      INS_LDX_ABS = 0xAE,
      INS_LDX_ABSY = 0xBE,

      //LDY:  Loads a byte of memory into the Y register setting
      //      the zero and negative flags as appropriate.
      INS_LDY_IM = 0xA0,
      INS_LDY_ZP = 0xA4,
      INS_LDY_ZPX = 0xB4,
      INS_LDY_ABS = 0xAC,
      INS_LDY_ABSX = 0xBC,

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

      //!Register Transfers
      //TAX:  Copies the current contents of A into the X.
      INS_TAX = 0xAA,

      //TAY:  Copies the current contents of the A into Y
      INS_TAY = 0xA8,

      //TXA:  Copies the current contents of the X into A.
      INS_TXA = 0x8A,

      //TYA:  Copies the current contents of the Y into A.
      INS_TYA = 0x98,

      //!Logical
      //AND:  A logical AND is performed, bit by bit, on the accumulator
      //      contents using the contents of a byte of memory.
      INS_AND_IM = 0x29,
      INS_AND_ZP = 0x25,
      INS_AND_ZPX = 0x35,
      INS_AND_ABS = 0x2D,
      INS_AND_ABSX = 0x3D,
      INS_AND_ABSY = 0x39,
      INS_AND_INDX = 0x21,
      INS_AND_INDY = 0x31,

      //EOR: An exclusive OR is performed, bit by bit, on
      //     the accumulator contents using the contents of a byte of memory.
      INS_EOR_IM = 0x49,
      INS_EOR_ZP = 0x45,
      INS_EOR_ZPX = 0x55,
      INS_EOR_ABS = 0x4D,
      INS_EOR_ABSX = 0x5D,
      INS_EOR_ABSY = 0x59,
      INS_EOR_INX = 0x41,
      INS_EOR_INY = 0x51,

      //ORA: An inclusive OR is performed, bit by bit, on
      //     the accumulator contents using the contents of a byte of memory.
      INS_ORA_IM = 0x09,
      INS_ORA_ZP = 0x05,
      INS_ORA_ZPX = 0x15,
      INS_ORA_ABS = 0x0D,
      INS_ORA_ABSX = 0x1D,
      INS_ORA_ABSY = 0x19,
      INS_ORA_INX = 0x01,
      INS_ORA_INY = 0x11,

      //BIT:  Test if one or more bits are set in a target memory location.
      //      The mask pattern in A is ANDed with the value in memory to set
      //      or cleaar the zero flag, but the result is not kept.
      //      Bits 7 and 6 of the value from memory are copied into N and V flags.
      INS_BIT_ZP = 0x24,
      INS_BIT_ABS = 0x2C,

      //!Arithmetic
      //ADC:  Adds the contents of a memory location to the acumulator with the
      //      carry biy. If overflow occurs the carry bit is set, this enables
      //      multiple byte addition to be performed
      INS_ADC_IM = 0x69,
      INS_ADC_ZP = 0x65,
      INS_ADC_ZPX = 0x75,
      INS_ADC_ABS = 0x6D,
      INS_ADC_ABSX = 0x7D,
      INS_ADC_ABSY = 0x79,
      INS_ADC_INDX = 0x61,
      INS_ADC_INDY = 0x71,

      //CPX:  Compares the contents of the X register with
      //      another memory held value.
      INS_CPX_IM = 0xE0,
      INS_CPX_ZP = 0xE4,
      INS_CPX_ABS = 0xEC,

      //!Increments & Decrements
      //INC:  Adds one to the value held at a specified memory
      //      location setting the zero and negative flags as appropriate.
      INS_INC_ZP = 0xE6,
      INS_INC_ZPX = 0xF6,
      INS_INC_ABS = 0xEE,
      INS_INC_ABSX = 0xFE,

      //INX:  Adds one to the X register setting the
      //      zero and negative flags as appropriate.
      INS_INX = 0xE8,

      //INY:  Adds one to the Y register setting the
      //      zero and negative flags as appropriate.
      INS_INY = 0xC8,

      //!Jumps & Calls
      //JSR:  pushes the address (minus one) of the return point
      //      on to the stack and then sets the program counter to
      //      the target memory address.
      INS_JSR = 0x20;

  /** @} */
};

#endif //EMULATOR