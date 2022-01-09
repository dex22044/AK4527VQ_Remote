#include "Network.h"

void Network::CreateSoundServer(int port) {
    soundSockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = INADDR_ANY;
    assert(bind(soundSockfd, (sockaddr*)&sa, sizeof(sockaddr_in)) == 0);
    assert(listen(soundSockfd, 1000) == 0);
}

void Network::AcceptSoundRemote() {
    soundSockfdRemote = accept(soundSockfd, NULL, NULL);
}