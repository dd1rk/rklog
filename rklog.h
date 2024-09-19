

#ifndef __RKLOG_H__
#define __RKLOG_H__

#include "types.h"

#define VERSION "0.03"

extern int printvers (void);

extern int readArgs(int argc, char **argv, t_rkLog *pConfig);
extern int usage(void);
extern char* GetAktuTime ( char* formatstr );
extern int printHeader(FILE *fpOutFile);


#endif 
