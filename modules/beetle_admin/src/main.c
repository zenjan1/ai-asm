/* beetle_admin: Beetle (Scarabaeus sacer) sacred scarab beetle insect (v1.0)
 * Beetle dung, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, leg_cm, crawl_speed, shell_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_cm,cr_sp,sh_idx,age_yr,active;} btl_t;
typedef struct{int n_dung,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_leg,t_cr,t_sh;} btl_state_t;
static btl_t dnl[N],dnf[N-2],dnb[N-4],dnh[N-6],dnm[N-6]; static btl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(btl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lc2,int cs,int si,int ay){if(*cnt>=mx)return -1;btl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_cm=lc2;x->cr_sp=cs;x->sh_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BTL] Beetle ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lc2=");pi(lc2);ps(" cs=");pi(cs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int beetle_init(void){if(init)return -1;st.n_dung=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_cr=0;st.t_sh=0;for(int i=0;i<N;i++)dnl[i].active=0;for(int i=0;i<N-2;i++)dnf[i].active=0;for(int i=0;i<N-4;i++)dnb[i].active=0;for(int i=0;i<N-6;i++)dnh[i].active=0;for(int i=0;i<N-6;i++)dnm[i].active=0;init=1;ps("[BTL] Beetle initialized\n");return 0;}
int beetle_dung(int lc,int bl,int bw,int lc2,int cs,int si,int ay){return add(dnl,&st.n_dung,&st.t_ln,N,lc,bl,bw,lc2,cs,si,ay);}
int beetle_feeding(int lc,int bl,int bw,int lc2,int cs,int si,int ay){return add(dnf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,lc2,cs,si,ay);}
int beetle_breeding(int lc,int bl,int bw,int lc2,int cs,int si,int ay){return add(dnb,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,lc2,cs,si,ay);}
int beetle_health(int lc,int bl,int bw,int lc2,int cs,int si,int ay){return add(dnh,&st.n_health,&st.t_cr,N-6,lc,bl,bw,lc2,cs,si,ay);}
int beetle_market(int lc,int bl,int bw,int lc2,int cs,int si,int ay){return add(dnm,&st.n_mkt,&st.t_sh,N-6,lc,bl,bw,lc2,cs,si,ay);}
void beetle_report(void){ps("[BTL] Dung: ");pi(st.n_dung);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHealth: ");pi(st.n_health);ps(" Cr=");pi(st.t_cr);ps("\nMkt: ");pi(st.n_mkt);ps(" Sh=");pi(st.t_sh);ps("\n");}
void beetle_state(void){ps("[BTL] Dung=");pi(st.n_dung);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Beetle Admin Demo ===\n\n");beetle_init();
ps("Beetle dung...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=2+(i%4),bw=5+(i*3),leg=1+(i%3),cs=1+(i*2),si=(i%8)+1,ay=(i%6)+1;beetle_dung(lc,bl,bw,leg,cs,si,ay);}
ps("\nBeetle feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=3+(i%3),bw=6+(i*2),leg=1+(i%2),cs=2+(i*1),si=(i%6)+1,ay=(i%5)+1;beetle_feeding(lc,bl,bw,leg,cs,si,ay);}
ps("\nBeetle breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=4+(i%3),bw=7+(i*2),leg=2+(i%2),cs=1+(i*3),si=(i%5)+1,ay=(i%4)+1;beetle_breeding(lc,bl,bw,leg,cs,si,ay);}
ps("\nBeetle health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=2+(i%5),bw=4+(i*4),leg=1+(i%4),cs=1+(i*4),si=(i%10)+1,ay=(i%3)+1;beetle_health(lc,bl,bw,leg,cs,si,ay);}
ps("\nBeetle market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=5+(i%3),bw=8+(i*2),leg=2+(i%2),cs=3+(i*2),si=(i%4)+1,ay=(i%3)+1;beetle_market(lc,bl,bw,leg,cs,si,ay);}
ps("\n");beetle_report();beetle_state();ps("\n=== Demo Complete ===\n");return 0;}
