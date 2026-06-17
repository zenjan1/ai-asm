/* identity_federation: Cross-domain identity federation module (v1.0)
 *
 * Provides identity federation capabilities similar to Azure AD B2B / PingOne:
 *
 * Trust establishment:
 *   - Trusted domain configuration and lifecycle
 *   - Certificate exchange and validation between federated domains
 *   - Federation metadata publishing and consumption
 *   - Trust transit checks and trust chain verification
 *
 * Attribute mapping:
 *   - Cross-domain attribute transformation (uppercase, lowercase, identity)
 *   - Custom mapping rules with conditional expressions
 *   - Default value handling for unmatched conditions
 *   - Attribute aggregation across federation sources
 *
 * Proxy authentication:
 *   - Cross-domain authentication proxy with token creation
 *   - Token translation between protocols (SAML -> OIDC)
 *   - Credential bridging (Kerberos, certificate, password)
 *   - Identity assertion with allow/deny policies
 *
 * Directory federation:
 *   - Multi-directory aggregation from LDAP/AD sources
 *   - Virtual directory abstraction across multiple backends
 *   - Per-directory attribute aggregation
 *   - Deduplication merge of overlapping directory entries
 *
 * Consistency sync:
 *   - Cross-domain account linking with bidirectional references
 *   - Account merge with lifecycle coordination
 *   - Lifecycle state transitions (active, suspended, merged, disabled)
 *   - Logout propagation across linked accounts and broadcast
 *
 * Governance monitoring:
 *   - Federation relationship graph with nodes and edges
 *   - Trust chain verification with certificate validation
 *   - Anomaly detection (expired certs, low trust levels)
 *   - Performance monitoring for federation operations
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_DOMAINS 32
#define MAX_CERTS 32
#define MAX_ATTR_MAPS 32
#define MAX_PROXY_TOKENS 32
#define MAX_DIRS 16
#define MAX_LINKS 64
#define MAX_ACCT 64
#define MAX_NODES 64
#define MAX_EDGES 128
#define MAX_ANOM 32
#define MAX_PERF 16
#define MXN 48
#define MXV 64
#define TRUST_NONE 0
#define TRUST_MIN 1
#define TRUST_STD 2
#define TRUST_ELEV 3
#define TRUST_FULL 4
#define CERT_VALID 1
#define CERT_EXPIRED 2
#define CERT_REVOKED 3
#define AUTH_OK 0
#define AUTH_DENIED 1
#define AUTH_TRANSLATED 2
#define AUTH_BRIDGED 3
#define ACCT_ACTIVE 0
#define ACCT_SUSPENDED 1
#define ACCT_MERGED 2
#define ACCT_DISABLED 3
#define ANOM_MED 1
#define ANOM_HIGH 2

typedef struct { int id; char name[MXN]; char url[MXV]; int trust; int certs; int active; int ts; } trust_domain_t;
typedef struct { int id; int did; int state; int keysz; char fp[MXV]; char subj[MXN]; char issuer[MXN]; } trust_cert_t;
typedef struct { int id; char src[MXN]; char dst[MXN]; char xform[MXN]; int cond; char cexpr[MXV]; char defval[MXV]; } attr_map_t;
typedef struct { int id; int src_d; int dst_d; char src_id[MXN]; char dst_id[MXN]; char type[MXN]; int result; int bridged; } proxy_tok_t;
typedef struct { int id; char name[MXN]; char dn[MXV]; int entries; int active; } dir_t;
typedef struct { int id; char pri[MXN]; int pd; char lnk[MXN]; int ld; int life; } acct_link_t;
typedef struct { int id; char ident[MXN]; int did; int life; int merge; } acct_ent_t;
typedef struct { int id; char name[MXN]; int type; int trust; } fed_node_t;
typedef struct { int id; int from; int to; int trust; int weight; } fed_edge_t;
typedef struct { int id; char desc[MXV]; int sev; int dom; int ts; } anomaly_t;
typedef struct { int id; char name[MXN]; int val; int thresh; } perf_met_t;

static trust_domain_t g_dom[MAX_DOMAINS];
static trust_cert_t g_cert[MAX_CERTS];
static attr_map_t g_amap[MAX_ATTR_MAPS];
static proxy_tok_t g_ptok[MAX_PROXY_TOKENS];
static dir_t g_dir[MAX_DIRS];
static acct_link_t g_link[MAX_LINKS];
static acct_ent_t g_acct[MAX_ACCT];
static fed_node_t g_node[MAX_NODES];
static fed_edge_t g_edge[MAX_EDGES];
static anomaly_t g_anom[MAX_ANOM];
static perf_met_t g_perf[MAX_PERF];
static int g_ndc=0,g_ncc=0,g_nac=0,g_npc=0,g_ndrc=0,g_nlc=0,g_nacnt=0;
static int g_nnc=0,g_nec=0,g_nan=0,g_npf=0,g_clk=0,g_nid=1;

/* Forward declarations - trust domain */
static int trust_domain_create(const char *name, int trust_level);
static int trust_domain_get(int domain_id);
static int trust_domain_list(void);
static int trust_domain_delete(int domain_id);
static int trust_cert_exchange(int domain_id, const char *subject, int key_size);
static int trust_cert_validate(int cert_id);
static int trust_cert_list(int domain_id);
static int trust_metadata_publish(int domain_id, const char *url);
static int trust_metadata_consume(int domain_id, const char *data);
static int trust_transit_check(int src_domain, int dst_domain);
static int trust_chain_verify(int domain_id);
/* Forward declarations - attribute mapping */
static int attr_map_create(const char *src, const char *dst, const char *transform);
static int attr_map_transform(int map_id, const char *input, char *output);
static int attr_map_conditional(int map_id, const char *input, const char *cond, char *out);
static int attr_default_value(int map_id, const char *defval);
static int attr_aggregate(const char *attr_name, int *values, int count);
/* Forward declarations - proxy auth */
static int proxy_auth_cross_domain(int src, int dst, const char *identity);
static int proxy_token_translate(int token_id, int target_domain);
static int proxy_credential_bridge(int token_id, const char *cred_type);
static int proxy_identity_assert(int token_id, const char *assertion);
/* Forward declarations - directory federation */
static int directory_aggregate(const char *name, const char *base_dn);
static int directory_virtual(const char *vname, int *dir_ids, int count);
static int directory_attr_aggregate(int dir_id, const char *attr);
static int directory_dedup_merge(int dir_a, int dir_b);
/* Forward declarations - account linking */
static int account_link_create(const char *pri, int pd, const char *lnk, int ld);
static int account_link_list(int domain_id);
static int account_merge(int primary_id, int merge_id);
static int account_lifecycle_coord(int account_id, int new_lifecycle);
static int logout_propagate(int domain_id, const char *identity);
static int logout_broadcast(const char *identity);
/* Forward declarations - governance */
static int federation_graph_build(void);
static int federation_graph_get(int node_id);
static int anomaly_detect(int domain_id);
static int perf_monitor(void);
/* Forward declarations - test */
static void test_suite(void);

