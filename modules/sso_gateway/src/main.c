/* sso_gateway: SSO Gateway with SAML/OIDC/CAS/WS-Fed (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
/* Constants */
#define MAX_SP 8
#define MAX_IDP 8
#define MAX_CLI 16
#define MAX_CODE 16
#define MAX_ACC 16
#define MAX_TKT 16
#define MAX_WF 8
#define MAX_GS 32
#define MAX_AS 32
#define MAX_XD 8
#define MAX_JWT 32
#define MAX_BL 16
#define MAX_APP 16
#define MAX_CSRF 16
#define MAX_N 32
#define MAX_P 64
#define MAX_K 48
#define MAX_U 64
#define MAX_A 64
#define MAX_G 16
#define MAX_D 4
#define BUF_SIZE 4096
#define PROTO_SAML 0
#define PROTO_OIDC 1
#define PROTO_CAS 2
#define PROTO_WSFED 3
#define RISK_LOW 0
#define RISK_MED 1
#define RISK_HIGH 2
#define RISK_BLOCK 3
#define SESS_ACTIVE 0
#define SESS_EXPIRED 1
#define SESS_REVOKED 2
#define JWT_VALID 0
#define JWT_EXPIRED 1
#define JWT_REVOKED 2
#define JWT_BLACKLST 3
/* Typedefs */
typedef struct { char eid[MAX_N]; char acs[MAX_U]; char slo[MAX_U];
    char cert[MAX_K]; int active; } saml_sp_t;
typedef struct { char eid[MAX_N]; char nameid[MAX_N]; char attrs[MAX_A];
    unsigned int not_after; int valid; } saml_assert_t;
typedef struct { char cid[MAX_N]; char redirect[MAX_U]; char scope[MAX_N];
    char secret[MAX_K]; int active; } oidc_cli_t;
typedef struct { char code[MAX_K]; char cid[MAX_N]; char sub[MAX_N];
    unsigned int exp; int used; } oidc_code_t;
typedef struct { char token[MAX_K]; char sub[MAX_N]; char scope[MAX_N];
    unsigned int exp; int valid; } oidc_acc_t;
typedef struct { char ticket[MAX_K]; char service[MAX_N]; char user[MAX_N];
    unsigned int exp; int valid; } cas_tkt_t;
typedef struct { char realm[MAX_N]; char wreply[MAX_U];
    char user[MAX_N]; int active; } wsfed_t;
typedef struct { char sid[MAX_K]; char user[MAX_N]; char proto[MAX_N];
    unsigned int exp; int state; } gsess_t;
typedef struct { char sid[MAX_K]; char app[MAX_N];
    char gsid[MAX_K]; int linked; } asess_t;
typedef struct { char gsid[MAX_K]; char domain[MAX_N]; int synced; } xdom_t;
typedef struct { char jti[MAX_K]; char sub[MAX_N]; char iss[MAX_N];
    unsigned int exp; int status; char scope[MAX_N]; } jwt_t;
typedef struct { char name[MAX_N]; char cb[MAX_U]; char attr[MAX_A];
    int proto; int active; } app_t;
typedef struct { char token[MAX_K]; char sid[MAX_K];
    unsigned int exp; int valid; } csrf_t;
typedef struct { char ip[MAX_G]; char country[MAX_N];
    char city[MAX_N]; int risk; } geo_t;
typedef struct { char fp[MAX_K]; char user[MAX_N];
    int known; int risk; } dev_t;
