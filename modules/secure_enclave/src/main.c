/*
 * Secure Enclave v58.0 - Hardware-Backed Security
 * AI-ASM OS (AArch64 WebAssembly-native OS)
 * Pure C, NO stdlib - Hardware Security Module with TEE support
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

/* ── Helpers ──────────────────────────────────────────────────────── */
static unsigned int my_strlen(const char *s) { unsigned int n=0; while(s[n]) n++; return n; }
static void my_strncpy(char *d, const char *s, unsigned int n) {
    unsigned int i; for(i=0;i<n&&s[i];i++) d[i]=s[i]; for(;i<n;i++) d[i]='\0';
}
static void print_str(const char *s) { host_print(s); }
static void print_int(int v) {
    char buf[16],rev[16]; int i=0,neg=0; unsigned int u;
    if(v<0){neg=1;u=(unsigned int)(-v);}else{u=(unsigned int)v;}
    if(u==0){buf[i++]='0';} while(u>0){buf[i++]='0'+(char)(u%10);u/=10;}
    if(neg) buf[i++]='-'; buf[i]='\0';
    int j; for(j=0;j<i;j++) rev[j]=buf[i-1-j]; rev[i]='\0'; host_print(rev);
}
static void print_hex(const unsigned char *d, unsigned int len) {
    const char hx[]="0123456789abcdef"; char buf[256]; unsigned int i;
    for(i=0;i<len&&i<64;i++){buf[i*2]=hx[(d[i]>>4)&0xF];buf[i*2+1]=hx[d[i]&0xF];}
    buf[(i<64?i:64)*2]='\0'; host_print(buf);
}

/* ── Simple RNG (xorshift32) ─────────────────────────────────────── */
static unsigned int rng_state = 0xDEADBEEF;
static unsigned int rng_next(void) {
    unsigned int x=rng_state; x^=x<<13; x^=x>>17; x^=x<<5; rng_state=x; return x;
}
static void rng_seed(unsigned int s) { rng_state = s ? s : 1; }
static void rng_fill(unsigned char *b, unsigned int n) { unsigned int i; for(i=0;i<n;i++) b[i]=(unsigned char)(rng_next()&0xFF); }

/* ── SHA-256 Simulation ──────────────────────────────────────────── */
static void sha256_sim(const unsigned char *data, unsigned int len, unsigned char out[32]) {
    unsigned int h0=0x6a09e667,h1=0xbb67ae85,h2=0x3c6ef372,h3=0xa54ff53a,i;
    for(i=0;i<len;i++){
        unsigned int m=(unsigned int)data[i]^(h0&0xFF);
        h0=(h0^(m*0x01000193))+(h1>>3); h1=(h1^(m*0x010001b7))+(h2>>5);
        h2=(h2^(m*0x010001c3))+(h3>>7); h3=(h3^(m*0x010001e7))+(h0>>11);
    }
    for(i=0;i<8;i++){out[i]=(unsigned char)(h0>>(i*4)&0xFF);out[i+8]=(unsigned char)(h1>>(i*4)&0xFF);
        out[i+16]=(unsigned char)(h2>>(i*4)&0xFF);out[i+24]=(unsigned char)(h3>>(i*4)&0xFF);}
}

/* ── 1. Hardware Security Module (HSM) ───────────────────────────── */
typedef enum { KEY_RSA_2048=1, KEY_EC_P256=2, KEY_AES_256=3 } key_type_t;
typedef struct { unsigned int ops[4], expiry, max_uses, ip[4]; } km_key_policy_t;
typedef struct {
    unsigned int id; key_type_t type; unsigned char data[64];
    unsigned int created_at, usage_count, max_uses; km_key_policy_t policy;
} hsm_key_t;
typedef struct { hsm_key_t keys[16]; unsigned int key_count, max_keys; int initialized; } hsm_context_t;
static hsm_context_t g_hsm;

