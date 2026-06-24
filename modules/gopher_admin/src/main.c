/* gopher_admin: Gopher (Geomys bursarius) plains pocket gopher rodent (v1.0)
 * Gopher burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, burrow_depth_cm, cheek_pouch_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,brw_dp,chk_idx,age_yr,active;} gopher_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_brw,t_chk;} gopher_state_t;
static gopher_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static gopher_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gopher_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int bd,int ci,int ay){if(*cnt>=mx)return -1;gopher_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->brw_dp=bd;x->chk_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GOPH] Gopher ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" bd=");pi(bd);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gopher_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_brw=0;st.t_chk=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GOPH] Gopher initialized\n");return 0;}
int gopher_burrow(int lc,int bl,int bw,int tc,int bd,int ci,int ay){return add(gpl,&st.n_burrow,&st.t_ln,N,lc,bl,bw,tc,bd,ci,ay);}
int gopher_feeding(int lc,int bl,int bw,int tc,int bd,int ci,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,bd,ci,ay);}
int gopher_breeding(int lc,int bl,int bw,int tc,int bd,int ci,int ay){return add(gbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,bd,ci,ay);}
int gopher_health(int lc,int bl,int bw,int tc,int bd,int ci,int ay){return add(ghl,&st.n_health,&st.t_brw,N-6,lc,bl,bw,tc,bd,ci,ay);}
int gopher_market(int lc,int bl,int bw,int tc,int bd,int ci,int ay){return add(gmk,&st.n_mkt,&st.t_chk,N-6,lc,bl,bw,tc,bd,ci,ay);}
void gopher_report(void){ps("[GOPH] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHlth: ");pi(st.n_health);ps(" Brw=");pi(st.t_brw);ps("\nMkt: ");pi(st.n_mkt);ps(" Chk=");pi(st.t_chk);ps("\n");}
void gopher_state(void){ps("[GOPH] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gopher Admin Demo ===\n\n");gopher_init();
/* 1=field 2=prairie 3=garden 4=burrow 5=ranch */
ps("Gopher burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=15+(i*2),bw=80+(i*10),tc=5+(i),bd=30+(i*8),ci=(i%5)+1,ay=(i%4)+1;gopher_burrow(lc,bl,bw,tc,bd,ci,ay);}
ps("\nGopher feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=16+(i*2),bw=85+(i*8),tc=6+(i),bd=35+(i*6),ci=(i%4)+1,ay=(i%3)+1;gopher_feeding(lc,bl,bw,tc,bd,ci,ay);}
ps("\nGopher breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=14+(i*3),bw=75+(i*12),tc=4+(i),bd=25+(i*9),ci=(i%3)+2,ay=(i%3)+1;gopher_breeding(lc,bl,bw,tc,bd,ci,ay);}
ps("\nGopher health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=18+(i*2),bw=90+(i*7),tc=7+(i),bd=40+(i*5),ci=(i%5)+1,ay=(i%4)+1;gopher_health(lc,bl,bw,tc,bd,ci,ay);}
ps("\nGopher market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=20+(i*2),bw=95+(i*6),tc=8+(i),bd=45+(i*4),ci=(i%4)+1,ay=(i%3)+1;gopher_market(lc,bl,bw,tc,bd,ci,ay);}
ps("\n");gopher_report();gopher_state();ps("\n=== Demo Complete ===\n");return 0;}
