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

int val=0;
int p1=0,p2=0;

void handler(int signum){
    // change the flow 
    printf("signal is coming from D \n");
    val++;
    printf("the kill command count is %d \n",val);
}

void handler_p1_cnt(int signum){
    printf("p1: start the reading\n");
    p1=1; 
}

void handler_p1_stp(int signum){
    printf("p1: stop the reading\n");
    p1=0;  
}

void handler_p2_cnt(int signum){
    printf("p2: start the reading\n");
    p2=1; 
}

void handler_p2_stp(int signum){
    printf("p2: stop the reading\n");
    p2=0;  
}
int main(){
    int pids[2];
    // FILE* f1=popen("./A","w");
    // int ffd1=fileno(f1);
    // FILE* f2=popen("./B","w");
    // int ffd2=fileno(f2);
    int pp1[2],pp2[2];
    pipe(pp1);
    pipe(pp2);
    int c1=fork();
    if(c1>0){
        pids[0]=c1;
        int c2=fork();
        if(c2>0){
            pids[1]=c2;
            setpgid(pids[0],pids[0]);
            setpgid(pids[1],pids[0]);
            signal(SIGUSR1,handler);
            struct pollfd pfds[2];
            int pidOfProcess=getpid();
            char * myfifo="/tmp/myfifo";
            mkfifo(myfifo,0666);
            int dfd=open(myfifo,O_WRONLY);
            write(dfd,&pidOfProcess,sizeof(pidOfProcess));
            close(dfd);
            // parent
            // char * myfifoA="/tmp/myfifoA";
            // char * myfifoB="/tmp/myfifoB";
            // mkfifo(myfifoA,0666);
            // mkfifo(myfifoB,0666);
            // int fdA=open(myfifoA,O_RDONLY);
            // int fdB=open(myfifoB,O_RDONLY);
            // int pidA,pidB;
            // read(fdA,&pidA,1024);
            // close(fdA);
            // read(fdB,&pidB,1024);
            // close(fdB);
            // printf("pids of A and B  %d %d \n",pidA,pidB);
            int ppA[2],ppB[2];
            pipe(ppB);
            pipe(ppA);
            int ch1=fork();
            if(ch1==0){
                close(ppA[1]);
                dup2(ppA[0],0);
                execv("./A",NULL);
            // _exit(EXIT_FAILURE);
            }
            int ffdA=ch1;
            int ch2=fork();
            if(ch2==0){
                close(ppB[1]);
                dup2(ppB[0],0);
                execv("./B",NULL);
            }
            close(ppA[0]);
            close(ppB[0]);
            int ffdB=ch2;
            setpgid(ffdA,ffdA);
            setpgid(ffdB,ffdA);
            printf("A : %d , B : %d",ffdA,ffdB);
            printf("grpid of A %d , B is %d",getpgid(ffdA),getpgid(ffdB));
            ///   
            char buf1[100];
            char * myfifo2="/tmp/myfifo2";
            mkfifo(myfifo2,0666);
            int fd2=open(myfifo2,O_RDONLY);
            char * myfifo1="/tmp/myfifo1";
            mkfifo(myfifo1,0666);
            int fd1=open(myfifo1,O_RDONLY);
            pfds[0].fd=fd1;
            pfds[1].fd=fd2;
            pfds[0].events=POLLIN;
            pfds[1].events=POLLIN;
            while(1){
                poll(pfds,2,500);
                for(int i=0;i<2;i++){
                    if(pfds[i].revents & POLLIN) {
                        printf("msg coming from P %d \n" ,i);
                        fflush(stdout);
                        read(pfds[i].fd,buf1,1024);
                        printf("parent printing : %s \n",buf1);
                        fflush(stdout);
                        if(val%2==0){

                            printf("G1 writing \n");
                            killpg(getpgid(pids[0]),SIGCONT);
                            sleep(1);
                            killpg(getpgid(ffdA),SIGSTOP);
                            sleep(1);
                            write(pp1[1],buf1,sizeof(buf1));
                            sleep(1);
                            write(pp2[1],buf1,sizeof(buf1));
                            sleep(1);
                        }
                        else{
                            printf("G2 writing \n");
                            killpg(getpgid(ffdA),SIGCONT);
                            sleep(1);
                            killpg(getpgid(pids[0]),SIGSTOP);
                            sleep(1);
                            write(ppA[1],buf1,sizeof(buf1));
                            sleep(1);
                            write(ppB[1],buf1,sizeof(buf1));
                            sleep(1);
                        }
                        // pfds[i].revents=0;
                    }
                }
                
            }
            wait(NULL);
        }
        else{
            // 2nd child
            // signal(SIGSTOP,SIG_DFL);
            signal(SIGCONT,handler_p2_cnt);
            char buf1[100];
            while(1){
                if(p2){
                    read(pp1[0],buf1,sizeof(buf1));
                    printf("2nd child : %s \n",buf1);
                    fflush(stdout);
                }

            }
        }
    }
    else {
        // 1st child 
        // signal(SIGSTOP,SIG_DFL);
        signal(SIGCONT,handler_p1_cnt);
        char buf1[100];
        while(1){
            if(p1){
                read(pp2[0],buf1,sizeof(buf1));
                printf("1st child : %s \n",buf1);
                fflush(stdout);
            }

        }
    }
}