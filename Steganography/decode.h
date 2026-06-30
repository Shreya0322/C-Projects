#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include "common.h"
#define MAX_EXTN_SIZE 4

/* 
 * Structure to store information decoded from the stego image.
 Information about decoded file
 */
typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /*Decoded file Info*/
    char *decode_fname;
    FILE *fptr_decode;
    

    /* Secret File Info */
    long size_secret_file;
    int secret_file_extn_size;    
} DecodeInfo;


/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_to_decode(DecodeInfo *decInfo);

/*  Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extenstion size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn( DecodeInfo *decInfo);

/* Decode secret data*/
Status decode_secret_data(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_data_from_image(long int size, FILE *fptr_stego_image,unsigned char str[]);

/* Decode a byte from LSB of image data array */
Status decode_byte_from_lsb(unsigned char *data, unsigned char *image_buffer);

/* Decode a size from LSB of image data array */
Status decode_size_from_lsb(DecodeInfo *decInfo,int *size_file);

/*Copy secret data to decode.txt file*/
Status copy_data_to_file(unsigned char *data, FILE* fptr_decode);


#endif
