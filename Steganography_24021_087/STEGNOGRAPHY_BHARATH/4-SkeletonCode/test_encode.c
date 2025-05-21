#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

#define _GNU_SOURCE

int main(int argc,char *argv[])//argc (count of cla) , argv[] (to receive array of strings from cla)
{
    EncodeInfo encInfo;//structre variable(encInfo)
    //checking operation type
    int res = check_operation_type(argv);
    if(res == e_encode)
    {
        printf("INFO : Selected Encoding\n");
        //read and validate encode args
        if(read_and_validate_encode_args(argv , &encInfo) == e_success)
        {
            printf("INFO : Read and validate encode args is successfull\n");

            //starting encoding
            if(do_encoding(&encInfo) == e_success)
            {
                printf("INFO : Encoding is success\n");
                int fcloseall(void);
            }
            else
            {
                printf("ERROR : Encoding is failure\n");
                return 3;
            }
        }
        else
        {
            printf("ERROR : Faild to read and validate encode args\n");
            return 2;
        }
    }
    else if(res == e_decode)
    {
        printf("INFO : Selected Decoding\n");
        DecodeInfo decode;//structure variable(decode)
        if(read_and_validate_decode_args(argv, &decode) == d_success)
        {
            printf("INFO : Read and validate decode args successfully\n");
            if(do_decoding(&decode) == d_success)
            {
                printf("INFO : Decoding is successful\n");
                int fcloseall(void);
            }
            else
            {
                printf("ERROR : Faild to decode\n");
                return 5;
            }
        }
        else
        {
            printf("ERROR : Faild to read and validate decode args\n");
            return 4;
        }
    }
    else
    {
        printf("ERROR : Unsupported Operation\n");
        printf("For Encoding : \n ./a.out -e beauitful.bmp secret.txt [stegno.bmp]\n");
        printf("For Decoding : \n./a.out -d stegno.bmp decode\n");
        return 1;
    }
    return 0;
}
