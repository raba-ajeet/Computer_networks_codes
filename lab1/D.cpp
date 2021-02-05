#include<iostream>
#include<stdio.h>
#include<sys/wait.h>  // for wait
#include<signal.h> // for signal
#include<sys/shm.h> // for shared memory 
#include<sys/ipc.h> // for ipc 
#include<sys/stat.h> // for fifo
// #include<sys/stat.h> // for fifo
#include <sys/types.h> 
#include <unistd.h> 
#include<fcntl.h>  // for files reading
using namespace std;
int val=0;
void handler(int signum){
    val=1;
    printf("interuppt happened");
}
int main(){
    signal(SIGINT,handler);
    char * myfifo="/tmp/myfifo";
    mkfifo(myfifo,0666);
    int pid;
     int dfd=open(myfifo,O_RDONLY);
     read(dfd,&pid,1024);
    close(dfd);
    while(1){
        // sleep(5);
        if(val) {
            kill(pid,SIGUSR1);
            val=0;
        }
    }
    return 0;
}