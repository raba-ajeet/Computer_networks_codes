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
int val=1;
void handler(int signum){
    // change the flow 
    printf("signal is coming from D \n");
    val=1-val;
}

int main(){

    FILE* f1=popen("./A","w");
    FILE* f2=popen("./B","w");
    int ffd1=fileno(f1);
    int ffd2=fileno(f2);
    // key_t key1=ftok("/tmp/killcounter",65);
    // int shmid1=shmget(key1,1024,0666|IPC_CREAT);
    // int * killcounter= (int*)shmat(shmid1,(void*)0,0);
    int pp1[2],pp2[2];
    pipe(pp1);
    pipe(pp2);
    int c1=fork();
    if(c1>0){
        int c2=fork();
        if(c2>0){
            struct pollfd pfds[2];
            signal(SIGUSR1,handler);
                int pid=getpid();
                char * myfifo="/tmp/myfifo";
                mkfifo(myfifo,0666);
                int dfd=open(myfifo,O_WRONLY);
                write(dfd,&pid,1024);
                close(dfd);
            // parent
            char buf1[100];
            char * myfifo1="/tmp/myfifo1";
            char * myfifo2="/tmp/myfifo2";
            mkfifo(myfifo1,0666);
            mkfifo(myfifo2,0666);
            int fd1=open(myfifo1,O_RDONLY);
            int fd2=open(myfifo2,O_RDONLY);
            pfds[0].fd=fd1;
            pfds[1].fd=fd2;
            pfds[0].events=POLLIN;
            pfds[1].events=POLLIN;
            // pfds[0].revents=0;
            // pfds[1].revents=0;
            while(1){
                poll(pfds,2,500);
                for(int i=0;i<2;i++){
                    if(pfds[i].revents & POLLIN) {
                        printf("msg coming from P %d \n" ,i);
                        read(pfds[i].fd,buf1,10);
                        printf("parent printing : %s \n",buf1);
                        fflush(stdout);
                        if(val){
                            write(ffd1,buf1,sizeof(buf1));
                            write(ffd2,buf1,sizeof(buf1));
                        }
                        else{
                            write(pp1[1],buf1,sizeof(buf1));
                            write(pp2[1],buf1,sizeof(buf1));
                        }
                        // pfds[i].revents=0;
                    }
                }
                
            }
            wait(NULL);
        }
        else{
            // 2nd child

            char buf1[100];
            while(1){
                read(pp1[0],buf1,sizeof(buf1));
                printf("2nd child : %s \n",buf1);
                fflush(stdout);

            }
        }
    }
    else {
        // 1st child 
            char buf1[100];
        while(1){
            read(pp2[0],buf1,sizeof(buf1));
            printf("1st child : %s \n",buf1);
                fflush(stdout);

        }
    }
}