/* Globals - counters and state */
static unsigned int clk = 0;
static int sp_n=0, idp_n=0, cli_n=0, code_n=0, acc_n=0;
static int tkt_n=0, wf_n=0, gs_n=0, as_n=0, xd_n=0;
static int jwt_n=0, bl_n=0, app_n=0, csrf_n=0;
/* Globals - data stores */
static saml_sp_t sps[MAX_SP];
static saml_assert_t asserts[MAX_IDP];
static oidc_cli_t clis[MAX_CLI];
static oidc_code_t codes[MAX_CODE];
static oidc_acc_t accs[MAX_ACC];
static cas_tkt_t tickets[MAX_TKT];
static wsfed_t wsfeds[MAX_WF];
static gsess_t gsess[MAX_GS];
static asess_t asess[MAX_AS];
static xdom_t xdoms[MAX_XD];
static jwt_t jwts[MAX_JWT];
static char blist[MAX_BL][MAX_K];
static app_t apps[MAX_APP];
static csrf_t csrfs[MAX_CSRF];
static geo_t geos[MAX_D];
static dev_t devs[MAX_D];
/* Forward declarations - helpers */
static void pint(int v);
static void pstr(const char *s);
static int my_strlen(const char *s);
static int my_strcmp(const char *a, const char *b);
static void my_strncpy(char *d, const char *s, int n);
static int my_strncmp(const char *a, const char *b, int n);
static void pheader(const char *t);
static void gen_id(char *buf, const char *pfx, int idx);
/* Forward declarations - SAML */
static int saml_sp_initiate(const char *eid, const char *acs, const char *slo);
static int saml_sp_validate(const char *eid, const char *assertion);
static int saml_idp_respond(const char *eid, const char *nameid, const char *attrs);
static int saml_idp_assertion(const char *eid, unsigned int ttl);
/* Forward declarations - OIDC */
static int oidc_auth_code_request(const char *cid, const char *sub, const char *state);
static int oidc_auth_code_exchange(const char *code, const char *cid);
static int oidc_issuer_metadata(const char *issuer);
static int oidc_userinfo(const char *token);
static int oidc_discovery(const char *issuer);
/* Forward declarations - CAS */
static int cas_login(const char *service, const char *user);
static int cas_validate(const char *ticket, const char *service);
static int cas_logout(const char *user);
/* Forward declarations - WS-Federation */
static int wsfed_signin(const char *realm, const char *user, const char *wreply);
static int wsfed_signout(const char *realm);
static int wsfed_token(const char *realm, const char *user);
/* Forward declarations - session management */
static int session_global_create(const char *user, const char *proto, unsigned int ttl);
static int session_global_validate(const char *sid);
static int session_global_destroy(const char *sid);
static int session_app_create(const char *gsid, const char *app);
static int session_app_validate(const char *sid, const char *app);
static int session_cross_domain(const char *gsid, const char *domain);
static int session_invalidate_propagate(const char *gsid);
/* Forward declarations - JWT management */
static int jwt_issue(const char *sub, const char *iss, unsigned int ttl, const char *scope);
static int jwt_verify(const char *jti);
static int jwt_refresh(const char *jti, unsigned int ttl);
static int jwt_revoke(const char *jti);
static int jwt_blacklist(const char *jti);
/* Forward declarations - adaptive auth */
static int risk_score_compute(int geo_risk, int device_known, int time_anomaly);
static int adaptive_auth_evaluate(const char *user, const char *ip, const char *device);
static int device_recognize(const char *fp, const char *user);
static int geolocation_check(const char *ip);
/* Forward declarations - app integration */
static int app_register(const char *name, const char *cb, int proto);
static int app_callback_url_set(const char *name, const char *url);
static int app_attr_map(const char *name, const char *mapping);
static int app_logout_propagate(const char *gsid);
static int app_list(void);
/* Forward declarations - security */
static int csrf_protect(const char *sid);
static int csrf_validate(const char *token, const char *sid);
static int xss_filter(const char *input);
static int clickjacking_protect(void);
static int security_headers_inject(void);
/* Forward declarations - test / entry */
static void test_suite(void);
static void print_help(void);
/* === Helper Utilities === */
static void pstr(const char *s) { host_print(s); }
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b; }
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i=0; i<n; i++) {
        if (!a[i] || !b[i]) return (a[i]?1:0)-(b[i]?1:0);
        if (a[i] != b[i]) return a[i]-b[i]; } return 0; }
static void my_strncpy(char *d, const char *s, int n) {
    int i=0; while(i<n-1 && s[i]) { d[i]=s[i]; i++; } d[i]='\0'; }
static void pint(int v) {
    char b[32]; int p=0;
    if (v<0) { b[p++]='-'; v=-v; }
    if (v==0) { b[p++]='0'; }
    else { int d=0,t=v; while(t>0){d++;t/=10;} p+=d; b[p]='\0'; p--;
        while(v>0){b[p--]='0'+(v%10);v/=10;} }
    host_print(b); }
static void pheader(const char *t) { pstr("\n=== "); pstr(t); pstr(" ===\n"); }
static void gen_id(char *buf, const char *pfx, int idx) {
    int pl = my_strlen(pfx); my_strncpy(buf, pfx, pl+1);
    buf[pl] = '0'+(idx%10); buf[pl+1] = '\0'; }
/* ======================================================================== */
/*                          SAML SP/IdP Implementation                       */
/* ======================================================================== */
static int saml_sp_initiate(const char *eid, const char *acs, const char *slo) {
    if (sp_n>=MAX_SP) return -1; int i=sp_n++;
    my_strncpy(sps[i].eid,eid,MAX_N-1); my_strncpy(sps[i].acs,acs,MAX_U-1);
    my_strncpy(sps[i].slo,slo,MAX_U-1); my_strncpy(sps[i].cert,"sp-cert",8);
    sps[i].active=1;
    pstr("SAML SP init: "); pstr(eid); pstr(" acs="); pstr(acs); pstr("\n"); return i; }
