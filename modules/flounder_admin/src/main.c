/* flounder_admin: Flounder (Platichthys flesus) flatfish aquaculture (v1.0)
 * Flounder habitat, feeding, breeding, harvest, market
 * Features: body_len_cm, body_wt_g, eye_side, scale_idx, camo_lvl, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,eye_sd,sc_idx,cm_lv,age_yr,active;} flou_t;
typedef struct{int n_hab,n_feed,n_breed,n_harv,n_mkt,t_ln,t_wt,t_ey,t_sc,t_cm;} flou_state_t;
static flou_t fhab[N],ffd[N-2],fbr[N-4],fhv[N-6],fmk[N-6]; static flou_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(flou_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int es,int si,int cl,int ay){if(*cnt>=mx)return -1;flou_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->eye_sd=es;x->sc_idx=si;x->cm_lv=cl;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FLOU] Flounder ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" es=");pi(es);ps(" si=");pi(si);ps(" cl=");pi(cl);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int flou_init(void){if(init)return -1;st.n_hab=0;st.n_feed=0;st.n_breed=0;st.n_harv=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ey=0;st.t_sc=0;st.t_cm=0;for(int i=0;i<N;i++)fhab[i].active=0;for(int i=0;i<N-2;i++)ffd[i].active=0;for(int i=0;i<N-4;i++)fbr[i].active=0;for(int i=0;i<N-6;i++)fhv[i].active=0;for(int i=0;i<N-6;i++)fmk[i].active=0;init=1;ps("[FLOU] Flounder initialized\n");return 0;}
int flou_habitat(int lc,int bl,int bw,int es,int si,int cl,int ay){return add(fhab,&st.n_hab,&st.t_ln,N,lc,bl,bw,es,si,cl,ay);}
int flou_feeding(int lc,int bl,int bw,int es,int si,int cl,int ay){return add(ffd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,es,si,cl,ay);}
int flou_breeding(int lc,int bl,int bw,int es,int si,int cl,int ay){return add(fbr,&st.n_breed,&st.t_ey,N-4,lc,bl,bw,es,si,cl,ay);}
int flou_harvest(int lc,int bl,int bw,int es,int si,int cl,int ay){return add(fhv,&st.n_harv,&st.t_sc,N-6,lc,bl,bw,es,si,cl,ay);}
int flou_market(int lc,int bl,int bw,int es,int si,int cl,int ay){return add(fmk,&st.n_mkt,&st.t_cm,N-6,lc,bl,bw,es,si,cl,ay);}
void flou_report(void){ps("[FLOU] Hab: ");pi(st.n_hab);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ey=");pi(st.t_ey);ps("\nHarv: ");pi(st.n_harv);ps(" Sc=");pi(st.t_sc);ps("\nMkt: ");pi(st.n_mkt);ps(" Cm=");pi(st.t_cm);ps("\n");}
void flou_state(void){ps("[FLOU] Hab=");pi(st.n_hab);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Flounder Admin Demo ===\n\n");flou_init();
/* 1=pond 2=tank 3=estuary 4=coast 5=market */
ps("Flounder habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*3),bw=300+(i*30),es=(i%2)+1,si=(i%6)+1,cl=(i%8)+1,ay=1+(i%5);flou_habitat(lc,bl,bw,es,si,cl,ay);}
ps("\nFlounder feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=28+(i*2),bw=330+(i*25),es=(i%2)+1,si=(i%5)+1,cl=(i%7)+1,ay=1+(i%4);flou_feeding(lc,bl,bw,es,si,cl,ay);}
ps("\nFlounder breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=26+(i*3),bw=310+(i*28),es=(i%2)+1,si=(i%4)+1,cl=(i%6)+1,ay=2+(i%4);flou_breeding(lc,bl,bw,es,si,cl,ay);}
ps("\nFlounder harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=30+(i*2),bw=360+(i*20),es=(i%2)+1,si=(i%7)+1,cl=(i%5)+1,ay=2+(i%5);flou_harvest(lc,bl,bw,es,si,cl,ay);}
ps("\nFlounder market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=32+(i%2),bw=400+(i*15),es=(i%2)+1,si=(i%3)+4,cl=(i%4)+3,ay=3+(i%3);flou_market(lc,bl,bw,es,si,cl,ay);}
ps("\n");flou_report();flou_state();ps("\n=== Demo Complete ===\n");return 0;}
