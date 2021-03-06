#if !defined C_PSEUDO_H
#define C_PSEUDO_H
// Added: CDC: 2018

#define RMB     0       /* Reserve Memory Bytes         */
#define FCB     1       /* Form Constant Bytes          */
#define FDB     2       /* Form Double Bytes (words)    */
#define FCC     3       /* Form Constant Characters     */
#define ORG     4       /* Origin                       */
#define EQU     5       /* Equate                       */
#define ZMB     6       /* Zero memory bytes            */
#define FILL    7       /* block fill constant bytes    */
#define OPT     8       /* assembler option             */
#define NULL_OP 9       /* null pseudo op               */
#define PAGE    10      /* new page                     */

// Templates...

int do_pseudo(int op);


#endif