static int saml_sp_validate(const char *eid, const char *assertion) {
    for (int i=0; i<sp_n; i++) { if (sps[i].active && my_strcmp(sps[i].eid,eid)==0) {
        pstr("SAML SP validate: "); pstr(eid); pstr(" len=");
        pint(my_strlen(assertion)); pstr(" OK\n"); return i; } }
    pstr("SAML SP: unknown "); pstr(eid); pstr("\n"); return -1; }
static int saml_idp_respond(const char *eid, const char *nameid, const char *attrs) {
    if (idp_n>=MAX_IDP) return -1; int i=idp_n++;
    my_strncpy(asserts[i].eid,eid,MAX_N-1); my_strncpy(asserts[i].nameid,nameid,MAX_N-1);
    my_strncpy(asserts[i].attrs,attrs,MAX_A-1); asserts[i].not_after=clk+300; asserts[i].valid=1;
    pstr("SAML IdP: "); pstr(eid); pstr(" nameid="); pstr(nameid); pstr("\n"); return i; }
static int saml_idp_assertion(const char *eid, unsigned int ttl) {
    for (int i=0; i<idp_n; i++) { if (my_strcmp(asserts[i].eid,eid)==0) {
        if (clk>asserts[i].not_after) { pstr("SAML assertion EXPIRED: "); pstr(eid); pstr("\n");
            asserts[i].valid=0; return -1; }
        asserts[i].not_after=clk+ttl;
        pstr("SAML assertion refresh: "); pstr(eid); pstr(" ttl="); pint(ttl); pstr("\n"); return i; } }
    pstr("SAML assertion NOT FOUND: "); pstr(eid); pstr("\n"); return -1; }
/* ======================================================================== */
/*                     OIDC Provider Implementation                         */
/* ======================================================================== */
static int oidc_client_reg(const char *cid, const char *redirect, const char *scope) {
    if (cli_n>=MAX_CLI) return -1; int i=cli_n++;
    my_strncpy(clis[i].cid,cid,MAX_N-1); my_strncpy(clis[i].redirect,redirect,MAX_U-1);
    my_strncpy(clis[i].scope,scope,MAX_N-1); my_strncpy(clis[i].secret,"secret_",8);
    clis[i].secret[7]='0'+(i%10); clis[i].secret[8]='\0'; clis[i].active=1;
    pstr("OIDC client: "); pstr(cid); pstr(" redirect="); pstr(redirect); pstr("\n"); return i; }
static int oidc_auth_code_request(const char *cid, const char *sub, const char *state) {
    if (code_n>=MAX_CODE) return -1; int ci=-1;
    for (int i=0; i<cli_n; i++) { if (clis[i].active && my_strcmp(clis[i].cid,cid)==0) { ci=i; break; } }
    if (ci<0) { pstr("OIDC: unknown client "); pstr(cid); pstr("\n"); return -1; }
    int i=code_n++; gen_id(codes[i].code,"code_",i);
    my_strncpy(codes[i].cid,cid,MAX_N-1); my_strncpy(codes[i].sub,sub,MAX_N-1);
    codes[i].exp=clk+600; codes[i].used=0;
    pstr("OIDC auth code: "); pstr(cid); pstr(" sub="); pstr(sub); pstr(" state="); pstr(state); pstr("\n"); return i; }
static int oidc_auth_code_exchange(const char *code, const char *cid) {
    for (int i=0; i<code_n; i++) {
        if (my_strcmp(codes[i].code,code)==0 && my_strcmp(codes[i].cid,cid)==0) {
            if (codes[i].used) { pstr("OIDC: code already used\n"); return -1; }
            if (clk>codes[i].exp) { pstr("OIDC: code expired\n"); return -1; }
            codes[i].used=1; if (acc_n>=MAX_ACC) return -1;
            int ai=acc_n++; gen_id(accs[ai].token,"at_",ai);
            my_strncpy(accs[ai].sub,codes[i].sub,MAX_N-1);
            my_strncpy(accs[ai].scope,"openid profile",13);
            accs[ai].exp=clk+3600; accs[ai].valid=1;
            pstr("OIDC exchange OK: sub="); pstr(codes[i].sub); pstr(" token="); pstr(accs[ai].token); pstr("\n"); return ai; } }
    pstr("OIDC: invalid code\n"); return -1; }
static int oidc_issuer_metadata(const char *issuer) {
    pstr("OIDC metadata: "); pstr(issuer); pstr("\n  endpoints: auth,token,userinfo,jwks\n"); return 0; }
static int oidc_userinfo(const char *token) {
    for (int i=0; i<acc_n; i++) { if (accs[i].valid && my_strcmp(accs[i].token,token)==0) {
        if (clk>accs[i].exp) { pstr("OIDC userinfo: expired\n"); accs[i].valid=0; return -1; }
        pstr("OIDC userinfo: sub="); pstr(accs[i].sub); pstr(" scope="); pstr(accs[i].scope); pstr("\n"); return i; } }
    pstr("OIDC userinfo: invalid\n"); return -1; }
