References:
  https://en.wikipedia.org/wiki/MOS_Technology_6502
  http://www.obelisk.me.uk/6502

Notes:

Zero page importance:
Only takes an 8bit addres so:
  -you have to fetch only one time from memory (improoves speed)
  -Only the LSB helds the instruction, making it 1 byte shorter (saves space)


Cycles per instruction:

                  Load Instructions
+-----------------+--------+-------+------------------------+
| Addressing Mode | Opcode | Bytes |         Cycles         |
+-----------------+--------+-------+------------------------+
| Immediate       | 0xA9   |   2   | 2                      |
| Zero Page       | 0xA5   |   2   | 3                      |
| Zero Page,x     | 0xB5   |   2   | 4                      |
| Absolute        | 0xAD   |   3   | 4                      |
| Absolute,x      | 0xBD   |   3   | 4 (+1 if page crossed) |
| Absolute,Y      | 0xB9   |   3   | 4 (+1 if page crossed) |   
| (Indirect,x)    | 0xA1   |   2   | 6                      |
| (Indirect),Y    | 0xB1   |   2   | 5 (+1 if page crossed) |
+-----------------+--------+-------+------------------------+

