#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("ERROR : Enter .bmp extension\n");
        return d_failure;
    }
    if(argv[3] != NULL)
    {
        strcpy(decInfo->secret_fname, argv[3]);
    }
    else
    {
        strcpy(decInfo->secret_fname, "decode");
    }
    return d_success;
}

Status Open_files(DecodeInfo *decInfo)
{
    //stego image file
    decInfo->fptr_stego_image=fopen(decInfo->stego_image_fname,"r");
    if(decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr,"ERROR : Unable to open file %s\n",decInfo->stego_image_fname);
        return d_failure;
    }

    //secret file
    decInfo->fptr_secret = fopen(decInfo->secret_fname,"w");
    if(decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr,"ERROR : Unable to open file %s\n",decInfo->secret_fname);
        return d_failure;
    }
    return d_success;
}

Status decode_data_to_image(char *data, int size, FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    int i;
    char image_buffer[8];
    for(i = 0; i< size; i++)
    {
        fread(image_buffer, 8, 1, fptr_stego_image);
        data[i] = (decode_lsb_to_byte(data[i], image_buffer));
    }
    data[i] = '\0';
    return d_success;
}

char decode_lsb_to_byte(char data, char *image_buffer)
{
    char ch=0;
    for(int i=0;i<8;i++)
    {
        ch = ((image_buffer[i] & 1) << i) | ch;
    }
    return ch;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    char data[strlen(magic_string)];
    decode_data_to_image(data, strlen(magic_string), decInfo->fptr_stego_image,decInfo);
    if(strcmp(magic_string, data) == 0)
    {
        return d_success;
    }
    else
    {
        printf("ERROR : Failed to decode magic string\n");
        return d_failure;
    }
}

int decode_size_to_lsb(char *buffer, int *data)
{
    *data = 0;
    for(int i=0; i<32; i++)
    {
        *data = ((buffer[i] & 1) << i) | *data;
    }
    return *data;
}

Status decode_secret_file_extn_size(int *size, FILE *fptr_stego_image)
{
    char buffer[32];
    fread(buffer, 32, 1, fptr_stego_image);
    decode_size_to_lsb(buffer, size);
    return d_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char extn[decInfo->size_secret_file_extn];
    if(decode_data_to_image(extn, decInfo->size_secret_file_extn, decInfo->fptr_stego_image, decInfo) == d_success)
    {
        strcat(decInfo->secret_fname, extn);
        decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
        return d_success;
    }
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_secret_file_extn_size(&(decInfo->decode_size_secret_file),decInfo->fptr_stego_image);
    return d_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char data[decInfo->decode_size_secret_file];
    decode_data_to_image(data, decInfo->decode_size_secret_file, decInfo->fptr_stego_image, decInfo);
    fprintf(decInfo->fptr_secret,"%s",data);
    return d_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if(Open_files(decInfo) == d_success)
    {
        printf("INFO : Open files successfully\n");
        if(decode_magic_string(MAGIC_STRING, decInfo) == d_success)
        {
            printf("INFO : Decoded magic string successfully\n");
            if(decode_secret_file_extn_size(&(decInfo->size_secret_file_extn), decInfo->fptr_stego_image) == d_success)
            {
                printf("INFO : Decoded secret file extn size successfully\n");
                if(decode_secret_file_extn(decInfo) == d_success)
                {
                    printf("INFO : Decoded secret file extn successfully\n");
                    if(decode_secret_file_size(decInfo) == d_success)
                    {
                        printf("INFO : Decoded secret file size successfully\n");
                        if(decode_secret_file_data(decInfo) == d_success)
                        {
                            printf("INFO : Decoded secret file data successfully\n");
                            
                        }
                        else
                        {
                            printf("ERROR : Failed to decode secret fie data\n");
                            return d_failure;
                        }
                    }
                    else{
                        printf("ERROR : Failed to decode secret file size");
                        return d_success;
                    }
                }
                else
                {
                    printf("ERROR : Failed to decode secret file extn\n");
                    return d_failure;
                }
            }
            else
            {
                printf("ERROR : Failed to decode secret file extn size\n");
                return d_failure;
            }
        }
        else
        {
            printf("ERROR : Failed to decode magic string\n");
            return d_failure;
        }
    }
    else
    {
        printf("ERROR : Failed to open files\n");
        return d_failure;
    }
    return d_success;
}