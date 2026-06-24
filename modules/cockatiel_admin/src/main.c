/* cockatiel_admin: Cockatiel (Nymphicus hollandicus) pet bird husbandry (v1.0)
 * Cockatiel housing, feeding, breeding, grooming, market
 * Features: body_len_cm, body_wt_g, wing_span_cm, feather_idx, feed_g, vocal_lvl
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,fthr_idx,feed_g,vocal_lvl,active;} ckta_t;
typedef struct{int n_house,n_feed,n_breed,n_groom,n_mkt,t_ln,t_wt,t_wing,t_fthr,t_feed;} ckta_state_t;
static ckta_t cktah[N],cktaf[N-2],cktab[N-4],cktgr[N-6],cktam[N-6]; static ckta_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ckta_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int fi,int fg,int vl){if(*cnt>=mx)return -1;ckta_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->fthr_idx=fi;x->feed_g=fg;x->vocal_lvl=vl;x->active=1;*sum+=bl;(*cnt)++;ps("[CKTA] Cockatiel ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" fi=");pi(fi);ps(" fg=");pi(fg);ps(" vl=");pi(vl);ps("\n");return *cnt-1;}
int ckta_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fthr=0;st.t_feed=0;for(int i=0;i<N;i++)cktah[i].active=0;for(int i=0;i<N-2;i++)cktaf[i].active=0;for(int i=0;i<N-4;i++)cktab[i].active=0;for(int i=0;i<N-6;i++)cktgr[i].active=0;for(int i=0;i<N-6;i++)cktam[i].active=0;init=1;ps("[CKTA] Cockatiel initialized\n");return 0;}
int ckta_housing(int lc,int bl,int bw,int ws,int fi,int fg,int vl){return add(cktah,&st.n_house,&st.t_ln,N,lc,bl,bw,ws,fi,fg,vl);}
int ckta_feeding(int lc,int bl,int bw,int ws,int fi,int fg,int vl){return add(cktaf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,fi,fg,vl);}
int ckta_breeding(int lc,int bl,int bw,int ws,int fi,int fg,int vl){return add(cktab,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,fi,fg,vl);}
int ckta_grooming(int lc,int bl,int bw,int ws,int fi,int fg,int vl){return add(cktgr,&st.n_groom,&st.t_fthr,N-6,lc,bl,bw,ws,fi,fg,vl);}
int ckta_market(int lc,int bl,int bw,int ws,int fi,int fg,int vl){return add(cktam,&st.n_mkt,&st.t_feed,N-6,lc,bl,bw,ws,fi,fg,vl);}
void ckta_report(void){ps("[CKTA] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nGroom: ");pi(st.n_groom);ps(" Fthr=");pi(st.t_fthr);ps("\nMkt: ");pi(st.n_mkt);ps(" Feed=");pi(st.t_feed);ps("\n");}
void ckta_state(void){ps("[CKTA] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cockatiel Admin Demo ===\n\n");ckta_init();
/* 1=cage 2=aviary 3=indoor 4=outdoor 5=market */
ps("Cockatiel housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*3),bw=80+(i*10),ws=40+(i*3),fi=(i%6)+1,fg=15+(i*3),vl=(i%8)+1;ckta_housing(lc,bl,bw,ws,fi,fg,vl);}
ps("\nCockatiel feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*3),bw=85+(i*8),ws=42+(i*2),fi=(i%5)+1,fg=16+(i*2),vl=(i%7)+1;ckta_feeding(lc,bl,bw,ws,fi,fg,vl);}
ps("\nCockatiel breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=34+(i*2),bw=90+(i*7),ws=44+(i*2),fi=(i%4)+1,fg=17+(i*2),vl=(i%6)+1;ckta_breeding(lc,bl,bw,ws,fi,fg,vl);}
ps("\nCockatiel grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*4),bw=75+(i*12),ws=38+(i*4),fi=(i%7)+1,fg=14+(i*4),vl=(i%9)+1;ckta_grooming(lc,bl,bw,ws,fi,fg,vl);}
ps("\nCockatiel market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*2),bw=95+(i*6),ws=46+(i*2),fi=(i%3)+4,fg=18+(i*2),vl=(i%5)+4;ckta_market(lc,bl,bw,ws,fi,fg,vl);}
ps("\n");ckta_report();ckta_state();ps("\n=== Demo Complete ===\n");return 0;}
