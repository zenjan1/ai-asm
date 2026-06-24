/* finch_admin: Finch (Fringilla coelebs) common finch small songbird (v1.0)
 * Finch housing, feeding, breeding, grooming, market
 * Features: body_len_cm, body_wt_g, wing_span_cm, plumage_idx, song_vol, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,plm_idx,sng_vl,age_yr,active;} finc_t;
typedef struct{int n_house,n_feed,n_breed,n_groom,n_mkt,t_ln,t_wt,t_wg,t_pl,t_sg;} finc_state_t;
static finc_t fhs[N],ffd[N-2],fbr[N-4],fgr[N-6],fmk[N-6]; static finc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(finc_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int px,int sv,int ay){if(*cnt>=mx)return -1;finc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->plm_idx=px;x->sng_vl=sv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FINC] Finch ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" px=");pi(px);ps(" sv=");pi(sv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int finc_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wg=0;st.t_pl=0;st.t_sg=0;for(int i=0;i<N;i++)fhs[i].active=0;for(int i=0;i<N-2;i++)ffd[i].active=0;for(int i=0;i<N-4;i++)fbr[i].active=0;for(int i=0;i<N-6;i++)fgr[i].active=0;for(int i=0;i<N-6;i++)fmk[i].active=0;init=1;ps("[FINC] Finch initialized\n");return 0;}
int finc_housing(int lc,int bl,int bw,int ws,int px,int sv,int ay){return add(fhs,&st.n_house,&st.t_ln,N,lc,bl,bw,ws,px,sv,ay);}
int finc_feeding(int lc,int bl,int bw,int ws,int px,int sv,int ay){return add(ffd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,px,sv,ay);}
int finc_breeding(int lc,int bl,int bw,int ws,int px,int sv,int ay){return add(fbr,&st.n_breed,&st.t_wg,N-4,lc,bl,bw,ws,px,sv,ay);}
int finc_grooming(int lc,int bl,int bw,int ws,int px,int sv,int ay){return add(fgr,&st.n_groom,&st.t_pl,N-6,lc,bl,bw,ws,px,sv,ay);}
int finc_market(int lc,int bl,int bw,int ws,int px,int sv,int ay){return add(fmk,&st.n_mkt,&st.t_sg,N-6,lc,bl,bw,ws,px,sv,ay);}
void finc_report(void){ps("[FINC] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wg=");pi(st.t_wg);ps("\nGroom: ");pi(st.n_groom);ps(" Pl=");pi(st.t_pl);ps("\nMkt: ");pi(st.n_mkt);ps(" Sg=");pi(st.t_sg);ps("\n");}
void finc_state(void){ps("[FINC] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Finch Admin Demo ===\n\n");finc_init();
/* 1=cage 2=aviary 3=garden 4=volary 5=market */
ps("Finch housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=14+(i%3),bw=20+(i*3),ws=22+(i*2),px=(i%6)+1,sv=50+(i*5),ay=1+(i%6);finc_housing(lc,bl,bw,ws,px,sv,ay);}
ps("\nFinch feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=15+(i%2),bw=22+(i*2),ws=23+(i%2),px=(i%5)+1,sv=55+(i*4),ay=1+(i%5);finc_feeding(lc,bl,bw,ws,px,sv,ay);}
ps("\nFinch breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=13+(i%3),bw=18+(i*3),ws=21+(i%3),px=(i%4)+1,sv=60+(i*3),ay=1+(i%4);finc_breeding(lc,bl,bw,ws,px,sv,ay);}
ps("\nFinch grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=16+(i%2),bw=25+(i*3),ws=24+(i%2),px=(i%7)+1,sv=45+(i*6),ay=2+(i%7);finc_grooming(lc,bl,bw,ws,px,sv,ay);}
ps("\nFinch market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=15+(i%2),bw=28+(i*2),ws=25+(i%2),px=(i%3)+4,sv=65+(i*3),ay=2+(i%4);finc_market(lc,bl,bw,ws,px,sv,ay);}
ps("\n");finc_report();finc_state();ps("\n=== Demo Complete ===\n");return 0;}
