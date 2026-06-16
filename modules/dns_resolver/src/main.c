/* dns_resolver: DNS resolution module (v46.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define DNS_TYPE_A 1
#define MAX_CACHE 64
#define MAX_DOMAIN 128
typedef struct { char domain[MAX_DOMAIN]; int type; char value[64]; unsigned int ttl; unsigned int created; int in_use; } dns_entry_t;
static dns_entry_t cache[MAX_CACHE]; static int cache_count = 0; static unsigned int clk = 0;
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) { while(*a&&*b){if(*a!=*b)return *a-*b;a++;b++;} return *a-*b; }
static void my_strncpy(char *d, const char *s, int n) { int i=0; while(i<n-1&&s[i]){d[i]=s[i];i++;} d[i]='\0'; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) { char b[32]; int p=0; if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0'; else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b); }
int dns_resolve(const char *domain) {
    for(int i=0;i<MAX_CACHE;i++) if(cache[i].in_use && my_strcmp(cache[i].domain,domain)==0 && (clk-cache[i].created)<cache[i].ttl) {
        pstr("  Cache: "); pstr(domain); pstr(" -> "); pstr(cache[i].value); pstr("\n"); return 0;
    }
    pstr("  Resolve: "); pstr(domain); pstr("\n");
    const char *r = "93.184.216.34";
    if(my_strcmp(domain,"google.com")==0) r="142.250.80.46";
    else if(my_strcmp(domain,"github.com")==0) r="140.82.121.4";
    if(cache_count<MAX_CACHE) { int i=cache_count++; my_strncpy(cache[i].domain,domain,MAX_DOMAIN-1); my_strncpy(cache[i].value,r,63); cache[i].ttl=3600; cache[i].created=clk++; cache[i].in_use=1; }
    pstr("  Result: "); pstr(r); pstr("\n"); return 0;
}
int dns_cache_clear(void) { for(int i=0;i<MAX_CACHE;i++) cache[i].in_use=0; cache_count=0; pstr("  Cache cleared\n"); return 0; }
int dns_cache_stats(void) { int u=0; for(int i=0;i<MAX_CACHE;i++) if(cache[i].in_use) u++; pstr("  Cache: "); pint(u); pstr("/"); pint(MAX_CACHE); pstr("\n"); return u; }
void _start(void) {
    unsigned int buf = host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]) { char *a=&av[pos]; int l=my_strlen(a); if(l==2&&a[0]=='-'&&a[1]=='h') help=1; else if(l==2&&a[0]=='-'&&a[1]=='t') test=1; while(pos<512&&av[pos]) pos++; pos++; }
    pstr("DNS Resolver v46.0\n");
    if(help) { pstr("Usage: dns_resolver [-h|-t]\n"); return; }
    if(test) {
        pstr("=== DNS Test ===\n\n");
        pstr("[1] Resolve\n"); dns_resolve("example.com"); dns_resolve("google.com"); dns_resolve("github.com"); pstr("\n");
        pstr("[2] Cache\n"); dns_resolve("example.com"); pstr("\n");
        pstr("[3] Stats\n"); dns_cache_stats(); pstr("\n");
        pstr("[4] Clear\n"); dns_cache_clear(); dns_cache_stats();
        pstr("\n=== Done ===\n"); return;
    }
    pstr("Use -h for help, -t for test\n");
}
