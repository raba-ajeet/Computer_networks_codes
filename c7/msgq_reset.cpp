#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

struct mesg_buffer{
    long mesg_type;
    char mesg_text[100];
} message;
key_t key1,key2;
int msgid1,msgid2;
int main(){
    key_t key;
    int msgid;
     key1=ftok("msg_testing.txt",65);
    key2=ftok("msg_server.txt",64);
    perror("ftok");// likhde ftok to bad
    printf("%d , %d \n",key1,key2);

    msgid1=msgget(key1,0666 | IPC_CREAT);
    msgid2=msgget(key2,0666 | IPC_CREAT);
    msgctl(msgid1,IPC_RMID,NULL);
    msgctl(msgid2,IPC_RMID,NULL);
    return 0;
}