/*
 *	MC6800/02 specific processing
 */

/* addressing modes */
#define IMMED	0	/* immediate */
#define IND	1	/* indexed */
#define OTHER	2	/* NOTA */

/*
 *	localinit --- machine specific initialization
 */
int localinit()
{
return(0);
}

/*
 *	do_op --- process mnemonic
 *
 *	Called with the base opcode and it's class. Optr points to
 *	the beginning of the operand field.
 */
int do_op(int opcode,int class)
{
	int	dist;	/* relative branch distance */
	int	amode;	/* indicated addressing mode */
	char	*peek;

	/* guess at addressing mode */
	peek = Optr;
	amode = OTHER;
	while( !delim(*peek) && *peek != EOS)  /* check for comma in operand field */
		if( *peek++ == ',' ){
			amode = IND;
			break;
			}
	if( *Optr == '#' ) amode = IMMED;

	switch(class){
		case INH:			/* inherent addressing */
			emit(opcode);
			return(0);
		case GEN:			/* general addressing */
			do_gen(opcode,amode);
			return(0);
		case REL:			/* relative branches */
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
		case NOIMM:
			if( amode == IMMED){
				error("Immediate Addressing Illegal");
				return(0);
				}
			if((opcode == 0x8D) && (amode == IND)){
				Cycles-=2;
				}
			do_gen(opcode,amode);

			return(0);
		case LONGIMM:
			if( amode == IMMED ){
				emit(opcode);
				Optr++;
				eval();
				eword(Result);
				return(0);
				}
			do_gen(opcode,amode);
			return(0);
		case GRP2:
			if( amode == IND ){
				Cycles++;
				do_indexed(opcode);
				return(0);
				}
			/* extended addressing */
			eval();
			emit(opcode+0x10);
			eword(Result);
			return(0);
		default:
			fatal("Error in Mnemonic table");
		}
}

/*
 *	do_gen --- process general addressing modes
 */
int do_gen(int op,int mode)
{
	if( mode == IMMED){
		Optr++;
		emit(op);
		eval();
		emit(lobyte(Result));
		return(0);
		}
	else if( mode == IND ){
		Cycles+=3;
		do_indexed(op+0x20);
		return(0);
		}
	else if( mode == OTHER){
		eval();
		if(Force_word){
			emit(op+0x30);
			eword(Result);
			Cycles+=2;
			return(0);
			}
		if(Force_byte){
			emit(op+0x10);
			emit(lobyte(Result));
			Cycles++;
			return(0);
			}
		if(Result>=0 && Result <=0xFF){
			emit(op+0x10);
			emit(lobyte(Result));
			Cycles++;
			return(0);
			}
		else {
			emit(op+0x30);
			eword(Result);
			Cycles+=2;
			return(0);
			}
		}
	else {
		error("Unknown Addressing Mode");
		return(0);
		}
}

/*
 *	do_indexed --- handle all wierd stuff for indexed addressing
 */
int do_indexed(int op)
{
	emit(op);
	eval();
	if( mapdn(*++Optr) != 'x' )
		warn("Indexed Addressing Assumed");
	if( Result < 0 || Result > 255)
		warn("Value Truncated");
	emit(lobyte(Result));
}
