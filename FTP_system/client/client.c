#include "test.h"

int connectsock(struct sockaddr_in,char *);
void cmd_chose(struct sockaddr_in, char *);
void ftp_login(struct sockaddr_in, char *);
void cmd_help();
void cmd_quit();
void cmd_ls(struct sockaddr_in, char *);
void cmd_pwd(struct sockaddr_in, char *);
void cmd_get(struct sockaddr_in, char *);
void cmd_put(struct sockaddr_in, char *);
void cmd_del(struct sockaddr_in, char *);
void cmd_about(struct sockaddr_in, char *);
int main(int argc, char **argv) {
    int    sockfd, n,rec_len;
    char    recvline[4096], sendline[4096];
    char    buf[MAXLINE];

    struct sockaddr_in  servaddr;

    if(argc != 2){
        printf("usage: ./client <ipaddress>\n");
        exit(0);
    }

    // /*创建套接字*/
    //int sockfd;
    // sockfd = socket(AF_INET,SOCK_STREAM,0);//创建tcp套接字
    // if(sockfd >= 0){
    //     printf("create socket successful!\n");
    // }
    // else{
    //     printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
    // }

    /*套接字设置*/
    memset(&servaddr, 0, sizeof(servaddr));//将servaddr置为0
    servaddr.sin_family = AF_INET;   //网络协议设置为IPV4
    servaddr.sin_port = htons(8000);//端口设置
    
    int f= inet_pton(AF_INET, argv[1], &servaddr.sin_addr);//ip转换为
    if(f<=0){
        printf("inet_pton error for %s\n",argv[1]);
    }
    else{
        printf("inet_pton successful\n");
    }

    // /*进行连接*/
    // f = connect(sockfd, (struct sockaddr* )&servaddr, sizeof(servaddr));
    // if(f < 0) {
    //     printf("connect error for %s(errno: %d)\n", strerror(errno),errno);
    //     exit(-1);
    // }
    // else{
    //     printf("connect successful...\n");
    // }

    ftp_login(servaddr,sendline);
    
    while(1){
        printf("ftp>");

        memset(sendline,0,sizeof(sendline));

        fgets(sendline,sizeof(sendline),stdin);//获取输入的命令

        cmd_chose(servaddr,sendline);

        //printf("%s\n",sendline);

        //write(sockfd,sendline,strlen(sendline));

        printf("\n");
    }

}
void cmd_chose(struct sockaddr_in servaddr, char *sendline){
    // int l=sizeof(sendline);
    // sendline[l]='\0';

    sendline[strlen(sendline)-1]='\0';

    //printf("%s\n",sendline);
    // if(strcmp(sendline,"login") == 0){
    //     ftp_login(servaddr,sendline);
    // }

    if(strcmp(sendline,"help") == 0){
        //printf("%s\n",sendline);
        cmd_help();
    }
    else if(strcmp(sendline,"?") == 0){
        //printf("%s\n",sendline);
        cmd_help();
    }
    else if(strcmp(sendline,"quit") == 0){
        //printf("%s\n",sendline);
        cmd_quit();
    }
    else if(strcmp(sendline,"exit") == 0){
        //printf("%s\n",sendline);
        cmd_quit();
    }
    else if(strcmp(sendline,"ls") == 0){
        //printf("%s\n",sendline);
        cmd_ls(servaddr,sendline);
    }
    else if(strcmp(sendline,"pwd") == 0){
        //printf("%s\n",sendline);
        cmd_pwd(servaddr,sendline);

    }
    else if(strcmp(sendline,"cd") == 0){
        //printf("%s\n",sendline);

    }
    else if(strncmp(sendline,"put",3) == 0){
        //printf("%s\n",sendline);
        cmd_put(servaddr,sendline);

    }
    else if(strncmp(sendline,"get",3) == 0){
        cmd_get(servaddr,sendline);
    }
    else if(strcmp(sendline,"about") == 0){
        cmd_about(servaddr,sendline);
    }
    else if(strncmp(sendline,"del",3) == 0){
        cmd_del(servaddr,sendline);
    }
    else{
        cmd_quit();

    }
}
int connectsock(struct sockaddr_in servaddr,char *sendline){
    //printf("hello world\n");
    /*创建套接字*/
    int sockfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);//创建tcp套接字
    if(sockfd < 0){
        //printf("create socket successful!\n");
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
    }
    // else{
    //     printf("create socket successful!\n");
    //     printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
    // }

    /*进行连接*/
    int f = connect(sockfd, (struct sockaddr* )&servaddr, sizeof(servaddr));
    if(f < 0) {
        printf("connect error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }
    // else{
    //     printf("connect successful...\n");
    // }
    // char sendline[4096];
    // strcpy(sendline,"ls");
    return sockfd;
}
void ftp_login(struct sockaddr_in servaddr, char *sendline){
    
    int sockfd;
    char userid[N],pasw[N];

    sockfd = connectsock(servaddr,sendline);

    strcpy(userid,"login");


    int f=write(sockfd,userid,strlen(userid));
    if(f < 0) {
        printf("send userid error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }
    memset(userid,0,sizeof(userid));
    printf("ftp>请输入你的账号：");

    fgets(userid,sizeof(userid),stdin);

    userid[strlen(userid)-1]='\0';
    f=write(sockfd,userid,strlen(userid));
    if(f < 0) {
        printf("send userid error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }

    printf("\nftp>请输入你的密码：");

    fgets(pasw,sizeof(pasw),stdin);

    
    pasw[strlen(pasw)-1]='\0';
    // printf("%s\n",userid);
    // printf("%s\n",pasw);
    
    //printf("%s\n",userid);
    f=write(sockfd,pasw,strlen(pasw));
    if(f < 0) {
        printf("send pasw error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }
    
    f=read(sockfd,pasw,strlen(pasw));
    if(f < 0) {
        printf("send pasw error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }
    //printf("%s\n",pasw);

    if(pasw[0]=='Y'){
        printf("ftp>230 登录成功\n");
    }
    else{
        printf("ftp>登录失败，请重新登录\n");
        ftp_login(servaddr,sendline);
    }
}
void cmd_help(){
    printf("214\n");
    printf("---------help------:展示能够操作的指令\n\n");
    printf("----------?--------:展示能够操作的指令\n\n");
    printf("----------ls-------:显示服务器的文件目录\n\n");
    printf("----get+filename---:从服务器下载文件\n\n");
    printf("----put+filename---:向服务器上传文件\n\n");
    printf("--------exit-------:退出程序\n\n");
    printf("--------quit-------:退出程序\n\n");
}
void cmd_quit(){
    printf("221");
    exit(0);
}
void cmd_ls(struct sockaddr_in servaddr, char *sendline){
     
    int sockfd;
    sockfd=connectsock(servaddr,sendline);

    int f=write(sockfd,sendline,strlen(sendline));
    if(f < 0) {
        printf("501send error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }
    printf("ftp>212");
    if(read(sockfd, sendline, MAXLINE) > 0)  //从sockfd中读取N字节内容放入commd中，
    {                                   //返回值为读取的字节数
        printf("%s ",sendline);
    }
    printf("\n");

    close(sockfd);
}
void cmd_pwd(struct sockaddr_in servaddr, char *sendline){

    int sockfd;
    sockfd=connectsock(servaddr,sendline);
    int f;
    f=write(sockfd,sendline,strlen(sendline));
    if(f < 0) {
        printf("501send error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }

    printf("ftp>212 ");
    // while( >= 0)  //从sockfd中读取N字节内容放入commd中，
    // {
        if(read(sockfd, sendline, MAXLINE) < 0){
            printf("receive error for %s(errno: %d)\n", strerror(errno),errno);
            exit(-1);
        }                               //返回值为读取的字节数
        printf("%s ",sendline);
    // }
    printf("\n");

    close(sockfd);
}
void cmd_get(struct sockaddr_in servaddr, char *sendline){

    int fd;
    int sockfd;
    char transfile[N];
    int nbytes;

    sockfd=connectsock(servaddr,sendline);

    //通过write函数向服务端发送数据
    if(write(sockfd, sendline, N) < 0)
    {
        printf("Write Error!At commd_get 1\n");
        exit(1);
    }
    //利用read函数来接受服务器发来的数据
    if(read(sockfd, transfile, N) < 0)
    {
        printf("Read Error!At commd_get 1\n");
        exit(1);
    }
    //用于检测服务器端文件是否打开成功
    if(transfile[0] =='N')
    {
        close(sockfd);
        printf("Can't Open The File!\n");
        return ;
    }
    //open函数创建一个文件，文件地址为(commd+4)，该地址从命令行输入获取
    if((fd=open(sendline+4, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0)
    {
        printf("Open Error!\n");
        exit(1);
    }
    //read函数从套接字中获取N字节数据放入buffer中，返回值为读取的字节数
    while((nbytes=read(sockfd, transfile, N)) > 0)
    {
        //write函数将buffer中的内容读取出来写入fd所指向的文件，返回值为实际写入的字节数
        if(write(fd, transfile, nbytes) < 0)
        {
            printf("Write Error!At commd_get 2");
        }
    }
    printf("ftp>226 ok get file successful\n");
    close(fd);
    close(sockfd);
}
void cmd_put(struct sockaddr_in servaddr, char *sendline){
    int fd;
    int sockfd;
    char transfile[N];
    int nbytes;

    sockfd=connectsock(servaddr,sendline);

    if(write(sockfd, sendline, N)<0)
    {
        printf("Wrtie Error!At commd_put 1\n");
        exit(1);
    }
    //open函数从(commd+4)中，读取文件路径，以只读的方式打开
    if((fd=open(sendline+4, O_RDONLY)) < 0)
    {
        printf("Open Error!\n");
        exit(1);
    }
    //从fd指向的文件中读取N个字节数据
    while((nbytes=read(fd, transfile, N)) > 0)
    {
        //从buffer中读取nbytes字节数据，写入套接字中
        if(write(sockfd, transfile, nbytes) < 0)
        {
            printf("Write Error!At commd_put 2");
        }
    }

    printf("226 ok put file successful\n");
    
    close(fd);
    close(sockfd);
}
void cmd_about(struct sockaddr_in servaddr, char *sendline){
    
    int sockfd;

    sockfd=connectsock(servaddr,sendline);
    int f=write(sockfd,sendline,strlen(sendline));
    if(f < 0) {
        printf("send userid error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }
    memset(sendline,0,sizeof(sendline));
    f=read(sockfd,sendline,strlen(sendline));
    if(f < 0) {
        printf("send userid error for %s(errno: %d)\n", strerror(errno),errno);
        exit(-1);
    }
    printf("ftp>Ubuntu 16.04.6 LTS amd64\n\n");
}

void cmd_del(struct sockaddr_in servaddr, char *sendline){
    int sockfd;
    sockfd=connectsock(servaddr,sendline);

    if(write(sockfd, sendline, N)<0)
    {
        printf("Wrtie Error!At commd_put 1\n");
        exit(1);
    }
    if(write(sockfd, sendline, N)<0)
    {
        printf("Wrtie Error!At commd_put 1\n");
        exit(1);
    }
    if(sendline[0]=='Y'){
        printf("226 ok delete file successful\n");
    }

}