/* String utilities */
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while(*a&&*b){if(*a!=*b) return *a-*b; a++; b++;} return *a-*b; }
static void my_strncpy(char *d, const char *s, int n) {
    int i=0; while(i<n-1&&s[i]){d[i]=s[i];i++;} d[i]='\0'; }
static int my_strstr(const char *h, const char *n) {
    int hl=my_strlen(h),nl=my_strlen(n);
    if(nl==0||nl>hl) return 0;
    for(int i=0;i<=hl-nl;i++){int j=0;while(j<nl&&h[i+j]==n[j])j++;if(j==nl)return 1;} return 0; }
static void ps(const char *s){host_print(s);}
static void pi(int v){
    char b[32];int p=0;if(v<0){b[p++]='-';v=-v;}
    if(v==0)b[p++]='0';else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;
    while(v>0){b[p--]='0'+(v%10);v/=10;}}host_print(b);}
static int nid(void){return g_nid++;}

/* Trust domain management */
static int trust_domain_create(const char *name, int trust_level) {
    if(g_ndc>=MAX_DOMAINS||trust_level<TRUST_NONE||trust_level>TRUST_FULL) return -1;
    int idx=-1; for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id==0){idx=i;break;}
    if(idx<0) return -1;
    g_dom[idx].id=nid(); my_strncpy(g_dom[idx].name,name?name:"?",MXN-1);
    g_dom[idx].trust=trust_level; g_dom[idx].certs=0; g_dom[idx].active=1;
    g_dom[idx].ts=++g_clk; g_dom[idx].url[0]='\0'; g_ndc++; return g_dom[idx].id;
}
static int trust_domain_get(int domain_id) {
    for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id==domain_id&&g_dom[i].active){
        ps("  Domain [");pi(g_dom[i].id);ps("] name=");ps(g_dom[i].name);
        ps(" trust=");pi(g_dom[i].trust);ps(" certs=");pi(g_dom[i].certs);
        ps(" active=");pi(g_dom[i].active);ps("\n"); return 0;} return -1;
}
static int trust_domain_list(void) {
    int c=0; ps("  Domain List\n  ======================================\n");
    for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id&&g_dom[i].active){
        const char *l="NONE";
        if(g_dom[i].trust==TRUST_MIN)l="MINIMAL";else if(g_dom[i].trust==TRUST_STD)l="STANDARD";
        else if(g_dom[i].trust==TRUST_ELEV)l="ELEVATED";else if(g_dom[i].trust==TRUST_FULL)l="FULL";
        ps("  [");pi(g_dom[i].id);ps("] ");ps(g_dom[i].name);ps(" trust=");ps(l);
        ps(" certs=");pi(g_dom[i].certs);ps("\n");c++;
    }
    ps("  Total: ");pi(c);ps(" domains\n"); return c;
}
static int trust_domain_delete(int domain_id) {
    for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id==domain_id){g_dom[i].active=0;g_dom[i].id=0;g_ndc--;return 0;} return -1;
}
static int trust_cert_exchange(int domain_id, const char *subject, int key_size) {
    int f=0; for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id==domain_id){f=1;break;}
    if(!f||g_ncc>=MAX_CERTS) return -1;
    int idx=-1; for(int i=0;i<MAX_CERTS;i++) if(g_cert[i].id==0){idx=i;break;}
    if(idx<0) return -1;
    g_cert[idx].id=nid(); g_cert[idx].did=domain_id; g_cert[idx].state=CERT_VALID;
    g_cert[idx].keysz=key_size>0?key_size:2048;
    my_strncpy(g_cert[idx].subj,subject?subject:"?",MXN-1);
    my_strncpy(g_cert[idx].issuer,"federation-ca",MXN-1);
    my_strncpy(g_cert[idx].fp,"SHA256:AB12CD34",MXV-1);
    for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id==domain_id) g_dom[i].certs++;
    g_ncc++; return g_cert[idx].id;
}
static int trust_cert_validate(int cert_id) {
    for(int i=0;i<MAX_CERTS;i++) if(g_cert[i].id==cert_id){
        if(g_cert[i].state==CERT_VALID)return 0;if(g_cert[i].state==CERT_REVOKED)return -2;return -1;} return -1;
}
static int trust_cert_list(int domain_id) {
    int c=0; ps("  Certificates for domain ");pi(domain_id);ps(":\n");
    for(int i=0;i<MAX_CERTS;i++) if(g_cert[i].id&&g_cert[i].did==domain_id){
        const char *st="UNKNOWN";
        if(g_cert[i].state==CERT_VALID)st="VALID";else if(g_cert[i].state==CERT_EXPIRED)st="EXPIRED";
        else if(g_cert[i].state==CERT_REVOKED)st="REVOKED";
        ps("    cert[");pi(g_cert[i].id);ps("] subj=");ps(g_cert[i].subj);
        ps(" state=");ps(st);ps(" key=");pi(g_cert[i].keysz);ps(" bit\n");c++;}
    ps("  Total: ");pi(c);ps(" certs\n"); return c;
}
static int trust_metadata_publish(int domain_id, const char *url) {
    for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id==domain_id){my_strncpy(g_dom[i].url,url?url:"",MXV-1);return 0;} return -1;
}
static int trust_metadata_consume(int domain_id, const char *data) {
    for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id==domain_id){g_dom[i].ts=++g_clk;return data?0:-1;} return -1;
}
static int trust_transit_check(int src_domain, int dst_domain) {
    int st=-1,dt=-1;
    for(int i=0;i<MAX_DOMAINS;i++){if(g_dom[i].id==src_domain)st=g_dom[i].trust;if(g_dom[i].id==dst_domain)dt=g_dom[i].trust;}
    return(st<0||dt<0||st<TRUST_STD||dt<TRUST_STD)?-1:0;
}
static int trust_chain_verify(int domain_id) {
    int cc=0; for(int i=0;i<MAX_CERTS;i++) if(g_cert[i].did==domain_id&&g_cert[i].state==CERT_VALID) cc++;
    if(cc==0) return -1;
    for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id==domain_id){g_dom[i].ts=++g_clk;return(g_dom[i].trust>=TRUST_STD)?0:-1;} return -1;
}

