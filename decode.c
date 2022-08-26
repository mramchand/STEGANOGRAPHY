#include<stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"


/* Read and Validate decode arguments 
Input       : command line arguments and address of structure DecodeInfo
Output      : Status (success or failure)
 */

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    char *ptr;
    ptr = strstr (argv[2], ".bmp");					//to check input file is ".bmp" or not
    if (strcmp (ptr, ".bmp")== 0)
    {
        decInfo->dec_image_fname = argv[2];		
    }
    else
    {
        return e_failure;
    }
    if (argv[3] == NULL)						//check argv[3] is passed or not 
    {
        printf ("INFO: Output file not mentioned. creating...-->de_secret.txt as default\n");
        decInfo->dec_secret_fname = "de_secret.txt";
        strcpy(decInfo->extn_secret_file , ".txt");
    }
    else
    {
        ptr = strstr (argv[3], ".txt");
        if (strcmp (ptr, ".txt")== 0)					//check input file is ".txt" or not
        {
            decInfo->dec_secret_fname = argv[3];	
            strcpy(decInfo->extn_secret_file , ".txt");						
        }
        else
        {
            return e_failure;
        }
    }
    return e_success;

}
/* Perform decoding 
Input       : Address of DecodeInfo 
Output      : Status(success or failure)
Status do_decoding (DecodeInfo *decInfo)
 */
Status do_decoding (DecodeInfo *decInfo)

