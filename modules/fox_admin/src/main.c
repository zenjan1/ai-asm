/* fox_admin: Fox (Vulpes vulpes) red fox canid management (v1.0)
 * Fox housing, feeding, breeding, grooming, market
 * Features: body_len_cm, body_wt_kg, tail_len_cm, fur_idx, hunt_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,fur_idx,hnt_idx,age_yr,active;} fox_t;
typedef struct{int n_house,n_feed,n_breed,n_groom,n_mkt,t_ln,t_wt,t_tl,t_fr,t_hn;} fox_state_t;
static fox_t fhs[N],ffd[N-2],fbr[N-4],fgr[N-6],fmk[N-6]; static fox_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fox_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int fi,int hi,int ay){if(*cnt>=mx)return -1;fox_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->fur_idx=fi;x->hnt_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FOX] Fox ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" fi=");pi(fi);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int fox_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tl=0;st.t_fr=0;st.t_hn=0;for(int i=0;i<N;i++)fhs[i].active=0;for(int i=0;i<N-2;i++)ffd[i].active=0;for(int i=0;i<N-4;i++)fbr[i].active=0;for(int i=0;i<N-6;i++)fgr[i].active=0;for(int i=0;i<N-6;i++)fmk[i].active=0;init=1;ps("[FOX] Fox initialized\n");return 0;}
int fox_housing(int lc,int bl,int bw,int tl,int fi,int hi,int ay){return add(fhs,&st.n_house,&st.t_ln,N,lc,bl,bw,tl,fi,hi,ay);}
int fox_feeding(int lc,int bl,int bw,int tl,int fi,int hi,int ay){return add(ffd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,fi,hi,ay);}
int fox_breeding(int lc,int bl,int bw,int tl,int fi,int hi,int ay){return add(fbr,&st.n_breed,&st.t_tl,N-4,lc,bl,bw,tl,fi,hi,ay);}
int fox_grooming(int lc,int bl,int bw,int tl,int fi,int hi,int ay){return add(fgr,&st.n_groom,&st.t_fr,N-6,lc,bl,bw,tl,fi,hi,ay);}
int fox_market(int lc,int bl,int bw,int tl,int fi,int hi,int ay){return add(fmk,&st.n_mkt,&st.t_hn,N-6,lc,bl,bw,tl,fi,hi,ay);}
void fox_report(void){ps("[FOX] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tl=");pi(st.t_tl);ps("\nGroom: ");pi(st.n_groom);ps(" Fr=");pi(st.t_fr);ps("\nMkt: ");pi(st.n_mkt);ps(" Hn=");pi(st.t_hn);ps("\n");}
void fox_state(void){ps("[FOX] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Fox Admin Demo ===\n\n");fox_init();
/* 1=den 2=enclosure 3=reserve 4=farm 5=market */
ps("Fox housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=60+(i*4),bw=5+(i*2),tl=35+(i*3),fi=(i%5)+1,hi=(i%8)+1,ay=1+(i%8);fox_housing(lc,bl,bw,tl,fi,hi,ay);}
ps("\nFox feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=65+(i*3),bw=6+(i%2),tl=38+(i*2),fi=(i%4)+1,hi=(i%7)+1,ay=2+(i%6);fox_feeding(lc,bl,bw,tl,fi,hi,ay);}
ps("\nFox breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=58+(i*5),bw=4+(i*3),tl=33+(i*4),fi=(i%6)+1,hi=(i%5)+1,ay=1+(i%5);fox_breeding(lc,bl,bw,tl,fi,hi,ay);}
ps("\nFox grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=62+(i*4),bw=5+(i%3),tl=36+(i%3),fi=(i%7)+1,hi=(i%9)+1,ay=3+(i%7);fox_grooming(lc,bl,bw,tl,fi,hi,ay);}
ps("\nFox market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=68+(i*3),bw=7+(i%2),tl=40+(i%2),fi=(i%3)+4,hi=(i%5)+4,ay=2+(i%4);fox_market(lc,bl,bw,tl,fi,hi,ay);}
ps("\n");fox_report();fox_state();ps("\n=== Demo Complete ===\n");return 0;}
