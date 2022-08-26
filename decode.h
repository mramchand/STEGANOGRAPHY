#ifndef DECODE_H
#define DECODE_H
#include<stdio.h>
#include "types.h"			
#include "common.h"

#define MAX_FILE_SUFFIX	4	

//structure declaration 

typedef struct _DecodeInfo
{
    // stego image info
    char *dec_image_fname;
    FILE *fptr_dec_image;

    // get secret file info
    char *dec_secret_fname;
    FILE *fptr_dec_secret;
    int file_extn_size;
    char extn_secret_file[MAX_FILE_SUFFIX];
    int size_secret_file;

}DecodeInfo;

/* Decode function prototype*/


/* read and validate Decode args from argv*/

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get file pointer for i/p and o/p files*/

Status open_files_dec(DecodeInfo *decInfo);

/* perform the decoding*/

Status do_decoding(DecodeInfo *decInfo);

/* Store magic string*/

Status decode_magic_string (const char *magic_string, DecodeInfo * decInfo);

/* decode encoded data from image*/

Status decode_data_from_image(const char *data, int size, FILE *fptr_dec_image);

/* decode a byte from lsb of image data array*/

Status decode_byte_to_lsb(char *dec_data, char *buffer, int t_size);

/* decode data from image*/

Status decode_data_from_image_extn(const char *data, int size, FILE *fptr_dec_image);

/* decode byte to lsb*/

Status decode_byte_to_lsb_extn(char *dec_data, char *buffer, int t_size);

/* decode size from encoded data */

Status decode_size (const int data,int size, FILE *fptr_dec_image);

/* decode  a byte (size) from lsb of image data*/

Status decode_size_to_lsb (char *dec,char *buffer,int s);

/* decode secret file extension*/

Status decode_secret_file_extn(const char *file_extn, DecodeInfo *decInfo);

/* decode secret file size from encode image*/

Status decode_secret_file_size (const int data,int file_size, DecodeInfo *decInfo);

/* decode  a byte (file_size) from lsb of image data*/

Status decode_secret_file_size_to_lsb (char *dec,char *buffer,int s);

/*decode secret file data from encode image */
Status decode_secret_file_data(DecodeInfo *decInfo);
#endif
