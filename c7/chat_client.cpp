#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include<unistd.h>
struct mesg_buffer{
    long mesg_type=1;
    char mesg_text[100];
} message;

int main(){
    key_t key1,key2;
    int msgid1,msgid2;
    key1=ftok("msgq_testing",65);
    key2=ftok("msgq_server",65);
    msgid1=msgget(key1,0666 | IPC_CREAT);
    msgid2=msgget(key2,0666 | IPC_CREAT);
    fgets(message.mesg_text,100,stdin); 
    int pidProcess=getpid();
    message.mesg_type=pidProcess;
    msgsnd(msgid1,&message,sizeof(message),0);
    struct msqid_ds buf;
    msgctl(msgid1,IPC_RMID,&buf);
    // for(int i=0;i<3;i++){
    //     msgrcv(msgid1, &message, sizeof(message), 0, 0); 
    //     printf("message recieved is : %s \n",message.mesg_text);
    // }
    for(int i=0;i<3;i++){
        printf("write data : ");
        fgets(message.mesg_text,100,stdin); 
        message.mesg_type=pidProcess;
        msgsnd(msgid1,&message,sizeof(message),0);
        printf("message send to server and is  : %s \n",message.mesg_text);
    }
    return 0;
}