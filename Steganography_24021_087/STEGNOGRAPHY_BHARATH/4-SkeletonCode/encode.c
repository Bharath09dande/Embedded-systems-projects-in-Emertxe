#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Function Definitions */
OperationType check_operation_type(char *argv[])
{
    // Check if argv[1] is encode or decode or any other
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    //checking 2nd cla is .bmp file or not, if it found returns address ,not found returns NULL
   if( strstr(argv[2],".bmp")!=NULL)//beautiful.bmp
   {
       //stores the file name argv[2](string address) inside the structure member (src_image_fname)
       encInfo->src_image_fname=argv[2];
   }
   else
   {
       //if it showing NULL retuen e_failure
       return e_failure;
   }
   //checking 2nd cla is .txt file or not, if it found returns address ,not found returns NULL
   if(strstr(argv[3],".txt")!=NULL)//secret.txt
   {
       //stores the file name(argv[3] (string address)) inside the another strucure member (secret_fname)
       encInfo->secret_fname=argv[3];
   }
   else
   {
        //if it showing NULL retuen e_failure
       return e_failure;
   }
   //checking 4th cla is present or not , if it is present returns address , not present returns NULL
   if(argv[4]!=NULL)
   {
       //file name(argv[4] (string address)) storing in structure member (stego_image_fname)
       encInfo->stego_image_fname=argv[4];
   }
   else
   {
        //if it is NULL (i.e.,not present) we create new file name (stego.bmp) and stored in structure member (stego_image_fname)
       encInfo->stego_image_fname="default.bmp";
   }
   //if all cla are correctly placed then returns the e_success
   return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    //image size stored in image_capacity structure member
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    //secret file size stored in size_secret_file structure member
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    //checking image capacity is sufficent for storing secret file information
    if(encInfo->image_capacity > (16 + 32 +32 +32 + (encInfo->size_secret_file * 8)))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

//grtting secret file size
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is started from offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file 
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");// storing file address into a file pointer
    //fopen retuns NULL if file is not found else returns address of the file
    // Do Error handling 
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
        // if file is not found printing error msg
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");// storing file address into a file pointer
    //fopen retuns NULL if file is not found else returns address of the file
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
        // if file is not found printing error msg
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");// storing file address into a file pointer
    //fopen retuns NULL if file is not found else returns address of the file
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
        // if file is not found printing error msg
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }
    
    // No failure return e_success
    return e_success;
}

//copying beautiful.bmp header(54 bytes) to stego.bmp(default.bmp) file
Status copy_bmp_header(EncodeInfo *encInfo)
{
    char str[54];
    fseek(encInfo->fptr_src_image,0,SEEK_SET);
    fread(str,54,1,encInfo->fptr_src_image);
    fwrite(str,54,1,encInfo->fptr_stego_image);
    return e_success;
}

//function to to take data from beautiful.bmp image and encoded data stored in output image
Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo)
{
    for(int i = 0; i < size; i++)
    {
        //reading 8 bytes from beautiful.bmp and stored in string (image_data)
        fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
        //calling function to encode 1 bit in 1 byte of image_data
        encode_byte_to_lsb(data[i],encInfo->image_data);
        //writting data in string(image_data) into output image
        fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}
//function to encoding each bit in each byte of the beautiful.bmp image stored in image_buffer
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE ) | ((data >> i) & 1);
    }
}
//function to encode magic string into beautiful.bmp image
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    //calling encode_data_to_image function to encode magic string into beautiful.bmp image
    encode_data_to_image(magic_string,strlen(magic_string),encInfo);
    //reruen e_success when encoding completed
    return e_success;
}

