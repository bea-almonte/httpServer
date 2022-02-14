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

char * readFile(std::string fileName, int &file_len) {
    //std::ifstream inFile("hello-there.jpg", std::ifstream::in);
    char * fileBuffer;
    int fd1 = open("simple.html", O_RDONLY, 0); 


    if (fd1 == -1) {
        std::cout << "This isn't how you read file.\n";
        
    } else {
        std::cout << "Okay... opened file\n";
    }

    // get file length
    if (read(fd1,&file_len, sizeof(char)) == -1) {
        std::cout << "Heck.\n";
    } else {
        std::cout << "I think this is right...\n";
        std::cout << "Length: " << file_len << std::endl;
    }

    // create memory
    fileBuffer = new char[file_len];

    read(fd1,fileBuffer,(file_len+1) * sizeof(char));
    //uffer = (char*)malloc((len+1)*sizeof(char));
    // read
    //read(fileBuffer,html_len);
    //std::cout << "Test: " << fileBuffer << std::endl;
    return fileBuffer;
}

int main() {
    std::string fileName = "test.html";
    int file_len = 0;
    char * fileBuffer = readFile(fileName,file_len);

    std::cout << fileBuffer;
}