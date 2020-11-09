# M6800-x-assembler
Modified MASM freeware cross-compiler for Motorola 6800

Extract from masm/src/README :-

------------------------------------------------------------------------------
Motorola Freeware Assemblers for Unix (masm) : GFC

------------------------------------------------------------------------------
Mods: Feb.2018 : CDC : as0.c build ( > gcc -o as6800 as0.c )
 
        : Added dummy entry to start of table0.h 
        : May be a compile bug, but access to table[0] in list 
        : gives a segmentation fault - perhaps due to 64-bit compile
        : Also modified simtab.c opcode search code in testing for fault
        : many .h headers added and function call syntax changed
        : with functions getting return type of int if none had
        : been specifed in original code.

        : Most of this also done to as11 specific files

        : Empty lines are now ignored
        : Any line with first non-whitespace character of
        : * ; or # are now ignored

        : Have also built for both linux mint and windows
        : files are in ../bin folder

------------------------------------------------------------------------------
...