//function to Encode encode_secret_file_extn_size into beautiful.bmp image
Status encode_secret_file_extn_size(int size , EncodeInfo *encInfo)
{
    //calling encode_size_to_lsb function to encode extn in lsb of beautiful.bmp image
    encode_size_to_lsb(size , encInfo);
    return e_success;
}
//function to encode extn of secret file in lsb of beautiful.bmp image and stored in output image
Status encode_size_to_lsb(int size, EncodeInfo *encInfo)
{
    char str[32];
    fread(str, 32, 1, encInfo->fptr_src_image);
    for(int i = 0; i < 32; i++)
    {
        str[i] = (str[i] & 0xFE) | ((size >> i) & 1);
    }
    fwrite(str, 32, 1, encInfo->fptr_stego_image);
}

//fuction to encode secret file extn into output image
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    //calling function to read data from beautiful.bmp and encoded data written in output image(stego.bmp)
    encode_data_to_image(file_extn, strlen(file_extn), encInfo);
    return e_success;
}

//function to encode secret file size and written into output image(stego.bmp)
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    //calling function to encode secret file size and written into output image(stego.bmp)
    encode_size_to_lsb(file_size , encInfo);
    return e_success;
}

//function to encode secret file data and written in output image(stego.bmp)
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char str[encInfo->size_secret_file];
    fseek(encInfo->fptr_secret,0,SEEK_SET);
    fread(str,encInfo->size_secret_file,1,encInfo->fptr_secret);
    encode_data_to_image(str,encInfo->size_secret_file,encInfo);
    return e_success;
}

//fuction to copy remaining img data to output image(stego.bmp)
Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    int len = encInfo->image_capacity + 54 - ftell(encInfo->fptr_src_image);
    char str[len];
    fread(str,len,1,encInfo->fptr_src_image);
    fwrite(str,len,1,encInfo->fptr_stego_image);
    return e_success;
}

//Encoding
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf("INFO : Open files is success\n");
        //checking image capacity is sufficent for storing secret file information
        if(check_capacity(encInfo) == e_success)
        {
            printf("INFO : Secret msg can fit in given image\n");
            //copying beautiful.bmp header(54 bytes) to stego.bmp(default.bmp) file
            if(copy_bmp_header(encInfo) == e_success)
            {
                printf("INFO : Copied BMP header successfully\n");
                //encoding magic string(#*) to beautiful.bmp file
                if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
                {
                    printf("INFO : Encoded magic string successfully\n");
                    //storing extenstion of secret file into extn_secret_file(string)
                    strcpy(encInfo->extn_secret_file, strstr(encInfo->secret_fname, "."));
                    //printf("%s\n",encInfo->extn_secret_file);
                    //calling encode_secret_file_extn_size function and checking encode complete or not
                    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo) == e_success)
                    {
                        printf("INFO : Encoded secret file extn size successfully\n");
                        //calling encode_secret_file_extn function to encode extn and stored in output image
                        if(encode_secret_file_extn(encInfo->extn_secret_file , encInfo) == e_success)
                        {
                            printf("INFO : Encoded secret file extn successfully\n");
                            //calling function to encode secret file size and written into output image(stego.bmp)
                            if(encode_secret_file_size(encInfo->size_secret_file , encInfo) == e_success)
                            {
                                printf("INFO : Encoded secret file size successfully\n");
                                //function to encode secret file data and written in output image(stego.bmp)
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("INFO : Encoded secret data successfully\n");
                                    //calling fuction to copy remaining img data to output image(stego.bmp)
                                    if(copy_remaining_img_data(encInfo) == e_success)
                                    {
                                        printf("INFO : Copied remaining data successfully\n");
                                    }
                                    else
                                    {
                                        printf("ERROR : Failed to Copied remaining data\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("ERROR : Failed to Encoded secret data\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("ERROR : Failed to encode secret file size\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("ERROR : Failed to encode secret file extn\n");
                            return e_success;
                        }
                    }
                    else
                    {
                        printf("ERROR : Failed to encode secret file extn size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("ERROR : Failed to encode magic string\n");
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR : Failed to copy BMP header\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR : Secret msg cant fit in given image\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR : failed to open files\n");
        return e_failure;
    }
    //encoding done successfully
    return e_success;
}