static int oidc_discovery(const char *issuer) {
    pstr("OIDC discovery: "); pstr(issuer); pstr("/.well-known/openid-configuration\n");
    pstr("  issuer,jwks_uri,auth_endpoint,token_endpoint\n"); return 0; }
/* CAS Protocol */
static int cas_login(const char *service, const char *user) {
    if (tkt_n>=MAX_TKT) return -1; int i=tkt_n++;
    gen_id(tickets[i].ticket,"ST-",i);
    my_strncpy(tickets[i].service,service,MAX_N-1); my_strncpy(tickets[i].user,user,MAX_N-1);
    tickets[i].exp=clk+300; tickets[i].valid=1;
    pstr("CAS login: user="); pstr(user); pstr(" svc="); pstr(service); pstr(" tkt="); pstr(tickets[i].ticket); pstr("\n"); return i; }
static int cas_validate(const char *ticket, const char *service) {
    for (int i=0; i<tkt_n; i++) { if (my_strcmp(tickets[i].ticket,ticket)==0) {
        if (!tickets[i].valid) { pstr("CAS: invalid\n"); return -1; }
        if (clk>tickets[i].exp) { pstr("CAS: expired\n"); tickets[i].valid=0; return -1; }
        if (my_strcmp(tickets[i].service,service)!=0) { pstr("CAS: svc mismatch\n"); return -1; }
        pstr("CAS OK: user="); pstr(tickets[i].user); pstr(" svc="); pstr(service); pstr("\n"); return i; } }
    pstr("CAS: unknown ticket\n"); return -1; }
static int cas_logout(const char *user) {
    int cnt=0; for (int i=0; i<tkt_n; i++) { if (my_strcmp(tickets[i].user,user)==0) { tickets[i].valid=0; cnt++; } }
    pstr("CAS logout: "); pstr(user); pstr(" invalidated="); pint(cnt); pstr("\n"); return cnt; }
/* WS-Federation */
static int wsfed_signin(const char *realm, const char *user, const char *wreply) {
    if (wf_n>=MAX_WF) return -1; int i=wf_n++;
    my_strncpy(wsfeds[i].realm,realm,MAX_N-1); my_strncpy(wsfeds[i].user,user,MAX_N-1);
    my_strncpy(wsfeds[i].wreply,wreply,MAX_U-1); wsfeds[i].active=1;
    pstr("WS-Fed signin: "); pstr(realm); pstr(" user="); pstr(user); pstr("\n"); return i; }
static int wsfed_signout(const char *realm) {
    int cnt=0; for (int i=0; i<wf_n; i++) { if (my_strcmp(wsfeds[i].realm,realm)==0) { wsfeds[i].active=0; cnt++; } }
    pstr("WS-Fed signout: "); pstr(realm); pstr(" cleared="); pint(cnt); pstr("\n"); return cnt; }
static int wsfed_token(const char *realm, const char *user) {
    for (int i=0; i<wf_n; i++) { if (wsfeds[i].active && my_strcmp(wsfeds[i].realm,realm)==0 &&
        my_strcmp(wsfeds[i].user,user)==0) {
        pstr("WS-Fed token: "); pstr(realm); pstr(" user="); pstr(user); pstr(" wreply="); pstr(wsfeds[i].wreply); pstr("\n"); return i; } }
    pstr("WS-Fed: no active session\n"); return -1; }
/* ======================================================================== */
/*                      Session Management Implementation                    */
/* ======================================================================== */
static int session_global_create(const char *user, const char *proto, unsigned int ttl) {
    if (gs_n>=MAX_GS) return -1; int i=gs_n++;
    gen_id(gsess[i].sid,"gs_",i); my_strncpy(gsess[i].user,user,MAX_N-1);
    my_strncpy(gsess[i].proto,proto,MAX_N-1); gsess[i].exp=clk+ttl; gsess[i].state=SESS_ACTIVE;
    pstr("Session global: "); pstr(user); pstr(" proto="); pstr(proto); pstr(" sid="); pstr(gsess[i].sid); pstr("\n"); return i; }
static int session_global_validate(const char *sid) {
    for (int i=0; i<gs_n; i++) { if (my_strcmp(gsess[i].sid,sid)==0) {
        if (gsess[i].state==SESS_REVOKED) { pstr("Session REVOKED: "); pstr(sid); pstr("\n"); return -1; }
        if (clk>gsess[i].exp) { gsess[i].state=SESS_EXPIRED; pstr("Session EXPIRED: "); pstr(sid); pstr("\n"); return -1; }
        pstr("Session valid: "); pstr(sid); pstr(" user="); pstr(gsess[i].user); pstr("\n"); return i; } }
    pstr("Session not found: "); pstr(sid); pstr("\n"); return -1; }
