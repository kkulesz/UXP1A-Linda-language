//
// Created by bartlomiej on 24.05.2021.
//

#ifndef UXP1A_LINDA_MASTERPROCESS_H
#define UXP1A_LINDA_MASTERPROCESS_H

#include "../tuple/TuplePattern.h"
#include "WaitingProcess.h"
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

class MasterProcess {
public:

    void run();

    MasterProcess(){
        memset(&serverAddr, 0, sizeof(serverAddr));
        memset(&clientAddr, 0, sizeof(clientAddr));
        serverAddr.sun_family = AF_UNIX;
        strncpy(serverAddr.sun_path, serverFilename.c_str(), PATH_LENGTH_LIMIT);
        serverAddressLength = sizeof(serverAddr);
        counter = 0;
    }
private:

    std::vector<Tuple> tuples;
    std::vector<WaitingProcessInfo> waitingProcesses;
    const static unsigned char PATH_LENGTH_LIMIT = 100;
    const static unsigned char QUEUE_LENGTH = 8;
    const std::string serverFilename = "/tmp/linda_server";
    struct sockaddr_un serverAddr;
    struct sockaddr_un clientAddr;
    char buffer[256];
    int socketFileDescriptor;
    socklen_t serverAddressLength;
    socklen_t clientAddressLength;
    long currentReturnCode;
    int clientSocket;
    const char* RESPONSE = "OK";
    int counter;

    void init();
    void acceptClient();
    void receive();
    void processMessage();
    void respond();
    void processOutput();
    void processRead();
    void processInput();
    bool sendTuple(const Tuple &tuple);
    void createAwaitingProcess(const TuplePattern &pattern, bool isInput);
    void checkWaitingProcesses(const Tuple& tuple);
    void getChildReturnValue(std::vector<WaitingProcessInfo>::iterator iterator);
};


#endif//UXP1A_LINDA_MASTERPROCESS_H