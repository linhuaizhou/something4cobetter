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
    // [@lhz] ���峤�ȱ���
    int send_len = 0;
    int recv_len = 0;
    // [@lhz] ���巢�ͻ������ͽ��ܻ�����
    char send_buf[100];
    char recv_buf[100];
    // [@lhz] ���������׽��֣����������׽���
    SOCKET s_server;
    // [@lhz] ����˵�ַ�ͻ��˵�ַ
    SOCKADDR_IN server_addr;

    initialization();

    // [@lhz] ���ӵ��˿�8000�ķ�����
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(9000);

    s_server = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        cout << "���ӵ�9000�˿ڵķ�����ʧ�ܣ�" << endl;
        WSACleanup();
        return 1;
    }
    else {
        cout << "���ӵ�9000�˿ڵķ������ɹ���" << endl;
    }
    /*
    // ���ӵ��˿�9000�ķ�����
    server_addr.sin_port = htons(9000);

    SOCKET s_server2 = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(s_server2, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        cout << "���ӵ�9000�˿ڵķ�����ʧ�ܣ�" << endl;
        closesocket(s_server);  // �ر�8000�˿ڵ��׽���
        WSACleanup();
        return 1;
    }
    else {
        cout << "���ӵ�9000�˿ڵķ������ɹ���" << endl;
    }
    */
    // [@lhz] ����/��������
    while (1) {
        cout << "�����뷢����Ϣ: ";
        cin >> send_buf;
        size_t input_len = strlen(send_buf);

        // [@lhz] ������Ϣ
        send_len = send(s_server, send_buf, input_len, 0);

        // [@lhz] ���͵�8000�˿ڵķ�����
        //send_len = send(s_server, send_buf, 100, 0);
        if (send_len < 0) {
            cout << "���͵�8000�˿ڵķ�����ʧ�ܣ�" << endl;
            break;
        }
        
        recv_len = recv(s_server, recv_buf, 100, 0);
        recv_buf[recv_len] = '\0';
        if (recv_len < 0) {
            cout << "����8000�˿ڷ���������Ϣʧ�ܣ�" << endl;
            break;
        }
        else {
            cout << "8000�˿ڷ���������Ϣ:" << recv_buf << endl;
        }

        /*

        // ���͵�9000�˿ڵķ�����
        send_len = send(s_server2, send_buf, input_len, 0);
        if (send_len < 0) {
            cout << "���͵�9000�˿ڵķ�����ʧ�ܣ�" << endl;
            break;
        }

        recv_len = recv(s_server2, recv_buf, 100, 0);
        recv_buf[recv_len] = '\0';
        if (recv_len < 0) {
            cout << "����9000�˿ڷ���������Ϣʧ�ܣ�" << endl;
            break;
        }
        else {
            cout << "9000�˿ڷ���������Ϣ:" << recv_buf << endl;
        }
        */
    }

    // [@lhz] �ر��׽���
    closesocket(s_server);
    //closesocket(s_server2);
    // [@lhz] �ͷ�DLL��Դ
    WSACleanup();

    return 0;
}

void initialization() {
    // [@lhz] ��ʼ���׽��ֿ�
    WORD w_req = MAKEWORD(2, 2);  // �汾��
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0) {
        cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
    }
    else {
        cout << "��ʼ���׽��ֿ�ɹ���" << endl;
    }
    // ���汾��
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
        cout << "�׽��ֿ�汾�Ų�����" << endl;
        WSACleanup();
    }
    else {
        cout << "�׽��ֿ�汾��ȷ��" << endl;
    }
    // ������˵�ַ��Ϣ
}
