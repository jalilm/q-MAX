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
#include "src/HeapKV.hpp"
#include "src/SkiplistKV.hpp"
#include "src/QmaxKV.hpp"
#include "src/xxhash.h"
#endif

// The number of ringbuffer
// **Must** be (# pmd threads + 1)
#define MAX_RINGBUFFER_NUM 10

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
int sum_pkt = 0;
int sum_sum = 0;

HeapKV *heap;
SkiplistKV *skiplist;
QMaxKV *qmax;

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
    parse_config(argc, argv);

    switch (mode) {
    case 1:
        heap = new HeapKV(q);
        break;
    case 2:
        skiplist = new SkiplistKV(q);
        break;
    case 3:
        qmax = new QMaxKV(q, gamma_p);
        break;
    }
    //int base = 29;
    //int sum_pkt = 0;
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
	    key k = t.key.src_port * 4294967296L + t.key.src_ip;
	   // if (sum_sum > 0) fprintf(stderr, "%x.%x.%x.%x, %x\n", (t.key.src_ip) & 0xff, (t.key.src_ip >> 8) & 0xff, (t.key.src_ip >> 16) & 0xff, (t.key.src_ip >> 24) & 0xff, t.key.src_port);
            //int j;
	    //for (j = 0; j < 34; j ++) {
		//    fprintf(stderr, "%02x ", t.array[j]);
	    //}
	    //fprintf(stderr, "\n");
	    //int value = t.key.src_port * 4294967296L + t.key.dst_port;
	    //int value = t.key.src_port * 65536 + t.key.dst_port;
	    //key k = t.key.src_ip;
	    //val v = t.size;
	    val v;
	    sum_pkt ++;
	    switch (mode) {
            case 1:
		v = XXH64((void *) &k, sizeof(key), 1235);
		heap->add(pair<key,val>(k, v));
		if (sum_pkt == INTERVAL) {
		    sum_pkt = 0;
		    //heap->clear(q);
		    delete heap;
		    heap = new HeapKV(q);
		}
                break;
            case 2:
		v = XXH64((void *) &k, sizeof(key), 1235);
		skiplist->add(pair<key,val>(k, v));
		if (sum_pkt == INTERVAL) {
		    sum_pkt = 0;
		    delete skiplist;
		    skiplist = new SkiplistKV(q);
		}
		break;
            case 3:
		v = XXH64((void *) &k, sizeof(key), 1235);
		//v = rand();
		qmax->insert(k, v);
		if (sum_pkt == INTERVAL) {
		   sum_pkt = 0;
		   delete qmax;
		   qmax = new QMaxKV(q, gamma_p);
		}
                break;
            }
#endif
	}
    } 

	return 0;
}
