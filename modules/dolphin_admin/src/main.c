/* dolphin_admin: Dolphin (Tursiops truncatus) bottlenose dolphin marine (v1.0)
 * Dolphin housing, feeding, training, health, market
 * Features: body_len_cm, body_wt_kg, fin_ht_cm, echo_idx, swim_speed, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_ht,echo_idx,swm_spd,age_yr,active;} dolf_t;
typedef struct{int n_house,n_feed,n_train,n_hlth,n_mkt,t_ln,t_wt,t_fin,t_ec,t_sw;} dolf_state_t;
static dolf_t dhs[N],dfd[N-2],dtr[N-4],dhl[N-6],dmk[N-6]; static dolf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dolf_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fh,int ei,int ss,int ay){if(*cnt>=mx)return -1;dolf_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_ht=fh;x->echo_idx=ei;x->swm_spd=ss;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DOLF] Dolphin ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fh=");pi(fh);ps(" ei=");pi(ei);ps(" ss=");pi(ss);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dolf_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_train=0;st.n_hlth=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_ec=0;st.t_sw=0;for(int i=0;i<N;i++)dhs[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dtr[i].active=0;for(int i=0;i<N-6;i++)dhl[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DOLF] Dolphin initialized\n");return 0;}
int dolf_housing(int lc,int bl,int bw,int fh,int ei,int ss,int ay){return add(dhs,&st.n_house,&st.t_ln,N,lc,bl,bw,fh,ei,ss,ay);}
int dolf_feeding(int lc,int bl,int bw,int fh,int ei,int ss,int ay){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fh,ei,ss,ay);}
int dolf_training(int lc,int bl,int bw,int fh,int ei,int ss,int ay){return add(dtr,&st.n_train,&st.t_fin,N-4,lc,bl,bw,fh,ei,ss,ay);}
int dolf_health(int lc,int bl,int bw,int fh,int ei,int ss,int ay){return add(dhl,&st.n_hlth,&st.t_ec,N-6,lc,bl,bw,fh,ei,ss,ay);}
int dolf_market(int lc,int bl,int bw,int fh,int ei,int ss,int ay){return add(dmk,&st.n_mkt,&st.t_sw,N-6,lc,bl,bw,fh,ei,ss,ay);}
void dolf_report(void){ps("[DOLF] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nTrain: ");pi(st.n_train);ps(" Fin=");pi(st.t_fin);ps("\nHlth: ");pi(st.n_hlth);ps(" Ec=");pi(st.t_ec);ps("\nMkt: ");pi(st.n_mkt);ps(" Sw=");pi(st.t_sw);ps("\n");}
void dolf_state(void){ps("[DOLF] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Train=");pi(st.n_train);ps(" Hlth=");pi(st.n_hlth);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dolphin Admin Demo ===\n\n");dolf_init();
/* 1=aquarium 2=sanctuary 3=ocean 4=reserve 5=market */
ps("Dolphin housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=150+(i*8),fh=25+(i*3),ei=(i%8)+1,ss=30+(i*5),ay=1+(i%10);dolf_housing(lc,bl,bw,fh,ei,ss,ay);}
ps("\nDolphin feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=160+(i*7),fh=28+(i*2),ei=(i%7)+1,ss=32+(i*4),ay=2+(i%8);dolf_feeding(lc,bl,bw,fh,ei,ss,ay);}
ps("\nDolphin training...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*7),bw=170+(i*6),fh=30+(i*2),ei=(i%6)+1,ss=35+(i*3),ay=2+(i%6);dolf_training(lc,bl,bw,fh,ei,ss,ay);}
ps("\nDolphin health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*12),bw=140+(i*10),fh=22+(i*4),ei=(i%9)+1,ss=28+(i*6),ay=3+(i%7);dolf_health(lc,bl,bw,fh,ei,ss,ay);}
ps("\nDolphin market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*6),bw=180+(i*5),fh=32+(i*2),ei=(i%5)+4,ss=38+(i*3),ay=3+(i%4);dolf_market(lc,bl,bw,fh,ei,ss,ay);}
ps("\n");dolf_report();dolf_state();ps("\n=== Demo Complete ===\n");return 0;}
