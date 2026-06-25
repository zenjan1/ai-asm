/* fox_admin: Fox (Vulpes vulpes) red fox cunning canid mammal (v1.0)
 * Fox den, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, run_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,rn_sp,fr_idx,age_yr,active;} fox_t;
typedef struct{int n_den,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_rn,t_fr;} fox_state_t;
static fox_t dns[N],dnf[N-2],dnb[N-4],dnh[N-6],dnm[N-6]; static fox_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fox_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int fi,int ay){if(*cnt>=mx)return -1;fox_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->rn_sp=rs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FOX] Fox ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int fox_init(void){if(init)return -1;st.n_den=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_rn=0;st.t_fr=0;for(int i=0;i<N;i++)dns[i].active=0;for(int i=0;i<N-2;i++)dnf[i].active=0;for(int i=0;i<N-4;i++)dnb[i].active=0;for(int i=0;i<N-6;i++)dnh[i].active=0;for(int i=0;i<N-6;i++)dnm[i].active=0;init=1;ps("[FOX] Fox initialized\n");return 0;}
int fox_den(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(dns,&st.n_den,&st.t_ln,N,lc,bl,bw,tc,rs,fi,ay);}
int fox_feeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(dnf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,fi,ay);}
int fox_breeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(dnb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,fi,ay);}
int fox_health(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(dnh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,tc,rs,fi,ay);}
int fox_market(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(dnm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,rs,fi,ay);}
void fox_report(void){ps("[FOX] Den: ");pi(st.n_den);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void fox_state(void){ps("[FOX] Den=");pi(st.n_den);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Fox Admin Demo ===\n\n");fox_init();
ps("Fox den...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=70+(i*5),bw=6+(i*1),tc=35+(i*3),rs=12+(i*3),fi=(i%8)+1,ay=(i%12)+1;fox_den(lc,bl,bw,tc,rs,fi,ay);}
ps("\nFox feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=73+(i*4),bw=6+(i*1),tc=36+(i*2),rs=13+(i*2),fi=(i%7)+1,ay=(i%10)+1;fox_feeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nFox breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=76+(i*3),bw=7+(i*1),tc=37+(i*2),rs=11+(i*4),fi=(i%6)+1,ay=(i%8)+1;fox_breeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nFox health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=68+(i*6),bw=5+(i*2),tc=34+(i*4),rs=10+(i*5),fi=(i%9)+1,ay=(i%7)+1;fox_health(lc,bl,bw,tc,rs,fi,ay);}
ps("\nFox market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=78+(i*3),bw=7+(i*1),tc=38+(i*2),rs=14+(i*2),fi=(i%5)+1,ay=(i%6)+1;fox_market(lc,bl,bw,tc,rs,fi,ay);}
ps("\n");fox_report();fox_state();ps("\n=== Demo Complete ===\n");return 0;}
