#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1 //character access
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8) //for accessing the one char we need to access 8 bytes
#define MAX_FILE_SUFFIX 4 //file extenstion size

typedef struct _EncodeInfo 
{
    /* Source Image info */
    char *src_image_fname;//character pointer to store the string(for storing the file name(.bmp))
    FILE *fptr_src_image;//for file operations we creat a file pointer to store the address
    uint image_capacity;//for storing the size of the file
    uint bits_per_pixel;//
    char image_data[MAX_IMAGE_BUF_SIZE];//string to store image data

    /* Secret File Info */
    char *secret_fname;//for storing the file address
    FILE *fptr_secret;//file pointer
    char extn_secret_file[MAX_FILE_SUFFIX];//for storing the extenstions
    char secret_data[MAX_SECRET_BUF_SIZE];//
    long size_secret_file;//for storing the size of the secret file(for running the file)

    /* Stego Image Info */
    char *stego_image_fname;//for storing the default file
    FILE *fptr_stego_image;//file pointer for stego image

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status Open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
//secret file size
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(EncodeInfo *encInfo);

/* Store Magic String */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo);

/*Encode Encode secret file extenstion size */
Status encode_secret_file_extn_size(int size , EncodeInfo *encInfo);

/*Encode size to lsb */
Status encode_size_to_lsb(int size , EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stegno image after encoding */
Status copy_remaining_img_data(EncodeInfo *encInfo);

#endif