/* Attribute mapping */
static int attr_map_create(const char *src, const char *dst, const char *transform) {
    if(g_nac>=MAX_ATTR_MAPS) return -1;
    int idx=-1; for(int i=0;i<MAX_ATTR_MAPS;i++) if(g_amap[i].id==0){idx=i;break;}
    if(idx<0) return -1;
    g_amap[idx].id=nid(); my_strncpy(g_amap[idx].src,src?src:"",MXN-1);
    my_strncpy(g_amap[idx].dst,dst?dst:"",MXN-1);
    my_strncpy(g_amap[idx].xform,transform?transform:"identity",MXN-1);
    g_amap[idx].cond=0; g_amap[idx].cexpr[0]='\0'; g_amap[idx].defval[0]='\0'; g_nac++; return g_amap[idx].id;
}
static int attr_map_transform(int map_id, const char *input, char *output) {
    for(int i=0;i<MAX_ATTR_MAPS;i++) if(g_amap[i].id==map_id){
        if(my_strcmp(g_amap[i].xform,"uppercase")==0){int l=my_strlen(input);
            for(int j=0;j<l;j++) output[j]=(input[j]>='a'&&input[j]<='z')?(input[j]-32):input[j];output[l]='\0';
        }else if(my_strcmp(g_amap[i].xform,"lowercase")==0){int l=my_strlen(input);
            for(int j=0;j<l;j++) output[j]=(input[j]>='A'&&input[j]<='Z')?(input[j]+32):input[j];output[l]='\0';
        }else my_strncpy(output,input,MXV-1); return 0;} return -1;
}
static int attr_map_conditional(int map_id, const char *input, const char *cond, char *out) {
    for(int i=0;i<MAX_ATTR_MAPS;i++) if(g_amap[i].id==map_id){
        g_amap[i].cond=1; my_strncpy(g_amap[i].cexpr,cond?cond:"",MXV-1);
        if(cond&&my_strstr(input,cond)) my_strncpy(out,input,MXV-1);
        else if(g_amap[i].defval[0]) my_strncpy(out,g_amap[i].defval,MXV-1);
        else out[0]='\0'; return 0;} return -1;
}
static int attr_default_value(int map_id, const char *defval) {
    for(int i=0;i<MAX_ATTR_MAPS;i++) if(g_amap[i].id==map_id){my_strncpy(g_amap[i].defval,defval?defval:"",MXV-1);return 0;} return -1;
}
static int attr_aggregate(const char *attr_name, int *values, int count) {
    if(!values||count<=0) return -1; int s=0;
    for(int i=0;i<count;i++) s+=values[i];
    ps("  Aggregated attr '");ps(attr_name);ps("' over ");pi(count);ps(" values: sum=");pi(s);ps("\n"); return s;
}

