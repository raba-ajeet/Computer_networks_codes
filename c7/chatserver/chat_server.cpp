#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<cstring>
struct mesg_buffer{
    long mesg_type;
    char mesg_text[100]="helow";
} message,message1;

key_t key1,key2;
int msgid1,msgid2;
int pidProcess;
int n; 
int pids[100];
void* reader_handler(void *){
    while(1){
         msgrcv(msgid2, &message, sizeof(message), 0, 0); 
        printf("  server  recieved message : %d ,  %s ",message.mesg_type,message.mesg_text);
        for(int i=0;i<n;i++){
            if(message.mesg_type!=pids[i]){
                int j=0;
                strncpy(message1.mesg_text, message.mesg_text, 18);
                message1.mesg_type=pids[i];
                msgsnd(msgid1,&message1,sizeof(message1),0);
                printf("message send to client %d and is  : %s ",pids[i],message.mesg_text);
            }
        }
    }
}



int main(){

    system("touch msg_testing.txt");
    system("touch msg_server.txt");
    key1=ftok("msg_testing.txt",65);
    key2=ftok("msg_server.txt",64);
    perror("ftok");// likhde ftok to bad
    printf("%d , %d \n",key1,key2);
    msgid1=msgget(key1,0666 | IPC_CREAT);
    msgid2=msgget(key2,0666 | IPC_CREAT);
    printf("%d , %d \n",msgid1,msgid2);
    
    scanf("%i",&n);
    // printf("fsfs \n");
    for(int i=0;i<n;i++){
        msgrcv(msgid1, &message, sizeof(message), 0, 0); 
        pids[i]=message.mesg_type;
        printf("message recieved is :%d and  %s \n",message.mesg_type,message.mesg_text);
    }
    printf("all clients got connected\n");
    pthread_t reader;
    pthread_create(&reader,NULL,reader_handler,NULL);
    pthread_join(reader,NULL);
    return 0;
}