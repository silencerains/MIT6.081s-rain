#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2];
  char s[10];

  if(argc > 1) {
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }

  pipe(p1);
  pipe(p2);
  if(fork() == 0) {
    if(read(p1[0], s, sizeof(s))) {
      fprintf(1, "%d: received %s\n", getpid(),s);
        write(p2[1], "pong", 5);
        exit(0);
    }
  } else {
    write(p1[1], "ping", 5);
    if(read(p2[0], s, 5)) {
      fprintf(1, "%d: received %s\n", getpid(),s);
    }
  }

  exit(0);
}

//int main(int argc, char* argv[])
//{
//  int p[2];
//  char buf[8];
//  pipe(p);
//  int pid = fork();
//  
//  if(argc > 1) {
//    fprintf(2, "Usage: pingpong\n");
//    exit(1);
//  }
//
//  if(pid == 0) {
//    if(read(p[0],buf,sizeof(buf))) {
//      fprintf(1,"%d: receive %s\n", getpid(),buf);
//      write(p[1],"pong",5);
//      exit(0);
//    }
//  } 
//  else {
//    write(p[1],"ping",5);
//    wait(0);
//    if(read(p[0],buf,sizeof(buf)))   
//      fprintf(1,"%d: receive %s\n", getpid(),buf);
//  }
//  exit(0);
//}
