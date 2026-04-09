Tool for disassembling RISC-V and porting it to ARM.

This project aims to provide a tool to assist with porting software from RISC-V to ARM when the source code is not available. It acts as a potential performance enhancing alternative to emulators and dynamic binary translation tools, by removing the overhead they require and allowing optimisation of the disassembled code. Furthermore, the disassembled code can also be ported to other ISAs easily. Some test cases are included in this repository to allow example running, but it does not include the full suite of tests used on the code.

# Running The Code

1. Assemble some RV32I code into machine code in ELF format (some are provided)
2. Place the machine code in the data/test/elf directory
3. Compile the project with the MakeFile
4. Run the code with ./build/disassemble
5. Enter the file name
