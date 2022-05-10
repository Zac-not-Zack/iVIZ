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
#define IP_SERVER "192.168.5.99"
#define BUFFER_SIZE 100000
#define HTML "/home/rt/Téléchargements/page.html"

struct typeInfo {
char name[256];
char OS[10];
float sizeHD;
char sizeHDDispo[10];
float sizeRAM;
char sizeRAMDispo[10];
char nameCPU[200];
float speedCPU;
char usageCPU[10];
char temp[10];
float uptime;
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

void decrypt (char str[]){
	for(int i=0;i<strlen(str);i++){
		str[i]=str[i]-3;
	}
}

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
	

	char *buffer2, *tmp2;
	size_t buff_size = 0;
	ssize_t line_size;
	int count;	
	FILE * fPtr;
	FILE * fTemp;   
	char buffer1[BUFFER_SIZE];
	char newline[BUFFER_SIZE];
	


	sockfd = socket(AF_INET, SOCK_STREAM, 0);//creating server socket
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP_SERVER);

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


	while(1){//while the server is accepting connection from client
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		
		if((childpid = fork()) == 0){//make sure that it's a child process and not parent to have separate instance
			close(sockfd);
			strcpy(str1,"<td class=\"tg-a\">");
			strcpy(str2,"</td>\n");
			while(recv(newSocket, (char*)&info, sizeof(info), 0)){//receive info and rewrite the html in permanence
				
				//decryption of potential sensitive information
				decrypt(info.OS);
				decrypt(info.sizeHDDispo);
				decrypt(info.sizeRAMDispo);
				decrypt(info.usageCPU);
				decrypt(info.temp);
				
				fp = fopen (HTML, "a+");
				int line=1;
				
				strcpy(nomClient,info.name);
				line_size=getline(&buffer2,&buff_size,fp);

				while (strstr(buffer2, nomClient)==NULL){//to go to the first line contains info
				line ++;
				line_size=getline(&buffer2,&buff_size,fp);

			}
	
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
					strcpy(str4,"");//initialise str4
					strcat(str4,str1);
					sprintf(str3,"%f",info.sizeHD);
					strcat(str4,str3);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);}
					
				else if(count == line+2){
					strcpy(str4,"");
					strcat(str4,str1);
					strcat(str4,info.sizeHDDispo);
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
					strcat(str4,info.sizeRAMDispo);
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
					strcat(str4,info.usageCPU);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);} 
					
				else if(count == line+8){
					strcpy(str4,"");
					strcat(str4,str1);
					strcat(str4,info.temp);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);
					strcpy(str4,"");}
					
				else if(count == line+9){
					strcpy(str4,"");
					strcat(str4,str1);
					sprintf(str3,"%s",inet_ntoa(newAddr.sin_addr));
					strcat(str4,str3);
					strcat(str4,str2);
					strcpy(newline,str4);
					fputs(newline, fTemp);
					strcpy(str4,"");}
					
				else if(count == line+10){
					strcpy(str4,"");
					strcat(str4,str1);
					sprintf(str3,"%f",info.uptime);
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
					
			bzero(&info, sizeof(info));
				
			}
			
		}

	}

	close(newSocket);


	return 0;
}
