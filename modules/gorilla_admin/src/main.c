/* gorilla_admin: Gorilla (Gorilla gorilla) western gorilla great ape (v1.0)
 * Gorilla habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, arm_span_cm, chest_cm, silverback_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,arm_sp,chst_cm,sb_idx,age_yr,active;} gorilla_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_arm,t_chst,t_sb;} gorilla_state_t;
static gorilla_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static gorilla_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gorilla_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int as,int cc,int si,int ay){if(*cnt>=mx)return -1;gorilla_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->arm_sp=as;x->chst_cm=cc;x->sb_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GOR] Gorilla ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" as=");pi(as);ps(" cc=");pi(cc);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gorilla_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_arm=0;st.t_chst=0;st.t_sb=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GOR] Gorilla initialized\n");return 0;}
int gorilla_habitat(int lc,int bl,int bw,int as,int cc,int si,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,as,cc,si,ay);}
int gorilla_feeding(int lc,int bl,int bw,int as,int cc,int si,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,as,cc,si,ay);}
int gorilla_breeding(int lc,int bl,int bw,int as,int cc,int si,int ay){return add(gbr,&st.n_breed,&st.t_arm,N-4,lc,bl,bw,as,cc,si,ay);}
int gorilla_health(int lc,int bl,int bw,int as,int cc,int si,int ay){return add(ghl,&st.n_health,&st.t_chst,N-6,lc,bl,bw,as,cc,si,ay);}
int gorilla_market(int lc,int bl,int bw,int as,int cc,int si,int ay){return add(gmk,&st.n_mkt,&st.t_sb,N-6,lc,bl,bw,as,cc,si,ay);}
void gorilla_report(void){ps("[GOR] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Arm=");pi(st.t_arm);ps("\nHlth: ");pi(st.n_health);ps(" Chst=");pi(st.t_chst);ps("\nMkt: ");pi(st.n_mkt);ps(" Sb=");pi(st.t_sb);ps("\n");}
void gorilla_state(void){ps("[GOR] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gorilla Admin Demo ===\n\n");gorilla_init();
/* 1=forest 2=sanctuary 3=zoo 4=reserve 5=park */
ps("Gorilla habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*8),bw=120+(i*10),as=200+(i*6),cc=100+(i*5),si=(i%3)+1,ay=(i%8)+1;gorilla_habitat(lc,bl,bw,as,cc,si,ay);}
ps("\nGorilla feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=155+(i*6),bw=125+(i*8),as=205+(i*5),cc=105+(i*4),si=(i%3)+1,ay=(i%6)+1;gorilla_feeding(lc,bl,bw,as,cc,si,ay);}
ps("\nGorilla breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=145+(i*10),bw=130+(i*8),as=195+(i*7),cc=95+(i*6),si=(i%2)+1,ay=(i%5)+2;gorilla_breeding(lc,bl,bw,as,cc,si,ay);}
ps("\nGorilla health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=160+(i*6),bw=135+(i*7),as=210+(i*5),cc=110+(i*4),si=(i%3)+1,ay=(i%6)+1;gorilla_health(lc,bl,bw,as,cc,si,ay);}
ps("\nGorilla market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=170+(i*5),bw=140+(i*6),as=215+(i*4),cc=115+(i*3),si=(i%2)+1,ay=(i%4)+2;gorilla_market(lc,bl,bw,as,cc,si,ay);}
ps("\n");gorilla_report();gorilla_state();ps("\n=== Demo Complete ===\n");return 0;}
