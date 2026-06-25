/* iguana_admin: Iguana (Iguana iguana) green iguana large lizard (v1.0)
 * Iguana enclosure, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, crest_cm, dewlap_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,cr_cm,dw_idx,age_yr,active;} iguana_t;
typedef struct{int n_enclos,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_cr,t_dw;} iguana_state_t;
static iguana_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static iguana_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(iguana_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int cc,int di,int ay){if(*cnt>=mx)return -1;iguana_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->cr_cm=cc;x->dw_idx=di;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[IGUA] Iguana ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" cc=");pi(cc);ps(" di=");pi(di);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int iguana_init(void){if(init)return -1;st.n_enclos=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_cr=0;st.t_dw=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[IGUA] Iguana initialized\n");return 0;}
int iguana_enclosure(int lc,int bl,int bw,int tc,int cc,int di,int ay){return add(gpl,&st.n_enclos,&st.t_ln,N,lc,bl,bw,tc,cc,di,ay);}
int iguana_feeding(int lc,int bl,int bw,int tc,int cc,int di,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,cc,di,ay);}
int iguana_breeding(int lc,int bl,int bw,int tc,int cc,int di,int ay){return add(gbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,cc,di,ay);}
int iguana_health(int lc,int bl,int bw,int tc,int cc,int di,int ay){return add(ghl,&st.n_health,&st.t_cr,N-6,lc,bl,bw,tc,cc,di,ay);}
int iguana_market(int lc,int bl,int bw,int tc,int cc,int di,int ay){return add(gmk,&st.n_mkt,&st.t_dw,N-6,lc,bl,bw,tc,cc,di,ay);}
void iguana_report(void){ps("[IGUA] Enclos: ");pi(st.n_enclos);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHlth: ");pi(st.n_health);ps(" Cr=");pi(st.t_cr);ps("\nMkt: ");pi(st.n_mkt);ps(" Dw=");pi(st.t_dw);ps("\n");}
void iguana_state(void){ps("[IGUA] Enclos=");pi(st.n_enclos);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Iguana Admin Demo ===\n\n");iguana_init();
/* 1=terrarium 2=garden 3=tree 4=vivarium 5=jungle */
ps("Iguana enclosure...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*6),bw=4+(i*2),tc=80+(i*5),cc=5+(i%4),di=(i%5)+1,ay=(i%5)+1;iguana_enclosure(lc,bl,bw,tc,cc,di,ay);}
ps("\nIguana feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=105+(i*5),bw=4+(i*2),tc=82+(i*4),cc=6+(i%3),di=(i%4)+1,ay=(i%4)+1;iguana_feeding(lc,bl,bw,tc,cc,di,ay);}
ps("\nIguana breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=95+(i*7),bw=5+(i*2),tc=78+(i*6),cc=4+(i%4),di=(i%3)+2,ay=(i%3)+1;iguana_breeding(lc,bl,bw,tc,cc,di,ay);}
ps("\nIguana health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=110+(i*4),bw=5+(i*2),tc=85+(i*4),cc=7+(i%3),di=(i%5)+1,ay=(i%4)+1;iguana_health(lc,bl,bw,tc,cc,di,ay);}
ps("\nIguana market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=115+(i*3),bw=6+(i*2),tc=88+(i*3),cc=8+(i%2),di=(i%4)+1,ay=(i%3)+1;iguana_market(lc,bl,bw,tc,cc,di,ay);}
ps("\n");iguana_report();iguana_state();ps("\n=== Demo Complete ===\n");return 0;}
