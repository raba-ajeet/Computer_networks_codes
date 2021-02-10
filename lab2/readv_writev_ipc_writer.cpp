#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include<sys/uio.h> // readv and writev

using namespace std;

int main(){
    char  buf1[10];
    char  buf2[10];
    char *myfifo="/tmp/myfifo";
    mkfifo(myfifo,0666);
    int fd=open(myfifo,1);
    struct iovec iov[2];
    ssize_t nwritten;
    iov[0].iov_base=buf1;
    iov[0].iov_len=sizeof(buf1);
    iov[1].iov_len=sizeof(buf2);
    iov[1].iov_base=buf2;
    while(1){
        cin>>buf1;
        cin>>buf2;
        writev(fd,iov,2);
    }
    close(fd);
}