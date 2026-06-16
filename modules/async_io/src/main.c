/* async_io: Asynchronous I/O framework (v46.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define MAX_OPS 64
#define OP_READ 1
#define OP_WRITE 2
#define OP_ACCEPT 3
#define OP_CONNECT 4
#define OP_STATE_PENDING 0
#define OP_STATE_RUNNING 1
#define OP_STATE_COMPLETE 2
typedef struct { int id; int type; int fd; int state; int result; void (*callback)(int); unsigned int submitted_at; } aio_op_t;
static aio_op_t operations[MAX_OPS]; static int op_count = 0; static int next_id = 1; static unsigned int clk = 0;
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) { char b[32]; int p=0; if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0'; else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b); }
void default_callback(int result) { pstr("  Callback: result="); pint(result); pstr("\n"); }
int aio_submit_read(int fd, void (*cb)(int)) {
    if(op_count>=MAX_OPS) return -1;
    int idx = op_count++;
    operations[idx].id = next_id++;
    operations[idx].type = OP_READ;
    operations[idx].fd = fd;
    operations[idx].state = OP_STATE_PENDING;
    operations[idx].callback = cb ? cb : default_callback;
    operations[idx].submitted_at = ++clk;
    pstr("  Submitted READ fd="); pint(fd); pstr(" op="); pint(operations[idx].id); pstr("\n");
    return operations[idx].id;
}
int aio_submit_write(int fd, void (*cb)(int)) {
    if(op_count>=MAX_OPS) return -1;
    int idx = op_count++;
    operations[idx].id = next_id++;
    operations[idx].type = OP_WRITE;
    operations[idx].fd = fd;
    operations[idx].state = OP_STATE_PENDING;
    operations[idx].callback = cb ? cb : default_callback;
    operations[idx].submitted_at = ++clk;
    pstr("  Submitted WRITE fd="); pint(fd); pstr(" op="); pint(operations[idx].id); pstr("\n");
    return operations[idx].id;
}
int aio_poll(void) {
    int completed = 0;
    for(int i=0;i<op_count;i++) {
        if(operations[i].state==OP_STATE_PENDING) {
            operations[i].state = OP_STATE_RUNNING;
            pstr("  Running: op="); pint(operations[i].id); pstr("\n");
        }
        if(operations[i].state==OP_STATE_RUNNING) {
            operations[i].state = OP_STATE_COMPLETE;
            operations[i].result = 100;
            if(operations[i].callback) operations[i].callback(operations[i].result);
            completed++;
        }
    }
    pstr("  Poll complete: "); pint(completed); pstr(" ops\n");
    return completed;
}
int aio_stats(void) {
    int pending=0, running=0, complete=0;
    for(int i=0;i<op_count;i++) {
        if(operations[i].state==OP_STATE_PENDING) pending++;
        else if(operations[i].state==OP_STATE_RUNNING) running++;
        else if(operations[i].state==OP_STATE_COMPLETE) complete++;
    }
    pstr("  Async I/O stats:\n");
    pstr("    Total ops: "); pint(op_count); pstr("\n");
    pstr("    Pending: "); pint(pending); pstr("\n");
    pstr("    Running: "); pint(running); pstr("\n");
    pstr("    Complete: "); pint(complete); pstr("\n");
    return op_count;
}
void _start(void) {
    unsigned int buf = host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]) { char *a=&av[pos]; int l=my_strlen(a); if(l==2&&a[0]=='-'&&a[1]=='h') help=1; else if(l==2&&a[0]=='-'&&a[1]=='t') test=1; while(pos<512&&av[pos]) pos++; pos++; }
    pstr("Async I/O v46.0 - io_uring Framework\n");
    if(help) { pstr("Usage: async_io [-h|-t]\n"); return; }
    if(test) {
        pstr("=== Async I/O Test ===\n\n");
        pstr("[1] Submit ops\n"); int op1=aio_submit_read(3, 0); int op2=aio_submit_write(4, 0); int op3=aio_submit_read(5, 0); pstr("\n");
        pstr("[2] Stats (before)\n"); aio_stats(); pstr("\n");
        pstr("[3] Poll\n"); aio_poll(); pstr("\n");
        pstr("[4] Stats (after)\n"); aio_stats();
        pstr("\n=== Done ===\n"); return;
    }
    pstr("Use -h for help, -t for test\n");
}
