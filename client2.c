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
#define DRIVER "sda1"

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

void encrypt (char str[]){
	for(int i=0;i<strlen(str);i++){
		str[i]=str[i]+3;
	}
}

int main(){

	
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	char str1[256], str2[256];
	float tmp;
	int i;
	struct typeInfo info;
	struct cpustat cpustat1;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP_SERVER);

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
		
			
		FILE* fp;

		//Name of machine
		fp = fopen ("/etc/hostname", "r");
		fscanf(fp, "%s", str1);
		strcpy(info.name,"Windows");
		printf("info.name %s\n",info.name);
		
		//OS of machine
		fp = fopen ("/proc/version", "r");
		fscanf(fp, "%s", str1);
		encrypt(str1);
		strcpy(info.OS,str1);
		printf("info.OS %s\n",info.OS);
		
		//Size HDD
		fp = fopen ("/proc/partitions", "r");
		while (fscanf(fp, "%*s %*s %s %s", str1,str2) != EOF ){
			if(strcmp(str2,DRIVER)==0){
				tmp=atof(str1)/(1024*1024);
				info.sizeHD=tmp;
				printf("info.sizeHD %.fGB\n",info.sizeHD);
			}
		}
		
		//Free HDD
		if (statvfs("/", &stat) == 0) {
			// error happens, just quits here
			tmp = (stat.f_frsize * stat.f_bavail)/(1024*1024);
			info.sizeHDDispo=tmp;
			printf("info.sizeHDDispo %.fMB\n", info.sizeHDDispo);
	  
		}
		char *buffer, *tmp2;
		size_t buff_size = 0;
		//RAM Size
		fp = fopen ("/proc/meminfo", "r");
		while (fscanf(fp, "%s %s %*s ", str1,str2) != EOF ){
			if(strcmp(str1,"MemTotal:")==0){
				tmp=atof(str2)/1000;
				info.sizeRAM=tmp;
				printf("info.sizeRAM %.2fMB\n", info.sizeRAM);
			}
		}
		
		
		//Free RAM Size
		fp = fopen ("/proc/meminfo", "r");
		while (fscanf(fp, "%s %s %*s ", str1,str2) != EOF ){
			if(strcmp(str1,"MemFree:")==0){
				info.sizeRAMDispo=atof(str2)/1000;
				printf("info.sizeRAMDispo %.2fMB\n",info.sizeRAMDispo);
			}
		}
		
		//CPU Name
		fp = fopen ("/proc/cpuinfo", "r");
	
		for (i=0;i<5;i++){
			getline(&buffer,&buff_size,fp);
		}
		tmp2=strtok(buffer," ");
		tmp2=strtok(NULL," ");
		tmp2=strtok(NULL," ");
		strcpy(str1,tmp2);
		strcat(str1," ");
		tmp2=strtok(NULL," ");
		strcpy(str2,tmp2);
		strcat(str1,str2);
		tmp2=strtok(NULL," ");
		//tmp=atof(tmp2);
		strcat(str1," ");
		tmp2=strcat(str1,tmp2);
		strcpy(info.nameCPU,tmp2);
		printf("info.nameCPU %s\n", info.nameCPU);
		
		//CPU usage
		fp = fopen ("/proc/stat", "r");
		int idle, total;
		double usage;
		char cpun[255];
		while (fscanf(fp, "%s %d %d %d %d %d %d %d %*d %*d %*d ", cpun, &(cpustat1.t_user), &(cpustat1.t_nice), 
			&(cpustat1.t_system), &(cpustat1.t_idle), &(cpustat1.t_iowait), &(cpustat1.t_irq),
			&(cpustat1.t_softirq))!= EOF ){
		if(strcmp(cpun,"cpu")==0){
				idle=cpustat1.t_idle+cpustat1.t_iowait;
				total=cpustat1.t_user + cpustat1.t_nice + cpustat1.t_system + cpustat1.t_idle + cpustat1.t_iowait + cpustat1.t_irq + cpustat1.t_softirq;
				usage= (double) (total-idle)/total*100;
				info.usageCPU=usage;
				printf("info.usageCPU %f%\n", info.usageCPU);
			}
		}
		//CPU Speed
		fp = fopen ("/proc/cpuinfo", "r");
		
		
		for (i=0;i<5;i++){
			getline(&buffer,&buff_size,fp);
		}
		tmp2=strtok(buffer," ");
		tmp2=strtok(NULL," ");
		tmp2=strtok(NULL," ");
		tmp2=strtok(NULL," ");
		tmp2=strtok(NULL," ");
		tmp2=strtok(NULL," ");
		tmp2=strtok(NULL," ");
		tmp2=strtok(NULL," ");
		tmp=atof(tmp2);
		info.speedCPU=tmp;
		printf("info.speedCPU %.2fGHz\n", info.speedCPU);
		
		//CPU Temperature
		//fp = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
		//fscanf(fp, "%s", str2);
		//tmp=atof(str2)/(1000);
		//info.temp=tmp;
		//printf("%f\n",info.temp);
		
		//UpTime
		fp = fopen ("/proc/uptime", "r");
		fscanf(fp, "%s %*s", str1);
		tmp=atof(str1)/(60);
		info.uptime=tmp;
		
		printf("%s %s %.2f %.2f %.2f %.2f %s %.2f %.2f %.2f %.2f \n", info.name, info.OS, 
				info.sizeHD, info.sizeHDDispo, info.sizeRAM, info.sizeRAMDispo, 
				info.nameCPU, info.speedCPU, info.usageCPU, info.temp, info.uptime);
		printf("\n");
		send(clientSocket, &info, sizeof(info), 0);
	
		sleep(10);
	}
		

		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: \t%s\n", buffer);
		}
	

	return 0;
}
