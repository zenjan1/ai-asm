/* carnivorous_admin: Carnivorous plant (Venus flytrap, pitcher, sundew) cultivation (v1.0)
 * Carnivorous cultivation, feeding, dormancy, propagation, market
 * Features: trap count, leaf span, prey caught, humidity, soil ph, feed week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,trap_ct,leaf_sp,prey_gm,humid_pct,soil_ph,feed_wk,active;} carn_t;
typedef struct{int n_cult,n_feed,n_dorm,n_prop,n_mkt,t_trap,t_leaf,t_prey,t_humid,t_soil;} carn_state_t;
static carn_t carncs[N],carnfs[N-2],carnds[N-4],carnprs[N-6],carnms[N-6]; static carn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(carn_t*a,int*cnt,int*sum,int mx,int lc,int tc,int ls,int pg,int hp,int sp,int fw){if(*cnt>=mx)return -1;carn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->trap_ct=tc;x->leaf_sp=ls;x->prey_gm=pg;x->humid_pct=hp;x->soil_ph=sp;x->feed_wk=fw;x->active=1;*sum+=tc;(*cnt)++;ps("[CARN] Carn ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" tc=");pi(tc);ps(" ls=");pi(ls);ps(" pg=");pi(pg);ps(" hp=");pi(hp);ps(" sp=");pi(sp);ps(" fw=");pi(fw);ps("\n");return *cnt-1;}
int carn_init(void){if(init)return -1;st.n_cult=0;st.n_feed=0;st.n_dorm=0;st.n_prop=0;st.n_mkt=0;st.t_trap=0;st.t_leaf=0;st.t_prey=0;st.t_humid=0;st.t_soil=0;for(int i=0;i<N;i++)carncs[i].active=0;for(int i=0;i<N-2;i++)carnfs[i].active=0;for(int i=0;i<N-4;i++)carnds[i].active=0;for(int i=0;i<N-6;i++)carnprs[i].active=0;for(int i=0;i<N-6;i++)carnms[i].active=0;init=1;ps("[CARN] Carnivorous initialized\n");return 0;}
int carn_cultivation(int lc,int tc,int ls,int pg,int hp,int sp,int fw){return add(carncs,&st.n_cult,&st.t_trap,N,lc,tc,ls,pg,hp,sp,fw);}
int carn_feeding(int lc,int tc,int ls,int pg,int hp,int sp,int fw){return add(carnfs,&st.n_feed,&st.t_leaf,N-2,lc,tc,ls,pg,hp,sp,fw);}
int carn_dormancy(int lc,int tc,int ls,int pg,int hp,int sp,int fw){return add(carnds,&st.n_dorm,&st.t_prey,N-4,lc,tc,ls,pg,hp,sp,fw);}
int carn_propagation(int lc,int tc,int ls,int pg,int hp,int sp,int fw){return add(carnprs,&st.n_prop,&st.t_humid,N-6,lc,tc,ls,pg,hp,sp,fw);}
int carn_market(int lc,int tc,int ls,int pg,int hp,int sp,int fw){return add(carnms,&st.n_mkt,&st.t_soil,N-6,lc,tc,ls,pg,hp,sp,fw);}
void carn_report(void){ps("[CARN] Cult: ");pi(st.n_cult);ps(" Trap=");pi(st.t_trap);ps("\nFeed: ");pi(st.n_feed);ps(" Leaf=");pi(st.t_leaf);ps("\nDorm: ");pi(st.n_dorm);ps(" Prey=");pi(st.t_prey);ps("\nProp: ");pi(st.n_prop);ps(" Humid=");pi(st.t_humid);ps("\nMkt: ");pi(st.n_mkt);ps(" Soil=");pi(st.t_soil);ps("\n");}
void carn_state(void){ps("[CARN] Cult=");pi(st.n_cult);ps(" Feed=");pi(st.n_feed);ps(" Dorm=");pi(st.n_dorm);ps(" Prop=");pi(st.n_prop);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Carnivorous Admin Demo ===\n\n");carn_init();
/* 1=bog 2=terrarium 3=greenhouse 4=outdoor 5=show */
ps("Carnivorous cultivation...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,tc=4+(i*2),ls=5+(i*3),pg=1+(i%5),hp=60+(i*3),sp=4+(i%3),fw=2+(i%4);carn_cultivation(lc,tc,ls,pg,hp,sp,fw);}
ps("\nCarnivorous feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,tc=5+(i*2),ls=6+(i*2),pg=2+(i%4),hp=65+(i*2),sp=4+(i%2),fw=3+(i%3);carn_feeding(lc,tc,ls,pg,hp,sp,fw);}
ps("\nCarnivorous dormancy...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,tc=3+(i*2),ls=4+(i*3),pg=1+(i%5),hp=55+(i*3),sp=5+(i%2),fw=1+(i%5);carn_dormancy(lc,tc,ls,pg,hp,sp,fw);}
ps("\nCarnivorous propagation...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,tc=6+(i*2),ls=7+(i*2),pg=2+(i%3),hp=70+(i*2),sp=4+(i%3),fw=4+(i%3);carn_propagation(lc,tc,ls,pg,hp,sp,fw);}
ps("\nCarnivorous market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,tc=8+(i*2),ls=8+(i*2),pg=3+(i%2),hp=75+(i*2),sp=3+(i%4),fw=5+(i%2);carn_market(lc,tc,ls,pg,hp,sp,fw);}
ps("\n");carn_report();carn_state();ps("\n=== Demo Complete ===\n");return 0;}
