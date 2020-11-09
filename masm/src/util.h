#if !defined C_UTIL_H
#define C_UTIL_H
// Added: CDC: 2018

int fatal(char *str);
int error(char *str);
int warn(char *str);
int delim(char c);
char *skip_white(char *ptr);
int eword(int wd);
int emit(int byte);
int f_record();
int hexout(int byte);
int imageinit();
int finish_outfile();
int print_line();
int any(char c,char *str);
char mapdn(char c);
int lobyte(int i);
int hibyte(int i);
int head(char *str1,char *str2);
int alpha(char c);
int alphan(char c);
int white(char c);
char *alloc(int nbytes);

int strIsWhitesSpaceOrComment( char *szString );


#endif
