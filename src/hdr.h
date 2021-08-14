#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <fstream>

#pragma comment (lib, "Ws2_32.lib")
using namespace std;

struct ajp_hdr {
    uint16_t magic = 0x3412;
    uint16_t len;
    uint8_t code;
    uint8_t method;
};

struct len_str {
    char* c_str = 0;
    int len = 0;

    void add(const char* s, int l) {
        if (c_str != NULL) {
            c_str = (char*)realloc(c_str, len + l);
        }
        else {
            c_str = (char*)malloc(len + l);
        }
        memcpy(&c_str[len], s, l);
        len += l;
    }
};

string getIp(string host);
SOCKET connect_host(string ip, string port);
len_str getPayload(string target, string ip, string port, string file, int mode);
void saveFile(SOCKET sock);