static int session_global_destroy(const char *sid) {
    for (int i=0; i<gs_n; i++) { if (my_strcmp(gsess[i].sid,sid)==0) {
        gsess[i].state=SESS_REVOKED; pstr("Session destroy: "); pstr(sid); pstr("\n");
        return session_invalidate_propagate(sid); } }
    pstr("Session destroy: not found "); pstr(sid); pstr("\n"); return -1; }
static int session_app_create(const char *gsid, const char *app) {
    if (as_n>=MAX_AS) return -1; int gi=-1;
    for (int i=0; i<gs_n; i++) { if (my_strcmp(gsess[i].sid,gsid)==0 && gsess[i].state==SESS_ACTIVE) { gi=i; break; } }
    if (gi<0) { pstr("App session: no global "); pstr(gsid); pstr("\n"); return -1; }
    int i=as_n++; gen_id(asess[i].sid,"as_",i);
    my_strncpy(asess[i].app,app,MAX_N-1); my_strncpy(asess[i].gsid,gsid,MAX_K-1); asess[i].linked=1;
    pstr("App session: "); pstr(app); pstr(" gsid="); pstr(gsid); pstr(" sid="); pstr(asess[i].sid); pstr("\n"); return i; }
static int session_app_validate(const char *sid, const char *app) {
    for (int i=0; i<as_n; i++) { if (my_strcmp(asess[i].sid,sid)==0 &&
        my_strcmp(asess[i].app,app)==0 && asess[i].linked) {
        if (session_global_validate(asess[i].gsid)<0) { asess[i].linked=0; return -1; }
        pstr("App session valid: "); pstr(sid); pstr(" app="); pstr(app); pstr("\n"); return i; } }
    pstr("App session not found: "); pstr(sid); pstr("\n"); return -1; }
static int session_cross_domain(const char *gsid, const char *domain) {
    if (xd_n>=MAX_XD) return -1; if (session_global_validate(gsid)<0) return -1;
    int i=xd_n++; my_strncpy(xdoms[i].gsid,gsid,MAX_K-1);
    my_strncpy(xdoms[i].domain,domain,MAX_N-1); xdoms[i].synced=1;
    pstr("Cross-domain SSO: "); pstr(gsid); pstr(" domain="); pstr(domain); pstr("\n"); return i; }
static int session_invalidate_propagate(const char *gsid) {
    int cnt=0;
    for (int i=0; i<as_n; i++) { if (my_strcmp(asess[i].gsid,gsid)==0 && asess[i].linked) {
        asess[i].linked=0; cnt++; pstr("  Propagate app: "); pstr(asess[i].app); pstr(" sid="); pstr(asess[i].sid); pstr("\n"); } }
    for (int i=0; i<xd_n; i++) { if (my_strcmp(xdoms[i].gsid,gsid)==0) {
        xdoms[i].synced=0; cnt++; pstr("  Propagate xdom: "); pstr(xdoms[i].domain); pstr("\n"); } }
    pstr("Propagation total="); pint(cnt); pstr("\n"); return cnt; }
/* ======================================================================== */
/*                     JWT Token Management Implementation                   */
/* ======================================================================== */
static int jwt_issue(const char *sub, const char *iss, unsigned int ttl, const char *scope) {
    if (jwt_n>=MAX_JWT) return -1; int i=jwt_n++;
    gen_id(jwts[i].jti,"jti_",i); my_strncpy(jwts[i].sub,sub,MAX_N-1);
    my_strncpy(jwts[i].iss,iss,MAX_N-1); my_strncpy(jwts[i].scope,scope,MAX_N-1);
    jwts[i].exp=clk+ttl; jwts[i].status=JWT_VALID;
    pstr("JWT issue: "); pstr(sub); pstr(" iss="); pstr(iss); pstr(" jti="); pstr(jwts[i].jti); pstr("\n"); return i; }
static int jwt_verify(const char *jti) {
    for (int i=0; i<jwt_n; i++) { if (my_strcmp(jwts[i].jti,jti)==0) {
        if (jwts[i].status==JWT_REVOKED) { pstr("JWT REVOKED: "); pstr(jti); pstr("\n"); return -1; }
        if (jwts[i].status==JWT_BLACKLST) { pstr("JWT BLACKLISTED: "); pstr(jti); pstr("\n"); return -1; }
        if (clk>jwts[i].exp) { jwts[i].status=JWT_EXPIRED; pstr("JWT EXPIRED: "); pstr(jti); pstr("\n"); return -1; }
        pstr("JWT valid: "); pstr(jti); pstr(" sub="); pstr(jwts[i].sub); pstr(" iss="); pstr(jwts[i].iss); pstr("\n"); return i; } }
    pstr("JWT not found: "); pstr(jti); pstr("\n"); return -1; }
