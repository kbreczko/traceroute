/* Kamil Breczko (280 990) */

#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include <vector>
#include <set>
#include <stdexcept>

#include "sender.h"
#include "receiver.h"

#define MAX_TTL 30
#define MAX_PACKET_PER_TTL 3
#define WAITING_TIME 1

using namespace std;

typedef chrono::high_resolution_clock::time_point time_point;

class Traceroute {
  private:
    int pid;
    int sockfd;
  public:
    Traceroute(int pid);

    void run(const char *ip_address);

    void validate(const char *ip_address);

    void print_route(u_int16_t ttl, vector<struct packet> received_packets, time_point point);
};


#endif