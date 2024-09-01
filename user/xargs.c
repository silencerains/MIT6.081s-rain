#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char *get_line(){
  char *argv = malloc(MAXARG);
  char *pos = argv;
  while(read(0, pos, sizeof(char)) != 0){  
    if(*pos == '\0' || *pos == '\n')
    {
      *pos = '\0';
      return argv;
    }
    pos ++;
  }
  free(argv);
  return 0;

}


int main(int argc, char* argv[])
{
  if(argc < 2) {
    fprintf(2, "Usage: xargs [command]\n");
    exit(1);
  } 
  
  char *arg;
  char *temp_argv[MAXARG];
  char *cmd;
  cmd = argv[1];
  printf("cmd:%s\n",cmd);
  for(int i = 0; i < argc - 1; ++i)
    temp_argv[i] = argv[1 + i];
  
  while((arg = get_line()) != 0 ) {
    if(fork() == 0) {
      temp_argv[argc - 1] = arg;
      exec(cmd, temp_argv);
    }
    else {
      wait(0);
    }
  }
  exit(0);
}
