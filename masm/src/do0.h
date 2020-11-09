#if !defined C_DO0_H
#define C_DO0_H

// Added: CDC: 2018

#define PAGE1   0x00
#define PAGE2   0x18
#define PAGE3   0x1A
#define PAGE4   0xCD

/* addressing modes */
#define IMMED	0	/* immediate */
#define IND	1	/* indexed */
#define OTHER	2	/* NOTA */

// Templates...

int localinit();
int do_op(int opcode,int class);
int do_gen(int op,int mode);
int do_indexed(int op);

#endif

