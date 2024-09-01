#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
  if(argc <= 1){
    fprintf(2,"usage: Need an argument\n");
    exit(1);
  }
  if(argc <= 2) {
    int i = atoi(argv[1]);
    sleep(i);
    printf("Nothing happends for a litter while\n");
    exit(0);
  } 
  else{
    fprintf(2,"uasge: Argument number out of bound!\n");
    exit(1);
  }
	
}
