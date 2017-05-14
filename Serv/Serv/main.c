//
//  main.c
//  Serv
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
#include <string.h>

#define BUFSIZE 255

int flag = 1;

void EndCircl (int sig){
    flag = 0;
}


int main(int argc, const char * argv[]) {

    umask(0);
    char name[] = "/Users/nikita-mac/Desktop/UNIX/KR2/fifo1";
    char ClientName[BUFSIZ];
    unlink(name);
    if (mknod(name, S_IFIFO | 0777, 0) == -1){
        perror("mknod");
        exit(0);
    }
    
    int i = 0;
    
    while (1) {
        int r_fifo;
        char ClientPidBuf[BUFSIZ];
        r_fifo = open(name, O_RDONLY);
        
        if (r_fifo == -1) {
            perror("open read");
            exit(0);
        }
        
        if (read(r_fifo, ClientName, BUFSIZ) == -1) {
            perror("read client name");
            exit(0);
        }
        
        if (read(r_fifo, ClientPidBuf, BUFSIZ) == -1) {
            perror("read client name");
            exit(0);
        }
        
        close(r_fifo);
        
        if (fork() == 0) {
            signal(SIGUSR1, EndCircl);
            int w_fifo;
            int which = PRIO_PROCESS;
            int pid = getpid();
            char PidBuf[BUFSIZ];
            char CN[BUFSIZ];
            strncpy(CN, ClientName,BUFSIZ);
            sprintf(PidBuf,"%d", pid);
            
            if ((w_fifo = open(CN, O_RDWR)) == -1) {
                perror("open write");
                exit(0);
            }
                
            if (write(w_fifo, PidBuf, BUFSIZ) == -1) {
                perror("write pid");
                exit(0);
            }
            int count = 0;
            while (flag) {
                char simbol = (char)(rand() % 128);
                if (write(w_fifo, &simbol, 1) == -1){
                    perror("write simbol");
                    exit(0);
                }
                if (rand() % 100 > 80 )
                    setpriority( which, pid, rand()%11);
                count++;
            }
            printf("%d\n", count);
            close(w_fifo);
            exit(0);
        }
        
        i++;
        printf("%d\n", i);
    }
    
    return 0;
}
