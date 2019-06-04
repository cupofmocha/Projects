/**
 * @biref To demonstrate how to use zutil.c and crc.c functions
 */

#include <stdio.h>   /* for printf(), perror()...   */
#include <stdlib.h>  /* for malloc()                */
#include <errno.h>   /* for errno                   */
#include "crc.h"     /* for crc()                   */
#include "zutil.h"   /* for mem_def() and mem_inf() */
#include "lab_png.h" /* simple PNG data structures  */

int main(int argc, char **argv)
{

    if (argc == 1)
    {
        fprintf(stderr, "Usage: %s <directory name>\n", argv[0]);
        exit(1);
    }
    // U8 *p_buffer = NULL; /* a buffer that contains some data to play with */
    // U32 crc_val = 0;     /* CRC value                                     */
    // int ret = 0;         /* return value for various routines             */
    // U64 len_def = 0;     /* compressed data length                        */
    // U64 len_inf = 0;     /* uncompressed data length                      */
    unsigned long long value;
    int offsetToChunkWidthData = PNG_SIG_SIZE + CHUNK_LEN_SIZE + CHUNK_TYPE_SIZE;
    int offsetToChunkHeightData = PNG_SIG_SIZE + CHUNK_LEN_SIZE + CHUNK_TYPE_SIZE + 4;

    U32 width = 0;  //4 bytes, 32 bits
    U32 height = 0; //4 bytes, 32 bits

    U32 totalHeight = 0; //increment height from each png file
    int dataLength;
    unsigned long totalDataLength = 0;

    FILE *fp; //file pointer
    fp = fopen(argv[1], "rb");

    fread(&value, 1, 8, fp);

    fclose(fp);

    fp = fopen(argv[1], "rb");

    fseek(fp, offsetToChunkWidthData, SEEK_SET);

    fread(&width, 1, 4, fp);
    fclose(fp);
    width = ntohl(width);

    for (int i = 1; i < argc; ++i)
    {
        fp = fopen(argv[i], "rb");

        fseek(fp, offsetToChunkHeightData, SEEK_SET);
        fread(&height, 4, 1, fp);

        totalHeight += ntohl(height);

        //Read IDAT's data length
        fseek(fp, 33, SEEK_SET);
        fread(&dataLength, sizeof(dataLength), 1, fp);

        dataLength = ntohl(dataLength);

        totalDataLength += dataLength;
        fclose(fp);
    }
    int totalBufferSizeFormula = ((totalDataLength * 4) + 1) * 18;
    int totalBufferSize = totalBufferSizeFormula;

    U8 totalBuffer[totalBufferSize];
    U64 lengthCounter = 0;

    for (int i = 1; i < argc; ++i)
    {
        int ret = 0; /* return value for various routines */
        int IDATDataOffset = PNG_SIG_SIZE + CHUNK_LEN_SIZE + CHUNK_TYPE_SIZE + DATA_IHDR_SIZE + CHUNK_CRC_SIZE;
        fp = fopen(argv[i], "rb");

        fseek(fp, IDATDataOffset, SEEK_SET); //offset of 33 to reach IDAT's data initial byte.
        fread(&dataLength, sizeof(dataLength), 1, fp);

        dataLength = ntohl(dataLength);
        U8 imageSourceBuffer[dataLength];
        int bufferSize = ((dataLength * 4) + 1) * 18;

        fseek(fp, IDATDataOffset + 8, SEEK_SET);
        fread(imageSourceBuffer, sizeof(imageSourceBuffer), 1, fp);

        U8 inputBuffer[bufferSize];
        U64 inputBufferLength;

        ret = mem_inf(inputBuffer, &inputBufferLength, imageSourceBuffer, dataLength);
        if (ret == 0)
        {
            printf("inputBufferLength = %lu, dataLength = %lu\n", inputBufferLength, dataLength);
        }
        else
            fprintf(stderr, "mem_inf failed. ret = %d.\n", ret);

        for (int j = 0; j < inputBufferLength; ++j)
        {
            totalBuffer[j + lengthCounter] = inputBuffer[j];
        }
        lengthCounter += inputBufferLength;
        fclose(fp);
    }

    int compressedBufferSize = ((totalDataLength * 4) + 1) * 18;
    U8 compressedBuffer[compressedBufferSize];
    U64 compressedDataLength = 0;
    int ret = 0;
    int crc_val = 0;

    ret = mem_def(compressedBuffer, &compressedDataLength, totalBuffer, lengthCounter, Z_DEFAULT_COMPRESSION);
    if (ret == 0)
    { /* success */
        printf("original len = %d, len_def = %lu\n", lengthCounter, compressedDataLength);
    }
    else
    { /* failure */
        fprintf(stderr, "mem_def failed. ret = %d.\n", ret);
        return ret;
    }

    // Variables for inputting the information from the first PNG file to write to Final PNG Result
    U32 input32b = 0;
    U64 input;
    U64 IHDRLengthType;
    U32 firstPictureWidth;
    U32 dataInfoAfterHeightWithoutInterlaceMethod;
    U8 interlaceMethod;
    U32 crcData;
    U8 newCRCBuffer[17];
    U32 newCRCBufferCounter = 17;
    U32 IDATBufferLength;
    U8 IDATBufferType[4];
    U8 CRCBuffer[compressedDataLength + 4];
    U64 CRCBufferLength = compressedDataLength + 4;
    U32 IDATLengthChunk = 0;
    U8 IDATCRCBuffer[4];
    U32 endCRCBufferCounter = 4;
    U32 totalHeightToWrite = htonl(totalHeight);

    // Get info from first picture provided
    fp = fopen(argv[1], "rb");
    fread(&input, 1, 8, fp);
    fread(&IHDRLengthType, 1, 8, fp);
    fread(&firstPictureWidth, 1, 4, fp);
    fread(&input32b, 1, 4, fp);
    fread(&dataInfoAfterHeightWithoutInterlaceMethod, 1, 4, fp);
    fread(&interlaceMethod, 1, 1, fp);
    fread(&crcData, 1, 4, fp);
    fread(&IDATLengthChunk, 4, 1, fp);
    fread(&IDATBufferType, sizeof(IDATBufferType), 1, fp);

    IDATLengthChunk = ntohl(IDATLengthChunk);
    U8 IDATReadBuffer[IDATLengthChunk];

    fread(&IDATReadBuffer, 1, IDATLengthChunk, fp);
    fread(&input32b, 4, 1, fp);
    fread(&input32b, 4, 1, fp);
    fread(&IDATCRCBuffer, 4, 1, fp);

    fclose(fp);

    // Create new concatenated PNG file with information retrieved before.
    fp = fopen("all.png", "w+");

    fwrite(&input, 1, 8, fp);
    fwrite(&IHDRLengthType, 1, 8, fp);
    fwrite(&firstPictureWidth, 1, 4, fp);
    fwrite(&totalHeightToWrite, 1, 4, fp);
    fwrite(&dataInfoAfterHeightWithoutInterlaceMethod, 1, 4, fp);
    fwrite(&interlaceMethod, 1, 1, fp);

    fseek(fp, 12, SEEK_SET);
    fread(&newCRCBuffer, newCRCBufferCounter, 1, fp);
    crc_val = crc(newCRCBuffer, newCRCBufferCounter);
    crc_val = htonl(crc_val);
    fwrite(&crc_val, 4, 1, fp);

    IDATBufferLength = (U32)htonl(compressedDataLength);
    fwrite(&IDATBufferLength, 4, 1, fp);

    for (int j = 0; j < 4; j++)
    {
        CRCBuffer[j] = IDATBufferType[j];
    }

    for (int m = 0; m < compressedDataLength; m++)
    {
        CRCBuffer[m + 4] = compressedBuffer[m];
    }

    crc_val = crc(CRCBuffer, CRCBufferLength);
    crc_val = htonl(crc_val);

    fwrite(&IDATBufferType, 4, 1, fp);

    fwrite(&compressedBuffer, 1, compressedDataLength, fp);
    fwrite(&crc_val, 4, 1, fp);

    fwrite(&input32b, 4, 1, fp);
    fwrite(&IDATCRCBuffer, 4, 1, fp);

    crc_val = crc(IDATCRCBuffer, endCRCBufferCounter);
    crc_val = htonl(crc_val);

    fwrite(&crc_val, 4, 1, fp);

    fclose(fp);

    return 0;
}