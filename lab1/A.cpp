#include <iostream>
#include <stdio.h>
#include <sys/wait.h> // for wait
#include <signal.h>   // for signal
#include <sys/shm.h>  // for shared memory
#include <sys/ipc.h>  // for ipc
#include <sys/stat.h> // for fifo
// #include<sys/stat.h> // for fifo
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> // for files reading
#include <poll.h>  //  for poll
using namespace std;


int main()
{
    char buf[100];
    cout << "A process: " << getpid()  << "\n";
    sleep(5);
    while (1)
    {
        int x = read(0, buf, sizeof(buf));
        if (x)
        {
            cout << "A process: " << buf << "\n";
            fflush(stdout);
        }
    }
}