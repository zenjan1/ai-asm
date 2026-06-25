/* earwig_admin: Earwig (Forficula auricularia) common pincher insect (v1.0)
 * Earwig garden, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, cerci_cm, crawl_speed, ant_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,cerc_cm,cr_sp,an_idx,age_yr,active;} erwg_t;
typedef struct{int n_garden,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_cerc,t_cr,t_an;} erwg_state_t;
static erwg_t grl[N],grf[N-2],grb[N-4],grh[N-6],grm[N-6]; static erwg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(erwg_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int cs,int ai,int ay){if(*cnt>=mx)return -1;erwg_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->cerc_cm=cc;x->cr_sp=cs;x->an_idx=ai;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ERWG] Earwig ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" cs=");pi(cs);ps(" ai=");pi(ai);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int earwig_init(void){if(init)return -1;st.n_garden=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_cerc=0;st.t_cr=0;st.t_an=0;for(int i=0;i<N;i++)grl[i].active=0;for(int i=0;i<N-2;i++)grf[i].active=0;for(int i=0;i<N-4;i++)grb[i].active=0;for(int i=0;i<N-6;i++)grh[i].active=0;for(int i=0;i<N-6;i++)grm[i].active=0;init=1;ps("[ERWG] Earwig initialized\n");return 0;}
int earwig_garden(int lc,int bl,int bw,int cc,int cs,int ai,int ay){return add(grl,&st.n_garden,&st.t_ln,N,lc,bl,bw,cc,cs,ai,ay);}
int earwig_feeding(int lc,int bl,int bw,int cc,int cs,int ai,int ay){return add(grf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,cs,ai,ay);}
int earwig_breeding(int lc,int bl,int bw,int cc,int cs,int ai,int ay){return add(grb,&st.n_breed,&st.t_cerc,N-4,lc,bl,bw,cc,cs,ai,ay);}
int earwig_health(int lc,int bl,int bw,int cc,int cs,int ai,int ay){return add(grh,&st.n_health,&st.t_cr,N-6,lc,bl,bw,cc,cs,ai,ay);}
int earwig_market(int lc,int bl,int bw,int cc,int cs,int ai,int ay){return add(grm,&st.n_mkt,&st.t_an,N-6,lc,bl,bw,cc,cs,ai,ay);}
void earwig_report(void){ps("[ERWG] Garden: ");pi(st.n_garden);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Cerc=");pi(st.t_cerc);ps("\nHealth: ");pi(st.n_health);ps(" Cr=");pi(st.t_cr);ps("\nMkt: ");pi(st.n_mkt);ps(" An=");pi(st.t_an);ps("\n");}
void earwig_state(void){ps("[ERWG] Garden=");pi(st.n_garden);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Earwig Admin Demo ===\n\n");earwig_init();
ps("Earwig garden...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=1+(i%2),bw=3+(i*1),cc=1+(i%3),cs=1+(i%3),ai=(i%8)+1,ay=(i%2)+1;earwig_garden(lc,bl,bw,cc,cs,ai,ay);}
ps("\nEarwig feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=1+(i%3),bw=4+(i%2),cc=1+(i%2),cs=1+(i%2),ai=(i%6)+1,ay=(i%2)+1;earwig_feeding(lc,bl,bw,cc,cs,ai,ay);}
ps("\nEarwig breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=2+(i%2),bw=4+(i%3),cc=2+(i%2),cs=1+(i%4),ai=(i%5)+1,ay=(i%2)+1;earwig_breeding(lc,bl,bw,cc,cs,ai,ay);}
ps("\nEarwig health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=1+(i%3),bw=3+(i%3),cc=1+(i%3),cs=1+(i%5),ai=(i%10)+1,ay=(i%2)+1;earwig_health(lc,bl,bw,cc,cs,ai,ay);}
ps("\nEarwig market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=2+(i%2),bw=5+(i%2),cc=2+(i%2),cs=2+(i%2),ai=(i%4)+1,ay=(i%2)+1;earwig_market(lc,bl,bw,cc,cs,ai,ay);}
ps("\n");earwig_report();earwig_state();ps("\n=== Demo Complete ===\n");return 0;}