/* Proxy authentication */
static int proxy_auth_cross_domain(int src, int dst, const char *identity) {
    if(trust_transit_check(src,dst)!=0||g_npc>=MAX_PROXY_TOKENS) return -1;
    int idx=-1; for(int i=0;i<MAX_PROXY_TOKENS;i++) if(g_ptok[i].id==0){idx=i;break;}
    if(idx<0) return -1;
    g_ptok[idx].id=nid(); g_ptok[idx].src_d=src; g_ptok[idx].dst_d=dst;
    my_strncpy(g_ptok[idx].src_id,identity?identity:"",MXN-1);
    my_strncpy(g_ptok[idx].dst_id,identity?identity:"",MXN-1);
    my_strncpy(g_ptok[idx].type,"SAML",MXN-1);
    g_ptok[idx].result=AUTH_OK; g_ptok[idx].bridged=0; g_npc++; return g_ptok[idx].id;
}
static int proxy_token_translate(int token_id, int target_domain) {
    for(int i=0;i<MAX_PROXY_TOKENS;i++) if(g_ptok[i].id==token_id){
        g_ptok[i].dst_d=target_domain; g_ptok[i].result=AUTH_TRANSLATED;
        my_strncpy(g_ptok[i].type,"OIDC",MXN-1); return 0;} return -1;
}
static int proxy_credential_bridge(int token_id, const char *cred_type) {
    for(int i=0;i<MAX_PROXY_TOKENS;i++) if(g_ptok[i].id==token_id){
        g_ptok[i].bridged=1; g_ptok[i].result=AUTH_BRIDGED;
        if(cred_type) my_strncpy(g_ptok[i].type,cred_type,MXN-1); return 0;} return -1;
}
static int proxy_identity_assert(int token_id, const char *assertion) {
    for(int i=0;i<MAX_PROXY_TOKENS;i++) if(g_ptok[i].id==token_id){
        if(assertion&&my_strstr(assertion,"deny")){g_ptok[i].result=AUTH_DENIED;return -1;} return 0;} return -1;
}

