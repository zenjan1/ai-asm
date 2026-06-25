/* seahorse_admin: Seahorse (Hippocampus abdominalis) marine fish (v1.0)
 * Seahorse reef, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, swim_speed, ridge_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,sw_sp,rg_idx,age_yr,active;} seah_t;
typedef struct{int n_reef,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_sw,t_rg;} seah_state_t;
static seah_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static seah_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(seah_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ss,int ri,int ay){if(*cnt>=mx)return -1;seah_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->sw_sp=ss;x->rg_idx=ri;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SEAH] Seahorse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ss=");pi(ss);ps(" ri=");pi(ri);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int seah_init(void){if(init)return -1;st.n_reef=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_sw=0;st.t_rg=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SEAH] Seahorse initialized\n");return 0;}
int seah_reef(int lc,int bl,int bw,int tc,int ss,int ri,int ay){return add(spl,&st.n_reef,&st.t_ln,N,lc,bl,bw,tc,ss,ri,ay);}
int seah_feeding(int lc,int bl,int bw,int tc,int ss,int ri,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ss,ri,ay);}
int seah_breeding(int lc,int bl,int bw,int tc,int ss,int ri,int ay){return add(sbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,ss,ri,ay);}
int seah_health(int lc,int bl,int bw,int tc,int ss,int ri,int ay){return add(shl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,tc,ss,ri,ay);}
int seah_market(int lc,int bl,int bw,int tc,int ss,int ri,int ay){return add(smk,&st.n_mkt,&st.t_rg,N-6,lc,bl,bw,tc,ss,ri,ay);}
void seah_report(void){ps("[SEAH] Reef: ");pi(st.n_reef);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Rg=");pi(st.t_rg);ps("\n");}
void seah_state(void){ps("[SEAH] Reef=");pi(st.n_reef);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Seahorse Admin Demo ===\n\n");seah_init();
ps("Seahorse reef...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*2),bw=5+(i*3),tc=8+(i%4),ss=1+(i%2),ri=(i%7)+1,ay=(i%3)+1;seah_reef(lc,bl,bw,tc,ss,ri,ay);}
ps("\nSeahorse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=12+(i*2),bw=8+(i*2),tc=9+(i%3),ss=1+(i%3),ri=(i%5)+1,ay=(i%3)+1;seah_feeding(lc,bl,bw,tc,ss,ri,ay);}
ps("\nSeahorse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=14+(i),bw=10+(i*2),tc=10+(i%2),ss=0+(i%2),ri=(i%6)+1,ay=(i%2)+1;seah_breeding(lc,bl,bw,tc,ss,ri,ay);}
ps("\nSeahorse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=8+(i*3),bw=6+(i*4),tc=7+(i%5),ss=2+(i%2),ri=(i%4)+1,ay=(i%4)+2;seah_health(lc,bl,bw,tc,ss,ri,ay);}
ps("\nSeahorse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=18+(i),bw=12+(i*2),tc=12+(i%2),ss=0+(i%3),ri=(i%8)+2,ay=(i%3)+3;seah_market(lc,bl,bw,tc,ss,ri,ay);}
ps("\n");seah_report();seah_state();ps("\n=== Demo Complete ===\n");return 0;}
