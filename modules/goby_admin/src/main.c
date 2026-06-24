/* goby_admin: Goby (Gobiidae) small gobionid fish (v1.0)
 * Goby tank, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fin_ct, scale_idx, color_phase, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_ct,sc_idx,cl_ph,age_yr,active;} goby_t;
typedef struct{int n_tank,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sc,t_cl;} goby_state_t;
static goby_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static goby_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(goby_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int si,int cp,int ay){if(*cnt>=mx)return -1;goby_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_ct=fc;x->sc_idx=si;x->cl_ph=cp;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GOBY] Goby ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" si=");pi(si);ps(" cp=");pi(cp);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int goby_init(void){if(init)return -1;st.n_tank=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sc=0;st.t_cl=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GOBY] Goby initialized\n");return 0;}
int goby_tank(int lc,int bl,int bw,int fc,int si,int cp,int ay){return add(gpl,&st.n_tank,&st.t_ln,N,lc,bl,bw,fc,si,cp,ay);}
int goby_feeding(int lc,int bl,int bw,int fc,int si,int cp,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,si,cp,ay);}
int goby_breeding(int lc,int bl,int bw,int fc,int si,int cp,int ay){return add(gbr,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,si,cp,ay);}
int goby_health(int lc,int bl,int bw,int fc,int si,int cp,int ay){return add(ghl,&st.n_health,&st.t_sc,N-6,lc,bl,bw,fc,si,cp,ay);}
int goby_market(int lc,int bl,int bw,int fc,int si,int cp,int ay){return add(gmk,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,fc,si,cp,ay);}
void goby_report(void){ps("[GOBY] Tank: ");pi(st.n_tank);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHlth: ");pi(st.n_health);ps(" Sc=");pi(st.t_sc);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void goby_state(void){ps("[GOBY] Tank=");pi(st.n_tank);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Goby Admin Demo ===\n\n");goby_init();
/* 1=reef 2=sand 3=rock 4=tidepool 5=aquarium */
ps("Goby tank...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=5+(i*2),bw=3+(i*3),fc=4+(i%4),si=(i%5)+1,cp=(i%6)+1,ay=(i%3)+1;goby_tank(lc,bl,bw,fc,si,cp,ay);}
ps("\nGoby feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=6+(i*2),bw=4+(i*2),fc=5+(i%3),si=(i%4)+1,cp=(i%5)+1,ay=(i%3)+1;goby_feeding(lc,bl,bw,fc,si,cp,ay);}
ps("\nGoby breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=4+(i*3),bw=5+(i*2),fc=3+(i%5),si=(i%3)+2,cp=(i%4)+2,ay=(i%3)+1;goby_breeding(lc,bl,bw,fc,si,cp,ay);}
ps("\nGoby health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=7+(i*2),bw=6+(i*2),fc=6+(i%3),si=(i%5)+1,cp=(i%6)+1,ay=(i%4)+1;goby_health(lc,bl,bw,fc,si,cp,ay);}
ps("\nGoby market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=8+(i*2),bw=7+(i*2),fc=7+(i%2),si=(i%4)+1,cp=(i%5)+1,ay=(i%3)+1;goby_market(lc,bl,bw,fc,si,cp,ay);}
ps("\n");goby_report();goby_state();ps("\n=== Demo Complete ===\n");return 0;}
