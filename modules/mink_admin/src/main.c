/* mink_admin: Mink (Neovison vison) American mink semi-aquatic mustelid (v1.0)
 * Mink den, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_len_cm, dive_sec, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,dv_sc,fr_idx,age_yr,active;} mink_t;
typedef struct{int n_den,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_dv,t_fr;} mink_state_t;
static mink_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static mink_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mink_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int ds,int fi,int ay){if(*cnt>=mx)return -1;mink_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->dv_sc=ds;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MINK] Mink ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" ds=");pi(ds);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mink_init(void){if(init)return -1;st.n_den=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_dv=0;st.t_fr=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MINK] Mink initialized\n");return 0;}
int mink_den(int lc,int bl,int bw,int tl,int ds,int fi,int ay){return add(apl,&st.n_den,&st.t_ln,N,lc,bl,bw,tl,ds,fi,ay);}
int mink_feeding(int lc,int bl,int bw,int tl,int ds,int fi,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,ds,fi,ay);}
int mink_breeding(int lc,int bl,int bw,int tl,int ds,int fi,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,ds,fi,ay);}
int mink_health(int lc,int bl,int bw,int tl,int ds,int fi,int ay){return add(ahd,&st.n_health,&st.t_dv,N-6,lc,bl,bw,tl,ds,fi,ay);}
int mink_market(int lc,int bl,int bw,int tl,int ds,int fi,int ay){return add(amd,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tl,ds,fi,ay);}
void mink_report(void){ps("[MINK] Den: ");pi(st.n_den);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Dv=");pi(st.t_dv);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void mink_state(void){ps("[MINK] Den=");pi(st.n_den);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Mink Admin Demo ===\n\n");mink_init();
ps("Mink den...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*2),bw=800+(i*40),tl=15+(i*2),ds=20+(i*5),fi=(i%7)+1,ay=1+(i%8);mink_den(lc,bl,bw,tl,ds,fi,ay);}
ps("\nMink feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*2),bw=820+(i*40),tl=16+(i*2),ds=22+(i*5),fi=(i%6)+2,ay=2+(i%6);mink_feeding(lc,bl,bw,tl,ds,fi,ay);}
ps("\nMink breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=40+(i*2),bw=850+(i*40),tl=18+(i*2),ds=25+(i*5),fi=(i%5)+1,ay=3+(i%5);mink_breeding(lc,bl,bw,tl,ds,fi,ay);}
ps("\nMink health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=33+(i*3),bw=780+(i*50),tl=14+(i*3),ds=18+(i*6),fi=(i%4)+3,ay=4+(i%4);mink_health(lc,bl,bw,tl,ds,fi,ay);}
ps("\nMink market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=45+(i*2),bw=900+(i*40),tl=20+(i*2),ds=30+(i*5),fi=(i%3)+4,ay=5+(i%3);mink_market(lc,bl,bw,tl,ds,fi,ay);}
ps("\n");mink_report();mink_state();ps("\n=== Demo Complete ===\n");return 0;}
