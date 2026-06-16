/* udp_support: UDP protocol module (v46.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define MAX_SOCKETS 32
#define MAX_DGRAM_SIZE 65535
#define UDP_STATE_CLOSED 0
#define UDP_STATE_BOUND 1
#define UDP_STATE_OPEN 2
typedef struct { int fd; unsigned int local_port; unsigned int remote_addr; unsigned int remote_port; int state; unsigned int sent; unsigned int recv; } udp_socket_t;
static udp_socket_t sockets[MAX_SOCKETS]; static int next_fd = 10;
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) { char b[32]; int p=0; if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0'; else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b); }
int udp_socket(void) {
    for(int i=0;i<MAX_SOCKETS;i++) if(sockets[i].state==UDP_STATE_CLOSED) {
        sockets[i].fd = next_fd++; sockets[i].state = UDP_STATE_OPEN; sockets[i].sent = 0; sockets[i].recv = 0;
        pstr("  Created UDP socket fd="); pint(sockets[i].fd); pstr("\n"); return sockets[i].fd;
    }
    return -1;
}
int udp_bind(int fd, unsigned int port) {
    for(int i=0;i<MAX_SOCKETS;i++) if(sockets[i].fd==fd && sockets[i].state==UDP_STATE_OPEN) {
        sockets[i].local_port = port; sockets[i].state = UDP_STATE_BOUND;
        pstr("  Bound fd="); pint(fd); pstr(" to port "); pint(port); pstr("\n"); return 0;
    }
    return -1;
}
int udp_sendto(int fd, unsigned int addr, unsigned int port, const char *data, int len) {
    for(int i=0;i<MAX_SOCKETS;i++) if(sockets[i].fd==fd && sockets[i].state>=UDP_STATE_OPEN) {
        sockets[i].remote_addr = addr; sockets[i].remote_port = port; sockets[i].sent += len;
        pstr("  Sent "); pint(len); pstr(" bytes from fd="); pint(fd); pstr(" to port "); pint(port); pstr("\n"); return len;
    }
    return -1;
}
int udp_recvfrom(int fd, char *buf, int maxlen) {
    for(int i=0;i<MAX_SOCKETS;i++) if(sockets[i].fd==fd && sockets[i].state==UDP_STATE_BOUND) {
        int len = 10;  /* Simulated */
        sockets[i].recv += len;
        pstr("  Recv "); pint(len); pstr(" bytes on fd="); pint(fd); pstr("\n"); return len;
    }
    return -1;
}
int udp_close(int fd) {
    for(int i=0;i<MAX_SOCKETS;i++) if(sockets[i].fd==fd) {
        pstr("  Closed fd="); pint(fd); pstr(" (sent="); pint(sockets[i].sent); pstr(" recv="); pint(sockets[i].recv); pstr(")\n");
        sockets[i].state = UDP_STATE_CLOSED; return 0;
    }
    return -1;
}
int udp_stats(void) {
    int active=0; for(int i=0;i<MAX_SOCKETS;i++) if(sockets[i].state>UDP_STATE_CLOSED) active++;
    pstr("  UDP sockets: "); pint(active); pstr("/"); pint(MAX_SOCKETS); pstr(" active\n"); return active;
}
void _start(void) {
    unsigned int buf = host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]) { char *a=&av[pos]; int l=my_strlen(a); if(l==2&&a[0]=='-'&&a[1]=='h') help=1; else if(l==2&&a[0]=='-'&&a[1]=='t') test=1; while(pos<512&&av[pos]) pos++; pos++; }
    pstr("UDP Support v46.0\n");
    if(help) { pstr("Usage: udp_support [-h|-t]\n"); return; }
    if(test) {
        pstr("=== UDP Test ===\n\n");
        pstr("[1] Create\n"); int s1=udp_socket(); int s2=udp_socket(); pstr("\n");
        pstr("[2] Bind\n"); udp_bind(s1, 8080); udp_bind(s2, 9090); pstr("\n");
        pstr("[3] Send\n"); udp_sendto(s1, 0x0A000001, 80, "Hello", 5); udp_sendto(s2, 0x0A000002, 443, "World", 5); pstr("\n");
        pstr("[4] Recv\n"); udp_recvfrom(s1, 0, 1024); udp_recvfrom(s2, 0, 1024); pstr("\n");
        pstr("[5] Stats\n"); udp_stats(); pstr("\n");
        pstr("[6] Close\n"); udp_close(s1); udp_close(s2); udp_stats();
        pstr("\n=== Done ===\n"); return;
    }
    pstr("Use -h for help, -t for test\n");
}
