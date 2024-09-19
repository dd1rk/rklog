#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "types.h"
#include "rklog.h"
#include "castles.h"


#define TIMESTAMP1 GetAktuTime("%y%m%d_%H%M")



/*----------------------------------------------------------------------*/
/* GetAktuTime()     Liefern des aktuellen Datums und der aktuellen     */
/*                   Uhrzeit als String                                 */
/*----------------------------------------------------------------------*/
char* GetAktuTime ( char* formatstr )
{
  /* Zeile mit Datum und Zeit           */ static char timestr[80];
  /* lokale-Variablen: -----------------*/
  /* die Datums-/Zeit-Struktur          */  struct tm *pAktuTime = NULL;
  /* TimeStruktur                       */      time_t timestruct;

  if (!pAktuTime) pAktuTime = NULL;

  memset(timestr,'\0',sizeof(timestr));

  /* Datum und Uhrzeit */
  time(&timestruct);
  pAktuTime = localtime(&timestruct);
  strftime(timestr,80,formatstr,localtime(&timestruct));

  return timestr;  /* static ! */
} /* GetAktuTime */

/**********************************************************************/
/**********************************************************************/
int printCota(void)
{
  fprintf(stderr,"\n");
  fprintf(stderr,"Schloss Kleeberg BOB-166 DL-05043\n");
  fprintf(stderr,"Schloss Pillham  BOB-167 DL-05044\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"\n");

  return 0;


} /* printCota */


/**********************************************************************/
/**********************************************************************/
int usage(void)
{
  fprintf(stderr,"\n");
  fprintf(stderr,
             "rklog - Minilogbuchprogramm für ADIF von DD1RK \n\n");
  fprintf(stderr, "-h - help\n");
  fprintf(stderr, "-? - help\n");
  fprintf(stderr, "-v - version\n");
  fprintf(stderr, "-c - prints cota-references\n");
  fprintf(stderr, "-o [FILE] - outputfile\n");
  fprintf(stderr, "-i [FILE] - importfile:\n");
  fprintf(stderr, "            write header and append importfile\n");

  fprintf(stderr,"\n");
  printvers();
  fprintf(stderr,"\n");

  return 0;


} /* usage */



/**********************************************************************/
/**********************************************************************/
int readArgs(int argc, char **argv, t_rkLog *pConfig)
{
  int opt = 0;

  while ((opt = getopt(argc,argv,"?hvco:i:")) != EOF) {
    switch (opt) {
      case 'v': printvers();    exit(0);                break;
      case 'h':
      case '?': usage();        exit(0);                break;
      case 'c': printCota();    exit(0);                break;
      case 'o': pConfig->szOutFile = strdup(optarg);    break;
      case 'i': pConfig->szInFile  = strdup(optarg);    break;

      default:  usage();     return 1;  break;

   }

  }

  return 0;
}



/**********************************************************************/
/**********************************************************************/
int printvers (void)
{
  fprintf(stderr,"rklog V%s (%s/%s)\n",VERSION,__TIME__,__DATE__);
  fflush(stderr);

  return 0;
} /* printvers */


/**********************************************************************/
/**********************************************************************/
int enterBAND(t_rkLog *pConfig)
{
  printf("Band    : ");
  scanf("%s",pConfig->szBAND);

  return strlen(pConfig->szBAND);
}


/**********************************************************************/
/**********************************************************************/
int enterMODE(t_rkLog *pConfig)
{
  printf("Mode    : ");
  scanf("%s",pConfig->szMODE);

  return strlen(pConfig->szMODE);
}

/**********************************************************************/
/**********************************************************************/
int enterFREQ(t_rkLog *pConfig)
{
  printf("Freq    : ");
  scanf("%s",pConfig->szFREQ);

  return strlen(pConfig->szFREQ);
}

/**********************************************************************/
/**********************************************************************/
int enterTIME(t_rkLog *pConfig)
{
  printf("Time    : ");
  scanf("%s",pConfig->szTIME);

  return strlen(pConfig->szTIME);
}

/**********************************************************************/
/**********************************************************************/
int enterDATE(t_rkLog *pConfig)
{
  printf("Date    : ");
  scanf("%s",pConfig->szDATE);

  return strlen(pConfig->szDATE);
}

/**********************************************************************/
/**********************************************************************/

/**********************************************************************/
/**********************************************************************/
int toUpperEntries(t_rkLog *pConfig)
{ 
	int i       = 0;
	int iLength = 0;

	iLength = (int) strlen(pConfig->szCALL);
	for (i = 0; i < iLength; i++) {
	  pConfig->szCALL[i] = toupper(pConfig->szCALL[i]);
	}

	iLength =  (int) strlen(pConfig->szMODE);
	for (i = 0; i < iLength; i++) {
	  pConfig->szMODE[i] = toupper(pConfig->szMODE[i]);
        }

	iLength =  (int) strlen(pConfig->szMYGRIDSQUARE);
	for (i = 0; i < iLength; i++) {
	  pConfig->szMYGRIDSQUARE[i] = toupper(pConfig->szMYGRIDSQUARE[i]);
        }

	iLength = (int) strlen(pConfig->szSTATIONCALLSIGN);
	for (i = 0; i < iLength; i++) {
	  pConfig->szSTATIONCALLSIGN[i] = toupper(pConfig->szSTATIONCALLSIGN[i]);
        }

	return 0;
}

/**********************************************************************/
/**********************************************************************/
int writeLog(t_rkLog *pConfig)
{
  char szBuffer[10];
  int  iH = 0;

  fprintf (pConfig->fpOutFile,"<CALL:%d>%s ",
		  (int) strlen(pConfig->szCALL),pConfig->szCALL);	


  if (strlen(pConfig->szTIME) == 0) {
    fprintf (pConfig->fpOutFile,"<QSO_DATE:%d>%s ",
		    (int) strlen(pConfig->szTIME),
		    pConfig->szTIME);
  } else {
    fprintf (pConfig->fpOutFile,"<QSO_DATE:6>%s ",GetAktuTime("%y%m%d"));
  }


  if (strlen(pConfig->szTIME) == 0) {

    strcpy(szBuffer,GetAktuTime("%H"));
    iH = atoi (szBuffer) - 1;

    fprintf (pConfig->fpOutFile,"<TIME_ON:6>%02d%s ",iH, GetAktuTime("%M%S"));
  } else {
    fprintf (pConfig->fpOutFile,"<TIME_ON:%d>%s ",(int) strlen(pConfig->szTIME),
		    pConfig->szTIME);
  }

  fprintf (pConfig->fpOutFile,"<BAND:%d>%s ",
		  (int) strlen(pConfig->szBAND), pConfig->szBAND);
  fprintf (pConfig->fpOutFile,"<MODE:%d>%s ",
		  (int) strlen(pConfig->szMODE),pConfig->szMODE);
  fprintf (pConfig->fpOutFile,"<FREQ:%d>%s ",
		  (int) strlen(pConfig->szFREQ),pConfig->szFREQ);
  fprintf (pConfig->fpOutFile,"<FREQ_RX:%d>%s ",
		  (int) strlen(pConfig->szFREQ),pConfig->szFREQ);
  fprintf (pConfig->fpOutFile,"<MY_GRIDSQUARE:%d>%s ",
		  (int) strlen(pConfig->szMYGRIDSQUARE),
		  pConfig->szMYGRIDSQUARE);
  fprintf (pConfig->fpOutFile,"<STATION_CALLSIGN:%d>%s ",
		  (int) strlen(pConfig->szSTATIONCALLSIGN),
		  pConfig->szSTATIONCALLSIGN);
  fprintf (pConfig->fpOutFile,"<RST_SENT:%d>%s ",
		  (int) strlen(pConfig->szRST_SENT),pConfig->szRST_SENT);
  fprintf (pConfig->fpOutFile,"<RST_RCVD:%d>%s ",
		  (int) strlen(pConfig->szRST_RCVD),pConfig->szRST_RCVD);
  fprintf (pConfig->fpOutFile,"<EOR>\n");

  fflush(pConfig->fpOutFile);

  return 0;

} /* writeLog */

/**********************************************************************/
/**********************************************************************/
int main (int argc, char ** argv)
{

  t_rkLog Config;
  t_rkLog *pConfig = &Config;
  char szBuffer[100];


  memset(pConfig,0x00,sizeof(t_rkLog));



  readArgs(argc,argv,pConfig);

  
  printvers();

  if (!pConfig->szOutFile) {
    char szBuffer[40];

    memset(szBuffer,0x00,sizeof(szBuffer));

    sprintf(szBuffer,"rklog_%s.adif",TIMESTAMP1);

    fprintf(stderr,"\nkein Logfile angegen using %s\n\n", szBuffer);

    pConfig->szOutFile = strdup(szBuffer);

  }


  if (pConfig->szOutFile) {
    pConfig->fpOutFile = fopen (pConfig->szOutFile,"a");
    if (!pConfig->fpOutFile) {
      fprintf(stderr,"Fehler beim Oeffnen von %s, using stderr\n",
                      pConfig->szOutFile);
      pConfig->fpOutFile = stderr;
    }

    printHeader(pConfig->fpOutFile);


  }

  if (pConfig->szInFile) {
#define SZLINELENGTH	1024
    char szLine[SZLINELENGTH];
    int i = 0;

    memset(szLine,'\0',SZLINELENGTH);

    pConfig->fpInFile = fopen (pConfig->szInFile,"r");

    if (!pConfig->fpInFile) {
      fprintf(stderr,"Fehler beim Oeffnen von %s, using stderr\n",
                      pConfig->szInFile);
    }

    fprintf(stderr,"Importmode\n");


    while (fgets(szLine, SZLINELENGTH, pConfig->fpInFile) != NULL ) {
      fprintf(pConfig->fpOutFile,"%s",szLine);
      i++;
    }

    fprintf(stderr,"import %d lines\n", i);

    return 0;

  }	   

  printf("Station : ");
  scanf("%s",pConfig->szSTATIONCALLSIGN);

  printf("Grid    : ");
  scanf("%s",pConfig->szMYGRIDSQUARE);

  printf("\n");

  enterBAND(pConfig);
  enterMODE(pConfig);
  enterFREQ(pConfig);

  do { 
    printf("\n");
    printf("help: @BAND - @MODE - @FREQ - @QUIT - @DATE - @TIME\n");
    printf("\n");
    printf("Call    : ");
    scanf("%s",szBuffer);

    if (szBuffer[0] == '@' ) {
      if (!strncmp(szBuffer,"@BAND",5) ||
          !strncmp(szBuffer,"@band",5) ) {
        enterBAND(pConfig);
        enterFREQ(pConfig);
      } else if (!strncmp(szBuffer,"@FREQ",5) ||
                 !strncmp(szBuffer,"@freq",5)) {
        enterFREQ(pConfig);
        enterBAND(pConfig);
      } else if (!strncmp(szBuffer,"@MODE",5) ||
                 !strncmp(szBuffer,"@mode",5) ) {
        enterMODE(pConfig);
      } else if (!strncmp(szBuffer,"@TIME",5) ||
                 !strncmp(szBuffer,"@time",5) ) {
        enterTIME(pConfig);
      } else if (!strncmp(szBuffer,"@DATE",5) ||
                 !strncmp(szBuffer,"@date",5) ) {
        enterDATE(pConfig);
      } else if (!strncmp(szBuffer,"@QUIT",5) ||
                 !strncmp(szBuffer,"@quit",5)) {
	      /* und raus */
	break;
      }
    } else {

      strcpy(pConfig->szCALL,szBuffer);

      printf("type \'-\' to use last value\n");

      printf("RST rec : ");
      scanf("%s",szBuffer);
      if (szBuffer[0] != '-') {
        strcpy(pConfig->szRST_RCVD,szBuffer);
      }

      printf("RST snd : ");
      scanf("%s",szBuffer);
      if (szBuffer[0] != '-') {
        strcpy(pConfig->szRST_SENT,szBuffer);
      }
    
      toUpperEntries(pConfig);
      writeLog(pConfig);

    }
  } while ( strncmp(szBuffer,"@QUIT",5) );



  printf("\n\n");

  return 0;
} /* main */


/**********************************************************************/
/**********************************************************************/
int printHeader(FILE *fpOutFile)
{
  if (!fpOutFile) return -1;


  fprintf(fpOutFile, "#ADIF erzeugt von rklog - dd1rk@darc-u30.de - Version %s\n",VERSION);
  fprintf(fpOutFile, "<CREATED_TIMESTAMP:15>%s\n",
		  GetAktuTime("%Y%m%d %H%M%S"));

  fprintf(fpOutFile, "<PROGRAMID:5>rklog\n");
  fprintf(fpOutFile, "<PROGRAMVERSION:%d>%s\n",
		        (int) strlen(VERSION),VERSION);

  fprintf(fpOutFile, "<ADIF_Ver:1>2\n");
  fprintf(fpOutFile, "<EOH>\n");

  return 0;
} /* printHeader */
