/* skunk_admin: Skunk (Mephitis mephitis) striped mammal (v1.0)
 * Skunk forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, run_speed, stripe_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,run_sp,st_idx,age_yr,active;} sknk_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_run,t_st;} sknk_state_t;
static sknk_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static sknk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sknk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int si,int ay){if(*cnt>=mx)return -1;sknk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->run_sp=rs;x->st_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SKNK] Skunk ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int sknk_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_run=0;st.t_st=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SKNK] Skunk initialized\n");return 0;}
int sknk_forest(int lc,int bl,int bw,int tc,int rs,int si,int ay){return add(spl,&st.n_forest,&st.t_ln,N,lc,bl,bw,tc,rs,si,ay);}
int sknk_feeding(int lc,int bl,int bw,int tc,int rs,int si,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,si,ay);}
int sknk_breeding(int lc,int bl,int bw,int tc,int rs,int si,int ay){return add(sbr,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,si,ay);}
int sknk_health(int lc,int bl,int bw,int tc,int rs,int si,int ay){return add(shl,&st.n_health,&st.t_run,N-6,lc,bl,bw,tc,rs,si,ay);}
int sknk_market(int lc,int bl,int bw,int tc,int rs,int si,int ay){return add(smk,&st.n_mkt,&st.t_st,N-6,lc,bl,bw,tc,rs,si,ay);}
void sknk_report(void){ps("[SKNK] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" St=");pi(st.t_st);ps("\n");}
void sknk_state(void){ps("[SKNK] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Skunk Admin Demo ===\n\n");sknk_init();
ps("Skunk forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*5),bw=3+(i*2),tc=20+(i*3),rs=8+(i*2),si=(i%6)+1,ay=(i%4)+1;sknk_forest(lc,bl,bw,tc,rs,si,ay);}
ps("\nSkunk feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=45+(i*4),bw=4+(i*2),tc=22+(i*2),rs=10+(i*2),si=(i%5)+1,ay=(i%3)+1;sknk_feeding(lc,bl,bw,tc,rs,si,ay);}
ps("\nSkunk breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=35+(i*6),bw=2+(i*3),tc=18+(i*4),rs=6+(i*3),si=(i%4)+1,ay=(i%3)+1;sknk_breeding(lc,bl,bw,tc,rs,si,ay);}
ps("\nSkunk health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=50+(i*3),bw=5+(i),tc=25+(i*2),rs=12+(i),si=(i%7)+1,ay=(i%5)+2;sknk_health(lc,bl,bw,tc,rs,si,ay);}
ps("\nSkunk market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=55+(i*2),bw=6+(i),tc=28+(i),rs=7+(i*2),si=(i%3)+3,ay=(i%4)+3;sknk_market(lc,bl,bw,tc,rs,si,ay);}
ps("\n");sknk_report();sknk_state();ps("\n=== Demo Complete ===\n");return 0;}
