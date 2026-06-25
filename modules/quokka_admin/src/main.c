/* quokka_admin: Quokka (Setonix brachyurus) quokka small marsupial (v1.0)
 * Quokka island, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_len_cm, smile_idx, hop_dist, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,sm_idx,hp_dst,age_yr,active;} qoka_t;
typedef struct{int n_island,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_sm,t_hp;} qoka_state_t;
static qoka_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static qoka_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(qoka_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int si,int hd,int ay){if(*cnt>=mx)return -1;qoka_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->sm_idx=si;x->hp_dst=hd;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[QOKA] Quokka ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" si=");pi(si);ps(" hd=");pi(hd);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int qoka_init(void){if(init)return -1;st.n_island=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_sm=0;st.t_hp=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[QOKA] Quokka initialized\n");return 0;}
int qoka_island(int lc,int bl,int bw,int tl,int si,int hd,int ay){return add(apl,&st.n_island,&st.t_ln,N,lc,bl,bw,tl,si,hd,ay);}
int qoka_feeding(int lc,int bl,int bw,int tl,int si,int hd,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,si,hd,ay);}
int qoka_breeding(int lc,int bl,int bw,int tl,int si,int hd,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,si,hd,ay);}
int qoka_health(int lc,int bl,int bw,int tl,int si,int hd,int ay){return add(ahd,&st.n_health,&st.t_sm,N-6,lc,bl,bw,tl,si,hd,ay);}
int qoka_market(int lc,int bl,int bw,int tl,int si,int hd,int ay){return add(amd,&st.n_mkt,&st.t_hp,N-6,lc,bl,bw,tl,si,hd,ay);}
void qoka_report(void){ps("[QOKA] Island: ");pi(st.n_island);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Sm=");pi(st.t_sm);ps("\nMkt: ");pi(st.n_mkt);ps(" Hp=");pi(st.t_hp);ps("\n");}
void qoka_state(void){ps("[QOKA] Island=");pi(st.n_island);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Quokka Admin Demo ===\n\n");qoka_init();
ps("Quokka island...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=3+(i*1),tl=20+(i*2),si=(i%8)+1,hd=30+(i*5),ay=1+(i%8);qoka_island(lc,bl,bw,tl,si,hd,ay);}
ps("\nQuokka feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*3),bw=3+(i*1),tl=21+(i*2),si=(i%7)+2,hd=32+(i*5),ay=2+(i%6);qoka_feeding(lc,bl,bw,tl,si,hd,ay);}
ps("\nQuokka breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=45+(i*3),bw=4+(i*1),tl=22+(i*2),si=(i%6)+1,hd=35+(i*5),ay=3+(i%5);qoka_breeding(lc,bl,bw,tl,si,hd,ay);}
ps("\nQuokka health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=2+(i*2),tl=19+(i*3),si=(i%5)+3,hd=28+(i*6),ay=4+(i%4);qoka_health(lc,bl,bw,tl,si,hd,ay);}
ps("\nQuokka market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=50+(i*3),bw=4+(i*1),tl=24+(i*2),si=(i%4)+4,hd=40+(i*5),ay=5+(i%3);qoka_market(lc,bl,bw,tl,si,hd,ay);}
ps("\n");qoka_report();qoka_state();ps("\n=== Demo Complete ===\n");return 0;}
