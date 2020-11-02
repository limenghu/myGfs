/*
    * 名称：client.cpp
    * 时间：2020.11.1 21:11
    * 创建者：李梦虎
    * 环境:ubuntu20.04 c++ 9.3.0 VScode
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


int main(){
    std::cout << "This is client" << std::endl;
    //socket
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == client){
        std::cout << "Error: socket" << std::endl;
        return 0;
    }
    //connect
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        std::cout << "Error: connect" << std::endl;
        return 0;
    }
    std::cout << "...connect" << std::endl;
    char data[255];
    char buf[255];
    while(true){
        std::cin >> data;
        send(client, data, strlen(data), 0);
        if(0 == strcmp(data,"exit")){
            std::cout << "...disconnect" << std::endl;
            break;
        }
        memset(buf, 0, sizeof(buf));
        int len = recv(client, buf, sizeof(buf), 0);
        buf[len] = '\0';
        std::cout << buf << std::endl;
    }
    close(client);
    return 0;
}
