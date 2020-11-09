#if defined _WIN32
 #define cWINDOWS
#endif
#if defined _WIN32
 #define cWINDOWS
#endif
#if defined _WIN64
 #define cWINDOWS
#endif
#if defined cWINDOWS
 #define unlink _unlink
 // Disable warnings about old style `c` calls having safer alternatives
 #pragma warning(disable:4996)
#else
 #include <unistd.h>
#endif
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
//
#include "as.h"
#include "table0.h"
#include "do0.h"
#include "pseudo.h"
#include "eval.h"
#include "symtab.h"
#include "util.h"
#include "ffwd.h"
#include "output.h"
//
#include "as.h"
#include "as.c"
#include "do0.c"
#include "pseudo.c"
#include "eval.c"
#include "symtab.c"
#include "util.c"
#include "ffwd.c"
#include "output.c"
