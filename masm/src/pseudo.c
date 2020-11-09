/*
 *      pseudo --- pseudo op processing
 */

#include "pseudo.h"

/*
 * ALPHABETIZED table of pseudo ops and some equivalent
 * pseudo ops added for compatibility with other assemblers.
 */

struct oper pseudo[] = {
"absolute",PSEUDO,NULL_OP,0,
"bsz",  PSEUDO, ZMB,    0,
"code", PSEUDO, NULL_OP,0,
"db",   PSEUDO, FCB,    0,
"ds",   PSEUDO, RMB,    0,
"dw",   PSEUDO, FDB,    0,
"end",  PSEUDO, NULL_OP,0,
"equ",  PSEUDO, EQU,    0,
"fcb",  PSEUDO, FCB,    0,
"fcc",  PSEUDO, FCC,    0,
"fdb",  PSEUDO, FDB,    0,
"fill", PSEUDO, FILL,   0,
"nam",  PSEUDO, NULL_OP,0,
"name", PSEUDO, NULL_OP,0,
"opt",  PSEUDO, OPT,    0,
"org",  PSEUDO, ORG,    0,
"origin",PSEUDO,ORG,    0,
"pag",  PSEUDO, PAGE,   0,
"page", PSEUDO, PAGE,   0,
"page0",PSEUDO, NULL_OP,0,
"page1",PSEUDO, NULL_OP,0,
"relative",PSEUDO,RMB,  0,
"rmb",  PSEUDO, RMB,    0,
"set",  PSEUDO, EQU,    0,
"spc",  PSEUDO, NULL_OP,0,
"ttl",  PSEUDO, NULL_OP,0,
"zmb",  PSEUDO, ZMB,    0
};

/*
 *      do_pseudo --- do pseudo op processing
 */
int do_pseudo(int op)
{
        char    fccdelim;
        int     j;
        int     fill;
        char    *skip_white();

        if( op != EQU && *Label )
                install(Label,Pc);

        P_force++;
        switch(op){
                case RMB:                       /* reserve memory bytes */
                        if( eval() ){
                                Pc +=  Result;
                                f_record();     /* flush out bytes */
                                }
                        else
                                error("RMB: Undefined Operand during Pass One");
                        break;
                case ZMB:                       /* zero memory bytes */
                        if( eval() )
                                while( Result-- )
                                        emit(0);
                        else
                                error("ZMB: Undefined Operand during Pass One");
                        break;
                case FILL:                      /* fill memory with constant */
                        eval();
                        fill = Result;
                        if( *Optr++ != ',' )
                                error("FILL: Bad fill");
                        else{
                                Optr = skip_white(Optr);
                                eval();
                                while( Result-- )
                                        emit(fill);
                                }
                        break;
                case FCB:                       /* form constant byte(s) */
                        do{
                                Optr = skip_white(Optr);
                                eval();
                                if( Result > 0xFF ){
                                        if(!Force_byte)
                                                warn("FCB: Value truncated");
                                        Result = lobyte(Result);
                                        }
                                emit(Result);
                        }while( *Optr++ == ',' );
                        break;
                case FDB:                       /* form double byte(s) */
                        do{
                                Optr = skip_white(Optr);
                                eval();
                                eword(Result);
                        }while( *Optr++ == ',' );
                        break;
                case FCC:                       /* form constant characters */
                        if(*Operand==EOS)
                                break;
                        fccdelim = *Optr++;
                        while( *Optr != EOS && *Optr != fccdelim)
                                emit(*Optr++);
                        if(*Optr == fccdelim)
                                Optr++;
                        else
                                error("FCC: Missing Delimiter");
                        break;
                case ORG:                       /* origin */
                        if( eval() ){
                                Old_pc = Pc = Result;
                                f_record();     /* flush out any bytes */
                                }
                        else
                                error("ORG: Undefined Operand during Pass One");
                        break;
                case EQU:                       /* equate */
                        if(*Label==EOS){
                                error("EQU: Requires label");
                                break;
                                }
                        if( eval() ){
                                install(Label,Result);
                                Old_pc = Result;        /* override normal */
                                }
                        else
                                error("EQU: Undefined Operand during Pass One");
                        break;
                case OPT:                       /* assembler option */
                        P_force=0;
                        if( head(Operand,"l") )
                                Lflag=1;
                        else if( head(Operand,"nol"))
                                Lflag=0;
                        else if( head(Operand,"c")){
                                Cflag=1;
                                Ctotal=0;
                                }
                        else if( head(Operand,"noc"))
                                Cflag=0;
                        else if( head(Operand,"contc")){
                                Cflag=1;
                                }
                        else if ( head(Operand,"s"))
                                Sflag = 1;
                        else if ( head(Operand,"cre"))
                                CREflag = 1;
                        else
                                error("OPT: Unrecognized");
                        break;
                case PAGE:                      /* go to a new page */
                        P_force=0;
                        N_page = 1;
                        if (Pass == 2 )
                         if (Lflag)  
                          {
                           printf ("\f");
                           printf ("%-10s",Argv[Cfn]);
                           printf ("                                   ");
                           printf ("page %3d\n",Page_num++);
                          }
                        break;
                case NULL_OP:                   /* ignored psuedo ops */
                        P_force=0;
                        break;
                default:
                        fatal("Pseudo error");
                }
}
