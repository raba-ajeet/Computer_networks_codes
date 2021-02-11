#include<stdio.h>
#include<sys/wait.h>  // for wait
#include<signal.h> // for signal
#include<sys/shm.h> // for shared memory 
#include<sys/ipc.h> // for ipc 
#include<sys/stat.h> // for fifo
#include<sys/stat.h> // for fifo
#include <sys/types.h> 
#include <unistd.h> 
#include<fcntl.h>  // for files reading
#include<poll.h> //  for poll
#include <sys/ipc.h> 
#include <sys/msg.h>  // message q 
#include <semaphore.h>
#include<pthread.h>
#include<stdlib.h>
#include<cstring>
#include<iostream>
using namespace std;
struct mesg_buffer{
    long mesg_type=1;
    char mesg_text[100]="hi";
};

mesg_buffer  message,message1;
key_t key;
int msgid;
int pid,cnt=0;
   int pids[2];

void* reader_handler(void *){
    while(1){
        msgrcv(msgid, &message1, sizeof(message1), pid, 0);
        printf("message recieved is from server: %s \n ",message1.mesg_text);

    }
}

void* writer_handler(void *){
    while(1){
        printf("write data : \n");
        cin>>message.mesg_text; // ,100,stdin);
        for(int i=0;i<cnt;i++){
            message.mesg_type=pids[i];
            msgsnd(msgid,&message,sizeof(message),0);
            printf("message send to %ld and is  : %s \n ",message.mesg_type,message.mesg_text);
        } 
         
    }
}

int main(){
    sem_t *s= sem_open("sem_s", O_CREAT |  O_RDWR , 0660, 0);
    sem_t *s1= sem_open("sem_s1", O_CREAT |  O_RDWR , 0660, 0);
    system("touch msg1");
    key=ftok("msg1",65);
    pid=getpid();
    printf("my pid is %d \n",pid);
    msgid=msgget(key,0666 | IPC_CREAT);
    printf("my msgid is %d \n",msgid);
    message.mesg_type=pid;
    msgsnd(msgid,&message,sizeof(message),0);
    msgsnd(msgid,&message,sizeof(message),0);
    sleep(10);
    while(cnt<2){
        // sleep(2);

        msgrcv(msgid, &message1, sizeof(message1), 0, 0);
        int ok=1;
        int currpid=message1.mesg_type; 
        if(currpid!=pid){
            int p=1;
            for(int i=0;i<cnt;i++){
                if(pids[i]==currpid) p=0;
            }               
            if(p){
                pids[cnt]=currpid;
                cnt++;
                ok=0;
            }
        }
        if(ok){
            mesg_buffer message2;
            strcpy(message2.mesg_text,message1.mesg_text);
            message2.mesg_type=message1.mesg_type;
            msgsnd(msgid, &message2, sizeof(message2), 0);
        }
    }
    sem_post(s);
    sem_wait(s1);
    printf("%d \n",pid); 
    for(int i=0;i<cnt;i++){
        printf("%d \n",pids[i]);
    }
    pthread_t reader,writer;
    pthread_create(&writer,NULL,writer_handler,NULL);
    pthread_create(&reader,NULL,reader_handler,NULL);
    pthread_join(writer,NULL);
    pthread_join(reader,NULL);
    return 0;
}