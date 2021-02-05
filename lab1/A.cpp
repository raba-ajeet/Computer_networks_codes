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
#include<poll.h> //  for poll
using namespace std;

int var=0;
void handler_cnt(int signum){
    printf("A: start the reading\n");
    var=1; 
}

void handler_stp(int signum){
    printf("A: stop the reading\n");
    var=0;  
}

int main(){
    char  buf[100];
    // signal(SIGSTOP,SIG_DFL);
    signal(SIGCONT,handler_cnt);
    // char * myfifo1="/tmp/myfifoA";
    // mkfifo(myfifo1,0666);
    // int fd1=open(myfifo1,O_WRONLY);
    // int c=getpid();
    // write(fd1,&c,1024);
        cout<<"A process: "<<getpid()<<" "<<var<<"\n";
        sleep(5);
    while(1){
        if(var==1){
            int x=read(0,buf,sizeof(buf));
           if(x){
            cout<<"A process: "<<buf<<"\n";
            fflush(stdout);
           }
        }
        else{
            
        }
    }
    while(1);

}