/* Kamil Breczko (280 990) */

#include "traceroute.h"

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "error: incorrect number of arguments \n");
        return -1;
    }

    try {
        int pid = getpid();
        Traceroute traceroute(pid);
        traceroute.run(argv[1]);
    } catch (const exception &e) {
        fprintf(stderr, "%s \n", e.what());
        return -1;
    }

    return 0;
}