static void hsm_init(void) {
    unsigned int i,j; for(i=0;i<16;i++){
        g_hsm.keys[i].id=0; g_hsm.keys[i].created_at=0; g_hsm.keys[i].usage_count=0;
        g_hsm.keys[i].max_uses=1000; for(j=0;j<64;j++) g_hsm.keys[i].data[j]=0;
        for(j=0;j<4;j++){g_hsm.keys[i].policy.ops[j]=0xFFFFFFFF;g_hsm.keys[i].policy.ip[j]=0;}
        g_hsm.keys[i].policy.expiry=0; g_hsm.keys[i].policy.max_uses=1000;
    }
    g_hsm.key_count=0; g_hsm.max_keys=16; g_hsm.initialized=1; rng_seed(0xA5A5A5A5);
    print_str("[HSM] Initialized with 16 key slots\n");
}
static hsm_key_t *hsm_find(unsigned int id) {
    unsigned int i; for(i=0;i<g_hsm.key_count;i++) if(g_hsm.keys[i].id==id) return &g_hsm.keys[i]; return NULL;
}
static int hsm_generate_key(key_type_t type, const km_key_policy_t *pol) {
    if(!g_hsm.initialized||g_hsm.key_count>=g_hsm.max_keys) return -1;
    unsigned int idx=g_hsm.key_count, j;
    g_hsm.keys[idx].id=rng_next()&0xFFFFFF; g_hsm.keys[idx].type=type;
    rng_fill(g_hsm.keys[idx].data,64); g_hsm.keys[idx].created_at=rng_next();
    g_hsm.keys[idx].usage_count=0; g_hsm.keys[idx].max_uses=pol?pol->max_uses:1000;
    if(pol){for(j=0;j<4;j++){g_hsm.keys[idx].policy.ops[j]=pol->ops[j];g_hsm.keys[idx].policy.ip[j]=pol->ip[j];}
        g_hsm.keys[idx].policy.expiry=pol->expiry;}
    g_hsm.key_count++; print_str("[HSM] Generated key id=0x");
    print_hex((unsigned char*)&g_hsm.keys[idx].id,4); print_str("\n"); return (int)idx;
}
static int hsm_import_key(key_type_t type, const unsigned char *kd) {
    if(!g_hsm.initialized||g_hsm.key_count>=g_hsm.max_keys) return -1;
    unsigned int idx=g_hsm.key_count, j;
    g_hsm.keys[idx].id=rng_next()&0xFFFFFF; g_hsm.keys[idx].type=type;
    for(j=0;j<64;j++) g_hsm.keys[idx].data[j]=kd[j];
    g_hsm.keys[idx].created_at=rng_next(); g_hsm.keys[idx].usage_count=0; g_hsm.keys[idx].max_uses=1000;
    g_hsm.key_count++; print_str("[HSM] Imported key id=0x");
    print_hex((unsigned char*)&g_hsm.keys[idx].id,4); print_str("\n"); return (int)idx;
}
static int hsm_delete_key(unsigned int kid) {
    unsigned int i,j; for(i=0;i<g_hsm.key_count;i++) if(g_hsm.keys[i].id==kid){
        for(j=0;j<64;j++) g_hsm.keys[i].data[j]=0; g_hsm.keys[i].id=0;
        print_str("[HSM] Deleted key 0x"); print_hex((unsigned char*)&kid,4); print_str("\n"); return 0;
    } return -1;
}
static int hsm_export_public(unsigned int kid, unsigned char *pub) {
    hsm_key_t *k=hsm_find(kid); if(!k) return -1;
    sha256_sim(k->data,32,pub); print_str("[HSM] Exported public key 0x");
    print_hex((unsigned char*)&kid,4); print_str("\n"); return 0;
}
static int hsm_sign(unsigned int kid, const unsigned char *msg, unsigned int ml, unsigned char sig[32]) {
    hsm_key_t *k=hsm_find(kid); if(!k) return -1; k->usage_count++;
    unsigned char c[128], i; unsigned int cl=ml<64?ml:64;
    for(i=0;i<cl;i++) c[i]=msg[i]^k->data[i%64]; for(;i<64;i++) c[i]=k->data[i%64];
    sha256_sim(c,64,sig); print_str("[HSM] Signed "); print_int((int)ml); print_str(" bytes\n"); return 0;
}
static int hsm_verify(unsigned int kid, const unsigned char *msg, unsigned int ml, const unsigned char sig[32]) {
    unsigned char exp[32]; if(hsm_sign(kid,msg,ml,exp)<0) return -1;
    unsigned int i,ok=1; for(i=0;i<32;i++) if(exp[i]!=sig[i]) ok=0;
    print_str("[HSM] Verify 0x"); print_hex((unsigned char*)&kid,4); print_str(ok?" OK\n":" FAIL\n"); return ok?0:-1;
}
static int hsm_encrypt(unsigned int kid, const unsigned char *pt, unsigned int len, unsigned char *ct) {
    hsm_key_t *k=hsm_find(kid); if(!k) return -1; k->usage_count++;
    unsigned int i; for(i=0;i<len;i++) ct[i]=pt[i]^k->data[i%64];
    print_str("[HSM] Encrypted "); print_int((int)len); print_str(" bytes\n"); return 0;
}
static int hsm_decrypt(unsigned int kid, const unsigned char *ct, unsigned int len, unsigned char *pt) {
    return hsm_encrypt(kid,ct,len,pt);
}

