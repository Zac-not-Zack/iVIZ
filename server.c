#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/statvfs.h>

#define PORT 30000
#define IP_SERVER "134.59.139.192"
#define BUFFER_SIZE 100000

struct typeInfo {
char name[256];
char OS[10];
float sizeHD;
float sizeHDDispo;
float sizeRAM;
float sizeRAMDispo;
char nameCPU[200];
float speedCPU;
float usageCPU;
float temp;
};

struct statvfs stat;

struct cpustat {
    unsigned long t_user;
    unsigned long t_nice;
    unsigned long t_system;
    unsigned long t_idle;
    unsigned long t_iowait;
    unsigned long t_irq;
    unsigned long t_softirq;
};

int main(){

	struct typeInfo info;
	
	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	FILE* fp;
	char str1[256], str2[256], nomClient[256], str3[256],str4[256];//haystack-str1, needle-str2 strstr(haystack, needle)
	float tmp;
	
	char haystack[20];
	char needle[20];
	char *buffer2, *tmp2;
	size_t buff_size = 0;
	ssize_t line_size;
	int count;	
	FILE * fPtr;
	FILE * fTemp;   
	char buffer1[BUFFER_SIZE];
	char newline[BUFFER_SIZE];
	


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", PORT);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);
			strcpy(str1,"<td class=\"tg-a\">");
			strcpy(str2,"</td>\n");
			while(recv(newSocket, &info, sizeof(info), 0)){
				
				fp = fopen ("/home/rt/Téléchargements/page.html", "a+");
				int line=1;
				//printf("%f\n", info.sizeHDDispo);
				

				strcpy(nomClient,info.name);
				line_size=getline(&buffer2,&buff_size,fp);
				//printf("%s",buffer2);
				//printf("%s",nomClient);
				while (strstr(buffer2, nomClient)==NULL){
				line ++;
				line_size=getline(&buffer2,&buff_size,fp);
				//printf("%s",buffer2);
			}
				//printf("%d",line);
				fPtr  = fopen("page.html", "r");
			fTemp = fopen("replace.tmp", "w"); 

			line=line+1;
			count = 0;
			while ((fgets(buffer1, BUFFER_SIZE, fPtr)) != NULL)
			{
				count++;

				if (count == line){
					strcat(str4,str1);
					strcat(str4,info.OS);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);}
					
				else if(count == line+1){
					strcpy(str4,"");
					strcat(str4,str1);
					sprintf(str3,"%f",info.sizeHD);
					strcat(str4,str3);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);}
					
				else if(count == line+2){
					strcpy(str4,"");
					strcat(str4,str1);
					sprintf(str3,"%f",info.sizeHDDispo);
					strcat(str4,str3);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);}
					
				else if(count == line+3){
					strcpy(str4,"");
					strcat(str4,str1);
					sprintf(str3,"%f",info.sizeRAM);
					strcat(str4,str3);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);}   
				 
				else if(count == line+4){
					strcpy(str4,"");
					strcat(str4,str1);
					sprintf(str3,"%f",info.sizeRAMDispo);
					strcat(str4,str3);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);}  
					
				else if(count == line+5){
					strcpy(str4,"");
					strcat(str4,str1);
					strcat(str4,info.nameCPU);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);}
				
				else if(count == line+6){
					strcpy(str4,"");
					strcat(str4,str1);
					sprintf(str3,"%f",info.speedCPU);
					strcat(str4,str3);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);}
				
				else if(count == line+7){
					strcpy(str4,"");
					strcat(str4,str1);
					sprintf(str3,"%f",info.usageCPU);
					strcat(str4,str3);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);} 
					
				else if(count == line+8){
					strcpy(str4,"");
					strcat(str4,str1);
					sprintf(str3,"%f",info.temp);
					strcat(str4,str3);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);
					strcpy(str4,"");}
					  
				else{
					fputs(buffer1, fTemp);}
			}

			fclose(fPtr);
			fclose(fTemp);

			remove("page.html");

			rename("replace.tmp", "page.html");
					//send(newSocket, buffer, strlen(buffer), 0);
					bzero(&info, sizeof(info));
				
			}
			
		}

	}

	close(newSocket);


	return 0;
}
