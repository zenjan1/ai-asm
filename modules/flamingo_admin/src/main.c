/* flamingo_admin: Flamingo (Phoenicopterus roseus) greater flamingo wading bird (v1.0)
 * Flamingo habitat, feeding, breeding, grooming, market
 * Features: body_len_cm, body_wt_g, leg_len_cm, neck_len_cm, color_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_ln,neck_ln,clr_idx,age_yr,active;} flam_t;
typedef struct{int n_hab,n_feed,n_breed,n_groom,n_mkt,t_ln,t_wt,t_lg,t_nk,t_cl;} flam_state_t;
static flam_t fhab[N],ffd[N-2],fbr[N-4],fgr[N-6],fmk[N-6]; static flam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(flam_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ll,int nl,int ci,int ay){if(*cnt>=mx)return -1;flam_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_ln=ll;x->neck_ln=nl;x->clr_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FLAM] Flamingo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ll=");pi(ll);ps(" nl=");pi(nl);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int flam_init(void){if(init)return -1;st.n_hab=0;st.n_feed=0;st.n_breed=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_lg=0;st.t_nk=0;st.t_cl=0;for(int i=0;i<N;i++)fhab[i].active=0;for(int i=0;i<N-2;i++)ffd[i].active=0;for(int i=0;i<N-4;i++)fbr[i].active=0;for(int i=0;i<N-6;i++)fgr[i].active=0;for(int i=0;i<N-6;i++)fmk[i].active=0;init=1;ps("[FLAM] Flamingo initialized\n");return 0;}
int flam_habitat(int lc,int bl,int bw,int ll,int nl,int ci,int ay){return add(fhab,&st.n_hab,&st.t_ln,N,lc,bl,bw,ll,nl,ci,ay);}
int flam_feeding(int lc,int bl,int bw,int ll,int nl,int ci,int ay){return add(ffd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ll,nl,ci,ay);}
int flam_breeding(int lc,int bl,int bw,int ll,int nl,int ci,int ay){return add(fbr,&st.n_breed,&st.t_lg,N-4,lc,bl,bw,ll,nl,ci,ay);}
int flam_grooming(int lc,int bl,int bw,int ll,int nl,int ci,int ay){return add(fgr,&st.n_groom,&st.t_nk,N-6,lc,bl,bw,ll,nl,ci,ay);}
int flam_market(int lc,int bl,int bw,int ll,int nl,int ci,int ay){return add(fmk,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,ll,nl,ci,ay);}
void flam_report(void){ps("[FLAM] Hab: ");pi(st.n_hab);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Lg=");pi(st.t_lg);ps("\nGroom: ");pi(st.n_groom);ps(" Nk=");pi(st.t_nk);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void flam_state(void){ps("[FLAM] Hab=");pi(st.n_hab);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Flamingo Admin Demo ===\n\n");flam_init();
/* 1=lagoon 2=lake 3=marsh 4=aviary 5=market */
ps("Flamingo habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*8),bw=2500+(i*200),ll=80+(i*5),nl=60+(i*6),ci=(i%5)+1,ay=1+(i%10);flam_habitat(lc,bl,bw,ll,nl,ci,ay);}
ps("\nFlamingo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=125+(i*6),bw=2700+(i*150),ll=85+(i*4),nl=65+(i*5),ci=(i%4)+1,ay=2+(i%8);flam_feeding(lc,bl,bw,ll,nl,ci,ay);}
ps("\nFlamingo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=130+(i*5),bw=2900+(i*120),ll=90+(i*3),nl=70+(i*4),ci=(i%6)+1,ay=2+(i%6);flam_breeding(lc,bl,bw,ll,nl,ci,ay);}
ps("\nFlamingo grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=115+(i*9),bw=2300+(i*250),ll=75+(i*6),nl=55+(i*7),ci=(i%7)+1,ay=3+(i%7);flam_grooming(lc,bl,bw,ll,nl,ci,ay);}
ps("\nFlamingo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=135+(i*4),bw=3100+(i*100),ll=95+(i*3),nl=75+(i*3),ci=(i%3)+4,ay=3+(i%4);flam_market(lc,bl,bw,ll,nl,ci,ay);}
ps("\n");flam_report();flam_state();ps("\n=== Demo Complete ===\n");return 0;}
