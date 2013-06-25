#ifndef _PBODATATYPES_H_
#define _PBODATATYPES_H_

#define PBOHEADER_PACKING_UNCOMPRESSED (0x00000000)
#define PBOHEADER_PACKING_PACKED (0x43707273)
#define PBOHEADER_PACKING_PRODUCTENTRY (0x56657273)

typedef unsigned short ushort;
typedef unsigned long ulong;

typedef struct pboHeader
{
 char  *filename; //a zero terminated string defining the path and filename,
                   //         relative to the name of this pbo.
                   //Zero length filenames ('\0') indicate first (optional), or last (non optional) entry in header. 
                   //  Other fields in the last entry are filled by zero bytes. 
 ulong   PackingMethod; //0x00000000 uncompressed
                        //0x43707273 packed
                        //0x56657273 Product Entry (resistance/elite/arma)
 ulong   OriginalSize;  // Unpacked: 0 or same value as the DataSize
                        // Packed: Size of file after unpacking. 
                        // This value is needed for byte boundary unpacking
                        // since unpacking itself can lead to bleeding of up
                        // to 7 extra bytes.
 ulong   Reserved;
 ulong   TimeStamp;     // meant to be the unix filetime of Jan 1 1970 +, but often 0
 ulong   DataSize;      // The size in the data block. 
                        // This is also the file size when not packed                       
} PboHeader;

typedef struct pboFileInfo {
  PboHeader *header;
  ulong   dataStart;     // Own dataitem, not present in file. It is the position in the file where the data starts.
} PboFileInfo; 

PboHeader *PboHeader_CreateByConsumeStream(FILE *stream);
void PboHeader_Release(PboHeader *header);
void PboFileInfo_Release(PboFileInfo *fileInfo);
boolean PboHeader_isEndOfHeader(PboHeader *header);

#endif /* _PBODATATYPES_H_ */