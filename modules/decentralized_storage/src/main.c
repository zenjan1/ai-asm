/* decentralized_storage: Content-addressed P2P storage (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);

#define MAX_CONTENT 32
#define MAX_SHARDS 16
#define MAX_REPLICAS 8
#define MAX_NODES 16
#define MAX_DHT_K 20
#define MAX_ACL 8
#define MAX_KEYS 8
#define CID_LEN 46
#define HASH_LEN 32
#define SHARD_DATA 256
#define NODE_ID_LEN 16
#define CID_V1 1
#define CODEC_RAW 0x55
#define CODEC_DAG_CBOR 0x71
#define SHA2_256 0x12
#define REPLICA_FACTOR 3
#define DHT_K 8
#define PROOF_INTERVAL 100

typedef struct { int version; int codec; int hash_algo; char cid[CID_LEN+1]; int size; int verified; } content_addr_t;
typedef struct { char data[SHARD_DATA]; int size; char hash[HASH_LEN+1]; int index; } shard_t;
typedef struct { char root_hash[HASH_LEN+1]; int shard_count; shard_t shards[MAX_SHARDS]; int merkle_levels; } merkle_dag_t;
typedef struct { int content_id; content_addr_t cid; merkle_dag_t dag; int replica_count; int node_ids[MAX_REPLICAS]; int encrypted; int acl_id; int created_at; } content_entry_t;
typedef struct { int node_id; char nid[NODE_ID_LEN+1]; int addr_hash; int status; int shard_count; int bandwidth; int storage_used; int uptime; int reputation; } peer_node_t;
typedef struct { int peer_ids[DHT_K]; int count; } dht_bucket_t;
typedef struct { peer_node_t peers[MAX_NODES]; int peer_count; dht_bucket_t routing[MAX_DHT_K]; int local_node; int total_xfers; int total_bytes; } p2p_net_t;
typedef struct { int id; int content_id; int node_id; int shard_idx; int consistency; int verified_at; } replica_t;
typedef struct { int node_id; int p_sub; int p_ver; int p_fail; int bw_earned; int st_earned; int reward; } incentive_t;
typedef struct { int id; int owner; int nodes[MAX_ACL]; int n_count; int rd; int wr; } acl_t;
typedef struct { int id; int owner; char khash[HASH_LEN+1]; int shared[MAX_ACL]; int s_count; int algo; } key_t;

static content_entry_t contents[MAX_CONTENT]; static int content_count = 0; static int next_cid = 1;
static replica_t replicas[MAX_CONTENT * MAX_REPLICAS]; static int rep_count = 0; static int next_rid = 1;
static p2p_net_t net; static int net_init = 0;
static incentive_t incents[MAX_NODES]; static int inc_count = 0;
static acl_t acls[MAX_CONTENT]; static int acl_count = 0;
static key_t keys[MAX_KEYS]; static int key_count = 0; static int next_kid = 1;
static unsigned int clk = 0;

static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) { while(*a&&*b){if(*a!=*b)return *a-*b;a++;b++;} return *a-*b; }
static void my_strcpy(char *d, const char *s) { while(*s)*d++=*s++; *d='\0'; }
static void my_strncpy(char *d, const char *s, int n) { int i=0; while(i<n-1&&s[i]){d[i]=s[i];i++;} d[i]='\0'; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) {
    char b[32]; int p=0;
    if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0';
    else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;
        while(v>0){b[p--]='0'+(v%10);v/=10;}}
    host_print(b);
}
static void phex(const char *h, int n) { for(int i=0;i<n;i++){char c[2]={h[i],0};host_print(c);} }

/* Hash (simulated SHA2-256) */
static void compute_hash(const char *data, int len, char *out) {
    unsigned int h0=0x6a09e667,h1=0xbb67ae85,h2=0x3c6ef372,h3=0xa54ff53a;
    for(int i=0;i<len;i++){
        unsigned int c=(unsigned char)data[i];
        h0=((h0<<5)|(h0>>27))^c^(i*0x01000193);
        h1=((h1<<7)|(h1>>25))^c^(i*0x1000000+0x61);
        h2=((h2<<11)|(h2>>21))^(h0+h1);
        h3=((h3<<13)|(h3>>19))^(h1+h2);
    }
    const char *hex="0123456789abcdef"; unsigned int hs[4]={h0,h1,h2,h3};
    for(int i=0;i<4;i++) for(int j=28;j>=0;j-=4) out[i*8+(7-j/4)]=hex[(hs[i]>>j)&0xf];
    out[32]='\0';
}