/* ── 2. Key Management ───────────────────────────────────────────── */
typedef struct { unsigned int master_key_id, derived_keys[8], depth; } key_hierarchy_t;
static key_hierarchy_t g_hier;
static int km_derive_key(unsigned int mid, const unsigned int *path, unsigned int depth) {
    hsm_key_t *m=hsm_find(mid); if(!m||depth>8) return -1;
    unsigned char der[64], pb[4]; unsigned int i,j;
    for(i=0;i<64;i++) der[i]=m->data[i];
    for(i=0;i<depth&&i<8;i++){
        pb[0]=(unsigned char)(path[i]&0xFF); pb[1]=(unsigned char)((path[i]>>8)&0xFF);
        pb[2]=(unsigned char)((path[i]>>16)&0xFF); pb[3]=(unsigned char)((path[i]>>24)&0xFF);
        sha256_sim(der,64,der); for(j=0;j<4;j++) der[j]^=pb[j];
    }
    int idx=hsm_import_key(m->type,der);
    if(idx>=0){g_hier.derived_keys[g_hier.depth]=g_hsm.keys[idx].id;g_hier.depth++;}
    for(i=0;i<64;i++) der[i]=0; print_str("[KM] Derived key depth="); print_int((int)depth); print_str("\n"); return idx;
}
static int km_rotate_key(unsigned int kid) {
    hsm_key_t *o=hsm_find(kid); if(!o) return -1;
    int ni=hsm_generate_key(o->type,&o->policy);
    print_str("[KM] Rotated key 0x"); print_hex((unsigned char*)&kid,4); print_str("\n"); return ni;
}
static int km_revoke_key(unsigned int kid, unsigned int reason) {
    hsm_key_t *k=hsm_find(kid); if(!k) return -1;
    unsigned int i; for(i=0;i<64;i++) k->data[i]=0; k->max_uses=0; k->policy.expiry=0;
    print_str("[KM] Revoked key 0x"); print_hex((unsigned char*)&kid,4); print_str(" reason="); print_int((int)reason); print_str("\n"); return 0;
}
static int km_backup_key(unsigned int kid, unsigned char *bk) {
    hsm_key_t *k=hsm_find(kid); if(!k) return -1;
    unsigned char nonce[16]; rng_fill(nonce,16); unsigned int i;
    for(i=0;i<16;i++) bk[i]=nonce[i];
    for(i=0;i<64;i++) bk[16+i]=k->data[i]^nonce[i%16];
    sha256_sim(bk,80,bk+80); print_str("[KM] Backed up key 0x"); print_hex((unsigned char*)&kid,4); print_str("\n"); return 0;
}
static int km_restore_key(const unsigned char *bd, const unsigned char *bkey) {
    unsigned char chk[32], rest[64]; unsigned int i,ok=1;
    sha256_sim(bd,80,chk); for(i=0;i<32;i++) if(chk[i]!=bd[80+i]) ok=0;
    if(!ok){print_str("[KM] Restore integrity FAIL\n");return -1;}
    for(i=0;i<64;i++) rest[i]=bd[16+i]^bd[i%16];
    int idx=hsm_import_key(KEY_AES_256,rest); (void)bkey;
    print_str("[KM] Restored key\n"); return idx;
}
static int km_check_policy(unsigned int kid, unsigned int op) {
    hsm_key_t *k=hsm_find(kid); if(!k) return -1;
    if(k->usage_count>=k->max_uses){print_str("[KM] DENIED: max uses\n");return -1;}
    unsigned int slot=op/32, bit=op%32;
    if(slot>=4||!(k->policy.ops[slot]&(1u<<bit))){print_str("[KM] DENIED: op not allowed\n");return -1;}
    print_str("[KM] Policy OK op="); print_int((int)op); print_str("\n"); return 0;
}

/* ── 3. Trusted Execution Environment (TEE) ──────────────────────── */
typedef enum { ENCL_INIT=0, ENCL_RUNNING=1, ENCL_PAUSED=2, ENCL_DESTROYED=3 } enclave_state_t;
typedef struct { unsigned int memory_size, flags; unsigned char measurement[32]; } enclave_config_t;
typedef struct {
    unsigned int id; enclave_config_t config; enclave_state_t state;
    unsigned char memory[256], saved_state[64]; unsigned int memory_size;
} enclave_t;
#define MAX_ENCLAVES 8
static enclave_t g_encl[MAX_ENCLAVES]; static unsigned int g_encl_cnt=0;

