
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"
#include "pboDataTypes.h"
#include "pboExtractor.h"

#define VERSION		"0.1"

static void version(char *myself) {
  printf("%s version %s (compiled %s)\n", myself, VERSION, __DATE__);
}

static void help(char *myself) {
  /* show some help how to use the program */
  printf("Usage: %s [options] <file to extract>\n", myself);
  printf("Options:\n");
  printf("  --file <input file> pbo file to read from. (Default is stdin)\n");
  printf("  --output <output file>   file to save the extracted file to (Default is stdout)\n");
  printf("  --help              show this help\n");
}

int main(int argc, char *argv[]) {
  int i;
  char *inFileName;
  char *outFileName;;
  char *fileToExtract;
  FILE *inFile;
  FILE *outFile;
  
  boolean optionReadStdIn;
  boolean optionWriteStdOut;

  /* analyze command line */
  
  inFileName = NULL;
  outFileName = NULL;
  fileToExtract = NULL;
  
  optionReadStdIn = TRUE;
  optionWriteStdOut = TRUE;
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      /* option */
      if (strcmp(argv[i], "--file") == 0) {
        optionReadStdIn = FALSE;
        i++;
        if (i < argc) {
            inFileName = argv[i];
        } else {
            error("no input file");
        }
      } else
      if (strcmp(argv[i], "--output") == 0) {
        optionWriteStdOut = FALSE;
        i++;
        if (i < argc) {
            outFileName = argv[i];
        } else {
            error("no output file");
        }
      } else
      if (strcmp(argv[i], "--help") == 0) {
        help(argv[0]);
        exit(0);
      } else {
        error("unrecognized option '%s'; try '%s --help'",
              argv[i], argv[0]);
      }
    } else {
      /* file */
      if (fileToExtract != NULL) {
        error("more than one file name not allowed");
      }
      fileToExtract = argv[i];
    }
  }
  
  if (fileToExtract == NULL) {
        error("fileToExtract is not specified");
  }
      
  if (optionReadStdIn) {
    inFile = stdin;
  } else {
    inFile = fopen(inFileName, "rb");
  }
  if (inFile == NULL) {
    error("cannot open input file '%s'", inFileName);
  }  
  
  if (optionWriteStdOut) {
    outFile = stdout;
  } else {
    outFile = fopen(outFileName, "wb");
  }
  if (outFile == NULL) {
    error("cannot open output file '%s'", outFileName);
  }
  
  PboExtractor_ExtractFile(fileToExtract,inFile,outFile);
  
  if (!optionReadStdIn) {
    fclose(inFile);
  }
  
  if (!optionWriteStdOut) {
    fclose(outFile);
  }
  return 0;
}
