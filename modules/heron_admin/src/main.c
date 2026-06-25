/* heron_admin: Heron (Ardea cinerea) grey heron wading bird (v1.0)
 * Heron habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, leg_cm, beak_cm, fish_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_cm,bk_cm,f_ct,age_yr,active;} heron_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_leg,t_bk,t_f;} heron_state_t;
static heron_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static heron_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(heron_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lc2,int bc,int fc,int ay){if(*cnt>=mx)return -1;heron_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_cm=lc2;x->bk_cm=bc;x->f_ct=fc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HERN] Heron ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lc2=");pi(lc2);ps(" bc=");pi(bc);ps(" fc=");pi(fc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int heron_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_bk=0;st.t_f=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HERN] Heron initialized\n");return 0;}
int heron_habitat(int lc,int bl,int bw,int leg,int bc,int fc,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,leg,bc,fc,ay);}
int heron_feeding(int lc,int bl,int bw,int leg,int bc,int fc,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,leg,bc,fc,ay);}
int heron_breeding(int lc,int bl,int bw,int leg,int bc,int fc,int ay){return add(gbr,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,leg,bc,fc,ay);}
int heron_health(int lc,int bl,int bw,int leg,int bc,int fc,int ay){return add(ghl,&st.n_health,&st.t_bk,N-6,lc,bl,bw,leg,bc,fc,ay);}
int heron_market(int lc,int bl,int bw,int leg,int bc,int fc,int ay){return add(gmk,&st.n_mkt,&st.t_f,N-6,lc,bl,bw,leg,bc,fc,ay);}
void heron_report(void){ps("[HERN] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHlth: ");pi(st.n_health);ps(" Bk=");pi(st.t_bk);ps("\nMkt: ");pi(st.n_mkt);ps(" F=");pi(st.t_f);ps("\n");}
void heron_state(void){ps("[HERN] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Heron Admin Demo ===\n\n");heron_init();
/* 1=marsh 2=river 3=lake 4=shore 5=estuary */
ps("Heron habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=90+(i*5),bw=2+(i*2),leg=12+(i*2),bk=15+(i%4),fc=3+(i*2),ay=(i%6)+1;heron_habitat(lc,bl,bw,leg,bk,fc,ay);}
ps("\nHeron feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=95+(i*4),bw=2+(i*2),leg=13+(i*2),bk=16+(i%3),fc=4+(i*2),ay=(i%5)+1;heron_feeding(lc,bl,bw,leg,bk,fc,ay);}
ps("\nHeron breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=85+(i*6),bw=3+(i*2),leg=11+(i*3),bk=14+(i%4),fc=2+(i*3),ay=(i%4)+1;heron_breeding(lc,bl,bw,leg,bk,fc,ay);}
ps("\nHeron health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=100+(i*4),bw=3+(i*2),leg=14+(i*2),bk=17+(i%3),fc=5+(i*2),ay=(i%4)+1;heron_health(lc,bl,bw,leg,bk,fc,ay);}
ps("\nHeron market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=105+(i*3),bw=3+(i*2),leg=15+(i*2),bk=18+(i%2),fc=6+(i*2),ay=(i%3)+2;heron_market(lc,bl,bw,leg,bk,fc,ay);}
ps("\n");heron_report();heron_state();ps("\n=== Demo Complete ===\n");return 0;}
