#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/statvfs.h>

#define BUFFER_SIZE 100000

int main(){

FILE* fp;
char str1[256], str2[256], nomClient[256], str3[256],str4[256];//haystack-str1, needle-str2 strstr(haystack, needle)
float tmp;
fp = fopen ("/home/rt/Test/page.html", "a+");
char haystack[20];
char needle[20];
char *ret;
char *buffer, *tmp2;
size_t buff_size = 0;
ssize_t line_size;
int count;	
FILE * fPtr;
FILE * fTemp;   
char buffer1[BUFFER_SIZE];
char newline[BUFFER_SIZE];
int line=1;
	    strcpy(str1,"<td class=\"tg-a\">");
	    strcpy(str2,"</td>\n");

		strcpy(nomClient,"Linux");
		line_size=getline(&buffer,&buff_size,fp);
		printf("%s",buffer);
		
		while (strstr(buffer, nomClient)==NULL){
		line ++;
		line_size=getline(&buffer,&buff_size,fp);
		printf("%s",buffer);
		}
		printf("%d",line);
		
	fPtr  = fopen("page.html", "r");
    fTemp = fopen("replace.tmp", "w"); 

	line=line+1;
    count = 0;
    char OS[256];
    strcpy(OS,"WIndows");
    float HD=4.5;
    while ((fgets(buffer1, BUFFER_SIZE, fPtr)) != NULL)
    {
        count++;

        if (count == line){
			strcat(str4,str1);
			strcat(str4,OS);
			strcat(str4,str2);
			strcpy(newline,str4);
            fputs(newline, fTemp);

           }
       
            
        else if(count == line+1){
			sprintf(str3,"%f",HD);
			strcpy(str4,"");
			strcat(str4,str1);
			strcat(str4,str3);
			strcat(str4,str2);
			strcpy(newline,str4);
            fputs(newline, fTemp);}
           
              
        else{
            fputs(buffer1, fTemp);}
    }

    fclose(fPtr);
    fclose(fTemp);

    remove("page.html");

    rename("replace.tmp", "page.html");

			
			}
			
