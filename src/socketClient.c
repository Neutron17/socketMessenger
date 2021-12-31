#include <stdio.h> 
#include <unistd.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <arpa/inet.h>
#include <getopt.h>

int port = 8080;
unsigned buff_sz = 1024;
bool isDebug = false;
char *message = NULL; 
char *ip = "127.0.0.1";

void parseArgs(int argc, char *argv[]);

int main(int argc, char *argv[]) { 
    parseArgs(argc, argv);
    struct sockaddr_in address; 
    int sock = 0; 
    char *buffer = (char *) calloc(buff_sz, sizeof(char)); 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("Socket creation error"); 
        return -1; 
    } 
   
    memset(&address, '0', sizeof(address)); 
   
    address.sin_family = AF_INET; 
    address.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, ip, &address.sin_addr)<=0) { 
        printf("Invalid address/ Address not supported"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *) &address, sizeof(address)) < 0) { 
        printf("Connection Failed"); 
        return -1; 
    } 
    send(sock, message, strlen(message), 0 ); 
    printf("message sent\n"); 
    read(sock, buffer, buff_sz); 
    printf("%s\n", buffer);
    if(isDebug) printf("debug: Finished, cleaning up resources\n");
    close(sock);
    free(buffer);
    return 0; 
} 

void parseArgs(int argc, char *argv[]) {
    int option;
    while((option = getopt(argc, argv, "i:p:m:t:dvs:")) != -1) {
        switch(option) {
            case 'i': {
                ip = optarg;
            }
            case 'd':
            case 'v': {
                isDebug = true;
                break;
            }
            case 'p': {
                port = atoi(optarg);
                break;
            }
            case 'm':
            case 't': {
                message = optarg;
                break;
            }
            case 's': {
                buff_sz = atoi("optarg");
                break;
            }
            case '?': {
                if(optopt == 'm' || optopt == 't' || optopt == 'i' || optopt == 's') {
                    fprintf(stderr, "%c option requires argument\n", optopt);
                    abort();
                }else if(isprint(optopt)) {
                    fprintf(stderr, "Unknown option '-%c'\n", optopt);
                }else{
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt
                    );
                }
                break;
            }
            default: abort();
        }
    }
    assert(message != NULL);
}
