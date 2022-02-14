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
#include <cstring>
#include <sstream> // stringstream
//#include <thread>

#define PORT 60002
typedef struct {
    int sock; // socket
    struct sockaddr address; // address of client
    unsigned int addr_len;   // length of address
} connection_t;

std::string parseResponse(char * buffer); // get file requested
std::string findLength(char * fileName); // find length of buffer
std::string findLengthImg(char * fileName); // find length for image ???
void readFile(char *fileName, int&sock); // send html file -- rename
void * Process(void * ptr); // create socket
void chooseFile(std::string fileName); // pick what kind of file send, send that file
void sendImage(char * fileName,int &sock); // send image (doesn't work...)

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

void sendImage(char * fileName,int &sock) {
    char * fileBuffer = new char[4096];
    long long bytesRecv = 0;
    int fd1 = open(fileName, O_RDONLY, 0); 

    // clear buffer
    memset(fileBuffer,0,4096);

    std::string file_len = findLengthImg(fileName); // find length of image

    // convert string to cstring
    char * charLength = new char[file_len.size()+1];
    std::strcpy(charLength,file_len.c_str());
    
    // send length
    send(sock, charLength,sizeof(charLength), 0);
    // send line breaks
    send(sock, "\r\n\r\n", sizeof("\r\n\r\n"), 0);
    
    // check if file can be opened
    if (fd1 == -1) {
        std::cout << "This isn't how you read file.\n";
        
    } else {
        std::cout << "Okay... opened file\n";
    }

    // keep sending as much memory in file until all bytes are sent
    do {
        memset(fileBuffer,0,4096);
        bytesRecv = read(fd1, fileBuffer, 4096);
        send(sock, fileBuffer, strlen(fileBuffer), 0);
    } while (bytesRecv > 0);


    std::cout << "Length of image: " << file_len << std::endl;

    // deallocate char
    delete[] fileBuffer;
    delete[] charLength;
}


void readFile(char * fileName,int &sock) {
    char * fileBuffer = new char[4096];
    int bytesRecv = 0;
    // try to open file
    int fd1 = open(fileName, O_RDONLY, 0); 

    // new
    memset(fileBuffer,0,4096);

    // find length of message
    std::string file_len = findLength(fileName);
    // convert length to cstring
    char * charLength = new char[file_len.size()+1];
    std::strcpy(charLength,file_len.c_str());

    // check if file opened successfully
    if (fd1 == -1) {
        std::cout << "This isn't how you read file.\n";
        
    } else {
        std::cout << "Okay... opened file\n";
    }

    // send length of message
    send(sock, charLength,sizeof(charLength), 0);
    // send line breaks
    send(sock, "\r\n\r\n", sizeof("\r\n\r\n"), 0);

    // send as many bytes until all bytes are sent
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

    std::cout << "Length of HTML file:" << file_len << std::endl;

    // deallocate memory
    delete[] fileBuffer;
    delete[] charLength;
}

std::string parseResponse(char * buffer) {
    std::stringstream ss;
    std::string request;
    std::string fileName;

    // GET, POST etc..
    ss << buffer;
    ss >> request;
    // file name requested
    ss << buffer;
    ss >> fileName;
    // delete '/' if not first page
    if (fileName.length() > 1) {
        fileName.erase(0,1);
    }

    std::cout << "RESULTS: " << request << " " << fileName << std::endl;

    return fileName;
}

std::string findLengthImg(char * fileName) {
    std::string stringLength;
    char * fileBuffer = new char[4096];
    int fd1 = open(fileName, O_RDONLY, 0); 
    int file_len = 0;
    long long bytesRecv = 0;
    
 
    memset(fileBuffer,0,4096);

    
    
    // keep reading until bytes are all read
    do {
        memset(fileBuffer,0,4096);
        bytesRecv = read(fd1, fileBuffer, 4096);
        file_len += bytesRecv;
    } while (bytesRecv > 0);

    stringLength = std::to_string(file_len);

    std::cout << "Initial Image Length: " << stringLength << std::endl;
    // deallocate char
    delete[] fileBuffer;

    return stringLength;
}
std::string findLength(char * fileName) {
    std::string stringLength;
    char * fileBuffer = new char[4096];
    int fd1 = open(fileName, O_RDONLY, 0); 
    int file_len = 0;
    int bytesRecv = 0;
    
    // clear buffer
    memset(fileBuffer,0,4096);

    // keep reading until all bytes are read
    do {
        memset(fileBuffer,0,4096);
        bytesRecv = read(fd1, fileBuffer, 4096);
        file_len += bytesRecv;
    } while (bytesRecv > 0);

    // convert length to string
    stringLength = std::to_string(file_len);

    std::cout << "Initial HTML Length: " << stringLength << std::endl;
 
    return stringLength;
}

void chooseFile(std::string fileName, int &sock){
    char * charName = new char[fileName.size()+1];
    char * response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-length: ";
    char * img_response = "HTTP/1.1 200 OK\r\nContent-Type: image/jpg\r\nContent-length: ";
    // change filename into cstring
    std::strcpy(charName,fileName.c_str());

    std::cout << "CHAR NAME: " << charName << std::endl;
    if (open(charName, O_RDONLY, 0) == -1) {
        std::cout << "File doesn't exist. Sending error message.\n";
    } else if (fileName == "/") {
        // send html front
        send(sock , response , strlen(response) , 0 );
        readFile("front.html",sock);
    } else if(fileName.find("html",0) != -1){
        std::cout << "Sending HTML file...\n";
        send(sock , response , strlen(response) , 0 );
        readFile(charName, sock);
    } else if(fileName.find("jpg",0) != -1) {
        std::cout << "Sending image file...\n";
        send(sock , img_response , strlen(img_response) , 0 );
        sendImage(charName, sock);
    } else {
        send(sock , response , strlen(response) , 0 );
        readFile("test3.html",sock);
    }

    // deallocate charNAme
    delete[] charName;
    
}

void * Process(void * ptr) {
    char * buffer = new char[4096];
    connection_t * conn;
    // For testing
    char * fileName = "test3.html";
    char * hello = "Hellot there\n";
    char * test = "<h1>Hello World</h1>";


    if (!ptr) {
        pthread_exit(0);
    }
    
    conn = (connection_t *)ptr;
    
    // clear buffer
    memset(buffer,0,4096);
    int bytesRecv = read(conn->sock, buffer, 4096);

        
    printf("%s\n",buffer);

    std::cout << "Trying to send.\n";

    // read file and send
    chooseFile(parseResponse(buffer),conn->sock);
             
    delete[] buffer;
    
    close(conn->sock);
    free(conn);
    pthread_exit(0);
}