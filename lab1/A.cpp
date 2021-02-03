#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
using namespace std;

int main(){
    char  buf[100];
    while(1){
        read(0,buf,sizeof(buf));
        cout<<"A process: "<<buf<<"\n";
        fflush(stdout);
    }
    

}