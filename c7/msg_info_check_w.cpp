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
        // message.mesg_type=i+1;
        printf("write data : ");
        fgets(message.mesg_text,100,stdin); 
        msgsnd(msgid,&message,sizeof(message),0);
        printf("message send is : %s \n",message.mesg_text);
    }
    return 0;
}