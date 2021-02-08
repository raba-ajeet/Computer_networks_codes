#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

struct mesg_buffer{
    long mesg_type=1;
    char mesg_text[100];
} message;

int main(){
    key_t key;
    int msgid;
    key=ftok("msgq_testing",65);
    msgid=msgget(key,0666 | IPC_CREAT);
    for(int i=0;i<3;i++){
        printf("write data : ");
        fgets(message.mesg_text,100,stdin); 
        if(message.mesg_text[1]=='2'){
            message.mesg_type=2;
        }
        msgsnd(msgid,&message,sizeof(message),0);
        printf("message send to %d and is  : %s \n",message.mesg_type,message.mesg_text);
    }
    return 0;
}