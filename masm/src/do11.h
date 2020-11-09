#if !defined C_DO11_H
#define C_DO11_H

// Added: CDC: 2018

#define PAGE1   0x00
#define PAGE2   0x18
#define PAGE3   0x1A
#define PAGE4   0xCD

/* addressing modes */
#define IMMED   0
#define INDX    1
#define INDY    2
#define LIMMED  3       /* long immediate */
#define OTHER   4


// Templates...

int localinit();
int do_op(int opcode,int class);
int bitop(int op,int mode,int class);
int do_gen(int op,int mode,int pnorm,int px,int py);
int do_indexed(int op);
int epage(int p);

#endif

