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
int main(){
    sem_unlink("sem_s");
    sem_unlink("sem_s1");
    sem_unlink("sem_s2");
    sem_unlink("sem_s3");
    sem_t *s= sem_open("sem_s", O_CREAT |  O_RDWR , 0660, 0);
    sem_t *s1= sem_open("sem_s1", O_CREAT |  O_RDWR , 0660, 0);
    sem_t *s2= sem_open("sem_s2", O_CREAT |  O_RDWR, 0660, 0);
    sem_t *s3= sem_open("sem_s3", O_CREAT |  O_RDWR, 0660, 0);
    sem_wait(s);
    sem_wait(s);
    sem_wait(s);
    sem_post(s1);
    sem_post(s2);
    sem_post(s3);
    int x; 
   cin>>x;
    key_t key;
    int msgid;
    system("touch msg1");
    key=ftok("msg1",65);
    
    msgid=msgget(key, 0666 |IPC_CREAT );
    msgctl(msgid,IPC_RMID,NULL);
    return 0;

}