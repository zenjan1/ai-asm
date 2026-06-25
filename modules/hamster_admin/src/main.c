/* hamster_admin: Hamster (Mesocricetus auratus) golden small rodent (v1.0)
 * Hamster cage, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, run_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,rn_sp,fr_idx,age_yr,active;} hmst_t;
typedef struct{int n_cage,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_rn,t_fr;} hmst_state_t;
static hmst_t cge[N],cgf[N-2],cgb[N-4],cgh[N-6],cgm[N-6]; static hmst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hmst_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int fi,int ay){if(*cnt>=mx)return -1;hmst_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->rn_sp=rs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HMST] Hamster ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hamster_init(void){if(init)return -1;st.n_cage=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_rn=0;st.t_fr=0;for(int i=0;i<N;i++)cge[i].active=0;for(int i=0;i<N-2;i++)cgf[i].active=0;for(int i=0;i<N-4;i++)cgb[i].active=0;for(int i=0;i<N-6;i++)cgh[i].active=0;for(int i=0;i<N-6;i++)cgm[i].active=0;init=1;ps("[HMST] Hamster initialized\n");return 0;}
int hamster_cage(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(cge,&st.n_cage,&st.t_ln,N,lc,bl,bw,tc,rs,fi,ay);}
int hamster_feeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(cgf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,fi,ay);}
int hamster_breeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(cgb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,fi,ay);}
int hamster_health(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(cgh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,tc,rs,fi,ay);}
int hamster_market(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(cgm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,rs,fi,ay);}
void hamster_report(void){ps("[HMST] Cage: ");pi(st.n_cage);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void hamster_state(void){ps("[HMST] Cage=");pi(st.n_cage);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hamster Admin Demo ===\n\n");hamster_init();
ps("Hamster cage...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=12+(i*1),bw=30+(i*5),tc=2+(i*1),rs=3+(i*1),fi=(i%6)+1,ay=(i%3)+1;hamster_cage(lc,bl,bw,tc,rs,fi,ay);}
ps("\nHamster feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=13+(i*1),bw=32+(i*4),tc=2+(i*1),rs=3+(i*1),fi=(i%5)+1,ay=(i%3)+1;hamster_feeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nHamster breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=14+(i*1),bw=34+(i*3),tc=2+(i*1),rs=2+(i*2),fi=(i%4)+1,ay=(i%2)+1;hamster_breeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nHamster health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=11+(i*2),bw=28+(i*6),tc=1+(i*1),rs=2+(i*2),fi=(i%7)+1,ay=(i%2)+1;hamster_health(lc,bl,bw,tc,rs,fi,ay);}
ps("\nHamster market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=15+(i*1),bw=36+(i*3),tc=3+(i*1),rs=4+(i*1),fi=(i%3)+1,ay=(i%2)+1;hamster_market(lc,bl,bw,tc,rs,fi,ay);}
ps("\n");hamster_report();hamster_state();ps("\n=== Demo Complete ===\n");return 0;}
