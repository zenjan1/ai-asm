/* snake_admin: Snake (Python reticulatus) reptile (v1.0)
 * Snake jungle, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, fang_cm, strike_speed, scale_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fang_cm,st_sp,sc_idx,age_yr,active;} snake_t;
typedef struct{int n_jungle,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fang,t_st,t_sc;} snake_state_t;
static snake_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static snake_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(snake_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int si,int ay){if(*cnt>=mx)return -1;snake_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fang_cm=fc;x->st_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SNAK] Snake ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int snake_init(void){if(init)return -1;st.n_jungle=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fang=0;st.t_st=0;st.t_sc=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SNAK] Snake initialized\n");return 0;}
int snake_jungle(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(spl,&st.n_jungle,&st.t_ln,N,lc,bl,bw,fc,ss,si,ay);}
int snake_feeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,si,ay);}
int snake_breeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(sbr,&st.n_breed,&st.t_fang,N-4,lc,bl,bw,fc,ss,si,ay);}
int snake_health(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(shl,&st.n_health,&st.t_st,N-6,lc,bl,bw,fc,ss,si,ay);}
int snake_market(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(smk,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,fc,ss,si,ay);}
void snake_report(void){ps("[SNAK] Jungle: ");pi(st.n_jungle);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fang=");pi(st.t_fang);ps("\nHealth: ");pi(st.n_health);ps(" St=");pi(st.t_st);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void snake_state(void){ps("[SNAK] Jungle=");pi(st.n_jungle);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Snake Admin Demo ===\n\n");snake_init();
ps("Snake jungle...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*40),bw=10+(i*5),fc=2+(i%3),ss=30+(i*8),si=(i%6)+1,ay=(i%5)+1;snake_jungle(lc,bl,bw,fc,ss,si,ay);}
ps("\nSnake feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=200+(i*30),bw=15+(i*4),fc=3+(i%2),ss=35+(i*6),si=(i%5)+1,ay=(i%4)+1;snake_feeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nSnake breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=120+(i*50),bw=8+(i*6),fc=1+(i%4),ss=25+(i*10),si=(i%4)+1,ay=(i%3)+1;snake_breeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nSnake health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=250+(i*20),bw=20+(i*3),fc=4+(i%2),ss=20+(i*5),si=(i%7)+1,ay=(i%6)+2;snake_health(lc,bl,bw,fc,ss,si,ay);}
ps("\nSnake market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=300+(i*15),bw=25+(i*2),fc=5+(i%2),ss=15+(i*7),si=(i%3)+3,ay=(i%4)+3;snake_market(lc,bl,bw,fc,ss,si,ay);}
ps("\n");snake_report();snake_state();ps("\n=== Demo Complete ===\n");return 0;}
