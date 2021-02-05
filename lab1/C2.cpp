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


int main(){
    char buf[100];
    char * myfifo2="/tmp/myfifo2";
    mkfifo(myfifo2,0666);
    int fd1=open(myfifo2,O_WRONLY);
    while(1){
        scanf("%s",buf);
        write(fd1,buf,10);
    }
            
}