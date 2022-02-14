#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <string>
//#include <thread>

#define PORT 60022

typedef struct {
    int sock; // socket
    struct sockaddr address; // address of client
    unsigned int addr_len;   // length of address
} connection_t;

std::string findLength(char * fileName);
char * readFile(char *fileName, int&sock);
void * Process(void * ptr);


int main() {
    int sock = -1;
    struct sockaddr_in address;
    connection_t * connection;
    pthread_t thread;
    // create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0) {
        std::cerr << "Error cannot create socket.\n";
        return -1;
    } 

    // bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(sock, (struct sockaddr*)&address,sizeof(struct sockaddr_in)) < 0) {
        std::cerr << "Error: cannot bind socket to port.\n";
        return -2;
    }

    // listen to port

    if (listen(sock,5) < 0) {
        std::cerr << "Error: Cannot listen on port.\n";
        return -3;
    }

    std::cout << "Ready and listening.\n\n";

    while (true) {
        // accept connection
        connection = (connection_t *)malloc(sizeof(connection_t));
        connection->sock = accept(sock, &connection->address, &connection->addr_len);
        if (connection->sock <= 0) {
            free(connection);
        } else {
            std::cout << "Created thread.\n";
            pthread_create(&thread,0,Process, (void*)connection);
            pthread_detach(thread);
        }
    }
}

char * readFile(char * fileName,int &sock) {
    //std::ifstream inFile("hello-there.jpg", std::ifstream::in);
    char * fileBuffer = new char[4096];
    int fd1 = open(fileName, O_RDONLY, 0); 

    // new
    memset(fileBuffer,0,4096);

    int bytesRecv = 0;

    std::string file_len = findLength(fileName);

    char * charLength = new char[file_len.size()+1];
    // send length
    send(sock, charLength,sizeof(charLength), 0);
    // /r/nr/n
    send(sock, "\r\n\r\n", sizeof("\r\n\r\n"), 0);
    do {
        memset(fileBuffer,0,4096);
        bytesRecv = read(fd1, fileBuffer, 4096);
        send(sock, fileBuffer, strlen(fileBuffer), 0);
    } while (bytesRecv > 0);


    if (fd1 == -1) {
        std::cout << "This isn't how you read file.\n";
        
    } else {
        std::cout << "Okay... opened file\n";
    }

    //file_len = bytesRecv;
    std::cout << "Test: " << file_len << std::endl;
    //std::cout << fileBuffer;
    return fileBuffer;
}

std::string findLength(char * fileName) {
    char * fileBuffer = new char[4096];
    int fd1 = open(fileName, O_RDONLY, 0); 
    int file_len = 0;
    std::string stringLength;
    // new
    memset(fileBuffer,0,4096);

    int bytesRecv = 0;
    
    

    do {
        memset(fileBuffer,0,4096);
        bytesRecv = read(fd1, fileBuffer, 4096);
        std::cout << fileBuffer;
        file_len += bytesRecv;
    } while (bytesRecv > 0);

    stringLength = std::to_string(file_len);

    std::cout << "Test: " << stringLength << std::endl;
    //std::cout << fileBuffer;
    return stringLength;
}

void * Process(void * ptr) {
    char * buffer = new char[4096];
    int len;
// read file test
    char * fileName = "test2.html";

    connection_t * conn;
    char * hello = "Hellot there\n";
    char * response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-length: ";
    char * test = "<h1>Hello World</h1>";

    
    //sprintf(some_len, "%d",length);
    //strcpy(response,some_len);
    //strcat(Sresponse,ending);
    //const char * response = Sresponse.c_str();
    //response.append(ending);

    if (!ptr) {
        pthread_exit(0);
    }
    
    conn = (connection_t *)ptr;
    // put in loop
 

    // read message length
    //read(conn->sock,&len,sizeof(int)); 
    if (1 > 0) {
        // allocate buffer memory
        //buffer = (char*)malloc((len+1)*sizeof(char));
        //buffer[len] = 0;
        // read length
        memset(buffer,0,4096);
        int bytesRecv = read(conn->sock, buffer, 4096);
        //read(conn->sock, buffer, len);

        printf("%s\n",buffer);
        //std::cout << buffer << buffer;

        std::cout << "Trying to send.\n";
        // header
        send(conn->sock , response , strlen(response) , 0 );
        // length
        
        
        // read file and send
        readFile(fileName, conn->sock);
        
        /* if (send(conn->sock, fileBuffer , html_len , 0 ) == -1) {
            std::cout << "Gahhh, you did it wrong.\n";
        } */
        
        free(buffer);
    }
    //is.close();
    //close(fd1);
    //delete[] fileBuffer;
    close(conn->sock);
    free(conn);
    pthread_exit(0);
}