{

    // checking and calling all functions step by step and if success decoding done



    if (open_files_dec(decInfo) == e_success)

    {

        printf ("INFO: All files are opened successfully\n");

        if (decode_magic_string (MAGIC_STRING, decInfo) == e_success)

        {

            printf ("INFO: Magic string decode successfully\n");

            if (decode_size (sizeof(int),strlen(decInfo->extn_secret_file), decInfo->fptr_dec_image) == e_success)

            {

                printf ("INFO: Extn. size Decode successfully\n");

                if (decode_secret_file_extn (decInfo->extn_secret_file, decInfo) == e_success)

                {

                    printf ("INFO: secret file extn. decode successfully\n");

                    if (decode_secret_file_size(sizeof(int),decInfo->size_secret_file,decInfo) == e_success)

                    {

                        printf ("INFO: secret file size decode successfully\n");	

                        if (decode_secret_file_data(decInfo) == e_success)

                        {

                            printf ("INFO: secret file data decode successfully\n");

                        }

                        else

                        {

                            printf ("ERROR: Failed to decode secret file data \n");

                            return e_failure;

                        }

                    }

                    else

                    {

                        printf ("ERROR: Failed to decode secret file size\n");

                        return e_failure;

                    }

                }

                else

                {

                    printf ("ERROR: Failed to decode secret file extn.\n");

                    return e_failure;

                }

            }

            else

            {

                printf ("ERROR: Failed to decode Extn. size\n");

                return e_failure;

            }

        }

        else

        {

            printf ("ERROR: Failed to decode Magic string\n");

            return e_failure;

        }

    }

    else

    {

        printf ("ERROR: Failed to Open files \n");

        return e_failure;

    }

    return e_success;

}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files_dec (DecodeInfo *decInfo)
{
    // Stego Image File
    printf("INFO : Opening Required files... \n");
    decInfo->fptr_dec_image = fopen (decInfo->dec_image_fname, "r");

    if (decInfo->fptr_dec_image == NULL)
    {
        perror ("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->dec_image_fname);
        return e_failure;
    }
    printf("INFO : Opended %s\n",decInfo->dec_image_fname);
    // default .txt file
    decInfo->fptr_dec_secret = fopen (decInfo->dec_secret_fname, "w");
    // error handling
    if (decInfo->fptr_dec_secret == NULL)
    {
        perror ("fopen");
        fprintf (stderr,"ERROR: Unable to open file %s\n", decInfo->dec_secret_fname);
        return e_failure;
    }
    printf("INFO : Opended %s\n",decInfo->dec_secret_fname);

    return e_success;
}
/*Decode the magic string signature
 * Input : magic string and structure address
 * Output : e_success or e_failure
 */
Status decode_magic_string (const char *magic_string, DecodeInfo * decInfo)
{
    printf("INFO : Decoding Magic string Signature... \n");
    decode_data_from_image (magic_string, strlen(magic_string), decInfo->fptr_dec_image);
    return e_success;
}
/* Decode data from image
Input       : Address containing data to be decoded, no of bytes and image file pointer
Output      : Status(success or failure)
 */

Status decode_data_from_image (const char *data, int size, FILE *fptr_dec_image)
{
    char buffer [size * 8];
    int k;
    char dec_data[k];
    if (strcmp (data, MAGIC_STRING) == 0)
    {
        fseek(fptr_dec_image, 54L, SEEK_SET);		// set file pointer to 54th position to check data is #*
    }
    for (k=0; k < size; k++)
    {
        fread(buffer,8,1,fptr_dec_image);
        decode_byte_to_lsb(dec_data, buffer,k);		// function call 
    }
    if (strcmp (data, MAGIC_STRING) == 0)				// check the decoded data is magic string or not
    {
        if (strcmp (dec_data, data) == 0)
            return e_success;
        else
            return e_failure;
    }
    return e_success;
}
Status decode_byte_to_lsb (char *dec_data, char *buffer,int t_size)
{
    unsigned char lsb_bit;
    dec_data[t_size]=0;
    int j=7;
    for (int i=0; i < 8; i++)
    {
        lsb_bit = buffer[i] & 1;
        if (lsb_bit == 1)
        {
            dec_data[t_size] = dec_data[t_size] | lsb_bit << j;
        }
        j--;
    }
}

/* Function Defination: Decode Size
 * * Input: size, stego image pointers
 * * Output: status: success if true
 * * Description: get/decode the file size from stego file
 *  */  

Status decode_size (const int data, int size, FILE *fptr_dec_image)
{
    int k;
    char buffer [size * 8];
    char dec[k];
    for (k=0; k < size;k++)
    {
        fread(buffer, 32, 1, fptr_dec_image);
        decode_size_to_lsb (dec,buffer,k);
    }
    if ( dec[0] == size)					// compare size with data
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status decode_size_to_lsb(char *dec, char *buffer,int s )
{
    unsigned char lsb_bit;
    dec[s] =0;
    int j=31,count=0;
    for (int i=0; i < 32; i++)
    {
        lsb_bit = buffer[i] & 1;
        if (lsb_bit == 1)
        {
            dec[s] = dec[s] | lsb_bit << j;
        }
        j--;
    }
}
/* Decode file extension
Input       : Address containing file extension, Address of DecodeInfo
Output      : Status(success or failure)
 */

Status decode_secret_file_extn (const char *file_extn, DecodeInfo *decInfo)
{
    printf ("INFO: decoding file name ( %s ) File extension... \n", decInfo->dec_secret_fname);
    decode_data_from_image_extn (file_extn, strlen(file_extn), decInfo->fptr_dec_image);
    return e_success;
}
Status decode_data_from_image_extn (const char *data, int size, FILE *fptr_dec_image)
{
    char buffer [size * 8];
    int k;
    char dec_data[k];
    fseek(fptr_dec_image, 102L, SEEK_SET);
    for (k=0; k < size; k++)
    {
        fread(buffer,8,1,fptr_dec_image);
        decode_byte_to_lsb_extn(dec_data, buffer,k);
    }
    return e_success;
}
Status decode_byte_to_lsb_extn (char *dec_data, char *buffer,int t_size)
{
    unsigned char lsb_bit;
    dec_data[t_size]=0;
    int j=7,count=0;
    for (int i=0; i < 8; i++)
    {
        lsb_bit = buffer[i] & 1;
        if (lsb_bit == 1)
        {
            dec_data[t_size] = dec_data[t_size] | lsb_bit << j;	
        }
        j--;		
    }
}
Status decode_secret_file_size (const int data, int file_size, DecodeInfo *decInfo)
{
    printf("INFO : Decoding %s File size... \n",decInfo->dec_secret_fname);
    int k;
    char buffer [data * 8];
    char dec[k];
    for (k=0; k < data;k++)
    {
        fread(buffer, 32, 1, decInfo->fptr_dec_image);
        decode_secret_file_size_to_lsb (dec,buffer,k);
    }
    file_size = dec[0];
    decInfo->size_secret_file = file_size;
    return e_success;
}
/* Decode size to  lsb
Input       : Data to be decoded and integer address
Output      : Status(success or failure)
 */
Status decode_secret_file_size_to_lsb(char *dec, char *buffer,int s )
{
    unsigned char lsb_bit;
    dec[s] =0;
    int j=31,count=0;
    for (int i=0; i < 32; i++)
    {
        lsb_bit = buffer[i] & 1;
        if (lsb_bit == 1)
        {
            dec[s] = dec[s] | lsb_bit << j;
        }
        j--;
    }
}
/* Decode secret file data 
Input       : Address of DecodeInfo
Output      : Status(success or failure)
 */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf ("INFO: decoding %s file data... \n ", decInfo->dec_secret_fname);

    int i,j;
    int k = decInfo->size_secret_file;
    unsigned char buffer[8], dec_data[k];
    fseek(decInfo->fptr_dec_image, 166L, SEEK_SET);
    for (i=0; i < k ; i++)
    {
        fread (buffer, 8,1, decInfo->fptr_dec_image);
        decode_byte_to_lsb(dec_data, buffer,i);
    }
    fwrite (dec_data,k,1,decInfo->fptr_dec_secret);
    return e_success;
}
