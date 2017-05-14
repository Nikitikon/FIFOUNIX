//
//  main.c
//  Client2
//
//  Created by Nikita-Mac on 14.05.17.
//  Copyright © 2017 Nikita-Mac. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

#define BUFSIZE 255

int flag = 1;

int ServerPid;

void Timer (){
    sleep(20);
    flag = 0;
}

int main(int argc, const char * argv[]) {
    umask(0);
    char ServerName[] = "/Users/nikita-mac/Desktop/UNIX/KR2/fifo1";
    char Name[] = "/Users/nikita-mac/Desktop/UNIX/KR2/fifo1.2";
    int pid = getpid();
    char BufPid[BUFSIZ];
    char buf[BUFSIZ];
    sprintf(BufPid,"%d", pid);
    unlink(Name);
    if (mknod(Name, S_IFIFO | 0777, 0) == -1){
        perror("mknod");
        exit(0);
    }
    
    int w_fifo = open(ServerName, O_WRONLY);
    
    if (w_fifo == -1) {
        perror("open write");
        exit(0);
    }
    
    if (write(w_fifo, Name, BUFSIZ) == -1) {
        perror("write");
        exit(0);
    }
    
    close(w_fifo);
    
    int r_fifo = 0;
    
    if ((r_fifo = open(Name, O_RDWR)) == -1) {
        perror("open read");
        exit(0);
    }
    if (read(r_fifo, BufPid, BUFSIZ) == -1) {
        perror("read pid");
        printf("%d", errno);
        exit(0);
    }
    
    ServerPid = atoi(BufPid);
    int count = 0;
    pthread_t _pthread;
    pthread_create(&_pthread, NULL, Timer, NULL);
    while (flag) {
        long c = 0;
        if ((c = read(r_fifo, buf, 1)) == -1) {
            if (!flag) {
                break;
            }
            perror("read");
            exit(0);
        }
        else
            count += c;
    }
//    kill(ServerPid, SIGUSR1);
    printf("%d\n", count);
    close(r_fifo);
    unlink(Name);
    return 0;
}
