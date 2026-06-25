/* meerkat_admin: Meerkat (Suricata suricatta) meerkat sentinel mongoose (v1.0)
 * Meerkat burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_len_cm, sent_vol, burrow_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,sn_vl,br_idx,age_yr,active;} mrkt_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_sn,t_br;} mrkt_state_t;
static mrkt_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static mrkt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mrkt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int sv,int bi,int ay){if(*cnt>=mx)return -1;mrkt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->sn_vl=sv;x->br_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MRKT] Meerkat ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" sv=");pi(sv);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mrkt_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_sn=0;st.t_br=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MRKT] Meerkat initialized\n");return 0;}
int mrkt_burrow(int lc,int bl,int bw,int tl,int sv,int bi,int ay){return add(apl,&st.n_burrow,&st.t_ln,N,lc,bl,bw,tl,sv,bi,ay);}
int mrkt_feeding(int lc,int bl,int bw,int tl,int sv,int bi,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,sv,bi,ay);}
int mrkt_breeding(int lc,int bl,int bw,int tl,int sv,int bi,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,sv,bi,ay);}
int mrkt_health(int lc,int bl,int bw,int tl,int sv,int bi,int ay){return add(ahd,&st.n_health,&st.t_sn,N-6,lc,bl,bw,tl,sv,bi,ay);}
int mrkt_market(int lc,int bl,int bw,int tl,int sv,int bi,int ay){return add(amd,&st.n_mkt,&st.t_br,N-6,lc,bl,bw,tl,sv,bi,ay);}
void mrkt_report(void){ps("[MRKT] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Sn=");pi(st.t_sn);ps("\nMkt: ");pi(st.n_mkt);ps(" Br=");pi(st.t_br);ps("\n");}
void mrkt_state(void){ps("[MRKT] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Meerkat Admin Demo ===\n\n");mrkt_init();
ps("Meerkat burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*2),bw=600+(i*40),tl=20+(i*3),sv=40+(i*5),bi=(i%7)+1,ay=1+(i%10);mrkt_burrow(lc,bl,bw,tl,sv,bi,ay);}
ps("\nMeerkat feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*2),bw=620+(i*40),tl=22+(i*3),sv=42+(i*5),bi=(i%6)+2,ay=2+(i%8);mrkt_feeding(lc,bl,bw,tl,sv,bi,ay);}
ps("\nMeerkat breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=35+(i*2),bw=650+(i*40),tl=24+(i*3),sv=45+(i*5),bi=(i%5)+1,ay=3+(i%6);mrkt_breeding(lc,bl,bw,tl,sv,bi,ay);}
ps("\nMeerkat health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*3),bw=580+(i*50),tl=18+(i*4),sv=38+(i*6),bi=(i%4)+3,ay=4+(i%5);mrkt_health(lc,bl,bw,tl,sv,bi,ay);}
ps("\nMeerkat market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=40+(i*2),bw=700+(i*40),tl=26+(i*3),sv=50+(i*5),bi=(i%3)+4,ay=5+(i%4);mrkt_market(lc,bl,bw,tl,sv,bi,ay);}
ps("\n");mrkt_report();mrkt_state();ps("\n=== Demo Complete ===\n");return 0;}