/* CID Generation (IPFS-style) */
static void generate_cid(int ver, int codec, const char *data, int sz, content_addr_t *cid) {
    cid->version=ver; cid->codec=codec; cid->hash_algo=SHA2_256; cid->size=sz; cid->verified=0;
    char hash[HASH_LEN+1]; compute_hash(data,sz,hash);
    int p=0; cid->cid[p++]='b';
    if(ver==CID_V1){cid->cid[p++]='a';cid->cid[p++]='f';}
    cid->cid[p++]='r';cid->cid[p++]='a';cid->cid[p++]='w';
    if(codec==CODEC_DAG_CBOR){cid->cid[p++]='d';cid->cid[p++]='c';}
    cid->cid[p++]='0';cid->cid[p++]='0';
    for(int i=0;i<4&&p<CID_LEN;i++) cid->cid[p++]=hash[i];
    while(p<CID_LEN){cid->cid[p]=hash[p%HASH_LEN];p++;}
    cid->cid[CID_LEN]='\0';
}

static int verify_content(int cid, const char *data, int sz) {
    if(cid<1||cid>content_count) return 0;
    content_entry_t *e=&contents[cid-1];
    char comp[HASH_LEN+1]; compute_hash(data,sz,comp);
    int m=1;
    for(int i=0;i<16;i++) if(e->cid.cid[10+i]!=comp[i]){m=0;break;}
    e->cid.verified=m; return m;
}

/* Data Sharding */
static void shard_content(int cid, const char *data, int total) {
    if(cid<1||cid>content_count) return;
    content_entry_t *e=&contents[cid-1];
    int cnt=(total+SHARD_DATA-1)/SHARD_DATA; if(cnt>MAX_SHARDS)cnt=MAX_SHARDS;
    e->dag.shard_count=cnt; e->dag.merkle_levels=0;
    int lv=cnt; while(lv>1){e->dag.merkle_levels++;lv=(lv+1)/2;}
    for(int i=0;i<cnt;i++){
        int off=i*SHARD_DATA, sz=total-off; if(sz>SHARD_DATA)sz=SHARD_DATA;
        e->dag.shards[i].index=i; e->dag.shards[i].size=sz;
        for(int j=0;j<sz;j++) e->dag.shards[i].data[j]=data[off+j];
        compute_hash(e->dag.shards[i].data,sz,e->dag.shards[i].hash);
    }
    char lh[MAX_SHARDS][HASH_LEN+1];
    for(int i=0;i<cnt;i++) my_strcpy(lh[i],e->dag.shards[i].hash);
    int lc=cnt;
    while(lc>1){
        int nc=(lc+1)/2;
        for(int i=0;i<nc;i++){
            char cb[HASH_LEN*2+1]; my_strcpy(cb,lh[i*2]);
            my_strcpy(cb+HASH_LEN,(i*2+1<lc)?lh[i*2+1]:lh[i*2]);
            compute_hash(cb,my_strlen(cb),lh[i]);
        }
        lc=nc;
    }
    my_strcpy(e->dag.root_hash,lh[0]);
}

static int reassemble(int cid, char *out, int *osz) {
    if(cid<1||cid>content_count) return -1;
    content_entry_t *e=&contents[cid-1]; int t=0;
    for(int i=0;i<e->dag.shard_count;i++)
        for(int j=0;j<e->dag.shards[i].size;j++) out[t++]=e->dag.shards[i].data[j];
    *osz=t; return 0;
}

/* P2P Network */
static void p2p_init(void) {
    if(net_init) return;
    for(int i=0;i<MAX_NODES;i++) net.peers[i].status=0;
    for(int i=0;i<MAX_DHT_K;i++) net.routing[i].count=0;
    net.peer_count=0; net.local_node=1; net.total_xfers=0; net.total_bytes=0;
    net_init=1;
}