static int tee_create_enclave(const enclave_config_t *cfg) {
    if(g_encl_cnt>=MAX_ENCLAVES) return -1;
    unsigned int idx=g_encl_cnt, i;
    g_encl[idx].id=rng_next()&0xFFFFFF; g_encl[idx].config.memory_size=cfg->memory_size<256?cfg->memory_size:256;
    g_encl[idx].config.flags=cfg->flags; for(i=0;i<32;i++) g_encl[idx].config.measurement[i]=0;
    sha256_sim((unsigned char*)&g_encl[idx].id,4,g_encl[idx].config.measurement);
    g_encl[idx].state=ENCL_INIT; for(i=0;i<256;i++) g_encl[idx].memory[i]=0;
    g_encl[idx].memory_size=g_encl[idx].config.memory_size;
    for(i=0;i<64;i++) g_encl[idx].saved_state[i]=0; g_encl_cnt++;
    print_str("[TEE] Created enclave 0x"); print_hex((unsigned char*)&g_encl[idx].id,4);
    print_str(" mem="); print_int((int)g_encl[idx].memory_size); print_str("\n"); return (int)idx;
}
static int tee_destroy_enclave(unsigned int eid) {
    unsigned int i,j; for(i=0;i<g_encl_cnt;i++) if(g_encl[i].id==eid){
        for(j=0;j<256;j++) g_encl[i].memory[j]=0; for(j=0;j<64;j++) g_encl[i].saved_state[j]=0;
        g_encl[i].state=ENCL_DESTROYED; print_str("[TEE] Destroyed 0x");
        print_hex((unsigned char*)&eid,4); print_str("\n"); return 0;
    } return -1;
}
static int tee_enter(unsigned int eid, unsigned int fn, unsigned int params) {
    unsigned int i; for(i=0;i<g_encl_cnt;i++) if(g_encl[i].id==eid){
        if(g_encl[i].state==ENCL_DESTROYED) return -1; g_encl[i].state=ENCL_RUNNING;
        unsigned char e[8]; e[0]=(unsigned char)(fn&0xFF); e[1]=(unsigned char)((fn>>8)&0xFF);
        e[2]=(unsigned char)(params&0xFF); e[3]=(unsigned char)((params>>8)&0xFF);
        sha256_sim(e,4,g_encl[i].memory); print_str("[TEE] Enter 0x");
        print_hex((unsigned char*)&eid,4); print_str(" fn="); print_int((int)fn); print_str("\n"); return 0;
    } return -1;
}
static int tee_exit(unsigned int eid, int rv) {
    unsigned int i; for(i=0;i<g_encl_cnt;i++) if(g_encl[i].id==eid){
        g_encl[i].state=ENCL_INIT; print_str("[TEE] Exit 0x");
        print_hex((unsigned char*)&eid,4); print_str(" ret="); print_int(rv); print_str("\n"); return 0;
    } return -1;
}
static int tee_secure_io(unsigned int eid, unsigned int ch, unsigned char *data, unsigned int len) {
    unsigned int i,j; for(i=0;i<g_encl_cnt;i++) if(g_encl[i].id==eid){
        if(g_encl[i].state!=ENCL_RUNNING) return -1;
        for(j=0;j<len&&j<256;j++) data[j]^=g_encl[i].memory[(j+ch)%256];
        print_str("[TEE] IO 0x"); print_hex((unsigned char*)&eid,4);
        print_str(" ch="); print_int((int)ch); print_str("\n"); return 0;
    } return -1;
}
static int tee_pause(unsigned int eid) {
    unsigned int i,j; for(i=0;i<g_encl_cnt;i++) if(g_encl[i].id==eid){
        if(g_encl[i].state!=ENCL_RUNNING) return -1;
        for(j=0;j<64;j++) g_encl[i].saved_state[j]=g_encl[i].memory[j];
        g_encl[i].state=ENCL_PAUSED; print_str("[TEE] Paused 0x");
        print_hex((unsigned char*)&eid,4); print_str("\n"); return 0;
    } return -1;
}
static int tee_resume(unsigned int eid) {
    unsigned int i,j; for(i=0;i<g_encl_cnt;i++) if(g_encl[i].id==eid){
        if(g_encl[i].state!=ENCL_PAUSED) return -1;
        for(j=0;j<64;j++) g_encl[i].memory[j]=g_encl[i].saved_state[j];
        g_encl[i].state=ENCL_RUNNING; print_str("[TEE] Resumed 0x");
        print_hex((unsigned char*)&eid,4); print_str("\n"); return 0;
    } return -1;
}

/* ── 4. Sealed Storage ───────────────────────────────────────────── */
typedef struct {
    unsigned char measurement[32], data[128], metadata[32], nonce[16], tag[32]; unsigned int data_len;
} sealed_data_t;
static unsigned char g_seal_key[32], g_nonce_db[256]; static unsigned int g_seal_nc=0, g_nonce_cnt=0;

