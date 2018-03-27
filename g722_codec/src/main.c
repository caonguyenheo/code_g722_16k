
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "g722_enc.h"
#include "g722_dec.h"

#define DEF_READ_SIZE   2048

static int
G722EncodeFile(char* fileName)
    {
    FILE       *pRawFile = NULL;
    FILE       *pEncFile = NULL;
    g722_enc_t enc;
    char       raw_data[DEF_READ_SIZE];
    char       enc_data[DEF_READ_SIZE * 2];
    int        raw_size = 0;
    int        enc_size = 0;
    char       fileNameBuf[512] = {0};

    pRawFile = fopen(fileName, "rb");
    if (pRawFile == NULL)
        return -1;

    sprintf(fileNameBuf, "%s.enc", fileName);
    pEncFile = fopen(fileNameBuf, "wb");
    if (pEncFile == NULL)
        return -1;

    g722_enc_init(&enc);

    do
        {
        raw_size = fread(raw_data, 1, sizeof(raw_data), pRawFile);
        if (raw_size)
            {
            g722_enc_encode(&enc, (short*)raw_data, raw_size/2, enc_data, &enc_size);
            printf("Raw data size %d encode to %d bytes\n", raw_size, enc_size);
            fwrite(enc_data, 1, enc_size, pEncFile);
            }
        }while (!feof(pRawFile));

    if(pRawFile)
        fclose(pRawFile);
    if(pEncFile)
        fclose(pEncFile);
    return 0;
    }

static int
G722DecodeFile(char* fileName)
    {
    FILE       *pEncFile = NULL;
    FILE       *pDecFile = NULL;
    g722_dec_t dec;
    char       enc_data[DEF_READ_SIZE];
    char       dec_data[DEF_READ_SIZE * 2];
    int        enc_size = 0;
    int        dec_size = 0;
    char       fileNameBuf[512] = {0};

    pEncFile = fopen(fileName, "rb");
    if(pEncFile == NULL)
        return -1;

    sprintf(fileNameBuf, "%s.dec", fileName);
    pDecFile = fopen(fileNameBuf, "wb");
    if(pDecFile == NULL)
        return -1;
    g722_dec_init(&dec);
    do
        {
        enc_size = fread(enc_data, 1, sizeof(enc_data), pEncFile);
        if(enc_size)
            {
            dec_size = enc_size * 2;
            g722_dec_decode(&dec, enc_data, enc_size, (int16_t*)dec_data, (size_t*)(&dec_size));
            printf("Enc data size %d decode to %d bytes\n", enc_size, dec_size);
            fwrite(dec_data, 1, dec_size, pDecFile);
            }
        }while(!feof(pEncFile));

    if(pEncFile)
        fclose(pEncFile);
    if(pDecFile)
        fclose(pDecFile);
    return 0;
    }

int main(int argc, char** argv)
    {
    char       *mode = NULL;
    char       *fileName = NULL;

    if(argc < 3)
        {
        printf("Syntax error : %s <enc/dec> <filename>\n", argv[0]);
        return -1;
        }

    if(argc >= 2)
        mode = argv[1];
    if(argc >= 3)
        fileName = argv[2];

    if(strcmp(mode, "dec") == 0)
        return G722DecodeFile(fileName);
    else if (strcmp(mode, "enc") == 0)
        return G722EncodeFile(fileName);
    else
        printf("Invalid mode\n");
    return 0;
    }
