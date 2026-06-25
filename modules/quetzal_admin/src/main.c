/* quetzal_admin: Quetzal (Pharomachrus mocinno) resplendent quetzal tropical bird (v1.0)
 * Quetzal canopy, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_len_cm, color_idx, fruit_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,cl_idx,fr_ct,age_yr,active;} qtzl_t;
typedef struct{int n_canopy,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_cl,t_fr;} qtzl_state_t;
static qtzl_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static qtzl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(qtzl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int ci,int fc,int ay){if(*cnt>=mx)return -1;qtzl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->cl_idx=ci;x->fr_ct=fc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[QTZL] Quetzal ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" ci=");pi(ci);ps(" fc=");pi(fc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int qtzl_init(void){if(init)return -1;st.n_canopy=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_cl=0;st.t_fr=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[QTZL] Quetzal initialized\n");return 0;}
int qtzl_canopy(int lc,int bl,int bw,int tl,int ci,int fc,int ay){return add(apl,&st.n_canopy,&st.t_ln,N,lc,bl,bw,tl,ci,fc,ay);}
int qtzl_feeding(int lc,int bl,int bw,int tl,int ci,int fc,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,ci,fc,ay);}
int qtzl_breeding(int lc,int bl,int bw,int tl,int ci,int fc,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,ci,fc,ay);}
int qtzl_health(int lc,int bl,int bw,int tl,int ci,int fc,int ay){return add(ahd,&st.n_health,&st.t_cl,N-6,lc,bl,bw,tl,ci,fc,ay);}
int qtzl_market(int lc,int bl,int bw,int tl,int ci,int fc,int ay){return add(amd,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tl,ci,fc,ay);}
void qtzl_report(void){ps("[QTZL] Canopy: ");pi(st.n_canopy);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void qtzl_state(void){ps("[QTZL] Canopy=");pi(st.n_canopy);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Quetzal Admin Demo ===\n\n");qtzl_init();
ps("Quetzal canopy...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*3),bw=150+(i*20),tl=50+(i*10),ci=(i%7)+1,fc=5+(i*3),ay=2+(i%10);qtzl_canopy(lc,bl,bw,tl,ci,fc,ay);}
ps("\nQuetzal feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*3),bw=155+(i*20),tl=55+(i*10),ci=(i%6)+2,fc=6+(i*3),ay=3+(i%8);qtzl_feeding(lc,bl,bw,tl,ci,fc,ay);}
ps("\nQuetzal breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=40+(i*3),bw=160+(i*20),tl=60+(i*10),ci=(i%5)+1,fc=7+(i*3),ay=4+(i%6);qtzl_breeding(lc,bl,bw,tl,ci,fc,ay);}
ps("\nQuetzal health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=33+(i*4),bw=145+(i*25),tl=45+(i*12),ci=(i%4)+3,fc=4+(i*4),ay=5+(i%5);qtzl_health(lc,bl,bw,tl,ci,fc,ay);}
ps("\nQuetzal market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=45+(i*3),bw=170+(i*20),tl=70+(i*10),ci=(i%3)+4,fc=8+(i*3),ay=6+(i%4);qtzl_market(lc,bl,bw,tl,ci,fc,ay);}
ps("\n");qtzl_report();qtzl_state();ps("\n=== Demo Complete ===\n");return 0;}
