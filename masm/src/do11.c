/*
 *      MC68HC11 specific processing
 */


#include "do11.h"

int     yflag = 0;      /* YNOIMM, YLIMM, and CPD flag */

/*
 *      localinit --- machine specific initialization
 */
int localinit()
{
return(0);
}

/*
 *      do_op --- process mnemonic
 *
 *	Called with the base opcode and it's class. Optr points to
 *	the beginning of the operand field.
 */
int do_op(int opcode,int class)
{
	char *skip_white();
	int     dist;   /* relative branch distance */
	int     amode;  /* indicated addressing mode */
	char	*peek;

 if (Debug) printf("DoOp: Opcode=%i, Class=%i\n", opcode, class);

	/* guess at addressing mode */
	peek = Optr;
	amode = OTHER;
	while( !delim(*peek) && *peek != EOS)  /* check for comma in operand field */
		if( *peek++ == ',' ){
			if( mapdn(*peek) == 'y' )
				amode = INDY;
			else
				amode = INDX;
			break;
			}
	if( *Optr == '#' ) amode = IMMED;

	yflag = 0;
	switch(class){
		case P2INH:
			emit(PAGE2);
		case INH:                       /* inherent addressing */
			emit(opcode);
			return(0);
		case REL:                       /* relative branches */
			eval();
			dist = Result - (Pc+2);
			emit(opcode);
			if( (dist >127 || dist <-128) && Pass==2){
				error("Branch out of Range");
				emit(lobyte(-2));
				return(0);
				}
			emit(lobyte(dist));
			return(0);
		case LONGIMM:
			if( amode == IMMED )
				amode = LIMMED;
		case NOIMM:
			if( amode == IMMED ){
				error("Immediate Addressing Illegal");
				return(0);
				}
		case GEN:                       /* general addressing */
			do_gen(opcode,amode,PAGE1,PAGE1,PAGE2);
			return(0);
		case GRP2:
			if( amode == INDY ){
				Cycles++;
				emit(PAGE2);
				amode = INDX;
				}
			if( amode == INDX )
				do_indexed(opcode);
			else{   /* extended addressing */
				eval();
				emit(opcode+0x10);
				eword(Result);
				}
			return(0);
		case CPD:               /* cmpd */
			if( amode == IMMED )
				amode = LIMMED;
			if( amode == INDY )
				yflag=1;
			do_gen(opcode,amode,PAGE3,PAGE3,PAGE4);
			return(0);
		case XNOIMM:            /* stx */
			if( amode == IMMED ){
				error("Immediate Addressing Illegal");
				return(0);
				}
		case XLIMM:             /* cpx, ldx */
			if( amode == IMMED )
				amode = LIMMED;
			do_gen(opcode,amode,PAGE1,PAGE1,PAGE4);
			return(0);
		case YNOIMM:            /* sty */
			if( amode == IMMED ){
				error("Immediate Addressing Illegal");
				return(0);
				}
		case YLIMM:             /* cpy, ldy */
			if(amode == INDY)
				yflag=1;
			if( amode == IMMED )
				amode = LIMMED;
			do_gen(opcode,amode,PAGE2,PAGE3,PAGE2);
			return(0);
		case BTB:               /* bset, bclr */
		case SETCLR:            /* brset, brclr */
			opcode = bitop(opcode,amode,class);

			if (amode == INDX)
				Cycles++;
			if( amode == INDY ){
				Cycles+=2;
				emit(PAGE2);
				amode = INDX;
				}
			emit(opcode);
			eval();
			emit(lobyte(Result));   /* address */
			if( amode == INDX )
				Optr += 2;      /* skip ,x or ,y */
			Optr = skip_white(Optr);
			eval();
			emit(lobyte(Result));   /* mask */
			if( class == SETCLR )
				return(0);
			Optr = skip_white(Optr);
			eval();
			dist = Result - (Pc+1);
			if( (dist >127 || dist <-128) && Pass==2){
				error("Branch out of Range");
				dist = Old_pc - (Pc+1);
				}
			emit(lobyte(dist));
			return(0);
		default:
			fatal("Error in Mnemonic table");
		}
}

/*
 *      bitop --- adjust opcode on bit manipulation instructions
 */
int bitop(int op,int mode,int class)
{
	if( mode == INDX || mode == INDY )
		return(op);
	if( class == SETCLR )
		return(op-8);
	else if(class==BTB)
		return(op-12);
	else
		fatal("bitop");
}

/*
 *      do_gen --- process general addressing modes
 */
int do_gen(int op,int mode,int pnorm,int px,int py)
{
	switch(mode){
	case LIMMED:
		Optr++;
		epage(pnorm);
		emit(op);
		eval();
		eword(Result);
		break;
	case IMMED:
		Optr++;
		epage(pnorm);
		emit(op);
		eval();
		emit(lobyte(Result));
		break;
	case INDY:
		if(yflag)
			Cycles += 2;
		else
			Cycles += 3;
		epage(py);
		do_indexed(op+0x20);
		break;
	case INDX:
		Cycles+=2;
		epage(px);
		do_indexed(op+0x20);
		break;
	case OTHER:
		eval();
		epage(pnorm);
		if(Force_word){
			emit(op+0x30);
			eword(Result);
			Cycles+=2;
			break;
			}
		if(Force_byte){
			emit(op+0x10);
			emit(lobyte(Result));
			Cycles++;
			break;
			}
		if(Result>=0 && Result <=0xFF){
			emit(op+0x10);
			emit(lobyte(Result));
			Cycles++;
			break;
			}
		else {
			emit(op+0x30);
			eword(Result);
			Cycles+=2;
			break;
			}
		break;
	default:
		error("Unknown Addressing Mode");
	}
}

/*
 *      do_indexed --- handle all wierd stuff for indexed addressing
 */
int do_indexed(int op)
{
	char c;

	emit(op);
	eval();
	if( *Optr++ != ',' )
		error("Syntax");
	c = mapdn(*Optr++);
	if( c != 'x' && c != 'y')
		warn("Indexed Addressing Assumed");
	if( Result < 0 || Result > 255)
		warn("Value Truncated");
	emit(lobyte(Result));
}

/*
 *      epage --- emit page prebyte
 */
int epage(int p)
{
	if( p != PAGE1 )        /* PAGE1 means no prebyte */
		emit(p);
}