static int p2p_register(const char *id, int ahash) {
    if(!net_init) p2p_init();
    if(net.peer_count>=MAX_NODES) return -1;
    for(int i=0;i<net.peer_count;i++)
        if(my_strcmp(net.peers[i].nid,id)==0) return net.peers[i].node_id;
    int idx=net.peer_count++;
    net.peers[idx].node_id=idx+1; my_strncpy(net.peers[idx].nid,id,NODE_ID_LEN);
    net.peers[idx].addr_hash=ahash; net.peers[idx].status=1;
    net.peers[idx].shard_count=0; net.peers[idx].bandwidth=100;
    net.peers[idx].storage_used=0; net.peers[idx].uptime=0; net.peers[idx].reputation=50;
    int bkt=ahash%MAX_DHT_K;
    if(net.routing[bkt].count<DHT_K) net.routing[bkt].peer_ids[net.routing[bkt].count++]=idx+1;
    pstr("  [p2p] peer "); pstr(id); pstr(" hash="); pint(ahash);
    pstr(" bkt="); pint(bkt); pstr("\n");
    return net.peers[idx].node_id;
}

static int dht_lookup(int thash) {
    int bkt=thash%MAX_DHT_K;
    if(net.routing[bkt].count==0) return -1;
    return net.routing[bkt].peer_ids[0];
}

/* Replica Management */
static int create_replicas(int cid, int factor) {
    if(cid<1||cid>content_count) return -1;
    if(factor>MAX_REPLICAS) factor=MAX_REPLICAS;
    content_entry_t *e=&contents[cid-1]; int placed=0;
    int best[MAX_REPLICAS]; for(int i=0;i<MAX_REPLICAS;i++) best[i]=-1;
    for(int r=0;r<factor;r++){
        int bi=-1,br=-1;
        for(int p=0;p<net.peer_count;p++){
            if(net.peers[p].status!=1) continue;
            int dup=0; for(int k=0;k<r;k++) if(best[k]==p){dup=1;break;}
            if(dup) continue;
            if(net.peers[p].reputation>br){br=net.peers[p].reputation;bi=p;}
        }
        if(bi<0) break; best[r]=bi;
        if(rep_count>=MAX_CONTENT*MAX_REPLICAS) break;
        int ri=rep_count++;
        replicas[ri].id=next_rid++; replicas[ri].content_id=cid;
        replicas[ri].node_id=net.peers[bi].node_id; replicas[ri].shard_idx=-1;
        replicas[ri].consistency=1; replicas[ri].verified_at=clk;
        e->node_ids[placed++]=net.peers[bi].node_id;
        net.peers[bi].shard_count+=e->dag.shard_count;
        net.peers[bi].storage_used+=e->cid.size;
    }
    e->replica_count=placed; return placed;
}

static int check_consistency(int cid) {
    int ok=1;
    for(int i=0;i<rep_count;i++){
        if(replicas[i].content_id==cid){
            if(clk-replicas[i].verified_at>PROOF_INTERVAL){replicas[i].consistency=0;ok=0;}
        }
    }
    return ok;
}

/* Storage Incentives */
static int find_incentive(int nid) {
    for(int i=0;i<inc_count;i++) if(incents[i].node_id==nid) return i;
    if(inc_count>=MAX_NODES) return -1;
    int i=inc_count++;
    incents[i].node_id=nid; incents[i].p_sub=0; incents[i].p_ver=0;
    incents[i].p_fail=0; incents[i].bw_earned=0; incents[i].st_earned=0; incents[i].reward=0;
    return i;
}

static int submit_proof(int nid) {
    int idx=find_incentive(nid); if(idx<0) return -1;
    incents[idx].p_sub++;
    int pass=((clk*7+nid*13)%10)>0;
    if(pass){incents[idx].p_ver++;incents[idx].st_earned+=10;incents[idx].reward+=10;}
    else incents[idx].p_fail++;
    return pass;
}

static int credit_bandwidth(int nid, int bytes) {
    int idx=find_incentive(nid); if(idx<0) return -1;
    int r=bytes/100; if(r<1)r=1;
    incents[idx].bw_earned+=r; incents[idx].reward+=r; return r;
}

/* Encrypted Storage - ACL */
static int create_acl(int owner, int rd, int wr) {
    if(acl_count>=MAX_CONTENT) return -1;
    int i=acl_count++;
    acls[i].id=i+1; acls[i].owner=owner; acls[i].n_count=0; acls[i].rd=rd; acls[i].wr=wr;
    return acls[i].id;
}

static int acl_allow(int aid, int nid) {
    for(int i=0;i<acl_count;i++)
        if(acls[i].id==aid){if(acls[i].n_count>=MAX_ACL)return -1;
            acls[i].nodes[acls[i].n_count++]=nid; return 0;}
    return -1;
}

