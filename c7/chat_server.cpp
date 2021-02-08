#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

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
    int n; 
    scanf("%d",n);
    int pids[n];
    for(int i=0;i<n;i++){
        msgrcv(msgid1, &message, sizeof(message), 0, 0); 
        pids[i]=message.mesg_type;
        printf("message recieved is :%d and  %s \n",message.mesg_type,message.mesg_text);
    }
    while(1){
        msgrcv(msgid1, &message, sizeof(message), 0, 0); 
        for(int i=0;i<n;i++){
            if(message.mesg_type!=pids[i]){
                msgsnd(msgqi);
            }
        }
    }
    // for(int i=0;i<3;i++){
    //     printf("write data : ");
    //     fgets(message.mesg_text,100,stdin); 
    //     if(message.mesg_text[1]=='2'){
    //         message.mesg_type=2;
    //     }
    //     msgsnd(msgid1,&message,sizeof(message),0);
    //     printf("message send to %d and is  : %s \n",message.mesg_type,message.mesg_text);
    // }
    return 0;
}