/* websocket_lib: WebSocket full-duplex communication (v46.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define WS_OPCODE_CONT 0x0
#define WS_OPCODE_TEXT 0x1
#define WS_OPCODE_BINARY 0x2
#define WS_OPCODE_CLOSE 0x8
#define WS_OPCODE_PING 0x9
#define WS_OPCODE_PONG 0xA
#define MAX_CONNECTIONS 32
#define WS_STATE_CLOSED 0
#define WS_STATE_CONNECTING 1
#define WS_STATE_OPEN 2
#define WS_STATE_CLOSING 3
typedef struct { int fd; int state; unsigned int msg_sent; unsigned int msg_recv; int is_client; } ws_connection_t;
static ws_connection_t connections[MAX_CONNECTIONS]; static int next_fd = 20;
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) { char b[32]; int p=0; if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0'; else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b); }
int ws_connect(const char *url) {
    for(int i=0;i<MAX_CONNECTIONS;i++) if(connections[i].state==WS_STATE_CLOSED) {
        connections[i].fd = next_fd++;
        connections[i].state = WS_STATE_OPEN;
        connections[i].is_client = 1;
        connections[i].msg_sent = 0;
        connections[i].msg_recv = 0;
        pstr("  Connected to "); pstr(url); pstr(" fd="); pint(connections[i].fd); pstr("\n");
        return connections[i].fd;
    }
    return -1;
}
int ws_send_text(int fd, const char *text) {
    for(int i=0;i<MAX_CONNECTIONS;i++) if(connections[i].fd==fd && connections[i].state==WS_STATE_OPEN) {
        connections[i].msg_sent++;
        pstr("  WS["); pint(fd); pstr("] TEXT: "); pstr(text); pstr("\n");
        return my_strlen(text);
    }
    return -1;
}
int ws_send_binary(int fd, const char *data, int len) {
    for(int i=0;i<MAX_CONNECTIONS;i++) if(connections[i].fd==fd && connections[i].state==WS_STATE_OPEN) {
        connections[i].msg_sent++;
        pstr("  WS["); pint(fd); pstr("] BINARY: "); pint(len); pstr(" bytes\n");
        return len;
    }
    return -1;
}
int ws_ping(int fd) {
    for(int i=0;i<MAX_CONNECTIONS;i++) if(connections[i].fd==fd) {
        pstr("  WS["); pint(fd); pstr("] PING -> PONG\n");
        return 0;
    }
    return -1;
}
int ws_close(int fd) {
    for(int i=0;i<MAX_CONNECTIONS;i++) if(connections[i].fd==fd) {
        pstr("  WS["); pint(fd); pstr("] closed (sent="); pint(connections[i].msg_sent);
        pstr(" recv="); pint(connections[i].msg_recv); pstr(")\n");
        connections[i].state = WS_STATE_CLOSED;
        return 0;
    }
    return -1;
}
int ws_stats(void) {
    int active=0;
    for(int i=0;i<MAX_CONNECTIONS;i++) if(connections[i].state==WS_STATE_OPEN) active++;
    pstr("  WebSocket: "); pint(active); pstr("/"); pint(MAX_CONNECTIONS); pstr(" active\n");
    return active;
}
void _start(void) {
    unsigned int buf = host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]) { char *a=&av[pos]; int l=my_strlen(a); if(l==2&&a[0]=='-'&&a[1]=='h') help=1; else if(l==2&&a[0]=='-'&&a[1]=='t') test=1; while(pos<512&&av[pos]) pos++; pos++; }
    pstr("WebSocket Library v46.0\n");
    if(help) { pstr("Usage: websocket_lib [-h|-t]\n"); return; }
    if(test) {
        pstr("=== WebSocket Test ===\n\n");
        pstr("[1] Connect\n"); int c1=ws_connect("ws://localhost:8080"); int c2=ws_connect("wss://api.example.com/ws"); pstr("\n");
        pstr("[2] Send Text\n"); ws_send_text(c1, "Hello WebSocket!"); ws_send_text(c2, "{\"action\":\"subscribe\"}"); pstr("\n");
        pstr("[3] Send Binary\n"); ws_send_binary(c1, "binary data", 11); pstr("\n");
        pstr("[4] Ping/Pong\n"); ws_ping(c1); ws_ping(c2); pstr("\n");
        pstr("[5] Stats\n"); ws_stats(); pstr("\n");
        pstr("[6] Close\n"); ws_close(c1); ws_close(c2); ws_stats();
        pstr("\n=== Done ===\n"); return;
    }
    pstr("Use -h for help, -t for test\n");
}