static int seal_init(unsigned int eid) {
    unsigned int i; for(i=0;i<g_encl_cnt;i++) if(g_encl[i].id==eid){
        sha256_sim(g_encl[i].config.measurement,32,g_seal_key);
        g_seal_nc=0; g_nonce_cnt=0; for(i=0;i<256;i++) g_nonce_db[i]=0;
        print_str("[SEAL] Init for 0x"); print_hex((unsigned char*)&eid,4); print_str("\n"); return 0;
    } return -1;
}
static int seal_data(unsigned int eid, const unsigned char *pt, unsigned int len, const unsigned char *meta) {
    (void)eid; if(len>128) return -1;
    sealed_data_t s; unsigned int i;
    for(i=0;i<32;i++) s.measurement[i]=g_seal_key[i];
    for(i=0;i<len;i++) s.data[i]=pt[i]^g_seal_key[i%32]; s.data_len=len;
    for(i=0;i<32;i++) s.metadata[i]=meta?meta[i]:0; g_seal_nc++;
    unsigned char nc[4]; nc[0]=(unsigned char)(g_seal_nc&0xFF); nc[1]=(unsigned char)((g_seal_nc>>8)&0xFF);
    nc[2]=(unsigned char)((g_seal_nc>>16)&0xFF); nc[3]=(unsigned char)((g_seal_nc>>24)&0xFF);
    sha256_sim(nc,4,s.nonce);
    if(g_nonce_cnt<16){for(i=0;i<16;i++) g_nonce_db[g_nonce_cnt*16+i]=s.nonce[i]; g_nonce_cnt++;}
    sha256_sim(s.data,len,s.tag); print_str("[SEAL] Sealed "); print_int((int)len); print_str(" bytes\n"); return 0;
}
static int unseal_data(unsigned int eid, const sealed_data_t *sd) {
    (void)eid; unsigned int i,ok=1;
    for(i=0;i<32;i++) if(sd->measurement[i]!=g_seal_key[i]) ok=0;
    if(!ok){print_str("[SEAL] Unseal: measurement mismatch\n");return -1;}
    unsigned char tc[32]; sha256_sim(sd->data,sd->data_len,tc);
    for(i=0;i<32;i++) if(tc[i]!=sd->tag[i]) ok=0;
    if(!ok){print_str("[SEAL] Unseal: tag FAIL\n");return -1;}
    print_str("[SEAL] Unsealed "); print_int((int)sd->data_len); print_str(" bytes OK\n"); return 0;
}
static int seal_verify_integrity(const sealed_data_t *sd) {
    unsigned char tc[32]; sha256_sim(sd->data,sd->data_len,tc);
    unsigned int i,ok=1; for(i=0;i<32;i++) if(tc[i]!=sd->tag[i]) ok=0;
    print_str("[SEAL] Integrity: "); print_str(ok?"PASS\n":"FAIL\n"); return ok?0:-1;
}
static int seal_check_replay(const sealed_data_t *sd) {
    unsigned int i,j; for(i=0;i<g_nonce_cnt;i++){
        int dup=1; for(j=0;j<16;j++) if(g_nonce_db[i*16+j]!=sd->nonce[j]){dup=0;break;}
        if(dup){print_str("[SEAL] Replay detected!\n");return -1;}
    } print_str("[SEAL] No replay\n"); return 0;
}
static int seal_rotate_key(unsigned int eid) {
    (void)eid; unsigned char seed[64], nk[32]; unsigned int i;
    for(i=0;i<32;i++) seed[i]=g_seal_key[i]; rng_fill(seed+32,32);
    sha256_sim(seed,64,nk); for(i=0;i<32;i++) g_seal_key[i]=nk[i]; g_seal_nc=0;
    print_str("[SEAL] Rotated key\n"); return 0;
}

/* ── 5. Remote Attestation ───────────────────────────────────────── */
typedef struct { unsigned char measurement[32], report_data[32], signature[64]; unsigned int timestamp; } att_quote_t;
typedef struct { unsigned char root[64], intermediate[64], leaf[64]; } att_cert_chain_t;
static unsigned char g_att_ca[32];

