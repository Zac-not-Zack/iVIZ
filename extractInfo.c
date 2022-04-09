#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/statvfs.h>

#define PATH "/"

struct typeInfo {
char name[256];
char OS[10];
int sizeHD;
int sizeHDDispo;
int sizeRAM;
char nameCPU[20];
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

	char str1[256], str2[256];
	float tmp;
	int tmp1;
	struct typeInfo info;
	struct cpustat cpustat1;
		
	FILE* fp;

	//Name of machine
	fp = fopen ("/etc/hostname", "r");
	fscanf(fp, "%s", str1);
	strcpy(info.name,str1);
	printf("%s\n",info.name);
	
	//OS of machine
	fp = fopen ("/proc/version", "r");
	fscanf(fp, "%s", str1);
	strcpy(info.OS,str1);
	printf("%s\n",info.OS);
	
	//RAM Size
	fp = fopen ("/proc/meminfo", "r");
	while (fscanf(fp, "%s %s %*s ", str1,str2) != EOF ){
        if(strcmp(str1,"MemTotal:")==0){
			tmp=atof(str2)/1000;
			info.sizeRAM=tmp;
			printf("%.2fMB\n",tmp);
		}
	}
	
	
	//Free RAM Size
	fp = fopen ("/proc/meminfo", "r");
	while (fscanf(fp, "%s %s %*s ", str1,str2) != EOF ){
        if(strcmp(str1,"MemFree:")==0){
			tmp=atof(str2)/1000;
			printf("%.2fMB\n",tmp);
		}
	}
	
	
	//Free HDD
	  if (statvfs("/", &stat) == 0) {
		// error happens, just quits here
		tmp = (stat.f_frsize * stat.f_bavail)/(1024*1024);
		info.sizeHDDispo=tmp;
		printf("%.fMB\n",tmp);
  
		}

	//Size HDD
	  fp = fopen ("/proc/partitions", "r");
	while (fscanf(fp, "%*s %*s %s %s", str1,str2) != EOF ){
        if(strcmp(str2,"sda1")==0){
			tmp1=atof(str1)/(1024*1024);
			info.sizeHD=tmp1;
			printf("%dGB\n",info.sizeHD);
		}
	}
	//CPU usage
	fp = fopen ("/proc/stat", "r");
	int idle, total;
	double usage;
	char cpun[255];
    while (fscanf(fp, "%s %d %d %d %d %d %d %d", cpun, &(cpustat1.t_user), &(cpustat1.t_nice), 
        &(cpustat1.t_system), &(cpustat1.t_idle), &(cpustat1.t_iowait), &(cpustat1.t_irq),
        &(cpustat1.t_softirq))!= EOF ){
	if(strcmp(cpun,"cpu")==0){
			idle=cpustat1.t_idle+cpustat1.t_iowait;
			total=cpustat1.t_user+cpustat1.t_nice+cpustat1.t_system+cpustat1.t_idle+cpustat1.t_iowait+cpustat1.t_irq+cpustat1.t_softirq;
			usage= (double) (total-idle)/total*100;
			info.usageCPU=usage;
			printf("%.2f%\n",usage);
		}
	}
	//CPU Speed
	fp = fopen ("/proc/cpuinfo", "r");
	char *buffer, *tmp2;
	size_t buff_size = 0;
	int i;
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
	printf("%fGHz\n",tmp);
	
	fp=popen("top","r");
	char path[1035];
	while (fgets(path, sizeof(path), fp) != NULL) {
    printf("%s", path);
  }



	
	//CPU Temperature
	//fp = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
	//fscanf(fp, "%f", tmp);
	//info.temp=tmp;
	//printf("%s\n",info.temp);
	
}
