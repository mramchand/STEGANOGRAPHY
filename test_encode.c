/*
NAME            : Ramchand
DATE            : 10/04/2022
DESCRIPTION     : Steganography project
*/

#include<stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"



int main(int argc, char **argv)
{
 if( check_operation_type(argv) == e_encode)
 {
   printf("Selected encoding\n");
   EncodeInfo encInfo;
     if ( read_and_validate_encode_args(argv, &encInfo) == e_success)
       {
       printf("Read and validate encode arguments is a success\n");	
       printf("Started encoding....................\n");
       if (do_encoding(&encInfo) == e_success)
       {
	    printf("------------Completed encoding--------------\n");
         } 
      else
      {
	      printf("--------------Encoding failed---------------\n");
	      return -1;
      }
     }
  else
  {
	  printf("Read and validate encode arguments is a failure\n");
	  return -1;
  }
 }
 else if( check_operation_type(argv)==e_decode)
 {
	 printf("Selected decoding\n");
	 DecodeInfo decInfo;
	 if ( read_and_validate_decode_args(argv, &decInfo) == e_success)
	 {
		 printf("Read and validate\n");
	   if(do_decoding(&decInfo) == e_success)
	   {
		   printf("......completed decoding......\n");
	   }
	   else
	   {
		   printf("NO\n");
	   }
	 }
	 else
	 {
		 printf("failed to decode\n");
		 return -1;
	 }
 
	 
	 
	 
 }
 else
 {
	 printf("Invalid option\n For encoding: ./a.out -e beautiful.bmp secret.txt stego.bmp\nFor decoding./a.out -d stego.bmp decode.txt\n");
 }
 return 0;
}
OperationType check_operation_type(char *argv[])
{
	if(strcmp(argv[1] , "-e")==0)
	return e_encode;
	else if(strcmp(argv[1], "-d") == 0)
	return e_decode;
	else
		return e_unsupported;
}
