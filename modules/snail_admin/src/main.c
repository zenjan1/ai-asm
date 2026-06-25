/* snail_admin: Snail (Helix aspersa) garden mollusk (v1.0)
 * Snail garden, feeding, breeding, health, market
 * Features: shell_cm, body_wt_g, body_cm, crawl_speed, spiral_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,shl_cm,bdy_wt,bdy_cm,cr_sp,sp_idx,age_yr,active;} snail_t;
typedef struct{int n_garden,n_feed,n_breed,n_health,n_mkt,t_sh,t_wt,t_bd,t_cr,t_sp;} snail_state_t;
static snail_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static snail_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(snail_t*a,int*cnt,int*sum,int mx,int lc,int sc,int bw,int bc,int cs,int si,int ay){if(*cnt>=mx)return -1;snail_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->shl_cm=sc;x->bdy_wt=bw;x->bdy_cm=bc;x->cr_sp=cs;x->sp_idx=si;x->age_yr=ay;x->active=1;*sum+=sc;(*cnt)++;ps("[SNAIL] Snail ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sc=");pi(sc);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" cs=");pi(cs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int snail_init(void){if(init)return -1;st.n_garden=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_sh=0;st.t_wt=0;st.t_bd=0;st.t_cr=0;st.t_sp=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SNAIL] Snail initialized\n");return 0;}
int snail_garden(int lc,int sc,int bw,int bc,int cs,int si,int ay){return add(spl,&st.n_garden,&st.t_sh,N,lc,sc,bw,bc,cs,si,ay);}
int snail_feeding(int lc,int sc,int bw,int bc,int cs,int si,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,sc,bw,bc,cs,si,ay);}
int snail_breeding(int lc,int sc,int bw,int bc,int cs,int si,int ay){return add(sbr,&st.n_breed,&st.t_bd,N-4,lc,sc,bw,bc,cs,si,ay);}
int snail_health(int lc,int sc,int bw,int bc,int cs,int si,int ay){return add(shl,&st.n_health,&st.t_cr,N-6,lc,sc,bw,bc,cs,si,ay);}
int snail_market(int lc,int sc,int bw,int bc,int cs,int si,int ay){return add(smk,&st.n_mkt,&st.t_sp,N-6,lc,sc,bw,bc,cs,si,ay);}
void snail_report(void){ps("[SNAIL] Garden: ");pi(st.n_garden);ps(" Sh=");pi(st.t_sh);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Bd=");pi(st.t_bd);ps("\nHealth: ");pi(st.n_health);ps(" Cr=");pi(st.t_cr);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void snail_state(void){ps("[SNAIL] Garden=");pi(st.n_garden);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Snail Admin Demo ===\n\n");snail_init();
ps("Snail garden...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sc=3+(i%3),bw=8+(i*3),bc=2+(i%2),cs=0+(i%2),si=(i%6)+1,ay=(i%3)+1;snail_garden(lc,sc,bw,bc,cs,si,ay);}
ps("\nSnail feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,sc=4+(i%2),bw=10+(i*2),bc=3+(i%2),cs=1+(i%2),si=(i%5)+1,ay=(i%3)+1;snail_feeding(lc,sc,bw,bc,cs,si,ay);}
ps("\nSnail breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sc=2+(i%4),bw=6+(i*4),bc=2+(i%3),cs=0+(i%3),si=(i%4)+1,ay=(i%2)+1;snail_breeding(lc,sc,bw,bc,cs,si,ay);}
ps("\nSnail health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sc=5+(i%2),bw=12+(i*2),bc=4+(i),cs=0+(i%2),si=(i%7)+1,ay=(i%4)+2;snail_health(lc,sc,bw,bc,cs,si,ay);}
ps("\nSnail market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sc=6+(i),bw=15+(i*2),bc=5+(i%2),cs=0+(i%3),si=(i%3)+3,ay=(i%2)+3;snail_market(lc,sc,bw,bc,cs,si,ay);}
ps("\n");snail_report();snail_state();ps("\n=== Demo Complete ===\n");return 0;}
