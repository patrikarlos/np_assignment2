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
/* You will to add includes here */


// Included to get the support library
#include "calcLib.c"


#include "protocol.h"

#define DEST_PORT 8000   
#define DSET_IP_ADDRESS  "127.0.0.1" 
#define BUFF_LEN 512

int main(int argc, char *argv[]){
  
  /* Do magic */
  /* socket文件描述符 */  
  int sock_fd; 

  /* 建立udp socket */  
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);  
  if(sock_fd < 0)  
  {  
    perror("socket create fail!");  
    exit(1);  
  }  

  /* 设置address */  
  struct sockaddr_in addr_serv;  
  int len;
  memset(&addr_serv, 0, sizeof(addr_serv));  
  addr_serv.sin_family = AF_INET;  
  addr_serv.sin_addr.s_addr = inet_addr(DSET_IP_ADDRESS);  
  addr_serv.sin_port = htons(DEST_PORT);  
  len = sizeof(addr_serv); 

  int send_num;  
  int recv_num;  
  char send_buf[BUFF_LEN];  
  char recv_buf[BUFF_LEN];
  struct calcMessage calcMessage1 = {22,0,17,1,0};

  //printf("client send: %s\n", send_buf);

  send_num = sendto(sock_fd, (char *)&calcMessage1, sizeof(calcMessage1)+1, 0, (struct sockaddr *)&addr_serv, len); 

  if(send_num < 0)  
  {  
    perror("sendto error:");  
    exit(1);  
  }  
  printf("udp connection build\n");

  struct calcProtocol calcProtocol;
  recv_num = recvfrom(sock_fd, (char*)&calcProtocol, sizeof(calcProtocol)+1, 0, (struct sockaddr *)&addr_serv, (socklen_t *)&len); 

  if(recv_num < 0)  
  {  
    perror("recvfrom error:");  
    exit(1);  
  }  

  switch (calcProtocol.arith)
  {
    case 1:
      printf("Task:\n%d add %d\n",calcProtocol.inValue1,calcProtocol.inValue2);
      break;
    case 2:
      printf("Task:\n%d sub %d\n",calcProtocol.inValue1,calcProtocol.inValue2);
      break;
    case 3:
      printf("Task:\n%d mul %d\n",calcProtocol.inValue1,calcProtocol.inValue2);
      break;
    case 4:
      printf("Task:\n%d div %d\n",calcProtocol.inValue1,calcProtocol.inValue2);
      break;
    case 5:
      printf("Task:\n%f fadd %f\n",calcProtocol.flValue1,calcProtocol.flValue2);
      break;
    case 6:
      printf("Task:\n%f fsub %f\n",calcProtocol.flValue1,calcProtocol.flValue2);
      break;
    case 7:
      printf("Task:\n%f fmul %f\n",calcProtocol.flValue1,calcProtocol.flValue2);
      break;
    case 8:
      printf("Task:\n%f fdiv %f\n",calcProtocol.flValue1,calcProtocol.flValue2);
      break;
  }
  

  recv_buf[recv_num] = '\0';  
  //printf("client receive %d bytes: %s\n", recv_num, recv_buf);  
    
  close(sock_fd);  
    
  return 0;  

}
