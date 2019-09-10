#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      /* open */ 
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <errno.h>
#include "util.h"
#include "tuple.h"
#include "ringbuffer.h"

#define ALGORITHM

#ifdef ALGORITHM
#include "src/Heap.hpp"
#include "src/Skiplist.hpp"
#include "src/Qmax.hpp"
#endif

// The number of ringbuffer
// **Must** be (# pmd threads + 1)
#define MAX_RINGBUFFER_NUM 10

#define RDM_NUM 150000000
unsigned rdm[RDM_NUM];

static inline char* ip2a(uint32_t ip, char* addr) {
    sprintf(addr, "%d.%d.%d.%d", (ip & 0xff), ((ip >> 8) & 0xff), ((ip >> 16) &
            0xff), ((ip >> 24) & 0xff));
    return addr;
}

/*
void print_tuple(FILE* f, tuple_t* t) {
    char ip1[30], ip2[30];

    fprintf(f, "%s(%u) <-> %s(%u) %u %ld\n",
            ip2a(t->key.src_ip, ip1), t->key.src_port,
            ip2a(t->key.dst_ip, ip2), t->key.dst_port,
            t->key.proto, t->size
            );
}
*/

int counter = 0;

int mode = 0;
int q = 0;
double gamma_p = 0;
bool next = false;

Heap *heap;
Skiplist *skiplist;
QMax *qmax;

#define INTERVAL 150000000

void handler(int sig) {
	printf("%d\n", counter);
	fflush(stdout);
	counter = 0;
#ifdef ALGORITHM_T
	next = true;
	
#endif
	alarm(1);
}

#ifdef ALGORITHM
void parse_config(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "./user_reader <algorithm> [args...]\n");
        exit(1);
    }

    if (strcmp(argv[1], "heap") == 0) {
        mode = 1;
        if (argc != 3) {
            fprintf(stderr, "./user_reader heap <q>\n");
            exit(1);
        }
        q = atoi(argv[2]);
    } else if (strcmp(argv[1], "skiplist") == 0) {
        mode = 2;
        if (argc != 3) {
            fprintf(stderr, "./user_reader skiplist <q>\n");
            exit(1);
        }
        q = atoi(argv[2]);
    } else if (strcmp(argv[1], "qmax") == 0) {
        mode = 3;
        if (argc != 4) {
            fprintf(stderr, "./user_reader qmax <q> <gamma>\n");
            exit(1);
        }
        q = atoi(argv[2]);
        gamma_p = strtod(argv[3], NULL);
    } else if (strcmp(argv[1], "null") == 0) {
        mode = 0;
        if (argc != 2) {
            fprintf(stderr, "./user_reader null\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Invalid algorithm\n");
        exit(1);
    }
    printf("mode = %d, q = %d, gamma = %lf\n", mode, q, gamma_p);
}
#endif

int main(int argc, char *argv[]) {
#ifdef ALGORITHM
    for (int i = 0; i < RDM_NUM; i ++) {
	    rdm[i] = rand();
    }

    parse_config(argc, argv);

    switch (mode) {
    case 1:
        heap = new Heap(q);
        break;
    case 2:
        skiplist = new Skiplist(q);
        break;
    case 3:
        qmax = new QMax(q, gamma_p);
        break;
    }
    //int base = 29;
    int sum_pkt = 0;
#endif

    tuple_t t;
    long long tot_cnt = 0;
    //HC_TYPE * hc = new HC_TYPE();
    int turn = 0;

    LOG_MSG("Initialize the ringbuffer.\n");

	ringbuffer_t * rbs[MAX_RINGBUFFER_NUM];

	for (int i = 0; i < MAX_RINGBUFFER_NUM; ++i) {
		char name[30];
		sprintf(name, "/rb_%d", i);
		rbs[i] = connect_ringbuffer_shm(name, sizeof(tuple_t));
		//printf("%x\n", rbs[i]);
	}
 
	fprintf(stderr, "connected.\n");	fflush(stdout);

	int idx = 0;

    // print number of pkts received per 5 sec
	signal(SIGALRM, handler);
	alarm(5);

	while (1) {
        if (t.flag == TUPLE_TERM) {
            break;
        } 
        else {
            while (read_ringbuffer(rbs[(idx) % MAX_RINGBUFFER_NUM], &t) < 0) {
                idx = (idx + 1) % MAX_RINGBUFFER_NUM;
            }
            counter++;
            // Insert to sketch here

#ifdef ALGORITHM
	    //base = (base ^ 0x34ba3129) * 251;
	    //int k = t.key.id * 4294967296L + t.key.src_ip;
            //int value = t.key.src_port * 4294967296L + t.key.dst_port;
	    int value = rdm[sum_pkt % RDM_NUM];
	    sum_pkt ++;
	    switch (mode) {
            case 1:
		heap->add(value);
		if (sum_pkt == INTERVAL) {
		    sum_pkt = 0;
		    //heap->clear(q);
		    delete heap;
		    heap = new Heap(q);
		}
                break;
            case 2:
		skiplist->add(value);
		if (sum_pkt == INTERVAL) {
		    sum_pkt = 0;
		    delete skiplist;
		    skiplist = new Skiplist(q);
		}
		break;
            case 3:
		qmax->insert(value);
		if (sum_pkt == INTERVAL) {
		   sum_pkt = 0;
		   delete qmax;
		   qmax = new QMax(q, gamma_p);
		}
                break;
            }
#endif
	}
    } 

	return 0;
}