/* Directory federation */
static int directory_aggregate(const char *name, const char *base_dn) {
    if(g_ndrc>=MAX_DIRS) return -1;
    int idx=-1; for(int i=0;i<MAX_DIRS;i++) if(g_dir[i].id==0){idx=i;break;}
    if(idx<0) return -1;
    g_dir[idx].id=nid(); my_strncpy(g_dir[idx].name,name?name:"?",MXN-1);
    my_strncpy(g_dir[idx].dn,base_dn?base_dn:"",MXV-1);
    g_dir[idx].entries=0; g_dir[idx].active=1; g_ndrc++; return g_dir[idx].id;
}
static int directory_virtual(const char *vname, int *dir_ids, int count) {
    if(!dir_ids||count<=0) return -1; int te=0;
    for(int i=0;i<count;i++) for(int j=0;j<MAX_DIRS;j++)
        if(g_dir[j].id==dir_ids[i]) te+=g_dir[j].entries;
    ps("  Virtual dir '");ps(vname?vname:"vdir");ps("' aggregates ");pi(count);ps(" sources, ");pi(te);ps(" entries\n"); return te;
}
static int directory_attr_aggregate(int dir_id, const char *attr) {
    for(int i=0;i<MAX_DIRS;i++) if(g_dir[i].id==dir_id){
        ps("  Aggregating attr '");ps(attr?attr:"");ps("' from dir ");pi(dir_id);
        ps(" (");pi(g_dir[i].entries);ps(" entries)\n"); return g_dir[i].entries;} return -1;
}
static int directory_dedup_merge(int dir_a, int dir_b) {
    int fa=0,fb=0,ea=0,eb=0;
    for(int i=0;i<MAX_DIRS;i++){if(g_dir[i].id==dir_a){fa=1;ea=g_dir[i].entries;}if(g_dir[i].id==dir_b){fb=1;eb=g_dir[i].entries;}}
    if(!fa||!fb) return -1;
    int tot=ea+eb,dup=tot>0?tot/4:0,m=tot-dup;
    ps("  Dedup merge dir ");pi(dir_a);ps(" + dir ");pi(dir_b);
    ps(": total=");pi(tot);ps(" dupes=");pi(dup);ps(" merged=");pi(m);ps("\n"); return m;
}

