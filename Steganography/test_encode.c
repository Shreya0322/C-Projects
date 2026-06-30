/*
Documentation:
Name: Shreya Shanbhag
Date: 02-02-2026
Description: LSB image steganography- This project demonstratesLeast Significant Bit (LSB) based image steganography,
 where secret information is embedded into the least significant bits of a bitmap (bmp) image without causing noticeable changes 
 to the image quality. The encoded image, called the stego image, securely contains the secret data. 
 The decoding module retrieves the embedded data by reversing the encoding process, ensuring reliable data extraction.
*/
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    //validate CLA
    if(argc<=1)
    {
        printf("Insufficient CLA's\n");
        return 0;

    }
    //Check for the type of operation to be performed (encoding/decoding)
    int type=check_operation_type(argv);
    if(type==e_encode)
    {
        printf("Encoding is selected\n");
        //declare structure variable.
        EncodeInfo enc;
        //Read and validate the CLA for encoding
        int res=read_and_validate_encode_args(argv, &enc);
        if(res==e_success)
        {
            printf("Read and Validate success\n");
            //Perform encoding
            int encode=do_encoding(&enc);
            //If encoding is successfull print a message and close all the files.
            if(encode==e_success)
            {
                printf("Encoding successfull\n");
                fclose(enc.fptr_src_image);
                fclose(enc.fptr_secret);
                fclose(enc.fptr_stego_image);
            }
            else if(encode==e_failure)
            {
                printf("Encoding failed\n");
                return e_failure;
            }
            
        }
        else if(res==e_failure)
        {
            printf("Error! Read and validate failed\n");
            return 0;
        }


    }
    else if(type==e_decode)
    {
        printf("Decoding is selected\n");
        //Declare structre variable
        DecodeInfo dec;
        //Read and validate CLA for decoding
        if(read_and_validate_decode_args(argv,&dec)==e_success)
        {
            printf("Read and Validate success\n");
            //Perform decoding
            int decode=do_decoding(&dec);
            //If decoding is successfull print a message and close all the files.
            if(decode==e_success)
            {
                printf("Decoding successfull!\n");
            }
            else 
            {
                printf("Decoding failed!\n");
                return e_failure;
            }
        }
        else
        {
            printf("Error! Read and validate failed\n");
            return 0;
        }
        
    }
    else
    {
        printf("Unsupported\n");
    }

    return 0;
}

//Function definition to chek the type of operation selected
OperationType check_operation_type(char *argv[])
{
    //if -e is selected, then return e_encode
    if(strcmp(argv[1],"-e")==0)
    {
        return e_encode;
    }
    //if -d is selected, then return e_decode
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    //if other than -e or -d is selected return e_unsupported
    return e_unsupported;
    
}
