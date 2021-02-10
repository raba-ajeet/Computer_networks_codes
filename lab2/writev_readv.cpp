#include<stdio.h> // input and output
#include<sys/uio.h> // readv and writev
#include<cstring>  // for string func
#include <fcntl.h> // for files reading
#include <unistd.h>
int main(){
    char str0[10];
    char str1[10];
    struct iovec iov[2];
    ssize_t nwritten;
    iov[0].iov_base=str0;
    iov[0].iov_len=10;
    iov[1].iov_len=2;
    iov[1].iov_base=str1;
    // int fd=open("m.txt",1);
    // nwritten=writev(fd,iov,2);
    // if(nwritten) printf("data written\n");
    // close(fd);
    
    int fd=open("m.txt",0);
    ssize_t nread;
    nread=readv(0,iov,2);
    if(nread>0) printf("data read %d\n" ,nread);
    printf("data from string s0 %s \n",str0);
    printf("data from string s1 %s\n",str1);

}