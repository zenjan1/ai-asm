/* sardine_admin: Sardine (Sardina pilchardus) small fish (v1.0)
 * Sardine ocean, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fin_cm, swim_speed, silver_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,sv_idx,age_yr,active;} sard_t;
typedef struct{int n_ocean,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_sv;} sard_state_t;
static sard_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static sard_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sard_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int sv,int ay){if(*cnt>=mx)return -1;sard_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->sv_idx=sv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SARD] Sardine ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" sv=");pi(sv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int sard_init(void){if(init)return -1;st.n_ocean=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_sv=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SARD] Sardine initialized\n");return 0;}
int sard_ocean(int lc,int bl,int bw,int fc,int ss,int sv,int ay){return add(spl,&st.n_ocean,&st.t_ln,N,lc,bl,bw,fc,ss,sv,ay);}
int sard_feeding(int lc,int bl,int bw,int fc,int ss,int sv,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,sv,ay);}
int sard_breeding(int lc,int bl,int bw,int fc,int ss,int sv,int ay){return add(sbr,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,sv,ay);}
int sard_health(int lc,int bl,int bw,int fc,int ss,int sv,int ay){return add(shl,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,sv,ay);}
int sard_market(int lc,int bl,int bw,int fc,int ss,int sv,int ay){return add(smk,&st.n_mkt,&st.t_sv,N-6,lc,bl,bw,fc,ss,sv,ay);}
void sard_report(void){ps("[SARD] Ocean: ");pi(st.n_ocean);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sv=");pi(st.t_sv);ps("\n");}
void sard_state(void){ps("[SARD] Ocean=");pi(st.n_ocean);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Sardine Admin Demo ===\n\n");sard_init();
ps("Sardine ocean...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=12+(i*2),bw=30+(i*8),fc=3+(i%3),ss=6+(i*2),sv=(i%7)+1,ay=(i%3)+1;sard_ocean(lc,bl,bw,fc,ss,sv,ay);}
ps("\nSardine feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=14+(i*2),bw=35+(i*6),fc=4+(i%2),ss=7+(i*2),sv=(i%5)+1,ay=(i%3)+1;sard_feeding(lc,bl,bw,fc,ss,sv,ay);}
ps("\nSardine breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=10+(i*3),bw=25+(i*10),fc=3+(i%4),ss=5+(i*3),sv=(i%6)+1,ay=(i%2)+1;sard_breeding(lc,bl,bw,fc,ss,sv,ay);}
ps("\nSardine health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=16+(i*2),bw=40+(i*5),fc=4+(i%3),ss=8+(i*2),sv=(i%4)+1,ay=(i%3)+2;sard_health(lc,bl,bw,fc,ss,sv,ay);}
ps("\nSardine market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=18+(i),bw=50+(i*4),fc=5+(i%2),ss=4+(i*3),sv=(i%8)+1,ay=(i%2)+2;sard_market(lc,bl,bw,fc,ss,sv,ay);}
ps("\n");sard_report();sard_state();ps("\n=== Demo Complete ===\n");return 0;}
