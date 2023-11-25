// simpleclient_winsock_loop.cpp
#include <iostream>
#include <string>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock\n";
        return -1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket\n";
        WSACleanup();
        return -1;
    }

    SOCKADDR_IN serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(3000);

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error connecting to server\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to the server. Enter messages to send (type 'exit' to quit):\n";

    std::string message;
    while (true) {
        std::getline(std::cin, message);

        // 发送消息到服务器
        send(clientSocket, message.c_str(), message.size(), 0);

        // 检查用户是否输入了 'exit'
        if (message == "exit") {
            break;
        }
    }

    // 关闭连接
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
