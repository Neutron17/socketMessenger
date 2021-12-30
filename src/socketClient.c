/*
 * =====================================================================================
 *
 *       Filename:  csockclient.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2021-12-29 19:42:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Neutron17, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
   
int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = argv[1]; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("Socket creation error"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1"/* "192.168.1.165" */, &serv_addr.sin_addr)<=0) { 
        printf("Invalid address/ Address not supported"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        printf("Connection Failed"); 
        return -1; 
    } 
    send(sock , hello , strlen(hello) , 0 ); 
    printf("message sent\n"); 
    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 
    return 0; 
} 