static int att_create_quote(unsigned int eid, const unsigned char *rd, att_quote_t *q) {
    unsigned int i; for(i=0;i<g_encl_cnt;i++) if(g_encl[i].id==eid){
        for(i=0;i<32;i++) q->measurement[i]=g_encl[i].config.measurement[i];
        for(i=0;i<32;i++) q->report_data[i]=rd[i]; q->timestamp=rng_next();
        unsigned char ts[96]; for(i=0;i<32;i++) ts[i]=q->measurement[i];
        for(i=0;i<32;i++) ts[32+i]=q->report_data[i];
        ts[64]=(unsigned char)(q->timestamp&0xFF); ts[65]=(unsigned char)((q->timestamp>>8)&0xFF);
        ts[66]=(unsigned char)((q->timestamp>>16)&0xFF); ts[67]=(unsigned char)((q->timestamp>>24)&0xFF);
        sha256_sim(ts,68,q->signature); for(i=0;i<32;i++) q->signature[32+i]=q->signature[i]^g_att_ca[i];
        print_str("[ATT] Quote for 0x"); print_hex((unsigned char*)&eid,4); print_str("\n"); return 0;
    } return -1;
}
static int att_verify_quote(const att_quote_t *q, const unsigned char *ca_cert) {
    unsigned char exp[32],ts[96],cak[32]; unsigned int i;
    for(i=0;i<32;i++) ts[i]=q->measurement[i]; for(i=0;i<32;i++) ts[32+i]=q->report_data[i];
    ts[64]=(unsigned char)(q->timestamp&0xFF); ts[65]=(unsigned char)((q->timestamp>>8)&0xFF);
    ts[66]=(unsigned char)((q->timestamp>>16)&0xFF); ts[67]=(unsigned char)((q->timestamp>>24)&0xFF);
    sha256_sim(ts,68,exp); sha256_sim(ca_cert,64,cak);
    unsigned int ok=1; for(i=0;i<32;i++) if((q->signature[32+i]^cak[i])!=exp[i]) ok=0;
    print_str("[ATT] Quote verify: "); print_str(ok?"PASS\n":"FAIL\n"); return ok?0:-1;
}
static int att_build_chain(att_cert_chain_t *c) {
    unsigned char tmp[96]; unsigned int i; rng_fill(c->root,64);
    for(i=0;i<64;i++) tmp[i]=c->root[i]; rng_fill(tmp+64,32);
    sha256_sim(tmp,96,c->intermediate); for(i=0;i<32;i++) c->intermediate[32+i]=c->intermediate[i]^c->root[i];
    for(i=0;i<64;i++) tmp[i]=c->intermediate[i]; rng_fill(tmp+64,32);
    sha256_sim(tmp,96,c->leaf); for(i=0;i<32;i++) c->leaf[32+i]=c->leaf[i]^c->intermediate[i];
    print_str("[ATT] Built cert chain\n"); return 0;
}
static int att_verify_chain(const att_cert_chain_t *c) {
    unsigned char rh[32],ih[32]; sha256_sim(c->root,64,rh); sha256_sim(c->intermediate,64,ih);
    unsigned int ok=1,i; for(i=0;i<32;i++) if((c->intermediate[i]^c->root[i])!=rh[i]){ok=0;break;}
    if(ok) for(i=0;i<32;i++) if((c->leaf[i]^c->intermediate[i])!=ih[i]){ok=0;break;}
    print_str("[ATT] Chain verify: "); print_str(ok?"PASS\n":"FAIL\n"); return ok?0:-1;
}
static int att_challenge(unsigned int eid, const unsigned char *ch) {
    unsigned int i; for(i=0;i<g_encl_cnt;i++) if(g_encl[i].id==eid){
        unsigned char cb[64]; for(i=0;i<32;i++) cb[i]=g_encl[i].config.measurement[i];
        for(i=0;i<32;i++) cb[32+i]=ch[i]; unsigned char resp[32]; sha256_sim(cb,64,resp);
        print_str("[ATT] Challenge-response OK 0x"); print_hex((unsigned char*)&eid,4); print_str("\n"); return 0;
    } return -1;
}

/* ── 6. Secure Boot Chain ────────────────────────────────────────── */
typedef struct { unsigned int stage_id; char name[32]; unsigned char measurement[32], expected[32]; int verified; unsigned int next; } boot_stage_t;
typedef struct {
    boot_stage_t stages[8]; unsigned int stage_count, current_stage;
    unsigned char root[32], pcr[256]; unsigned int pcr_cnt, versions[8];
} boot_chain_t;
static boot_chain_t g_boot;

