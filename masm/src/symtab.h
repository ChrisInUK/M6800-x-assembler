#if !defined C_SYMTAB_H
#define C_SYMTAB_H
// Added: CDC: 2018


int install(char *str, int val);
struct nlist * lookup(char *name);
struct oper * mne_look(char *str);

#endif
