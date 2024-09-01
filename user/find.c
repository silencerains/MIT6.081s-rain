#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char*                                                                                                                                
fmtname(char *path)                                                                                                                  
{                                                                                                                                    
  static char buf[DIRSIZ+1];                                                                                                         
  char *p;                                                                                                                           
                                                                                                                                     
  // Find first character after last slash.                                                                                          
  for(p=path+strlen(path); p >= path && *p != '/'; p--)                                                                              
    ;                                                                                                                                
  p++;                                                                                                                               
                                                                                                                                     
  // Return blank-padded name.                                                                                                       
  if(strlen(p) >= DIRSIZ)                                                                                                            
    return p;                                                                                                                        
  memmove(buf, p, strlen(p)); 
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p)); 
  return buf;                                                                                                                        
}   

void eq_print(char *fileName, char *findName){
	if(strcmp(fmtname(fileName), findName) == 0){
		printf("%s\n", fileName);
	}
}

void find(char *path, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path,0)) < 0) {
    fprintf(2,"find: cannot open %s\n",path);
    close(fd);
    return ;
  } 
  if(fstat(fd,&st) < 0) {
    fprintf(2,"find: cannot stat %s\n",path);
    close(fd);
    return ;
  }

  switch(st.type) {
    case T_DEVICE:
    case T_FILE:
      eq_print(path,filename);
      break;
    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        //printf("de.name:%s, de.inum:%d\n", de.name, de.inum);
        if(de.inum == 0 || strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0) 
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0) {
          continue;
        }
        find(buf, filename);
      }
      break;
  }
  close(fd);
}


int
main(int argc, char *argv[])
{
  if(argc < 2 || argc > 3)
  {
    fprintf(2, "Usage: find [file_path] [file_name]\n");
    exit(1);
  }

  if(argc == 2)
    find(".", argv[1]);
  else
    find(argv[1], argv[2]);
  exit(0);
}

