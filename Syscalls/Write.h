#ifndef WRITE_H
#define WRITE_H
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>

int write(int fd, const void *buf, int bytes) {
    int fd = open(buf,bytes);
    if(fd == -1) {
       return 1;
    } else {
        mmap(NULL,bytes,PROT_WRITE,MAP_SHARED,fd,0);
    }
    return 0;
}

#endif // WRITE_H