#pragma once

#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>

class Network {
public:
    int soundSockfd;
    int soundSockfdRemote;
    void CreateSoundServer(int port);
    void AcceptSoundRemote();
};