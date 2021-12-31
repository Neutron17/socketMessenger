#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <getopt.h>

void parseArgs(int argc, char *argv[]);

int max_conns = 3;
int port = 8080;
char *text;
bool isDebug = false;
int buff_sz = 1024;

int main(int argc, char *argv[]) {
    parseArgs(argc, argv);
    int sockFD, clientFD;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if((sockFD = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Error: socket()\n");
        return EXIT_FAILURE;
    }
    int opt = 1;
    if (setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR | 0, 
                                                  &opt, sizeof(opt))) { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(port); 
    if(bind(sockFD, (struct sockaddr *) &address, sizeof(address)) < 0) {
        fprintf(stderr, "Error: bind()\n");
        return EXIT_FAILURE;
    }
    if(listen(sockFD, max_conns) < 0) {
        fprintf(stderr, "Error: listen()\n");
        return EXIT_FAILURE;
    }
    if(isDebug) printf("debug: allocating buffer with size: %d\n", buff_sz);
    char *buff = (char *)calloc(buff_sz, sizeof(char));
    while(1) {
        if((clientFD = accept(sockFD, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            fprintf(stderr, "Error: accept()\n");
            return EXIT_FAILURE;
        }
        read(clientFD, buff, buff_sz);
        printf("%s\n", buff);
        send(clientFD, text, strlen(text), 0);
        close(clientFD);
    }
    if(isDebug) printf("debug: Finished, cleaning up resources\n");
    free(buff);
    close(sockFD);
    return EXIT_SUCCESS;
}

void parseArgs(int argc, char *argv[]) {
    int option;
    while((option = getopt(argc, argv, "p:r:t:dvs:")) != -1) {
        switch(option) {
            case 'd':
            case 'v': {
                isDebug = true;
                break;
            }
            case 'p': {
                port = atoi(optarg);
                break;
            }
            case 'r':
            case 't': {
                text = optarg;
                break;
            }
            case 's': {
                buff_sz = atoi("optarg");
                break;
            }
            case '?': {
                if(optopt == 'r' || optopt == 't') {
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
    assert(text != NULL);
}
