#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/*
Function definition to read and validate the CLA's. 

This function checks the files in the CLA has required extensions or not before decoding.
It returns failure if input image file doesn't have the required format.
If output file is given in CLA then it checks for the required format, if not given then it 
created a new output file
*/

Status read_and_validate_decode_args(char *argv[], DecodeInfo *dec)
{
     //1. check for .bmp file
    //checks in the value at the second index position of argv[] contains .bmp or not
    //if present returns the starting address of the substring if no return null
    if(strstr(argv[2],".bmp")!=NULL)
    {
        dec->stego_image_fname=argv[2];
    }
    else
    {
        printf("No .bmp file is passed as the argument\n");
        return e_failure;
    }
    //2. check for output file name if passed validate otherwise create a file and store file name
    if(argv[3]!=NULL)
    {
        if(strstr(argv[3],".txt")!=NULL)
        {
            dec->decode_fname=argv[3];
        }
    }
    else
    {
        dec->decode_fname="decoded.txt";
    }
    return e_success;
}

/*
This function definition performs decoding by calling the required functions
1. Open files to open all the files in the required mode.
2. Decode the magic string and validate it
3. decode secret file extension size
3. Decode secret file size.
5. Decode secret file extension
6. decode secret file data and save it in decoded text file.
If any one of the above steps fail the function returns failure, resulting to fail decoding.
*/
Status do_decoding(DecodeInfo *dec)
{
    //functin call to open the files in the required mode 
    if(open_files_to_decode(dec)==e_success)
    {
        printf("All files opened successfully!\n");

        //decode magic string
        if(decode_magic_string(dec)==e_success)
        {
            printf("Magic string decoded successfully!\n");

            //decode secret file extension size
            if(decode_secret_file_extn_size(dec)==e_success)
            {
                printf("File extension size decoded successfully\n");

                //decode secret file size
                if(decode_secret_file_size(dec)==e_success)
                {
                    printf("File size decoded successfully!\n");
                    
                    //decpde secret file extension
                    if(decode_secret_file_extn(dec)==e_success)
                    {
                        printf("File extension decoded successfully\n");

                        //decode secret data
                        if(decode_secret_data(dec)==e_success)
                        {
                            printf("Secret data decoded successfully\n");
                        }
                        else  
                        {
                            printf("Failed to decode secret data\n");
                            return e_failure;
                        }
                    }
                    else  
                    {
                        printf("Failed to decode file extension\n");
                        return e_failure;
                    }

                }
                else  
                {
                    printf("Failed to decode file size\n");
                    return e_failure;
                }
            }
            else  
            {
                printf("Failed to decode the file extension size\n");
                return e_failure;
            }
        }
        else
        {
            printf("Failed to decode magic string\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to open the files\n");
        return e_failure;
    }
    return e_success;
}

/*Function definition to open the files
input: file pointers for stego image, decoded text file
output: stego image is opened in read mode
        decoded text file is opened in write mode

*/
Status open_files_to_decode(DecodeInfo *dec)
{
    //file pointer for stego image
    dec->fptr_stego_image=fopen(dec->stego_image_fname,"r");
    if(dec->fptr_stego_image==NULL)
    {
        //print error message if fails to open the file 
        fprintf(stderr, "ERROR: Unable to open file %s\n", dec->stego_image_fname);
        return e_failure;
    }


    //file pointer for decoded text file
    dec->fptr_decode=fopen(dec->decode_fname,"w");
    if(dec->fptr_decode==NULL)
    {
        //print error message if fails to open the file 
        fprintf(stderr, "ERROR: Unable to open file %s\n", dec->decode_fname);
        return e_failure;
    }
    return e_success;
}

/*
Function definition for decoding magic string
In this function first the stego image pointer is moved to 54th position by skipping the header.
From that position fetch the required number of bytes from the image get all the lsb's convert to string.
*/
Status decode_magic_string(DecodeInfo *dec)
{
    unsigned char str[3];
    fseek(dec->fptr_stego_image,54,SEEK_SET);
    decode_data_from_image(2, dec->fptr_stego_image,str);
    if(strcmp(MAGIC_STRING,str)!=0)
    {
        printf("Failed to decode the correct magic string\n");
        return e_failure;
    }
    printf("Magic string: %s\n",str);
    return e_success;
}

/*
This function decodes the size of secret file extension from the stego image
* It calls the function that Fetches the next 32 bytes from the stego image, extracts lsb's and convets it to integer
*/
Status decode_secret_file_extn_size(DecodeInfo *dec)
{
    int size;
    decode_size_from_lsb(dec,&size);
    dec->secret_file_extn_size=size;
    printf("Extension size: %d\n",dec->secret_file_extn_size);
    return e_success;
}


/*
This function decodes the size of secret file from the stego image
* It calls the function that Fetches the next 32 bytes from the source image extracts lsb's and convets it to integer
*/
Status decode_secret_file_size(DecodeInfo *dec)
{
    int size;
    decode_size_from_lsb(dec,&size);
    dec->size_secret_file=size;
    printf("Secret file size: %ld\n",dec->size_secret_file);
    return e_success;

}

/*
This function decodes the secret file extension from stego image
* As the size of secret file extension is known, get the required no of bytes from stego image
* Extract lsb values and convert into string
*/
Status decode_secret_file_extn( DecodeInfo *dec)
{
    unsigned char file_extn[dec->secret_file_extn_size+1];
    decode_data_from_image(dec->secret_file_extn_size, dec->fptr_stego_image,file_extn);
    printf("Secret file extension: %s\n",file_extn);
    return e_success;
}

/*
This function decodes the secret file data from the stego image
* As the size of secret file is known, get the required no of bytes from stego image
* Extract lsb values and convert into string
*/
Status decode_secret_data(DecodeInfo *dec)
{
    unsigned char data[dec->size_secret_file+1];
     decode_data_from_image(dec->size_secret_file,dec->fptr_stego_image,data);
     if(copy_data_to_file(data,dec->fptr_decode)==e_success)
     {
        printf("secret data copied to file\n");
        return e_success;
     }
     else
     {
        printf("Failed to copy secret data to file\n");
        return e_failure;
     }
     return e_success;


}

/*
This function decodes the characters of magic string, secret file extension and secret data from stego image
* To decode each character(1 byte) it requires 8 bytes if data from stego image.
* It reads 8 bytes from the stego image calls the decode byte function to perform decoding 
* The decoded bytes are converted to string
*/
Status decode_data_from_image(long int size, FILE *fptr_stego_image,unsigned char str[])
{
    unsigned char buffer[8];
    int i;
    for(i=0;i<size;i++)
    {
        fread(buffer,8,1,fptr_stego_image);
        decode_byte_from_lsb(&str[i],buffer);
    }
    str[i]='\0';
    return e_success;
}

/*
This function decodes each byte of data from the lsb of each byte of the stego image
* This function gets 1 byte data of the stego image as the input
* Performs bitwise opeations to decode lsb's of each byte and stored it in character array
*/
Status decode_byte_from_lsb(unsigned char *data, unsigned char *image_buffer)
{
    unsigned char ch=0;

    for(int i=0;i<8;i++)
    {
        int bit=image_buffer[i] & 1;
        bit=bit<<i;
        ch=ch|bit;
    }
    *data=ch;
    return e_success;
}

/*
This function decodes the sizes of the files from stego image
* To decode size from the stego image get the 32 bytes from stego image
* Perfroms the bitwise operation to decode the size from lsb of each byte of the stego image
*/
Status decode_size_from_lsb(DecodeInfo *dec,int *size_file)
{
    unsigned char buffer[32];
    int ch=0;
    fread(buffer,32,1,dec->fptr_stego_image);
    for(int i=0;i<32;i++)
    {
        int bit=buffer[i] & 1;
        bit=bit<<i;
        ch=ch|bit;
    }
    *size_file=ch;
    return e_success;

}
/*
This function write the secret data to the decoded text file using file pointer
*/
Status copy_data_to_file(unsigned char *data, FILE* fptr_decode)
{
  fprintf(fptr_decode,"%s",data);
  return e_success;

}