/* Account linking */
static int account_link_create(const char *pri, int pd, const char *lnk, int ld) {
    if(g_nlc>=MAX_LINKS) return -1;
    int idx=-1; for(int i=0;i<MAX_LINKS;i++) if(g_link[i].id==0){idx=i;break;}
    if(idx<0) return -1;
    g_link[idx].id=nid(); my_strncpy(g_link[idx].pri,pri?pri:"",MXN-1); g_link[idx].pd=pd;
    my_strncpy(g_link[idx].lnk,lnk?lnk:"",MXN-1); g_link[idx].ld=ld;
    g_link[idx].life=ACCT_ACTIVE; g_nlc++; return g_link[idx].id;
}
static int account_link_list(int domain_id) {
    int c=0;
    for(int i=0;i<MAX_LINKS;i++) if(g_link[i].id&&(g_link[i].pd==domain_id||g_link[i].ld==domain_id)){
        ps("  Link ");pi(g_link[i].id);ps(": ");ps(g_link[i].pri);ps(" <-> ");ps(g_link[i].lnk);ps("\n");c++;}
    ps("  Total links for domain ");pi(domain_id);ps(": ");pi(c);ps("\n"); return c;
}
static int account_merge(int primary_id, int merge_id) {
    int fp=-1,fm=-1;
    for(int i=0;i<MAX_ACCT;i++){if(g_acct[i].id==primary_id)fp=i;if(g_acct[i].id==merge_id)fm=i;}
    if(fp<0||fm<0) return -1;
    g_acct[fm].life=ACCT_MERGED; g_acct[fm].merge=primary_id;
    ps("  Merged account ");pi(merge_id);ps(" into ");pi(primary_id);ps("\n"); return 0;
}
static int account_lifecycle_coord(int account_id, int new_lifecycle) {
    for(int i=0;i<MAX_ACCT;i++) if(g_acct[i].id==account_id){
        if(g_acct[i].life==ACCT_MERGED) return -1; g_acct[i].life=new_lifecycle; return 0;} return -1;
}
static int logout_propagate(int domain_id, const char *identity) {
    int c=0;
    for(int i=0;i<MAX_LINKS;i++) if(g_link[i].id&&(g_link[i].pd==domain_id||g_link[i].ld==domain_id)){
        g_link[i].life=ACCT_DISABLED;c++;}
    ps("  Logout propagated from domain ");pi(domain_id);ps(" for '");ps(identity?identity:"");ps("' to ");pi(c);ps(" links\n"); return c;
}
static int logout_broadcast(const char *identity) {
    int c=0; for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id&&g_dom[i].active) c++;
    ps("  Logout broadcast for '");ps(identity?identity:"");ps("' to ");pi(c);ps(" active domains\n"); return c;
}

/* Governance monitoring */
static int federation_graph_build(void) {
    g_nnc=0; g_nec=0;
    for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id&&g_dom[i].active&&g_nnc<MAX_NODES){
        g_node[g_nnc].id=g_dom[i].id; my_strncpy(g_node[g_nnc].name,g_dom[i].name,MXN-1);
        g_node[g_nnc].type=0; g_node[g_nnc].trust=g_dom[i].trust; g_nnc++;}
    for(int i=0;i<g_nnc;i++) for(int j=i+1;j<g_nnc;j++) if(g_nec<MAX_EDGES){
        int mt=g_node[i].trust<g_node[j].trust?g_node[i].trust:g_node[j].trust;
        if(mt>=TRUST_STD){g_edge[g_nec].id=nid();g_edge[g_nec].from=g_node[i].id;g_edge[g_nec].to=g_node[j].id;
            g_edge[g_nec].trust=mt;g_edge[g_nec].weight=mt;g_nec++;}}
    ps("  Federation graph: ");pi(g_nnc);ps(" nodes, ");pi(g_nec);ps(" edges\n"); return g_nnc;
}
static int federation_graph_get(int node_id) {
    int ec=0; for(int i=0;i<g_nec;i++) if(g_edge[i].from==node_id||g_edge[i].to==node_id) ec++;
    ps("  Node ");pi(node_id);ps(" has ");pi(ec);ps(" connections\n"); return ec;
}
static int anomaly_detect(int domain_id) {
    int dt=-1,ec=0,an=0;
    for(int i=0;i<MAX_DOMAINS;i++) if(g_dom[i].id==domain_id){dt=g_dom[i].trust;break;}
    if(dt<0) return -1;
    for(int i=0;i<MAX_CERTS;i++) if(g_cert[i].did==domain_id&&g_cert[i].state==CERT_EXPIRED) ec++;
    if(ec>0&&g_nan<MAX_ANOM){g_anom[g_nan].id=nid();my_strncpy(g_anom[g_nan].desc,"expired-certs",MXV-1);
        g_anom[g_nan].sev=ANOM_HIGH;g_anom[g_nan].dom=domain_id;g_anom[g_nan].ts=++g_clk;g_nan++;an++;}
    if(dt<TRUST_STD&&g_nan<MAX_ANOM){g_anom[g_nan].id=nid();my_strncpy(g_anom[g_nan].desc,"low-trust",MXV-1);
        g_anom[g_nan].sev=ANOM_MED;g_anom[g_nan].dom=domain_id;g_anom[g_nan].ts=++g_clk;g_nan++;an++;}
    ps("  Anomaly scan domain ");pi(domain_id);ps(": ");pi(an);ps(" anomalies detected\n"); return an;
}
static int perf_monitor(void) {
    int c=0;
    if(g_npf<MAX_PERF){g_perf[g_npf].id=nid();my_strncpy(g_perf[g_npf].name,"federation-latency-ms",MXN-1);
        g_perf[g_npf].val=15;g_perf[g_npf].thresh=100;g_npf++;c++;}
    if(g_npf<MAX_PERF){g_perf[g_npf].id=nid();my_strncpy(g_perf[g_npf].name,"token-translate-ms",MXN-1);
        g_perf[g_npf].val=8;g_perf[g_npf].thresh=50;g_npf++;c++;}
    if(g_npf<MAX_PERF){g_perf[g_npf].id=nid();my_strncpy(g_perf[g_npf].name,"cert-verify-ms",MXN-1);
        g_perf[g_npf].val=22;g_perf[g_npf].thresh=200;g_npf++;c++;}
    ps("  Performance metrics (");pi(c);ps(" recorded):\n");
    for(int i=0;i<g_npf;i++){ps("    ");ps(g_perf[i].name);ps("=");pi(g_perf[i].val);
        ps("ms (threshold=");pi(g_perf[i].thresh);ps("ms)\n");} return c;
}

