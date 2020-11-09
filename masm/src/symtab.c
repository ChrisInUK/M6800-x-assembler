
#include "symtab.h"

/*
 *      install --- add a symbol to the table
 */
int install(char *str, int val)
{
        struct link *lp;
	struct nlist *np,*p,*backp;
	struct nlist *lookup();
	int     i;

	if( !alpha(*str) ){
                printf("`%s` : ", str);
		error("Illegal Symbol Name");
		return(NO);
		}
	if( (np = lookup(str)) != NULL ){
		if( Pass==2 ){
			if( np->def == val )
				return(YES);
			else{
				error("Phasing Error");
				return(NO);
				}
			}
		error("Symbol Redefined");
		return(NO);
		}
	/* enter new symbol */

	if (Debug) printf("Installing %s as $%x\n",str,val);

	np = (struct nlist *) alloc(sizeof(struct nlist));
	if( np == (struct nlist *)ERR ){
		error("Symbol table full");
		return(NO);
		}
	np->name = alloc(strlen(str)+1);
	if( np->name == (char *)ERR ){
		error("Symbol table full");
		return(NO);
		}
	strcpy(np->name,str);
	np->def = val;
	np->Lnext = NULL;  
        np->Rnext = NULL;
           lp = (struct link *) alloc(sizeof(struct link));
           np->L_list = lp;
           lp->L_num = Line_num;
           lp->next = NULL;
        p = root;
          backp = NULL;
           while (p != NULL) 
            {
              backp = p;
              i = strcmp (str,p->name);
               if (i<0)
                   p=p->Lnext;
                  else p=p->Rnext;
            }
          if (backp == NULL)
              root = np;
             else if (strcmp(str,backp->name)<0)
                  backp->Lnext = np;
                 else backp->Rnext = np;
          return (YES);  
}

/*
 *      lookup --- find string in symbol table
 */
struct nlist * lookup(char *name)
{
	struct nlist *np;
	int     i;

        np = root;
         while (np != NULL)
          {
            i = strcmp(name,np->name);
             if (i == 0)
               {
                 Last_sym = np->def;
                 return (np);
               }
             else if (i < 0)
                 np = np->Lnext;
                else np = np->Rnext;
          }
      Last_sym = 0;
      if (Pass == 2)
          error ("symbol Undefined on pass 2");
        return (NULL); 
}


#define NMNE (sizeof(table)/ sizeof(struct oper))
#define NPSE (sizeof(pseudo)/ sizeof(struct oper))
/*
 *      mne_look --- mnemonic lookup
 *
 *      Return pointer to an oper structure if found.
 *      Searches both the machine mnemonic table and the pseudo table.
 */

#if 1

// CDC: 2018 : Fixing (!) segmentation fault in old code

struct oper * mne_look(char *str)
 { 
 int iAt;

 //x if (Debug) printf("mne_lookup: Look for opdcode: `%s`\n", str);

 // Check for machine opcode...

 // CDC: Have added a dummy entry at start of table in table0.c and table11.c
 // CDC: and now iterate this loop from 1 instaead of 0 ...
 for ( iAt=1; iAt < NMNE; iAt++)
  {
  //x  if (Debug) printf("mne_lookup: `%s` vs [%i]=%s\n", str, iAt, table[iAt].mnemonic );
  if ( strcmp( str, table[iAt].mnemonic) == 0 )
    {
    return( &table[iAt] );
    }
  }

 //x if (Debug) printf("mne_lookup: Look for pseudo opdcode: `%s`\n", str);

 // Check for pseudo opcode...
 for ( iAt=0; iAt < NPSE; iAt++)
  {
  if ( strcmp( pseudo[iAt].mnemonic, str) == 0 )
    {
    return( &pseudo[iAt] );
    }
  }

 return(NULL);
 }

#else

struct oper * mne_look(char *str)
 { 
 struct oper *low,*high,*mid;
 int     cond;

  /* Search machine mnemonics first */
 low =  &table[0];
 high = &table[ NMNE-1 ];
 while (low <= high)
   {
   mid = low + (high-low)/2;
   if ( ( cond = strcmp(str,mid->mnemonic)) < 0) high = mid - 1;
   else if (cond > 0) low = mid + 1;
   else	return(mid);
   } 

 /* Check for pseudo ops */
 low =  &pseudo[0];
 high = &pseudo[ NPSE-1 ];
 while (low <= high)
   {
   mid = low + (high-low)/2;
   if ( ( cond = strcmp(str,mid->mnemonic)) < 0) high = mid - 1;
   else if (cond > 0) low = mid + 1;
   else return(mid);
   }

 return(NULL);
 }
#endif