static int acl_check(int aid, int nid, int wr) {
    for(int i=0;i<acl_count;i++)
        if(acls[i].id==aid){
            if(acls[i].owner==nid) return 1;
            for(int j=0;j<acls[i].n_count;j++)
                if(acls[i].nodes[j]==nid){if(wr&&!acls[i].wr)return 0; return acls[i].rd;}
            return 0;
        }
    return 0;
}

/* Key Distribution */
static int generate_key(int owner, int algo) {
    if(key_count>=MAX_KEYS) return -1;
    int i=key_count++;
    keys[i].id=next_kid++; keys[i].owner=owner; keys[i].s_count=0; keys[i].algo=algo;
    char seed[32]; for(int j=0;j<31;j++) seed[j]='0'+((owner*7+j*13+algo)%10); seed[31]='\0';
    compute_hash(seed,31,keys[i].khash);
    return keys[i].id;
}

static int share_key(int kid, int with) {
    for(int i=0;i<key_count;i++)
        if(keys[i].id==kid){if(keys[i].s_count>=MAX_ACL)return -1;
            keys[i].shared[keys[i].s_count++]=with;
            pstr("  [crypto] key "); pint(kid); pstr(" shared -> node "); pint(with); pstr("\n");
            return 0;}
    return -1;
}

static int crypto_xor(int kid, char *data, int sz) {
    for(int i=0;i<key_count;i++)
        if(keys[i].id==kid){for(int j=0;j<sz;j++) data[j]=data[j]^keys[i].khash[j%HASH_LEN]; return 0;}
    return -1;
}

/* Content Store */
static int put_content(const char *data, int sz, int enc, int owner) {
    if(content_count>=MAX_CONTENT) return -1;
    int idx=content_count++, cid=next_cid++;
    contents[idx].content_id=cid; contents[idx].created_at=clk;
    generate_cid(CID_V1,CODEC_RAW,data,sz,&contents[idx].cid);
    shard_content(cid,data,sz);
    if(enc&&key_count>0){
        int kid=keys[key_count-1].id;
        for(int s=0;s<contents[idx].dag.shard_count;s++)
            crypto_xor(kid,contents[idx].dag.shards[s].data,contents[idx].dag.shards[s].size);
        contents[idx].encrypted=1; contents[idx].acl_id=create_acl(owner,1,1);
    } else { contents[idx].encrypted=0; contents[idx].acl_id=0; }
    pstr("  [store] content "); pint(cid); pstr(" CID="); phex(contents[idx].cid.cid,20);
    pstr("... sz="); pint(sz); pstr(" shards="); pint(contents[idx].dag.shard_count);
    if(enc) pstr(" ENC"); pstr("\n");
    return cid;
}

