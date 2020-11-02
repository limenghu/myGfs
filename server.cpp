/*
    * 名称：serve.cpp
    * 时间：2020.10.31 20:08
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
    std::cout << "This is server." << std::endl;
    //socket
    int listenfd = socket(AF_INET,SOCK_STREAM,0);//创建一个socket，AF_INET为ipv4 Internet协议，SOCK_STREAM为建立tcp连接，提供字节流
    if (-1 == listenfd) {
        std::cout << "Error:socket" << std::endl;
        return 0;
    }
    //bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);//htons-"host to network short",主机字节顺序为HBO-host byte order,网络字节顺序为NBO
    addr.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY表示绑定本机所有地址
    if(-1 == bind(listenfd,(struct sockaddr*)&addr,sizeof(addr))) {
        std::cout << "Error:bind" << std::endl;
        return 0;
    }
    //listen
    if(-1 == listen(listenfd, 5)){//socket可以排队的最大连接个数-5
        std::cout << "Error:listen" << std::endl;
        return 0;
    }
    //accept
    int conn;
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    while (true){
        std::cout << "...listening" << std::endl;
        conn = accept(listenfd,(struct sockaddr*)&clientAddr,&clientAddrLen);//返回值为描述子，表示与3返回客户的tcp连接
        if(conn < 0){
            std::cout << "Error:accept" << std::endl;
            continue;
        }
        inet_ntop(AF_INET,&clientAddr.sin_addr,clientIP,INET_ADDRSTRLEN);//IP地址转换函数,inet_ntop-网格二进制结构到ASCII类型
        std::cout << "...connect" << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;//ntohs-network to host short

        char buf[255];
        while(true){
            memset(buf, 0, sizeof(buf));
            int len = recv(conn, buf, sizeof(buf), 0);
            buf[len] = '\0';
            if (0 == strcmp(buf, "exit")){
                std::cout << "...disconnect" << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
                break;
            }
            std::cout << buf << std::endl;
            send(conn, buf, len, 0);
        }

        close(conn);
    }
    close(listenfd);//使相应socket描数字的引用计数-1,引用计数为0时，发送终止连接请求
    return 0;
}
