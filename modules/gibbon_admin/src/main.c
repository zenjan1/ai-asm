/* gibbon_admin: Gibbon (Hylobates lar) lar gibbon primate (v1.0)
 * Gibbon habitat, feeding, breeding, grooming, market
 * Features: body_len_cm, body_wt_kg, arm_span_cm, swing_idx, call_vol, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,arm_sp,swg_idx,cl_vl,age_yr,active;} gibbon_t;
typedef struct{int n_habit,n_feed,n_breed,n_groom,n_mkt,t_ln,t_wt,t_arm,t_swg,t_cl;} gibbon_state_t;
static gibbon_t gpl[N],gfd[N-2],gbr[N-4],ggr[N-6],gmk[N-6]; static gibbon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gibbon_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int as,int si,int cv,int ay){if(*cnt>=mx)return -1;gibbon_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->arm_sp=as;x->swg_idx=si;x->cl_vl=cv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GIB] Gibbon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" as=");pi(as);ps(" si=");pi(si);ps(" cv=");pi(cv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gibbon_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_arm=0;st.t_swg=0;st.t_cl=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ggr[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GIB] Gibbon initialized\n");return 0;}
int gibbon_habitat(int lc,int bl,int bw,int as,int si,int cv,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,as,si,cv,ay);}
int gibbon_feeding(int lc,int bl,int bw,int as,int si,int cv,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,as,si,cv,ay);}
int gibbon_breeding(int lc,int bl,int bw,int as,int si,int cv,int ay){return add(gbr,&st.n_breed,&st.t_arm,N-4,lc,bl,bw,as,si,cv,ay);}
int gibbon_grooming(int lc,int bl,int bw,int as,int si,int cv,int ay){return add(ggr,&st.n_groom,&st.t_swg,N-6,lc,bl,bw,as,si,cv,ay);}
int gibbon_market(int lc,int bl,int bw,int as,int si,int cv,int ay){return add(gmk,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,as,si,cv,ay);}
void gibbon_report(void){ps("[GIB] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Arm=");pi(st.t_arm);ps("\nGroom: ");pi(st.n_groom);ps(" Swg=");pi(st.t_swg);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void gibbon_state(void){ps("[GIB] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gibbon Admin Demo ===\n\n");gibbon_init();
/* 1=forest 2=jungle 3=canopy 4=reserve 5=sanctuary */
ps("Gibbon habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*3),bw=6+(i*2),as=70+(i*4),si=(i%5)+1,cv=30+(i*8),ay=(i%6)+1;gibbon_habitat(lc,bl,bw,as,si,cv,ay);}
ps("\nGibbon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=52+(i*3),bw=7+(i*2),as=72+(i*3),si=(i%4)+1,cv=35+(i*6),ay=(i%5)+1;gibbon_feeding(lc,bl,bw,as,si,cv,ay);}
ps("\nGibbon breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=48+(i*4),bw=8+(i*2),as=68+(i*4),si=(i%3)+2,cv=25+(i*7),ay=(i%4)+2;gibbon_breeding(lc,bl,bw,as,si,cv,ay);}
ps("\nGibbon grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=55+(i*3),bw=5+(i*2),as=75+(i*3),si=(i%5)+1,cv=40+(i*5),ay=(i%5)+1;gibbon_grooming(lc,bl,bw,as,si,cv,ay);}
ps("\nGibbon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=58+(i*2),bw=9+(i*2),as=80+(i*3),si=(i%4)+1,cv=45+(i*4),ay=(i%4)+1;gibbon_market(lc,bl,bw,as,si,cv,ay);}
ps("\n");gibbon_report();gibbon_state();ps("\n=== Demo Complete ===\n");return 0;}
