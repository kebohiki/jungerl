/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <tiuser.h>
#include <fcntl.h>

#include "stack.h"

CLIENT* clnt_open(host, port, proto)
char* host; int port; char* proto;
{
    int fd;
    CLIENT* clnt;
    struct hostent* hent;
    struct in_addr* inp;
    struct t_call sndcall;
    struct t_call rcvcall;
    struct t_bind reqbind;
    struct t_bind retbind;
    struct sockaddr_in req_addr;
    struct sockaddr_in ret_addr;
    struct sockaddr_in addr;
    struct netbuf svc_addr;
    char devpath[80];

    strcpy(devpath, "/dev/");
    strcat(devpath, proto);
    if ((fd = t_open(dev, O_RDWR, NULL)) < 0) {
	perror("t_open");
	exit(1);
    }
    req_addr.sin_family = AF_INET;
    req_addr.sin_port = htons(0);
    req_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    reqbind.addr.maxlen = sizeof(struct sockaddr_in);
    reqbind.addr.len = sizeof(struct sockaddr_in);
    reqbind.addr.buf = (char*) &req_addr;
    reqbind.qlen = 0;

    retbind.addr.maxlen = sizeof(struct sockaddr_in);
    retbind.addr.len = sizeof(struct sockaddr_in);
    retbind.addr.buf = (char*) &ret_addr;

    if (t_bind(fd, &reqbind, &retbind) < 0) {
	perror("t_bind");
	exit(1);
    }

    printf("bound to %s:%d\n", 
	   inet_ntoa(ret_addr.sin_addr), 
	   ntohs(ret_addr.sin_port));

    if ((hent = gethostbyname(host)) == NULL) {
	fprintf(stderr, "gethostbyname: host not found\n");
	exit(1);
    }

    if (hent->h_length > 1)
	fprintf(stderr, "warning: more than one ip address!\n");
    if (hent->h_addrtype != AF_INET) {
	fprintf(stderr, "host is not in the internet domain!\n");
	exit(1);
    }

    inp = (struct in_addr*) *hent->h_addr_list;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(inp->s_addr);

    svc_addr.maxlen = sizeof(struct sockaddr_in);
    svc_addr.len = sizeof(struct sockaddr_in);
    svc_addr.buf = (char*) &addr;

    clnt = clnt_vc_create(fd, &svc_addr, STACK_PROG, STACK_VERS, 
			  1024, 1024);
    if (clnt == (CLIENT*) NULL) {
	clnt_pcreateerror(host);
	exit(1);
    }
    return clnt;
}

void clnt_close(clnt)
CLIENT* clnt;
{
    clnt_destroy(clnt);
}

void* null(clnt)
CLIENT* clnt;
{
    char* stackproc_null_1_arg;
    void* result_1 = stackproc_null_1((void *)&stackproc_null_1_arg, clnt);
    if (result_1 == (void *) NULL) {
	clnt_perror(clnt, "call failed");
    }
    return result_1;
}

int* pop(clnt)
CLIENT* clnt;
{
    char *  stackproc_pop_1_arg;
    int* result_2 = stackproc_pop_1((void *)&stackproc_pop_1_arg, clnt);
    if (result_2 == (int *) NULL) {
	clnt_perror(clnt, "call failed");
    }
    return result_2;
}


bool_t* push(clnt, value)
CLIENT* clnt; int value;
{
    int  stackproc_push_1_arg = value;
    bool_t* result_3 = stackproc_push_1(&stackproc_push_1_arg, clnt);
    if (result_3 == (bool_t *) NULL) {
	clnt_perror(clnt, "call failed");
    }
    return result_3;
}


main(argc, argv)
    int argc;
    char *argv[];
{
    char *host;
    short port;
    CLIENT* clnt;
    int i;

    if (argc < 3) {
	printf("usage:  %s host port\n", argv[0]);
	exit(1);
    }
    host = argv[1];
    port = atoi(argv[2]);

    clnt = clnt_open(host, port, "tcp");
    printf("client open\n");

    for (i = 0; i < 10; i++) {
	if (push(clnt, i) != NULL)
	    printf("PUSHED %d\n", i);
    }

    for (i = 0; i < 10; i++) {
	int* v = pop(clnt);
	if (v != NULL) {
	    printf("POPPED %d\n", *v);
	}
    }

    clnt_close(clnt);

    exit(0);

}
