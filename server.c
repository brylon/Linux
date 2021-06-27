#include"test.h"

#define MAXLINE 4096 

void cmd_chose(char  *sendline,int sockfd);
void ftp_login(int sockfd);
void cmd_ls(int sockfd);
void cmd_get(int sockfd,char *filename);
void cmd_put(int sockfd,char *filename);
void cmd_pwd(int sockfd);
void cmd_about(int sockfd);

void cmd_del(int sockfd,char *filename);

int main(int argc, char** argv){
      int    listenfd, connfd;
      struct sockaddr_in     servaddr;
      char    buff[4096];
      int     n;
      pid_t pid;
      if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
          printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
          exit(0);
      }
      memset(&servaddr, 0, sizeof(servaddr));
      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
      servaddr.sin_port = htons(8000);
      if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
          printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
          exit(0);
      }
      if( listen(listenfd, 10) == -1){
          printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
          exit(0);
      }
      
      printf("220 ======waiting for client's request======\n");
     


      while(1){
        printf("server> ");
         if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
              printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
              //continue;
          }
         else{
              printf("client connect\n");
          }
        memset(buff,0,sizeof(buff));

        pid=fork();
        if(pid==0){

        
        if(read(connfd,buff,4096)<0){
           perror("connect:");
         }
         
         cmd_chose(buff,connfd);
        
        printf("recv msg from client: %s\n\n", buff);
        
        
        //close(connfd);
      //close(listenfd);
      }
      }
}
void cmd_chose(char *recvline,int sockfd){
    //sendline[sizeof(sendline)]= '\0';
    if(strcmp(recvline,"help") == 0){
      //printf("%s\n",recvline);
        //cmd_help();
    }
    else if(strcmp(recvline,"login") == 0){
      ftp_login(sockfd);
    }
    else if(strcmp(recvline,"?") == 0){
      //printf("%s\n",recvline);
        //cmd_help();
    }
    else if(strcmp(recvline,"about") == 0){
      //printf("%s\n",recvline);
        //cmd_help();
      cmd_about(sockfd);
    }
    else if(strcmp(recvline,"quit") == 0){
      //printf("%s\n",recvline);
        //cmd_quit();
    }
    else if(strcmp(recvline,"exit") == 0){
      //printf("%s\n",recvline);
        //cmd_quit();
    }
    else if(strcmp(recvline,"ls") == 0){
      //printf("%s\n",recvline);
        cmd_ls(sockfd);
    }
    else if(strcmp(recvline,"pwd") == 0){
      //printf("%s\n",recvline);
      cmd_pwd(sockfd);

    }
    else if(strcmp(recvline,"cd") == 0){
      //printf("%s\n",recvline);

    }
    else if(strncmp(recvline,"put",3) == 0){
      //printf("%s\n",recvline);
      cmd_put(sockfd,recvline+4);

    }
    else if(strncmp(recvline,"get",3) == 0){
        cmd_get(sockfd,recvline+4);
    }
    else if(strncmp(recvline,"del",3) == 0){
      cmd_del(sockfd,recvline+4);
    }
    else{
      //exit(0);
    }
}
void cmd_about(int sockfd){
  //printf("ftp>Ubuntu 16.04.6 LTS amd64\n");
  char sendline[MAXLINE];
  strcpy(sendline,"Ubuntu 16.04.6 LTS amd64");
  int f=write(sockfd,sendline,strlen(sendline));
    if(f < 0) {
        printf("send userid error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }
}
void ftp_login(int sockfd){
  char userid[N],pasw[N];
  int f;
  //printf("hello\n");
  f=read(sockfd,userid,sizeof(userid));
  if(f < 0) {
      printf("send pasw error for %s(errno: %d)\n", strerror(errno),errno);
      exit(-1);
  }
  //printf("%s\n",userid);
  f=read(sockfd,pasw,sizeof(pasw));
  if(f < 0) {
      printf("send pasw error for %s(errno: %d)\n", strerror(errno),errno);
      exit(-1);
  }
  //printf("%s\n",userid);
  //printf("%s\n",pasw);
  if(strcmp(userid,"admin")==0&&strcmp(pasw,"123456")==0){
    userid[0]='Y';
    f=write(sockfd,userid,sizeof(userid));
    if(f < 0) {
        printf("send userid error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }
  }
  else{
    userid[0]='N';
    f=write(sockfd,userid,strlen(userid));
    if(f < 0) {
        printf("send userid error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }
  }
  printf("server>230\n");
}
void cmd_ls(int sockfd){
  //printf("hello world\n");
  DIR * mydir =NULL;
  struct dirent *myitem = NULL;
  char recvline[MAXLINE] ;
  memset(recvline,0,sizeof(recvline));
  //opendir为用来打开参数name 指定的目录, 并返回DIR*形态的目录流
  //mydir中存有相关目录的信息
  if((mydir=opendir(".")) == NULL){
    printf("OpenDir Error!\n");
    exit(1);
  }
  char reword[1024];
  bzero(reword,1024);
  while((myitem = readdir(mydir)) != NULL){//用来读取目录,返回是dirent结构体指针
    if(sprintf(recvline, myitem->d_name, MAXLINE) < 0)//把文件名写入commd指向的缓冲区
        {
            printf("Sprintf Error!\n");
            exit(1);
        }
        strcat(reword,recvline);
        strcat(reword,"\n");
        
    }
    if(write(sockfd, reword, 1024) < 0 )//将commd缓冲区的内容发送会client
        {
            printf("Write Error!\n");
            exit(1);
        }
    printf("213\n");
    closedir(mydir);//关闭目录流
    close(sockfd);
}
void cmd_pwd(int sockfd){
  char recvline[MAXLINE]={0};
  //memset(recvline,0,sizeof(recvline));

  getcwd(recvline, sizeof(recvline));
  printf("212%s\n",recvline);
  // while(1){
    if(write(sockfd, recvline, MAXLINE) < 0 ){//将commd缓冲区的内容发送会client
      printf("Write Error!\n");
      exit(1);
    }
    // else{
    //   printf("hello \n");
    // }
  // }
}
void cmd_get(int sockfd,char *filename){
    //printf("hello\n");
    int fd, nbytes;
    char buffer[N];
    bzero(buffer, N);

    printf("get filename : [ %s ]\n",filename);
    if((fd=open(filename, O_RDONLY)) < 0)//以只读的方式打开client要下载的文件
    {
        printf("Open file Error!\n");
        buffer[0]='N';
        if(write(sockfd, buffer, N) <0)
        {
            printf("Write Error!At commd_get 1\n");
            exit(1);
        }
    }

    buffer[0] = 'Y';    //此处标示出文件读取成功
    if(write(sockfd, buffer, N) <0)
    {
        printf("Write Error! At commd_get 2!\n");
        close(fd);
        exit(1);
    }

    while((nbytes=read(fd, buffer, N)) > 0)//将文件内容读到buffer中
    {
        if(write(sockfd, buffer, nbytes) < 0)//将buffer发送回client
        {
            printf("Write Error! At commd_get 3!\n");
            close(fd);
            exit(1);
        }
    }
    printf("226\n");
    close(fd);
    close(sockfd);
}
void cmd_put(int sockfd,char *filename){
    int fd, nbytes;
    char buffer[N];
    bzero(buffer, N);

    printf("get filename : [ %s ]\n",filename);
    if((fd=open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0)//以只写的方式打开文件，若文件存在则清空，若文件不存在则新建文件
    {
        printf("Open file Error!\n");
        exit(0);
    }

    while((nbytes=read(sockfd, buffer, N)) > 0)//将client发送的文件写入buffer
    {
        if(write(fd, buffer, nbytes) < 0)//将buffer中的内容写到文件中
        {
            printf("Write Error! At commd_put 1!\n");
            close(fd);
            exit(1);
        }
    }
    printf("226\n");
    close(fd);
    close(sockfd);
}

void cmd_del(int sockfd,char *filename){
  char message[N];
  message[0] = 'Y';
  if (remove(filename) < 0)
        strcpy(message, "N");
  write(sockfd, message, N);
}

// void server_rm(int data_fd, int control_fd, char *command) {
//     char filename[N];
//     get_filename(filename, command + 3);

//     control_response(control_fd, START);

//     char message[N];
//     memset(message, 0, N);
//     if (remove(filename) < 0)
//         strcpy(message, "file does not exist.");
//     else
//         strcpy(message, "remove file successfully.");

//     printf("%s\n", message);

//     write(data_fd, message, N);

//     control_response(control_fd, END);
// }