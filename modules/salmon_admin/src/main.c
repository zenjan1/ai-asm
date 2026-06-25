/* salmon_admin: Salmon (Salmo salar) migratory fish (v1.0)
 * Salmon river, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, fin_cm, swim_speed, scale_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,sc_idx,age_yr,active;} salm_t;
typedef struct{int n_river,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_sc;} salm_state_t;
static salm_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static salm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(salm_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int si,int ay){if(*cnt>=mx)return -1;salm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SALM] Salmon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int salm_init(void){if(init)return -1;st.n_river=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_sc=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SALM] Salmon initialized\n");return 0;}
int salm_river(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(spl,&st.n_river,&st.t_ln,N,lc,bl,bw,fc,ss,si,ay);}
int salm_feeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,si,ay);}
int salm_breeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(sbr,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,si,ay);}
int salm_health(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(shl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,si,ay);}
int salm_market(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(smk,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,fc,ss,si,ay);}
void salm_report(void){ps("[SALM] River: ");pi(st.n_river);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void salm_state(void){ps("[SALM] River=");pi(st.n_river);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Salmon Admin Demo ===\n\n");salm_init();
ps("Salmon river...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*5),bw=2+(i%4),fc=8+(i%3),ss=10+(i*2),si=(i%6)+1,ay=(i%5)+1;salm_river(lc,bl,bw,fc,ss,si,ay);}
ps("\nSalmon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=55+(i*4),bw=3+(i%3),fc=9+(i%2),ss=12+(i*2),si=(i%5)+1,ay=(i%4)+1;salm_feeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nSalmon breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=60+(i*3),bw=4+(i%2),fc=10+(i%2),ss=8+(i*3),si=(i%4)+1,ay=(i%3)+2;salm_breeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nSalmon health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=45+(i*6),bw=2+(i%5),fc=7+(i%4),ss=15+(i*2),si=(i%3)+1,ay=(i%6)+1;salm_health(lc,bl,bw,fc,ss,si,ay);}
ps("\nSalmon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=70+(i*3),bw=5+(i%2),fc=11+(i%2),ss=6+(i*4),si=(i%5)+2,ay=(i%4)+3;salm_market(lc,bl,bw,fc,ss,si,ay);}
ps("\n");salm_report();salm_state();ps("\n=== Demo Complete ===\n");return 0;}