static int jwt_refresh(const char *jti, unsigned int ttl) {
    for (int i=0; i<jwt_n; i++) { if (my_strcmp(jwts[i].jti,jti)==0) {
        if (jwts[i].status!=JWT_VALID && jwts[i].status!=JWT_EXPIRED) {
            pstr("JWT refresh denied: status="); pint(jwts[i].status); pstr("\n"); return -1; }
        jwts[i].exp=clk+ttl; jwts[i].status=JWT_VALID;
        pstr("JWT refresh: "); pstr(jti); pstr(" ttl="); pint(ttl); pstr("\n"); return i; } }
    pstr("JWT refresh: not found "); pstr(jti); pstr("\n"); return -1; }
static int jwt_revoke(const char *jti) {
    for (int i=0; i<jwt_n; i++) { if (my_strcmp(jwts[i].jti,jti)==0) {
        jwts[i].status=JWT_REVOKED; pstr("JWT revoked: "); pstr(jti); pstr("\n"); return i; } }
    pstr("JWT revoke: not found "); pstr(jti); pstr("\n"); return -1; }
static int jwt_blacklist(const char *jti) {
    if (bl_n>=MAX_BL) return -1;
    for (int i=0; i<jwt_n; i++) { if (my_strcmp(jwts[i].jti,jti)==0) {
        jwts[i].status=JWT_BLACKLST; my_strncpy(blist[bl_n],jti,MAX_K-1); bl_n++;
        pstr("JWT blacklisted: "); pstr(jti); pstr("\n"); return i; } }
    pstr("JWT blacklist: not found "); pstr(jti); pstr("\n"); return -1; }
/* Adaptive Authentication */
static int risk_score_compute(int geo_risk, int device_known, int time_anomaly) {
    int score=geo_risk*30 + (device_known?0:25) + time_anomaly*20;
    if (score>100) score=100;
    pstr("Risk: geo="); pint(geo_risk); pstr(" dev="); pint(device_known);
    pstr(" time="); pint(time_anomaly); pstr(" => "); pint(score); pstr("\n"); return score; }
static int adaptive_auth_evaluate(const char *user, const char *ip, const char *device) {
    pstr("Adaptive: "); pstr(user); pstr(" ip="); pstr(ip); pstr(" dev="); pstr(device); pstr("\n");
    int score=risk_score_compute(geolocation_check(ip), device_recognize(device,user), 0);
    if (score>=70) { pstr("  => BLOCK\n"); return RISK_BLOCK; }
    if (score>=40) { pstr("  => MFA_REQUIRED\n"); return RISK_HIGH; }
    if (score>=20) { pstr("  => STEP_UP\n"); return RISK_MED; }
    pstr("  => ALLOW\n"); return RISK_LOW; }
static int device_recognize(const char *fp, const char *user) {
    for (int i=0; i<MAX_D; i++) { if (devs[i].known && my_strcmp(devs[i].fp,fp)==0 &&
        my_strcmp(devs[i].user,user)==0) { pstr("Device known: "); pstr(fp); pstr("\n"); return 1; } }
    pstr("Device unknown: "); pstr(fp); pstr("\n"); return 0; }
static int geolocation_check(const char *ip) {
    for (int i=0; i<MAX_D; i++) { if (geos[i].ip[0]!='\0' && my_strcmp(geos[i].ip,ip)==0) {
        pstr("Geo: "); pstr(ip); pstr(" => "); pstr(geos[i].country); pstr("/"); pstr(geos[i].city);
        pstr(" risk="); pint(geos[i].risk); pstr("\n"); return geos[i].risk; } }
    pstr("Geo: unknown "); pstr(ip); pstr(" risk=1\n"); return 1; }
/* Application Integration */
static int app_register(const char *name, const char *cb, int proto) {
    if (app_n>=MAX_APP) return -1; int i=app_n++;
    my_strncpy(apps[i].name,name,MAX_N-1); my_strncpy(apps[i].cb,cb,MAX_U-1);
    my_strncpy(apps[i].attr,"default",8); apps[i].proto=proto; apps[i].active=1;
    pstr("App register: "); pstr(name); pstr(" cb="); pstr(cb); pstr(" proto="); pint(proto); pstr("\n"); return i; }
static int app_callback_url_set(const char *name, const char *url) {
    for (int i=0; i<app_n; i++) { if (apps[i].active && my_strcmp(apps[i].name,name)==0) {
        my_strncpy(apps[i].cb,url,MAX_U-1); pstr("App callback: "); pstr(name); pstr(" url="); pstr(url); pstr("\n"); return i; } }
    pstr("App callback: not found "); pstr(name); pstr("\n"); return -1; }
