/* danio_admin: Danio (Danio rerio) zebrafish aquatic management (v1.0)
 * Danio housing, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, stripe_idx, fin_span, swim_speed, age_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,strp_idx,fin_sp,swm_spd,age_wk,active;} dani_t;
typedef struct{int n_house,n_feed,n_breed,n_hlth,n_mkt,t_ln,t_wt,t_str,t_fin,t_sw;} dani_state_t;
static dani_t dhs[N],dfd[N-2],dbr[N-4],dhl[N-6],dmk[N-6]; static dani_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dani_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int si,int fs,int ss,int aw){if(*cnt>=mx)return -1;dani_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->strp_idx=si;x->fin_sp=fs;x->swm_spd=ss;x->age_wk=aw;x->active=1;*sum+=bl;(*cnt)++;ps("[DANI] Danio ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" si=");pi(si);ps(" fs=");pi(fs);ps(" ss=");pi(ss);ps(" aw=");pi(aw);ps("\n");return *cnt-1;}
int dani_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_hlth=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_str=0;st.t_fin=0;st.t_sw=0;for(int i=0;i<N;i++)dhs[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dbr[i].active=0;for(int i=0;i<N-6;i++)dhl[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DANI] Danio initialized\n");return 0;}
int dani_housing(int lc,int bl,int bw,int si,int fs,int ss,int aw){return add(dhs,&st.n_house,&st.t_ln,N,lc,bl,bw,si,fs,ss,aw);}
int dani_feeding(int lc,int bl,int bw,int si,int fs,int ss,int aw){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,si,fs,ss,aw);}
int dani_breeding(int lc,int bl,int bw,int si,int fs,int ss,int aw){return add(dbr,&st.n_breed,&st.t_str,N-4,lc,bl,bw,si,fs,ss,aw);}
int dani_health(int lc,int bl,int bw,int si,int fs,int ss,int aw){return add(dhl,&st.n_hlth,&st.t_fin,N-6,lc,bl,bw,si,fs,ss,aw);}
int dani_market(int lc,int bl,int bw,int si,int fs,int ss,int aw){return add(dmk,&st.n_mkt,&st.t_sw,N-6,lc,bl,bw,si,fs,ss,aw);}
void dani_report(void){ps("[DANI] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Str=");pi(st.t_str);ps("\nHlth: ");pi(st.n_hlth);ps(" Fin=");pi(st.t_fin);ps("\nMkt: ");pi(st.n_mkt);ps(" Sw=");pi(st.t_sw);ps("\n");}
void dani_state(void){ps("[DANI] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_hlth);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Danio Admin Demo ===\n\n");dani_init();
/* 1=tank 2=pond 3=lab 4=aquarium 5=market */
ps("Danio housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=3+(i%2),bw=1+(i%3),si=(i%7)+1,fs=2+(i%3),ss=10+(i*3),aw=2+(i%8);dani_housing(lc,bl,bw,si,fs,ss,aw);}
ps("\nDanio feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=4+(i%2),bw=2+(i%2),si=(i%6)+1,fs=3+(i%2),ss=12+(i*2),aw=3+(i%6);dani_feeding(lc,bl,bw,si,fs,ss,aw);}
ps("\nDanio breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=3+(i%3),bw=1+(i%3),si=(i%5)+1,fs=2+(i%4),ss=8+(i*4),aw=4+(i%5);dani_breeding(lc,bl,bw,si,fs,ss,aw);}
ps("\nDanio health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=4+(i%2),bw=2+(i%2),si=(i%8)+1,fs=3+(i%3),ss=15+(i*2),aw=1+(i%10);dani_health(lc,bl,bw,si,fs,ss,aw);}
ps("\nDanio market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=5+(i%2),bw=2+(i%2),si=(i%4)+3,fs=4+(i%2),ss=18+(i*2),aw=6+(i%4);dani_market(lc,bl,bw,si,fs,ss,aw);}
ps("\n");dani_report();dani_state();ps("\n=== Demo Complete ===\n");return 0;}
