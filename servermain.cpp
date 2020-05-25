#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <errno.h> 
#include <netdb.h> 
#include <stdarg.h> 
#include <string.h> 
#include <sys/time.h>
/* You will to add includes here */


// Included to get the support library
#include "calcLib.c"

#include "protocol.h"
#define SERV_PORT   8000  
#define BUFF_LEN 512

using namespace std;

/* Needs to be global, to be rechable by callback and main */
int loopCount=0;
int terminate=0;


/* Call back function, will be called when the SIGALRM is raised when the timer expires. */
void checkJobbList(int signum){
  // As anybody can call the handler, its good coding to check the signal number that called it.

  printf("Let me be, I want to sleep.\n");

  if(loopCount>20){
    printf("I had enough.\n");
    terminate=1;
  }
  
  return;
}


int main(int argc, char *argv[]){
  
  /* Do more magic */
  /* sock_fd --- socket文件描述符 创建udp套接字*/  
  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock_fd < 0)  
  {  
    perror("socket");  
    exit(1);  
  }  
  
  /* 将套接字和IP、端口绑定 */  
  struct sockaddr_in addr_serv;  
  int len;  
  memset(&addr_serv, 0, sizeof(struct sockaddr_in));  //每个字节都用0填充
  addr_serv.sin_family = AF_INET;  //使用IPV4地址
  addr_serv.sin_port = htons(SERV_PORT);  //端口
  /* INADDR_ANY表示不管是哪个网卡接收到数据，只要目的端口是SERV_PORT，就会被该应用程序接收到 */  
  addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);  //自动获取IP地址
  len = sizeof(addr_serv);  
    
  /* 绑定socket */  
  if(bind(sock_fd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)  
  {  
    perror("bind error:");  
    exit(1);  
  }  
  
    
  int recv_num;  
  int send_num;  
  char send_buf[BUFF_LEN];  
  char recv_buf[BUFF_LEN];  
  struct sockaddr_in addr_client; 
  struct calcMessage calcMessage2; 
  
  while(1)  
  {  
    printf("server wait:\n");  
    sleep(2);
      
    recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);  
      
    if(recv_num < 0)  
    {  
      perror("recvfrom error:");  
      loopCount++;
      printf("LoopCount: %d\n", loopCount);
    }  
    else
    {
      memcpy(&calcMessage2, recv_buf, sizeof(calcMessage2)+1);
      printf("calcMessage from client:\n");
      printf("calcMessage type: %d, message: %d, protocal: %d, major_version: %d, minor_version: %d\n\n",
      calcMessage2.type,calcMessage2.message,calcMessage2.protocol,calcMessage2.major_version,calcMessage2.minor_version);
      // recv_buf[recv_num] = '\0';  
      // printf("server receive %d bytes: %s\n", recv_num, recv_buf);  

      // send_num = sendto(sock_fd, send_buf, recv_num, 0, (struct sockaddr *)&addr_client, len);  
        
      // if(send_num < 0)  
      // {  
      //   perror("sendto error:");  
      //   exit(1);  
      // }  
    }
  }  
    
  close(sock_fd);  
  
   /* 
     Prepare to setup a reoccurring event every 10s. If it_interval, or it_value is omitted, it will be a single alarm 10s after it has been set. 
  */
  struct itimerval alarmTime;
  alarmTime.it_interval.tv_sec=10;
  alarmTime.it_interval.tv_usec=10;
  alarmTime.it_value.tv_sec=10;
  alarmTime.it_value.tv_usec=10;

  /* Regiter a callback function, associated with the SIGALRM signal, which will be raised when the alarm goes of */
  signal(SIGALRM, checkJobbList);
  setitimer(ITIMER_REAL,&alarmTime,NULL); // Start/register the alarm. 

  
  while(terminate==0){
    printf("This is the main loop, %d time.\n",loopCount);
    sleep(1);
    loopCount++;
  }

  printf("done.\n");
  return 0;  
}
