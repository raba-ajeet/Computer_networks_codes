#include<stdio.h> 
#include<sys/ipc.h> 
#include<signal.h>
#include<time.h>
#include<sys/msg.h> 
#include<sys/ipc.h> 
#include<string.h>
#include<unistd.h>
struct mesg_buffer{
    long mesg_type;
    char mesg_text[100];
} message;

int main(){
    key_t key;
    int msgid;
    key=ftok("msgq_testing",65);
    msgid=msgget(key,0666 | IPC_CREAT);
    struct msqid_ds buf;
    msgctl(msgid,IPC_STAT,&buf);
    printf("buf.msg_qnum : %d\n",buf.msg_qnum);
	printf("buf.msg_lspid %d\n",buf.msg_lspid);
	printf("buf.msg_lrpid %d\n",buf.msg_lrpid);
	printf("buf.msg_stime %s",ctime(&buf.msg_stime));
	printf("buf.msg_rtime %s",ctime(&buf.msg_rtime));
	printf("buf.msg_ctime %s",ctime(&buf.msg_ctime));
	printf("buf.msg_qbytes %d\n",buf.msg_qbytes);
    msgctl(msgid,IPC_RMID,NULL);
    return 0;
}