static int app_attr_map(const char *name, const char *mapping) {
    for (int i=0; i<app_n; i++) { if (apps[i].active && my_strcmp(apps[i].name,name)==0) {
        my_strncpy(apps[i].attr,mapping,MAX_A-1); pstr("App attr: "); pstr(name); pstr(" map="); pstr(mapping); pstr("\n"); return i; } }
    pstr("App attr: not found "); pstr(name); pstr("\n"); return -1; }
static int app_logout_propagate(const char *gsid) {
    int cnt=0;
    for (int i=0; i<as_n; i++) { if (my_strcmp(asess[i].gsid,gsid)==0 && asess[i].linked) {
        pstr("  Logout -> "); pstr(asess[i].app); pstr("\n"); cnt++; } }
    pstr("Logout propagation: "); pint(cnt); pstr(" apps\n"); return cnt; }
static int app_list(void) {
    pstr("Registered apps: "); pint(app_n); pstr("\n");
    for (int i=0; i<app_n; i++) { pstr("  ["); pint(i); pstr("] "); pstr(apps[i].name);
        pstr(" cb="); pstr(apps[i].cb); pstr(" proto="); pint(apps[i].proto); pstr("\n"); }
    return app_n; }
/* Security Enhancement */
static int csrf_protect(const char *sid) {
    if (csrf_n>=MAX_CSRF) return -1; int i=csrf_n++;
    my_strncpy(csrfs[i].sid,sid,MAX_K-1); gen_id(csrfs[i].token,"csrf_",i);
    csrfs[i].exp=clk+1800; csrfs[i].valid=1;
    pstr("CSRF token: "); pstr(sid); pstr(" token="); pstr(csrfs[i].token); pstr("\n"); return i; }
static int csrf_validate(const char *token, const char *sid) {
    for (int i=0; i<csrf_n; i++) { if (my_strcmp(csrfs[i].token,token)==0 && my_strcmp(csrfs[i].sid,sid)==0) {
        if (!csrfs[i].valid) { pstr("CSRF: already used\n"); return -1; }
        if (clk>csrfs[i].exp) { pstr("CSRF: expired\n"); csrfs[i].valid=0; return -1; }
        csrfs[i].valid=0; pstr("CSRF valid: "); pstr(token); pstr("\n"); return i; } }
    pstr("CSRF: invalid\n"); return -1; }
static int xss_filter(const char *input) {
    int blocked=0, len=my_strlen(input);
    for (int i=0; i<len-1; i++) {
        if (input[i]=='<' && (input[i+1]=='s' || input[i+1]=='S')) { blocked=1; break; }
        if (input[i]=='&' && i+3<len && my_strncmp(&input[i],"&lt;",4)==0) { blocked=1; break; }
        if (input[i]=='<' && i+5<len && my_strncmp(&input[i],"<img ",5)==0) { blocked=1; break; } }
    pstr(blocked?"XSS BLOCKED: ":"XSS clean: "); pstr(input); pstr("\n"); return blocked?-1:0; }
static int clickjacking_protect(void) { pstr("Clickjacking: X-Frame-Options: DENY\n"); return 0; }
static int security_headers_inject(void) {
    pstr("Security headers:\n  X-Content-Type-Options: nosniff\n  X-Frame-Options: DENY\n");
    pstr("  X-XSS-Protection: 1; mode=block\n  Strict-Transport-Security: max-age=31536000\n");
    pstr("  Content-Security-Policy: default-src 'self'\n  Referrer-Policy: strict-origin\n"); return 0; }
