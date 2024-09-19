#ifndef __TYPES_H__
#define __TYPES_H__

#define BUFSIZE	40

typedef struct {
  char *szOutFile;
  FILE *fpOutFile;
  char *szInFile;
  FILE *fpInFile;

  char szCALL[BUFSIZE];
  char szBAND[BUFSIZE];
  char szTIME[BUFSIZE];
  char szDATE[BUFSIZE];
  char szMODE[BUFSIZE];
  char szFREQ[BUFSIZE];
  char szMYGRIDSQUARE[BUFSIZE];
  char szSTATIONCALLSIGN[BUFSIZE];
  char szRST_SENT[BUFSIZE];
  char szRST_RCVD[BUFSIZE];

} t_rkLog;

#endif

