/* memory_pool: Memory pool manager (v46.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define MAX_POOLS 16
#define MAX_POOL_SIZE 1024
#define POOL_STATE_EMPTY 0
#define POOL_STATE_ACTIVE 1
typedef struct { int id; unsigned int block_size; int block_count; int free_count; unsigned int bitmap[MAX_POOL_SIZE/32]; int state; } mem_pool_t;
static mem_pool_t pools[MAX_POOLS]; static int pool_count = 0; static int next_id = 1;
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) { char b[32]; int p=0; if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0'; else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b); }
int pool_create(unsigned int block_size, int block_count) {
    if(pool_count>=MAX_POOLS) return -1;
    int idx = pool_count++;
    pools[idx].id = next_id++;
    pools[idx].block_size = block_size;
    pools[idx].block_count = block_count;
    pools[idx].free_count = block_count;
    pools[idx].state = POOL_STATE_ACTIVE;
    for(int i=0;i<MAX_POOL_SIZE/32;i++) pools[idx].bitmap[i] = 0;
    pstr("  Created pool "); pint(pools[idx].id); pstr(": "); pint(block_count); pstr(" x "); pint(block_size); pstr(" bytes\n");
    return pools[idx].id;
}
int pool_alloc(int pool_id) {
    for(int i=0;i<MAX_POOLS;i++) if(pools[i].id==pool_id && pools[i].state==POOL_STATE_ACTIVE) {
        if(pools[i].free_count==0) return -1;
        for(int b=0;b<pools[i].block_count;b++) {
            int word = b/32; int bit = b%32;
            if(!(pools[i].bitmap[word] & (1<<bit))) {
                pools[i].bitmap[word] |= (1<<bit);
                pools[i].free_count--;
                pstr("  Pool "); pint(pool_id); pstr(" alloc block "); pint(b); pstr("\n");
                return b;
            }
        }
    }
    return -1;
}
int pool_free(int pool_id, int block) {
    for(int i=0;i<MAX_POOLS;i++) if(pools[i].id==pool_id && pools[i].state==POOL_STATE_ACTIVE) {
        int word = block/32; int bit = block%32;
        if(pools[i].bitmap[word] & (1<<bit)) {
            pools[i].bitmap[word] &= ~(1<<bit);
            pools[i].free_count++;
            pstr("  Pool "); pint(pool_id); pstr(" free block "); pint(block); pstr("\n");
            return 0;
        }
    }
    return -1;
}
int pool_stats(int pool_id) {
    for(int i=0;i<MAX_POOLS;i++) if(pools[i].id==pool_id) {
        int used = pools[i].block_count - pools[i].free_count;
        pstr("  Pool "); pint(pool_id); pstr(":\n");
        pstr("    Block size: "); pint(pools[i].block_size); pstr("\n");
        pstr("    Total: "); pint(pools[i].block_count); pstr("\n");
        pstr("    Used: "); pint(used); pstr("\n");
        pstr("    Free: "); pint(pools[i].free_count); pstr("\n");
        return 0;
    }
    return -1;
}
void _start(void) {
    unsigned int buf = host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]) { char *a=&av[pos]; int l=my_strlen(a); if(l==2&&a[0]=='-'&&a[1]=='h') help=1; else if(l==2&&a[0]=='-'&&a[1]=='t') test=1; while(pos<512&&av[pos]) pos++; pos++; }
    pstr("Memory Pool v46.0 - Pool Allocator\n");
    if(help) { pstr("Usage: memory_pool [-h|-t]\n"); return; }
    if(test) {
        pstr("=== Memory Pool Test ===\n\n");
        pstr("[1] Create pools\n"); int p1=pool_create(64, 32); int p2=pool_create(256, 16); int p3=pool_create(1024, 8); pstr("\n");
        pstr("[2] Alloc\n"); int b1=pool_alloc(p1); int b2=pool_alloc(p1); int b3=pool_alloc(p2); int b4=pool_alloc(p3); pstr("\n");
        pstr("[3] Stats\n"); pool_stats(p1); pool_stats(p2); pool_stats(p3); pstr("\n");
        pstr("[4] Free\n"); pool_free(p1, b1); pool_free(p2, b3); pstr("\n");
        pstr("[5] Final\n"); pool_stats(p1); pool_stats(p2);
        pstr("\n=== Done ===\n"); return;
    }
    pstr("Use -h for help, -t for test\n");
}