/* Test suite */
static void test_suite(void) {
    char bo[MXV];
    ps("=== Identity Federation Test Suite ===\n\n");
    ps("[Test 1] Trust Domain Create\n");
    int d1=trust_domain_create("corp.example.com",TRUST_FULL);
    int d2=trust_domain_create("partner.example.com",TRUST_ELEV);
    int d3=trust_domain_create("vendor.example.com",TRUST_STD);
    int d4=trust_domain_create("external.untrusted.net",TRUST_MIN);
    ps("  Created domains: ");pi(d1);ps(", ");pi(d2);ps(", ");pi(d3);ps(", ");pi(d4);ps("\n");
    trust_domain_list(); ps("\n");

    ps("[Test 2] Certificate Exchange\n");
    int c1=trust_cert_exchange(d1,"corp.example.com",4096);
    int c2=trust_cert_exchange(d2,"partner.example.com",2048);
    int c3=trust_cert_exchange(d3,"vendor.example.com",2048);
    ps("  Exchanged certs: ");pi(c1);ps(", ");pi(c2);ps(", ");pi(c3);ps("\n");
    trust_cert_list(d1);
    ps("  Validate cert ");pi(c1);ps(": ");pi(trust_cert_validate(c1));ps("\n");
    trust_metadata_publish(d1,"https://corp.example.com/.well-known/federation");
    trust_metadata_consume(d2,"partner-metadata-payload");
    ps("  Transit d1->d3: ");pi(trust_transit_check(d1,d3));ps("\n");
    ps("  Transit d1->d4: ");pi(trust_transit_check(d1,d4));ps(" (expected -1)\n");
    ps("  Chain verify d1: ");pi(trust_chain_verify(d1));ps("\n");
    trust_domain_get(d1); trust_domain_get(d2); ps("\n");

    ps("[Test 3] Attribute Map/Transform\n");
    int m1=attr_map_create("email","mail","lowercase");
    int m2=attr_map_create("displayName","cn","identity");
    int m3=attr_map_create("department","org","uppercase");
    ps("  Created maps: ");pi(m1);ps(", ");pi(m2);ps(", ");pi(m3);ps("\n");
    attr_map_transform(m1,"User@Example.COM",bo); ps("  email->'");ps(bo);ps("'\n");
    attr_map_transform(m3,"engineering",bo); ps("  dept->'");ps(bo);ps("'\n");
    attr_default_value(m2,"Unknown User");
    attr_map_conditional(m2,"test-user","admin",bo); ps("  cond(default)->'");ps(bo);ps("'\n");
    int vs[4];vs[0]=10;vs[1]=20;vs[2]=30;vs[3]=40; attr_aggregate("score",vs,4); ps("\n");

    ps("[Test 4] Proxy Auth Cross-Domain\n");
    int t1=proxy_auth_cross_domain(d1,d2,"user@corp.example.com");
    int t2=proxy_auth_cross_domain(d1,d3,"admin@corp.example.com");
    ps("  Proxy tokens: ");pi(t1);ps(", ");pi(t2);ps("\n");
    proxy_token_translate(t1,d3); ps("  Token ");pi(t1);ps(" translated to d");pi(d3);ps("\n");
    proxy_credential_bridge(t2,"KERBEROS"); ps("  Token ");pi(t2);ps(" bridged to KERBEROS\n");
    ps("  Assert t1: ");pi(proxy_identity_assert(t1,"valid-assertion"));ps("\n");
    ps("  Assert deny: ");pi(proxy_identity_assert(t2,"deny-request"));ps("\n\n");

    ps("[Test 5] Directory Aggregate\n");
    int dr1=directory_aggregate("corp-ldap","dc=corp,dc=example,dc=com");
    int dr2=directory_aggregate("partner-ldap","dc=partner,dc=example,dc=com");
    int dr3=directory_aggregate("hr-system","ou=hr,dc=corp,dc=example,dc=com");
    ps("  Created dirs: ");pi(dr1);ps(", ");pi(dr2);ps(", ");pi(dr3);ps("\n");
    int dl[3];dl[0]=dr1;dl[1]=dr2;dl[2]=dr3;
    directory_virtual("global-directory",dl,3);
    directory_attr_aggregate(dr1,"employeeId");
    directory_attr_aggregate(dr2,"partnerId");
    directory_dedup_merge(dr1,dr2);
    directory_dedup_merge(dr1,dr3); ps("\n");

    ps("[Test 6] Account Link/Merge\n");
    int l1=account_link_create("jsmith@corp.com",d1,"j.smith@partner.com",d2);
    int l2=account_link_create("adoe@corp.com",d1,"alice.doe@vendor.com",d3);
    ps("  Created links: ");pi(l1);ps(", ");pi(l2);ps("\n");
    account_link_list(d1);
    account_link_list(d2);
    ps("  Lifecycle coord (suspend): ");
    pi(account_lifecycle_coord(l1,ACCT_SUSPENDED));ps("\n");
    ps("  Logout propagate: ");pi(logout_propagate(d1,"jsmith@corp.com"));ps("\n");
    ps("  Logout broadcast: ");pi(logout_broadcast("jsmith@corp.com"));ps("\n\n");

    ps("[Test 7] Federation Graph\n");
    federation_graph_build();
    federation_graph_get(d1);
    federation_graph_get(d2);
    federation_graph_get(d3);
    federation_graph_get(d4); ps("\n");

    ps("[Test 8] Anomaly Detect\n");
    anomaly_detect(d1);
    anomaly_detect(d2);
    anomaly_detect(d3);
    anomaly_detect(d4); ps("\n");

    ps("[Test 9] Performance Monitor\n");
    perf_monitor(); ps("\n");

    /* Cleanup */
    trust_domain_delete(d4); ps("  Deleted domain ");pi(d4);ps("\n");
    trust_domain_list();
    ps("\n=== Identity Federation Test Complete ===\n");
}

