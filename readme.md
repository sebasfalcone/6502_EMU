OPCODES

INS_LDA_IM:
Loads a byte of memory into the accumulator setting the zero and negative flags as appropriate.
+----+--------------------+--------------------------+
| C  |    Carry Flag      |       Not affected       |
+----+--------------------+--------------------------+
| Z  | Zero Flag          | Set if A = 0             |
| I  | Interrupt Disable  | Not affected             |
| D  | Decimal Mode Flag  | Not affected             |
| B  | Break Command      | Not affected             |
| V  | Overflow Flag      | Not affected             |
| N  | Negative Flag      | Set if bit 7 of A is set |
+----+--------------------+--------------------------+

