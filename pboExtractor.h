#ifndef _PBOEXTRACTOR_H_
#define _PBOEXTRACTOR_H_

PboFileInfo *PboExtractor_getHeaderOfFile(char *filename,FILE *stream);

void PboExtractor_ExtractFile(char *filename,FILE *inputStream,FILE *outputStream);

#endif /* _PBOEXTRACTOR_H_ */