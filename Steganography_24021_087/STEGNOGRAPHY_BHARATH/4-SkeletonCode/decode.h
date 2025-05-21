#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_FILE_SUFFIX 4
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)

typedef struct DecodeInfo
{
    /*Stego image info*/
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /*secret file info*/
    char secret_fname[50];
    FILE *fptr_secret;
    char *secret_file_extn[MAX_FILE_SUFFIX];
    int decode_size_secret_file;
    int size_secret_file_extn;

}DecodeInfo;

/*Decode function prototypes*/

/*Check operation type*/
OperationType check_operation_type(char *argv[]);

/*Read and validate Decode args from argv*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/*Do decoding*/
Status do_decoding(DecodeInfo *decInfo);

/*file pointers for input and output files*/
Status open_files(DecodeInfo *decInfo);

/*Decode magic string*/
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/*Decode secret file extn*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/*Decode secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/*Decode secret file extention size*/
Status decode_secret_file_extn_size(int *size, FILE *fptr_stego_image);

/*Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/*Decode function , for real decoding*/
Status decode_data_to_image(char *data, int size, FILE *fptr_stego_image, DecodeInfo *decInfo);

/*Decode LSBs into byte*/
char decode_lsb_to_byte(char data , char *image_buffer);

/*Decode LSB into size*/
int decode_size_to_lsb(char *buffer, int *data);

#endif