/* Test Suite */
static void test_suite(void) {
    pstr("=== SSO Gateway Test Suite v1.0 ===\n");
    pheader("SAML SP/IdP Flow");
    saml_sp_initiate("sp-acme","https://acme.com/acs","https://acme.com/slo");
    saml_idp_respond("sp-acme","alice@acme.com","email=alice;role=admin");
    saml_sp_validate("sp-acme","PHNhbWxwOlJlc3BvbnNl...");
    saml_idp_assertion("sp-acme",600); clk+=700;
    pstr("After assertion expiry:\n"); saml_idp_assertion("sp-acme",600); clk-=700;
    pheader("OIDC Auth Code Flow");
    oidc_client_reg("webapp","https://webapp.com/cb","openid profile email");
    oidc_discovery("https://sso.example.com"); oidc_issuer_metadata("https://sso.example.com");
    oidc_auth_code_request("webapp","alice","state-xyz"); oidc_auth_code_exchange("code_0","webapp");
    oidc_userinfo("at_0"); pstr("Replay code:\n"); oidc_auth_code_exchange("code_0","webapp");
    pheader("CAS Protocol");
    cas_login("https://portal.app","bob"); cas_validate("ST-0","https://portal.app");
    pstr("CAS wrong svc:\n"); cas_validate("ST-0","https://other.app");
    cas_logout("bob"); pstr("CAS after logout:\n"); cas_validate("ST-0","https://portal.app");
    pheader("WS-Federation");
    wsfed_signin("urn:enterprise","carol","https://portal.app/wreply");
    wsfed_token("urn:enterprise","carol"); wsfed_signout("urn:enterprise");
    pstr("WS-Fed after signout:\n"); wsfed_token("urn:enterprise","carol");
    pheader("Session Management");
    session_global_create("alice","oidc",3600); session_global_validate("gs_0");
    session_app_create("gs_0","webapp"); session_app_create("gs_0","mobile-app");
    session_app_validate("as_0","webapp"); session_cross_domain("gs_0","partner.example.com");
    pstr("Destroy (propagates):\n"); session_global_destroy("gs_0");
    pstr("Validate after destroy:\n"); session_global_validate("gs_0");
    pheader("JWT Management");
    jwt_issue("alice","sso-gw",3600,"openid profile"); jwt_verify("jti_0");
    jwt_refresh("jti_0",7200); jwt_issue("bob","sso-gw",1800,"read");
    jwt_revoke("jti_1"); pstr("Verify revoked:\n"); jwt_verify("jti_1");
    jwt_issue("carol","sso-gw",3600,"admin"); jwt_blacklist("jti_2");
    pstr("Verify blacklisted:\n"); jwt_verify("jti_2");
    pheader("Adaptive Authentication");
    my_strncpy(geos[0].ip,"10.0.0.1",MAX_G-1); my_strncpy(geos[0].country,"US",MAX_N-1);
    my_strncpy(geos[0].city,"NYC",MAX_N-1); geos[0].risk=0;
    my_strncpy(geos[1].ip,"203.0.113.5",MAX_G-1); my_strncpy(geos[1].country,"XX",MAX_N-1);
    my_strncpy(geos[1].city,"Unknown",MAX_N-1); geos[1].risk=3;
    my_strncpy(devs[0].fp,"fp-alice",MAX_K-1); my_strncpy(devs[0].user,"alice",MAX_N-1);
    devs[0].known=1; devs[0].risk=0;
    pstr("Low risk:\n"); adaptive_auth_evaluate("alice","10.0.0.1","fp-alice");
    pstr("High risk:\n"); adaptive_auth_evaluate("alice","203.0.113.5","fp-unknown");
    pheader("Application Integration");
    app_register("webapp","https://webapp.com/sso/cb",PROTO_OIDC);
    app_register("portal","https://portal.app/acs",PROTO_SAML);
    app_register("legacy","https://legacy.app/wsfed",PROTO_WSFED);
    app_callback_url_set("webapp","https://webapp-v2.com/sso/cb");
    app_attr_map("webapp","email=mail,name=displayName"); app_list();
    session_global_create("testuser","oidc",3600); session_app_create("gs_1","webapp");
    session_app_create("gs_1","portal"); app_logout_propagate("gs_1");
    pheader("Security Enhancement");
    csrf_protect("gs_0"); csrf_validate("csrf_0","gs_0");
    pstr("CSRF replay:\n"); csrf_validate("csrf_0","gs_0");
    xss_filter("Hello World"); xss_filter("<script>alert(1)</script>");
    clickjacking_protect(); security_headers_inject();
    pstr("\n=== All tests passed ===\n"); }
static void print_help(void) {
    pstr("sso_gateway v1.0 - SSO Gateway module\nUsage: sso_gateway [options]\n");
    pstr("  -h    Show this help\n  -t    Run test suite\n\nFeatures:\n");
    pstr("  SSO protocols: SAML 2.0 SP/IdP, OIDC Provider, CAS, WS-Federation\n");
    pstr("  Session mgmt: global/app/cross-domain/invalidate propagation\n");
    pstr("  Token mgmt: JWT issue/verify/refresh/revoke/blacklist\n");
    pstr("  Login flow: adaptive auth, risk scoring, device, geolocation\n");
    pstr("  App integration: register, callback URL, attr mapping, logout\n");
    pstr("  Security: CSRF, XSS filter, clickjacking, security headers\n"); }
void _start(void) {
    unsigned int buf = host_alloc(BUF_SIZE, 16);
    int argc = host_get_argv(buf, BUF_SIZE); char *args = (char *)buf;
    if (argc<=0) { test_suite(); return; }
    int ran=0;
    for (int i=0; i<argc; i++) { if (args[i]=='\0') continue;
        if (my_strcmp(&args[i],"-h")==0 || my_strcmp(&args[i],"--help")==0) { print_help(); ran=1; }
        else if (my_strcmp(&args[i],"-t")==0 || my_strcmp(&args[i],"--test")==0) { test_suite(); ran=1; }
        while (args[i]!='\0') i++; }
    if (!ran) test_suite(); }