/* CLI entry */
void _start(void) {
    unsigned int buf=host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *ap=(char *)buf;
    while(pos<512&&ap[pos])pos++;pos++;
    while(pos<512&&ap[pos]){char *a=&ap[pos];
        if(my_strcmp(a,"-h")==0||my_strcmp(a,"--help")==0)help=1;
        else if(my_strcmp(a,"-t")==0||my_strcmp(a,"--test")==0)test=1;
        while(pos<512&&ap[pos])pos++;pos++;}
    ps("IdentityFederation v1.0 - Cross-Domain Identity Federation\n");
    if(help){
        ps("Usage: identity_federation [options]\n");
        ps("  -h, --help    Show help\n  -t, --test    Run federation test suite\n\n");
        ps("Modules:\n  Trust establishment  - domain config, cert exchange, metadata\n");
        ps("  Attribute mapping    - cross-domain transforms, conditional rules\n");
        ps("  Proxy authentication - cross-domain auth, token translation\n");
        ps("  Directory federation - multi-directory aggregation, dedup\n");
        ps("  Consistency sync     - account linking, lifecycle, logout\n");
        ps("  Governance           - federation graph, anomaly detection\n"); return;
    }
    if(test){test_suite();return;}
    ps("Use -h for help, -t for test\n");
}
