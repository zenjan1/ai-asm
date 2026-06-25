/* tetra_admin: Tetra (Tetraodon tetra) freshwater fish (v1.0)
 * Tetra aquarium, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fin_cm, swim_speed, color_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,cl_idx,age_yr,active;} ttra_t;
typedef struct{int n_aqrm,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_cl;} ttra_state_t;
static ttra_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static ttra_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ttra_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int ci,int ay){if(*cnt>=mx)return -1;ttra_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->cl_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TTRA] Tetra ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ttra_init(void){if(init)return -1;st.n_aqrm=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_cl=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[TTRA] Tetra initialized\n");return 0;}
int ttra_aquarium(int lc,int bl,int bw,int fc,int ss,int ci,int ay){return add(spl,&st.n_aqrm,&st.t_ln,N,lc,bl,bw,fc,ss,ci,ay);}
int ttra_feeding(int lc,int bl,int bw,int fc,int ss,int ci,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,ci,ay);}
int ttra_breeding(int lc,int bl,int bw,int fc,int ss,int ci,int ay){return add(sbr,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,ci,ay);}
int ttra_health(int lc,int bl,int bw,int fc,int ss,int ci,int ay){return add(shl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,ci,ay);}
int ttra_market(int lc,int bl,int bw,int fc,int ss,int ci,int ay){return add(smk,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,fc,ss,ci,ay);}
void ttra_report(void){ps("[TTRA] Aquarium: ");pi(st.n_aqrm);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void ttra_state(void){ps("[TTRA] Aqrm=");pi(st.n_aqrm);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tetra Admin Demo ===\n\n");ttra_init();
ps("Tetra aquarium...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=4+(i%3),bw=5+(i*2),fc=2+(i%2),ss=3+(i*2),ci=(i%6)+1,ay=(i%2)+1;ttra_aquarium(lc,bl,bw,fc,ss,ci,ay);}
ps("\nTetra feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=5+(i%2),bw=8+(i*2),fc=3+(i%2),ss=4+(i*2),ci=(i%5)+1,ay=(i%2)+1;ttra_feeding(lc,bl,bw,fc,ss,ci,ay);}
ps("\nTetra breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=3+(i*2),bw=4+(i*3),fc=2+(i%3),ss=2+(i*3),ci=(i%4)+1,ay=(i%2)+1;ttra_breeding(lc,bl,bw,fc,ss,ci,ay);}
ps("\nTetra health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=6+(i%2),bw=10+(i),fc=3+(i%2),ss=5+(i),ci=(i%7)+1,ay=(i%3)+2;ttra_health(lc,bl,bw,fc,ss,ci,ay);}
ps("\nTetra market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=7+(i),bw=12+(i),fc=4+(i%2),ss=2+(i*2),ci=(i%3)+3,ay=(i%2)+2;ttra_market(lc,bl,bw,fc,ss,ci,ay);}
ps("\n");ttra_report();ttra_state();ps("\n=== Demo Complete ===\n");return 0;}
