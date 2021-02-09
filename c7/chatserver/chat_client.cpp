#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
struct mesg_buffer{
    long mesg_type;
    char mesg_text[100];
} message,message1,message2;

key_t key1,key2;
int msgid1,msgid2;
int pidProcess;
void* reader_handler(void *){
    while(1){
      msgrcv(msgid1, &message1, sizeof(message1), pidProcess, 0);
         printf("message recieved is from server: %s ",message1.mesg_text);

    }
}

void* writer_handler(void *){
    while(1){
        printf("write data : ");
        fgets(message2.mesg_text,100,stdin); 
        message2.mesg_type=pidProcess;
        msgsnd(msgid2,&message2,sizeof(message2),0);
        printf("message send to server and is  : %s ",message2.mesg_text);
        sleep(2);
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
    fgets(message.mesg_text,100,stdin); 
    pidProcess=getpid();
    message.mesg_type=pidProcess;
    msgsnd(msgid1,&message,sizeof(message),0);

    // threads for reading and writing 
    pthread_t reader,writer;
    pthread_create(&reader,NULL,reader_handler,NULL);
    pthread_create(&writer,NULL,writer_handler,NULL);
    pthread_join(reader,NULL);
    pthread_join(writer,NULL);
    return 0;
}