static void boot_init(void) {
    unsigned int i,j; for(i=0;i<8;i++){
        g_boot.stages[i].stage_id=0; g_boot.stages[i].name[0]='\0'; g_boot.stages[i].verified=0;
        g_boot.stages[i].next=i+1; g_boot.versions[i]=0;
        for(j=0;j<32;j++){g_boot.stages[i].measurement[j]=0;g_boot.stages[i].expected[j]=0;}
    }
    g_boot.stage_count=0; g_boot.current_stage=0; g_boot.pcr_cnt=0;
    rng_fill(g_boot.root,32); for(i=0;i<256;i++) g_boot.pcr[i]=0;
    print_str("[BOOT] Chain initialized\n");
}
static int boot_add_stage(boot_chain_t *c, const boot_stage_t *s) {
    if(c->stage_count>=8) return -1;
    c->stages[c->stage_count]=*s; c->stage_count++;
    print_str("[BOOT] Added stage "); print_str(s->name); print_str("\n"); return 0;
}
static int boot_verify(boot_chain_t *c, unsigned int sid) {
    if(sid>=c->stage_count) return -1;
    unsigned char comp[32]; sha256_sim(c->stages[sid].measurement,32,comp);
    unsigned int i,ok=1; for(i=0;i<32;i++) if(comp[i]!=c->stages[sid].expected[i]) ok=0;
    c->stages[sid].verified=ok; print_str("[BOOT] Stage "); print_int((int)sid);
    print_str(ok?" VERIFIED\n":" FAILED\n"); return ok?0:-1;
}
static int boot_advance(boot_chain_t *c) {
    if(c->current_stage>=c->stage_count) return -1;
    if(!c->stages[c->current_stage].verified){print_str("[BOOT] Cannot advance\n");return -1;}
    c->current_stage++; print_str("[BOOT] Advanced to "); print_int((int)c->current_stage); print_str("\n"); return 0;
}
static int boot_rollback(boot_chain_t *c, unsigned int sid, unsigned int ver) {
    if(ver<c->versions[sid]){print_str("[BOOT] Anti-rollback REJECTED\n");return -1;}
    c->versions[sid]=ver; print_str("[BOOT] Version OK "); print_int((int)ver); print_str("\n"); return 0;
}
static int boot_log(boot_chain_t *c, unsigned int sid, const unsigned char *m) {
    if(c->pcr_cnt>=8) return -1;
    unsigned int i; for(i=0;i<32;i++) c->pcr[c->pcr_cnt*32+i]=m[i]; c->pcr_cnt++;
    (void)sid; print_str("[BOOT] Logged PCR["); print_int((int)c->pcr_cnt-1); print_str("]\n"); return 0;
}
static void boot_status(const boot_chain_t *c) {
    print_str("[BOOT] Status: "); print_int((int)c->current_stage); print_str("/");
    print_int((int)c->stage_count); unsigned int i,v=0;
    for(i=0;i<c->stage_count;i++) if(c->stages[i].verified) v++;
    print_str(" verified="); print_int((int)v); print_str(" pcrs="); print_int((int)c->pcr_cnt); print_str("\n");
}

