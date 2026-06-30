#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */
 
/*
This function checks the files in the CLA has required extensions or not before encoding.
It returns failure if input image file and secret file doesn't have the required format.
If output file is given in CLA then it checks for the required format, if not given then it 
created a new output file
*/
 
Status read_and_validate_encode_args(char *argv[], EncodeInfo *enc)
{
    //1. check for .bmp file
    //checks in the value at the second index position of argv[] contains .bmp or not
    //if present returns the starting address of the substring if no return null
    if(strstr(argv[2],".bmp")!=NULL)
    {
        enc->src_image_fname=argv[2];
    }
    else
    {
        printf("No .bmp file is passed as argument\n");
        return e_failure;
    }

    //2. check for .txt file (similar to src image file)
    if(strstr(argv[3],".txt")!=NULL)
    {
        enc->secret_fname=argv[3];
    }
    else
    {
       printf("No .txt file is passed\n");
       return e_failure;
    }

    //3. check for output file name if passed validate otherwise create a file and store file name
    if(argv[4]!=NULL)
    {
        if(strstr(argv[4],".bmp")!=NULL)
        {
            enc->stego_image_fname=argv[4];
        }
    }
    else
    {
        enc->stego_image_fname ="stego.bmp";
    }
    
    return e_success;
}

/*
This function performs the all the encoding steps. If any of the steps fail gives error message and program gets terminated.
1. Open the input and output files in the required mode
2. Checks for the image capacity to encode the data
3. Copies bmp header
4. Encode Magic String
5. Encode secret file extension size
6. Encode secret file size
7. Encode secret file extension
8. Encode secret data
9. Encode remaining data to output file
*/
Status do_encoding(EncodeInfo *enc)
{
    //open the files in respective modes
    if(open_files(enc)==e_success)
    {
        printf("Files opened successfully\n");
        if(check_capacity(enc)==e_success)
        {
            printf("Can encode secret data in image\n");
            if(copy_bmp_header(enc->fptr_src_image,enc->fptr_stego_image)==e_success)
            {
                printf("Copy bmp header success!\n");

                //Magic string encoding
                
                if(encode_magic_string(MAGIC_STRING, enc)==e_success)
                {
                    printf("Magic String encoded successfully!\n");

                    //encode secret file extension size
                   if(encode_secret_file_extn_size(4, enc)==e_success)
                    {
                        printf("Secret file extension size encoded successfully!\n");
                        
                        //encode secret file size
                        if(encode_secret_file_size(enc->size_secret_file, enc)==e_success)
                        {
                            printf("secret file size encoded successfully!\n");

                            //encode secret file extension
                            if(encode_secret_file_extn(".txt", enc)==e_success)
                            {
                                printf("Secret file extesion encoded successfully!\n");

                                //encode secret file data to image
                                if(encode_secret_file_data(enc)==e_success)
                                {
                                    printf("Secret data encoded successfully\n");

                                    //copy remaining data to stego image
                                    if(copy_remaining_img_data(enc->fptr_src_image, enc->fptr_stego_image)==e_success)
                                    {
                                        printf("Remaining data copied successfully\n");
                            
                                    }
                                    else
                                    {
                                        printf("Failed to copy remaining data\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("Failed to encode secret data\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("Failes to encode secret file extension\n");
                                return e_failure;

                            }
                            
                        }
                        else
                        {
                            printf("Failed to encode secret file size\n");
                            return e_failure;
                        }

                    }
                    else
                    {
                        printf("Failed to encode secret file extension size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Failed to encode magic string\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Cpoy bmp header failed\n");
                return e_failure;
            }
            
        }
        else 
        {
            printf("ERROR! Insufficient image size to encode secret data\n");
            return e_failure;
        }
    }
    else 
    {
        printf("Error in opening files\n");
        return e_failure;
    }
    
    return e_success;

}

/*
This function opens all the files in the respective modes using the file pointers declared in the EncodeInfo structure.
If failed to open returns failure.
1. open source image file in read mode
2. open secret text file in read mode
3. open stego image file in write mode
*/
Status open_files(EncodeInfo *enc)
{
   //1. file pointer for input image file
   enc->fptr_src_image=fopen(enc->src_image_fname,"r");
   if(enc->fptr_src_image==NULL)
   {
      fprintf(stderr, "ERROR: Unable to open file %s\n", enc->src_image_fname);
      return e_failure;
   }

   //2. file pointer for input text file
   enc->fptr_secret=fopen(enc->secret_fname,"r");
   if(enc->fptr_secret==NULL)
   {
     fprintf(stderr, "ERROR: Unable to open file %s\n", enc->secret_fname);
     return e_failure;
   }
   
   //3. file pointer for output file
   enc->fptr_stego_image=fopen(enc->stego_image_fname,"w");
   if(enc->fptr_stego_image==NULL)
   {
      fprintf(stderr, "ERROR: Unable to open file %s\n", enc->stego_image_fname);
     return e_failure;
   }
   return e_success;
}

/*
This function checks the image capacity. If the required size is greater than the size of input image returns failure.
1. get the image size and store in image capacity structure member
2. get secret file size and store in size_secret file structure member
3.calculare the required size
4. compare image cpacity with required size
*/
Status check_capacity(EncodeInfo *enc)
{
    enc->image_capacity=get_image_size_for_bmp(enc->fptr_src_image);
    enc->size_secret_file=get_file_size(enc->fptr_secret);
    uint required_size=16+32+32+32+(enc->size_secret_file*8);
    if(enc->image_capacity>required_size)
    {
        return e_success;
    }
    else
    {
        return e_failure;

    }
}

/*
This function calculates and returns the size of the image.
* The bmp header contains the details of image such as height of the pixel , width of the pixel etc.
* 18 th byte of the header contains width of the pixel
* 22 nd byte of the header contains height of the pixel
* Total size of the image is calculates as width*height*3(as image has 3 colours ie, RGB)

*/
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width,height;

    //18th byte in the image header has width of pixel so get 18th byte size
    fseek(fptr_image,18,SEEK_SET);
    //Read the width
    fread(&width, sizeof(int),1,fptr_image);
    printf("width=%d\n",width);

    fread(&height,sizeof(int),1,fptr_image);
    printf("height=%d\n",height);

    uint total_size= width*height*3;
    return total_size;

}
/*
This function return the totla file size
* File size is the total number of character in the file
*/

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    uint size=ftell(fptr);
    rewind(fptr);
    return size;
} 

/*
This function copies the bmp header data from source image file to stego image file.
* The header of both source and stego image must be same
* Using source file pointer get the header contents from source file and store it in an array
* Read the contents from array using fread and write the header contents to stego image file using fwrite.
*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    unsigned char arr[54];
    fseek(fptr_src_image,0,SEEK_SET);
    fread(arr,1,54,fptr_src_image);
    fwrite(arr,1,54,fptr_dest_image);
    return e_success;
}

/*
This function encodes the magic string into the stego image
* Move the source file pointer to 54 th byte of the source image.
* As the size of magic string is known get the required no of bytes from source image
* Encode values to the lsb of each byte and store it in stego image
*/
Status encode_magic_string(char *magic_string, EncodeInfo *enc)
{
    //function call encode magic string
    encode_data_to_image(magic_string, strlen(magic_string), enc->fptr_src_image, enc->fptr_stego_image);
    return e_success;

}

/*
This function encodes the size of secret file extension
* It calls the function that Fetches the next 32 bytes from the source image and encode 4 byte into the lsb 
*/
Status encode_secret_file_extn_size(int size, EncodeInfo *enc)
{
    //Function call to encode the size into source image
      encode_size_to_lsb(4,enc);
      return e_success;
}

/*
This function encodes the size of secret file
* It calls the function that Fetches the next 32 bytes from the source image and encode 4 byte into the lsb
*/
Status encode_secret_file_size(long file_size, EncodeInfo *enc)
{
     encode_size_to_lsb(file_size, enc);
     return e_success;
}

/*
This function encodes the secret file extension
* As the size of secret file extension is known, get the required no of bytes from source image
* Encode values to the lsb of each byte and store it in stego image
*/
Status encode_secret_file_extn(unsigned char *file_extn, EncodeInfo *enc)
{
    encode_data_to_image(file_extn, strlen(file_extn), enc->fptr_src_image, enc->fptr_stego_image);
    return e_success;
    
}

/*
This function encodes the secret file data into the stego image
* As the size of secret file is known, get the required no of bytes from source image
* Encode values to the lsb of each byte and store it in stego image
*/
Status encode_secret_file_data(EncodeInfo *enc)
{
    char data[enc->size_secret_file];
    fseek(enc->fptr_secret,0,SEEK_SET);
    fread(data,1,enc->size_secret_file,enc->fptr_secret);
    encode_data_to_image(data, enc->size_secret_file, enc->fptr_src_image, enc->fptr_stego_image);
    return e_success;
}
/*
This function copies the remaining RGB data from source image to stego image 
*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    unsigned char ch;
    while(fread(&ch,1,1,fptr_src)>0)
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
    
}

/*
This function encodes the characters of magic string, secret file extension and secret data into stego image
* To encode each character(1 byte) it requires 8 bytes if data from source image.
* It reads 8 bytes from the source image calls the encode byte function to perform encoding 
* The encoded bytes are written into the stego image.
*/
Status encode_data_to_image(unsigned char *data, long int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    unsigned char arr[8];
    for(int i=0;i<size;i++)
    {
        fread(arr,8,1,fptr_src_image);
        encode_byte_to_lsb(data[i], arr);
        fwrite(arr,8,1,fptr_stego_image);

    }
    return e_success;

}

/*
This function encodes each byte of data into the lsb of each byte of the source image
* This function gets the character data which is to be encoded and number of bytes required from source image as input
* Performs bitwise opeations to encode each bit of character data to the lsb of each byte of the source image
*/
Status encode_byte_to_lsb(unsigned char data, unsigned char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        image_buffer[i]=(image_buffer[i] & 0xFE)|((data>>i) & 1);
    }
}

/*
This function encodes the sizes of the files into the image
* This function gets the size of files as the input
* To encode size it requires 32 bytes of source image.
* Perfroms the bitwise operation to encode the size to the lsb of each byte of the source image
*/
Status encode_size_to_lsb(long int data, EncodeInfo *enc)
{
    unsigned char buffer[32];
    fread(buffer,32,1,enc->fptr_src_image);
    for(int i=0;i<32;i++)
    {
        buffer[i]=(buffer[i] & 0xFE) | ((data>>i) & 1);
    }
    fwrite(buffer,32,1,enc->fptr_stego_image);
    
}

 