/* Status */
static void show_status(void) {
    pstr("=== Storage Status ===\n");
    pstr("  Contents: "); pint(content_count); pstr("  Replicas: "); pint(rep_count);
    pstr("  Peers: "); pint(net.peer_count); pstr("\n");
    pstr("  Xfers: "); pint(net.total_xfers); pstr("  Bytes: "); pint(net.total_bytes);
    pstr("  ACLs: "); pint(acl_count); pstr("  Keys: "); pint(key_count); pstr("\n");
    if(content_count>0){
        pstr("--- Content ---\n");
        for(int i=0;i<content_count;i++){
            pstr("  ["); pint(contents[i].content_id); pstr("] ");
            phex(contents[i].cid.cid,16); pstr("... "); pint(contents[i].cid.size); pstr("B ");
            pint(contents[i].dag.shard_count); pstr("sh "); pint(contents[i].replica_count); pstr("rep";
            if(contents[i].encrypted) pstr(" [ENC]"); pstr("\n");
        }
    }
    if(net.peer_count>0){
        pstr("--- Peers ---\n");
        for(int i=0;i<net.peer_count;i++){
            pstr("  "); pstr(net.peers[i].nid); pstr(" rep="); pint(net.peers[i].reputation);
            pstr(" bw="); pint(net.peers[i].bandwidth); pstr(" sh="); pint(net.peers[i].shard_count); pstr("\n");
        }
    }
    if(inc_count>0){
        pstr("--- Incentives ---\n");
        for(int i=0;i<inc_count;i++){
            pstr("  n="); pint(incents[i].node_id); pstr(" p="); pint(incents[i].p_ver);
            pstr("/"); pint(incents[i].p_sub); pstr(" rew="); pint(incents[i].reward); pstr("\n");
        }
    }
}

/* Test */
static void run_test(void) {
    pstr("=== Decentralized Storage Test ===\n\n");
    p2p_init();
    pstr("[1] Register Peers\n");
    int p1=p2p_register("QmNode001",1001), p2=p2p_register("QmNode002",2002);
    int p3=p2p_register("QmNode003",3003), p4=p2p_register("QmNode004",4004);
    pstr("\n[2] Key Generation\n");
    int k1=generate_key(p1,0x01); share_key(k1,p2); share_key(k1,p3);
    pstr("\n[3] Store Content\n");
    char d1[512]; for(int i=0;i<511;i++) d1[i]='A'+(i%26); d1[511]='\0';
    int c1=put_content(d1,512,0,p1);
    char d2[200]; for(int i=0;i<199;i++) d2[i]='0'+(i%10); d2[199]='\0';
    int c2=put_content(d2,200,1,p1);
    pstr("\n[4] Verification\n");
    int v1=verify_content(c1,d1,512); pstr("  c1: "); pstr(v1?"PASS":"FAIL"); pstr("\n");
    int v2=verify_content(c2,d2,200); pstr("  c2(enc): "); pstr(v2?"PASS":"FAIL"); pstr("\n");
    pstr("\n[5] Replicas (factor="); pint(REPLICA_FACTOR); pstr(")\n");
    int r1=create_replicas(c1,REPLICA_FACTOR); pstr("  c1: "); pint(r1); pstr(" replicas\n");
    int r2=create_replicas(c2,2); pstr("  c2: "); pint(r2); pstr(" replicas\n");
    pstr("  consistency: "); pstr(check_consistency(c1)?"OK":"STALE"); pstr("\n");
    pstr("\n[6] Shard Reassembly\n");
    char rbuf[1024]; int rsz=0;
    int rc=reassemble(c2,rbuf,&rsz);
    pstr("  reassembled: "); pint(rsz); pstr("B");
    if(rc==0){crypto_xor(k1,rbuf,rsz>64?64:rsz); pstr(" (decrypted 64B)");}
    pstr("\n");
    pstr("\n[7] Access Control\n");
    if(contents[c2-1].acl_id>0){
        int aid=contents[c2-1].acl_id;
        pstr("  owner: "); pint(acl_check(aid,p1,1)); pstr("\n");
        acl_allow(aid,p2); pstr("  p2 read: "); pint(acl_check(aid,p2,0)); pstr("\n");
        pstr("  p4 read: "); pint(acl_check(aid,p4,0)); pstr(" (denied)\n");
    }
    pstr("\n[8] DHT Routing\n");
    int f=dht_lookup(3003); pstr("  lookup(3003): "); pint(f); pstr("\n");
    f=dht_lookup(9999); pstr("  lookup(9999): "); pint(f); pstr("\n");
    pstr("\n[9] Proof of Storage\n");
    for(int r=0;r<5;r++){submit_proof(p1);submit_proof(p2);submit_proof(p3);}
    credit_bandwidth(p1,5000); credit_bandwidth(p2,3000); credit_bandwidth(p3,1000);
    clk+=PROOF_INTERVAL+1;
    pstr("\n[10] Consistency Recheck\n");
    pstr("  "); pstr(check_consistency(c1)?"OK":"STALE-refresh"); pstr("\n\n");
    show_status();
    pstr("\n=== Test Complete ===\n");
}

void _start(void) {
    unsigned int buf=host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]){char *a=&av[pos]; int l=my_strlen(a);
        if(l==2&&a[0]=='-'&&a[1]=='h') help=1;
        else if(l==2&&a[0]=='-'&&a[1]=='t') test=1;
        while(pos<512&&av[pos]) pos++; pos++;}
    pstr("Decentralized Storage v1.0 - Content-Addressed P2P\n");
    if(help){
        pstr("Usage: decentralized_storage [-h|-t]\n");
        pstr("  -h  Show this help\n");
        pstr("  -t  Run integration test\n\n");
        pstr("Features:\n");
        pstr("  Content addressing  CID generation, IPFS protocol, verification\n");
        pstr("  Data sharding       File splitting, Merkle DAG, reassembly\n");
        pstr("  Replica management  Multi-replica, placement, consistency\n");
        pstr("  P2P network         Node discovery, DHT routing, transfers\n");
        pstr("  Storage incentives  Proof of storage, bandwidth rewards\n");
        pstr("  Encrypted storage   E2E encryption, ACL, key distribution\n");
        return;
    }
    if(test){run_test(); return;}
    pstr("Use -h for help, -t for test\n");
}