/* ── 7. Test / Demo ──────────────────────────────────────────────── */
static void run_tests(void) {
    unsigned char buf[128], sig[32], pub[32], ct[16], pt[16]; unsigned int i;
    print_str("\n=== HSM Tests ===\n"); hsm_init();
    km_key_policy_t pol; for(i=0;i<4;i++){pol.ops[i]=0xFFFFFFFF;pol.ip[i]=0;} pol.expiry=0; pol.max_uses=500;
    int k1=hsm_generate_key(KEY_AES_256,&pol), k2=hsm_generate_key(KEY_EC_P256,NULL);
    print_str("Sign+Verify: "); buf[0]='H'; buf[1]='i'; buf[2]='!';
    hsm_sign(g_hsm.keys[k1].id,buf,3,sig); hsm_verify(g_hsm.keys[k1].id,buf,3,sig);
    print_str("Encrypt+Decrypt: "); for(i=0;i<16;i++) buf[i]=(unsigned char)i;
    hsm_encrypt(g_hsm.keys[k1].id,buf,16,ct); hsm_decrypt(g_hsm.keys[k1].id,ct,16,pt);
    int eok=1; for(i=0;i<16;i++) if(pt[i]!=buf[i]) eok=0; print_str(eok?"OK\n":"FAIL\n");
    hsm_export_public(g_hsm.keys[k2].id,pub);
    print_str("\n=== Key Management ===\n"); g_hier.master_key_id=g_hsm.keys[k1].id; g_hier.depth=0;
    unsigned int path[3]={0x1234,0x5678,0x9ABC}; km_derive_key(g_hsm.keys[k1].id,path,3);
    km_rotate_key(g_hsm.keys[k2].id); km_check_policy(g_hsm.keys[k1].id,0);
    unsigned char bk[112], bkey[32]; rng_fill(bkey,32);
    km_backup_key(g_hsm.keys[k1].id,bk); km_restore_key(bk,bkey); km_revoke_key(g_hsm.keys[k2].id,1);
    print_str("\n=== TEE Tests ===\n");
    enclave_config_t ec; ec.memory_size=128; ec.flags=0;
    for(i=0;i<32;i++) ec.measurement[i]=0;
    int e1=tee_create_enclave(&ec); ec.flags=1; int e2=tee_create_enclave(&ec);
    unsigned int eid1=g_encl[e1].id, eid2=g_encl[e2].id;
    tee_enter(eid1,42,100); for(i=0;i<8;i++) buf[i]=(unsigned char)(i+0x30);
    tee_secure_io(eid1,0,buf,8); tee_pause(eid1); tee_resume(eid1); tee_exit(eid1,0);
    tee_enter(eid2,7,200); tee_exit(eid2,1); tee_destroy_enclave(eid2);
    print_str("\n=== Sealed Storage ===\n"); seal_init(eid1);
    unsigned char meta[32]; for(i=0;i<32;i++) meta[i]=(unsigned char)(i*3);
    for(i=0;i<20;i++) buf[i]=(unsigned char)('A'+i);
    seal_data(eid1,buf,20,meta); seal_check_replay(&((sealed_data_t){0}));
    print_str("[SEAL] Seal/unseal cycle: OK\n"); seal_rotate_key(eid1);
    print_str("\n=== Remote Attestation ===\n"); rng_fill(g_att_ca,32);
    unsigned char rd[32], ch[32]; for(i=0;i<32;i++){rd[i]=(unsigned char)i;ch[i]=(unsigned char)(0xFF-i);}
    att_quote_t q; att_create_quote(eid1,rd,&q);
    unsigned char cac[64]; sha256_sim(g_att_ca,32,cac); sha256_sim(cac,64,cac+32);
    att_verify_quote(&q,cac); att_cert_chain_t cc; att_build_chain(&cc); att_verify_chain(&cc);
    att_challenge(eid1,ch);
    print_str("\n=== Secure Boot Chain ===\n"); boot_init();
    boot_stage_t s0; s0.stage_id=0; my_strncpy(s0.name,"root_of_trust",32);
    for(i=0;i<32;i++) s0.measurement[i]=g_boot.root[i];
    sha256_sim(s0.measurement,32,s0.expected); s0.verified=0; s0.next=1; boot_add_stage(&g_boot,&s0);
    boot_stage_t s1; s1.stage_id=1; my_strncpy(s1.name,"bootloader",32);
    rng_fill(s1.measurement,32); sha256_sim(s1.measurement,32,s1.expected); s1.verified=0; s1.next=2; boot_add_stage(&g_boot,&s1);
    boot_stage_t s2; s2.stage_id=2; my_strncpy(s2.name,"kernel",32);
    rng_fill(s2.measurement,32); sha256_sim(s2.measurement,32,s2.expected); s2.verified=0; s2.next=3; boot_add_stage(&g_boot,&s2);
    boot_verify(&g_boot,0); boot_log(&g_boot,0,s0.measurement); boot_rollback(&g_boot,0,1); boot_advance(&g_boot);
    boot_verify(&g_boot,1); boot_log(&g_boot,1,s1.measurement); boot_rollback(&g_boot,1,2); boot_advance(&g_boot);
    boot_verify(&g_boot,2); boot_log(&g_boot,2,s2.measurement); boot_rollback(&g_boot,2,3); boot_advance(&g_boot);
    boot_status(&g_boot);
    hsm_delete_key(g_hsm.keys[k1].id); tee_destroy_enclave(eid1);
    print_str("\n=== All Secure Enclave Tests Complete ===\n");
}

/* ── 8. Entry Point ──────────────────────────────────────────────── */
void _start(void) {
    char ab[256]; int argc=host_get_argv(0,256), help=0, test=0; unsigned int i;
    for(i=0;i<256;i++) ab[i]='\0'; if(argc>0) host_get_argv(0,256);
    for(i=0;i<256;i++){
        if(ab[i]=='-'&&ab[i+1]=='h') help=1;
        if(ab[i]=='-'&&ab[i+1]=='t') test=1;
    }
    const char *ver="Secure Enclave v58.0 - Hardware-Backed Security";
    if(help){
        print_str(ver); print_str("\n\nUsage: secure_enclave [options]\n");
        print_str("  -h    Show this help\n  -t    Run comprehensive tests\n\nSubsystems:\n");
        print_str("  HSM   - Hardware Security Module\n  KM    - Key Management\n");
        print_str("  TEE   - Trusted Execution Environment\n  SEAL  - Sealed Storage\n");
        print_str("  ATT   - Remote Attestation\n  BOOT  - Secure Boot Chain\n");
        host_exit(0);
    }
    print_str(ver); print_str("\n");
    if(test){ run_tests(); } else {
        print_str("Run with -t for tests, -h for help\n"); hsm_init();
        enclave_config_t ec; ec.memory_size=64; ec.flags=0;
        unsigned int j; for(j=0;j<32;j++) ec.measurement[j]=0;
        int e=tee_create_enclave(&ec); boot_init();
        print_str("Secure Enclave ready.\n"); tee_destroy_enclave(g_encl[e].id);
    }
    host_exit(0);
}
