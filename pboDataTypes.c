#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"
#include "pboDataTypes.h"

PboHeader *PboHeader_CreateByConsumeStream(FILE *stream) {
    PboHeader *header;
    char buffer[255];
    int charsRead;

    charsRead = 0;
    buffer[0] = 0;
    header = (PboHeader*)allocate(sizeof(PboHeader));

    while ((charsRead < 255) && (!feof(stream))) {
        fread((void*)&(buffer[charsRead]),sizeof(char),1,stream);
        if (buffer[charsRead] == '\0') {
            break;
        }
        charsRead++;
    }

    header->filename = (char*)allocate(charsRead+1);
    strncpy(header->filename,buffer,charsRead+1);
    fread(&(header->PackingMethod),PBOULONGSIZE,1,stream);
    fread(&(header->OriginalSize),PBOULONGSIZE,1,stream);
    fread(&(header->Reserved),PBOULONGSIZE,1,stream);
    fread(&(header->TimeStamp),PBOULONGSIZE,1,stream);
    fread(&(header->DataSize),PBOULONGSIZE,1,stream);
    return header;
}

void PboHeader_Release(PboHeader *header) {
    if (header) {
        release(header->filename);
        release(header);
    }
}

void PboFileInfo_Release(PboFileInfo *fileInfo) {
    if (fileInfo) {
        release(fileInfo->header);
        release(fileInfo);
    }
}

boolean PboHeader_isEndOfHeader(PboHeader *header) {
    if (!header) return FALSE;
    return  ((strcmp(header->filename,"") == 0) && (header->PackingMethod != PBOHEADER_PACKING_PRODUCTENTRY) && (header->DataSize == 0));
}
