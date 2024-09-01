#include "kernel/types.h"
#include "user/user.h"

#define LEN 36

int seive(int pipe_read) {
  int p[2];
  pipe(p);
  int r_buf = 0, r_size = 0, first = 0;
  r_size = read(pipe_read, &r_buf, sizeof(int));
  if(r_size > 0) {
    first = r_buf;
    printf("prime %d\n",first);
  } else {
    exit(0);
  }
  int pid = fork();
  if(pid == 0) {
    close(p[1]);
    seive(p[0]);
  } else {
    close(p[0]);
    while(r_size > 0){
      if(r_buf % first){
        write(p[1],&r_buf,sizeof(int));
      }
      r_size = read(pipe_read,&r_buf,sizeof(int));
    }
    close(pipe_read);
    close(p[1]);
    wait(0);
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  if(argc != 1) {
    printf("usage: No need argument\n");
    exit(0);
  }
  int p[2];
  pipe(p);
  int pid = fork();
  if(pid == 0){
    close(p[1]);
    seive(p[0]);
  } else {
    close(p[0]);
    printf("prime 2\n");
    for(int i = 2; i < LEN; ++i){
      if(i % 2){
        write(p[1],&i,sizeof(int));
      }
    }
    close(p[1]);
    wait(0);
  }
  exit(0);
}
