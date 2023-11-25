#include<iostream>
#include<winsock.h>
#include <limits>
#pragma comment(lib,"ws2_32.lib")

/*
* Date: 20231124
* Author: @lhz
* Note: socket Client
*/
using namespace std;

void initialization();



int main() {
    // [@lhz] 定义长度变量
    int send_len = 0;
    int recv_len = 0;
    // [@lhz] 定义发送缓冲区和接受缓冲区
    char send_buf[100];
    char recv_buf[100];
    // [@lhz] 定义服务端套接字，接受请求套接字
    SOCKET s_server;
    // [@lhz] 服务端地址客户端地址
    SOCKADDR_IN server_addr;

    initialization();

    // [@lhz] 连接到端口8000的服务器
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(9000);

    s_server = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        cout << "连接到9000端口的服务器失败！" << endl;
        WSACleanup();
        return 1;
    }
    else {
        cout << "连接到9000端口的服务器成功！" << endl;
    }
    /*
    // 连接到端口9000的服务器
    server_addr.sin_port = htons(9000);

    SOCKET s_server2 = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(s_server2, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        cout << "连接到9000端口的服务器失败！" << endl;
        closesocket(s_server);  // 关闭8000端口的套接字
        WSACleanup();
        return 1;
    }
    else {
        cout << "连接到9000端口的服务器成功！" << endl;
    }
    */
    // [@lhz] 发送/接收数据
    while (1) {
        cout << "请输入发送信息: ";
        cin >> send_buf;
        size_t input_len = strlen(send_buf);

        // [@lhz] 发送消息
        send_len = send(s_server, send_buf, input_len, 0);

        // [@lhz] 发送到8000端口的服务器
        //send_len = send(s_server, send_buf, 100, 0);
        if (send_len < 0) {
            cout << "发送到8000端口的服务器失败！" << endl;
            break;
        }
        
        recv_len = recv(s_server, recv_buf, 100, 0);
        recv_buf[recv_len] = '\0';
        if (recv_len < 0) {
            cout << "接受8000端口服务器的消息失败！" << endl;
            break;
        }
        else {
            cout << "8000端口服务器的信息:" << recv_buf << endl;
        }

        /*

        // 发送到9000端口的服务器
        send_len = send(s_server2, send_buf, input_len, 0);
        if (send_len < 0) {
            cout << "发送到9000端口的服务器失败！" << endl;
            break;
        }

        recv_len = recv(s_server2, recv_buf, 100, 0);
        recv_buf[recv_len] = '\0';
        if (recv_len < 0) {
            cout << "接受9000端口服务器的消息失败！" << endl;
            break;
        }
        else {
            cout << "9000端口服务器的信息:" << recv_buf << endl;
        }
        */
    }

    // [@lhz] 关闭套接字
    closesocket(s_server);
    //closesocket(s_server2);
    // [@lhz] 释放DLL资源
    WSACleanup();

    return 0;
}

void initialization() {
    // [@lhz] 初始化套接字库
    WORD w_req = MAKEWORD(2, 2);  // 版本号
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0) {
        cout << "初始化套接字库失败！" << endl;
    }
    else {
        cout << "初始化套接字库成功！" << endl;
    }
    // 检测版本号
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
        cout << "套接字库版本号不符！" << endl;
        WSACleanup();
    }
    else {
        cout << "套接字库版本正确！" << endl;
    }
    // 填充服务端地址信息
}
