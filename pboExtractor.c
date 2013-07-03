#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"
#include "pboDataTypes.h"
#include "pboExtractor.h"

PboFileInfo *PboExtractor_getHeaderOfFile(char *filename,FILE *stream)
 {
    PboHeader *currentHeader;
    PboFileInfo *searchedHeader;
    int headerSize;
    int dataSize;

    currentHeader = NULL;
    searchedHeader = NULL;
    headerSize = 0;
    dataSize = 0;

    rewind(stream);
    do {
        if (currentHeader != NULL && (searchedHeader == NULL || searchedHeader->header != currentHeader)) {
            PboHeader_Release(currentHeader);
        }
        currentHeader = PboHeader_CreateByConsumeStream(stream);

        if (strcmp(filename,currentHeader->filename) == 0) {
            searchedHeader = (PboFileInfo*)allocate(sizeof(PboFileInfo));
            searchedHeader->header = currentHeader;
            searchedHeader->dataStart = dataSize; //For now only the size of the previous dataelements, later also the headers added.
        }

        headerSize += 5 * PBOULONGSIZE + strlen(currentHeader->filename)+1;
        dataSize += currentHeader->DataSize;

    } while (!PboHeader_isEndOfHeader(currentHeader) && !feof(stream));

    if (searchedHeader != NULL) {
        searchedHeader->dataStart = searchedHeader->dataStart + headerSize;
    } else {
        error("Could not find searched file %s",filename);
    }

    return searchedHeader;
}


void PboExtractor_copyFileContents(FILE *inputStream, FILE *outputStream,ulong size) {
    char *buffer[1024*4]; //4kb buffer
    int reallyRead;
    int dataToRead;
    while (size) {
        dataToRead = (size < (1024*4))?size:(1024*4);
        reallyRead = fread (&buffer, sizeof(char), dataToRead, inputStream);
        fwrite(&buffer, sizeof(char), reallyRead, outputStream);
        size -= dataToRead;
    }
}

void PboExtractor_ExtractFile(char *filename,FILE *inputStream,FILE *outputStream) {
    PboFileInfo *searchedHeader;
    searchedHeader = PboExtractor_getHeaderOfFile(filename,inputStream);
    if (fseek(inputStream,searchedHeader->dataStart,SEEK_SET) != 0) {
        error("Searched file was not at the expected position");
    }
    PboExtractor_copyFileContents(inputStream,outputStream,searchedHeader->header->DataSize);
    PboFileInfo_Release(searchedHeader);
}
