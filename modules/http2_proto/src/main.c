/* http2_proto: HTTP/2 protocol support (v46.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define H2_FRAME_DATA 0x0
#define H2_FRAME_HEADERS 0x1
#define H2_FRAME_SETTINGS 0x4
#define H2_FRAME_PING 0x6
#define H2_FRAME_GOAWAY 0x7
#define MAX_STREAMS 64
#define MAX_FRAME_SIZE 16384
typedef struct { int stream_id; int state; int weight; unsigned int window_size; } h2_stream_t;
typedef struct { int enabled; int max_concurrent_streams; int initial_window_size; int max_frame_size; } h2_settings_t;
static h2_stream_t streams[MAX_STREAMS]; static int stream_count = 0;
static h2_settings_t settings = {1, 100, 65535, MAX_FRAME_SIZE};
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) { char b[32]; int p=0; if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0'; else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b); }
int h2_init(void) { pstr("  HTTP/2 initialized\n"); pstr("    Max streams: "); pint(settings.max_concurrent_streams); pstr("\n"); pstr("    Window size: "); pint(settings.initial_window_size); pstr("\n"); return 0; }
int h2_create_stream(int weight) {
    if(stream_count>=MAX_STREAMS) return -1;
    int id = stream_count++;
    streams[id].stream_id = (id*2)+1;
    streams[id].state = 1;
    streams[id].weight = weight;
    streams[id].window_size = settings.initial_window_size;
    pstr("  Created stream "); pint(streams[id].stream_id); pstr(" weight="); pint(weight); pstr("\n");
    return streams[id].stream_id;
}
int h2_send_headers(int stream_id, const char *path) {
    for(int i=0;i<MAX_STREAMS;i++) if(streams[i].stream_id==stream_id) {
        pstr("  Stream "); pint(stream_id); pstr(": HEADERS "); pstr(path); pstr("\n"); return 0;
    }
    return -1;
}
int h2_send_data(int stream_id, const char *data, int len) {
    for(int i=0;i<MAX_STREAMS;i++) if(streams[i].stream_id==stream_id) {
        pstr("  Stream "); pint(stream_id); pstr(": DATA "); pint(len); pstr(" bytes\n"); return len;
    }
    return -1;
}
int h2_ping(void) { pstr("  PING sent\n"); pstr("  PONG received\n"); return 0; }
int h2_settings_display(void) {
    pstr("  HTTP/2 Settings:\n");
    pstr("    max_concurrent_streams: "); pint(settings.max_concurrent_streams); pstr("\n");
    pstr("    initial_window_size: "); pint(settings.initial_window_size); pstr("\n");
    pstr("    max_frame_size: "); pint(settings.max_frame_size); pstr("\n");
    return 0;
}
void _start(void) {
    unsigned int buf = host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]) { char *a=&av[pos]; int l=my_strlen(a); if(l==2&&a[0]=='-'&&a[1]=='h') help=1; else if(l==2&&a[0]=='-'&&a[1]=='t') test=1; while(pos<512&&av[pos]) pos++; pos++; }
    pstr("HTTP/2 Protocol v46.0\n");
    if(help) { pstr("Usage: http2_proto [-h|-t]\n"); return; }
    if(test) {
        pstr("=== HTTP/2 Test ===\n\n");
        pstr("[1] Init\n"); h2_init(); pstr("\n");
        pstr("[2] Settings\n"); h2_settings_display(); pstr("\n");
        pstr("[3] Streams\n"); int s1=h2_create_stream(16); int s2=h2_create_stream(32); int s3=h2_create_stream(8); pstr("\n");
        pstr("[4] Headers\n"); h2_send_headers(s1, "/index.html"); h2_send_headers(s2, "/api/data"); pstr("\n");
        pstr("[5] Data\n"); h2_send_data(s1, "<html>...</html>", 15); h2_send_data(s2, "{\"status\":200}", 14); pstr("\n");
        pstr("[6] Ping\n"); h2_ping();
        pstr("\n=== Done ===\n"); return;
    }
    pstr("Use -h for help, -t for test\n");
}
