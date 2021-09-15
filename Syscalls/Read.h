#ifndef READ_H
#define READ_H
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>

int read(int fd, const void *buf, int bytes) {
    fd = open(buf,bytes);
    if(fd == -1) {
       return 1;
    } else {
        mmap(NULL,bytes,PROT_READ,MAP_SHARED,fd,0);
    }
    return 0;
}
#endif // READ_H
