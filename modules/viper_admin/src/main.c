/* viper_admin: Viper (Vipera berus) venomous snake (v1.0)
 * Viper heathland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, fang_cm, strike_speed, scale_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fang_cm,st_sp,sc_idx,age_yr,active;} vipr_t;
typedef struct{int n_heath,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fang,t_st,t_sc;} vipr_state_t;
static vipr_t vpl[N],vfd[N-2],vbr[N-4],vhl[N-6],vmk[N-6]; static vipr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vipr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int si,int ay){if(*cnt>=mx)return -1;vipr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fang_cm=fc;x->st_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[VIPR] Viper ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int vipr_init(void){if(init)return -1;st.n_heath=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fang=0;st.t_st=0;st.t_sc=0;for(int i=0;i<N;i++)vpl[i].active=0;for(int i=0;i<N-2;i++)vfd[i].active=0;for(int i=0;i<N-4;i++)vbr[i].active=0;for(int i=0;i<N-6;i++)vhl[i].active=0;for(int i=0;i<N-6;i++)vmk[i].active=0;init=1;ps("[VIPR] Viper initialized\n");return 0;}
int vipr_heathland(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(vpl,&st.n_heath,&st.t_ln,N,lc,bl,bw,fc,ss,si,ay);}
int vipr_feeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(vfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,si,ay);}
int vipr_breeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(vbr,&st.n_breed,&st.t_fang,N-4,lc,bl,bw,fc,ss,si,ay);}
int vipr_health(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(vhl,&st.n_health,&st.t_st,N-6,lc,bl,bw,fc,ss,si,ay);}
int vipr_market(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(vmk,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,fc,ss,si,ay);}
void vipr_report(void){ps("[VIPR] Heath: ");pi(st.n_heath);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fang=");pi(st.t_fang);ps("\nHealth: ");pi(st.n_health);ps(" St=");pi(st.t_st);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void vipr_state(void){ps("[VIPR] Heath=");pi(st.n_heath);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Viper Admin Demo ===\n\n");vipr_init();
ps("Viper heathland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=60+(i*5),bw=0+(i%2),fc=1+(i%2),ss=15+(i*4),si=(i%6)+1,ay=(i%4)+1;vipr_heathland(lc,bl,bw,fc,ss,si,ay);}
ps("\nViper feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=65+(i*4),bw=1+(i%2),fc=1+(i%3),ss=18+(i*3),si=(i%5)+1,ay=(i%3)+1;vipr_feeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nViper breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=55+(i*6),bw=0+(i%3),fc=0+(i%2),ss=12+(i*5),si=(i%4)+1,ay=(i%3)+1;vipr_breeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nViper health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=70+(i*3),bw=1+(i%2),fc=1+(i%2),ss=20+(i*2),si=(i%7)+1,ay=(i%5)+2;vipr_health(lc,bl,bw,fc,ss,si,ay);}
ps("\nViper market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=75+(i*2),bw=1+(i%2),fc=2+(i%2),ss=10+(i*3),si=(i%3)+3,ay=(i%4)+3;vipr_market(lc,bl,bw,fc,ss,si,ay);}
ps("\n");vipr_report();vipr_state();ps("\n=== Demo Complete ===\n");return 0;}
