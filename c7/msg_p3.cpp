#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

struct mesg_buffer{
    long mesg_type;
    char mesg_text[100];
} message;

int main(){
    key_t key;
    int msgid;
    key=ftok("msgq_testing",65);
    msgid=msgget(key,0666 | IPC_CREAT);
    for(int i=0;i<3;i++){
        msgrcv(msgid, &message, sizeof(message), 2, 0); 
        printf("message recieved is  : %s \n",message.mesg_text);
    }
    msgctl(msgid,IPC_RMID,NULL);
    return 0;
}