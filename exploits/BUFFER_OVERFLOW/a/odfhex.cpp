/**************************************
odfhex - objdump hex extractor
by steve hanna v.01
   vividmachines.com
   shanna@uiuc.edu
you are free to modify this code
but please attribute me if you
change the code. bugfixes & additions
are welcome please email me!
to compile:
g++ odfhex.cpp -o odfhex


note: the XOR option works
perfectly, but i haven't implemented
the full x86 payload decoder yet.
so that option is mostly useless.

this program extracts the hex values
from an "objdump -d <binaryname>".
after doing this, it converts the
hex into escaped hex for use in
a c/c++ program.
happy shellcoding!
***************************************/


#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define HEX_PER_LINE 17
char symbols[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
const int MAX_BASE = 36;
int GetIndex(char * pString, char search);
int BaseToDec(char* number, int base)
{
       if( base < 2 || base > MAX_BASE)
          return 0; //Failed
       int NumLength = strlen(number);
       int PlaceValue, total = 0;

       PlaceValue = (int)pow(base,NumLength-1);


      for(int i=0;i<NumLength;i++)
      {
          total += GetIndex(symbols,*number)*PlaceValue;
          number++;
          PlaceValue /= base; //Next digit's place value (previous/base)
      }
     return total;
}

int GetIndex(char * pString, char search)
{
     int index = 0;
     while(*pString != '0')
     {
       if(*pString==search)
               break;
       pString++;
       index++;
     }
     return index;
}


int main(int argc, char** argv)
{
       FILE* dump = NULL;
       long length = 0;
       char* content;
       int i=0;
       int count =0;
       int total=0;
       int XORvalue=0;
       bool XORit = false;
       char HexNumber[3]={'\0'};

       printf("\nOdfhex - object dump shellcode extractor - by steve hanna - v.01\n");

       if(argc < 2)
       {

             printf("%s: <object file dump> [-x xor offset in decimal] \n",argv[0]);
             return -1;
       }

       dump = fopen(argv[1],"r");
       if(!dump)
       {
              printf("Error: Couldn't open file.\n");
              return -1;
       }

       fseek(dump,0,SEEK_END);

       length = ftell(dump);

       content = new char[length+1];
       memset(content,0,sizeof(content));
       printf("Trying to extract the hex of %s which is %d bytes long\n",argv[1],length);

       if (argc > 3 &&  !strcmp(argv[2],"-x"))
       {
              XORit =true;
              XORvalue = BaseToDec(argv[3],16);
              printf("XORing with 0x%02x\n",XORvalue);
       }
       fseek(dump,0,SEEK_SET);
       for(int i=0; i < length; i++)
       {
              content[i] = fgetc(dump);
       }
       fclose(dump);

       while(count !=4)
       {

              if(content[i] == ':')
                     count++;
              i++;
       }
       count = 0;
		
	   printf("\"");
       while(i < length)
       {
			if( (content[i-1] == ' ' || content[i-1]=='\t') &&
                (content[i+2] == ' ' ) &&
                (content[i] != ' ') &&
                (content[i+1] != ' ') &&
                ((content[i]>='0' && content[i]<='9') || (content[i]>='a' && content[i]<='f')) &&
                ((content[i+1]>='0' && content[i+1]<='9') || (content[i+1]>='a' && content[i+1]<='f'))
              )
				{
					if(XORit)
					{
				        HexNumber[0] = content[i];
				        HexNumber[1] = content[i+1];

					    printf("\\x%02x",BaseToDec(HexNumber,16) ^ XORvalue);
					}
					else
						printf("\\x%c%c",content[i],content[i+1]);

                     count++;
                     total++;   
            }
		 
			if(i+1 == length)
			{
				 printf("\";\n");
			}
			else if(count == HEX_PER_LINE) 
			{
				printf("\"\\\n\"");
				count =0;
			}
       i++;
       }
	   
       delete[] content;
       printf("\n%d bytes extracted.\n\n",total);
       return 0;
}
