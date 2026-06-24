/* buck_admin: Buck (Deer) wildlife habitat and population management (v1.0)
 * Buck population planning, habitat eval, hunting quota, census, market
 * Features: antler points, weight, herd size, territory area, age, rut season
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,antl_pt,wght_kg,herd_sz,terr_ac,age_yr,rut_sn,active;} buck_t;
typedef struct{int n_pop,n_hab,n_hunt,n_census,n_mkt,t_antl,t_wght,t_herd,t_terr,t_age;} buck_state_t;
static buck_t buckps[N],buckhs[N-2],buckhunts[N-4],buckcs[N-6],buckms[N-6]; static buck_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(buck_t*a,int*cnt,int*sum,int mx,int lc,int ap,int wk,int hs,int ta,int ay,int rs){if(*cnt>=mx)return -1;buck_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->antl_pt=ap;x->wght_kg=wk;x->herd_sz=hs;x->terr_ac=ta;x->age_yr=ay;x->rut_sn=rs;x->active=1;*sum+=ap;(*cnt)++;ps("[BUCK] Buck ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ap=");pi(ap);ps(" wk=");pi(wk);ps(" hs=");pi(hs);ps(" ta=");pi(ta);ps(" ay=");pi(ay);ps(" rs=");pi(rs);ps("\n");return *cnt-1;}
int buck_init(void){if(init)return -1;st.n_pop=0;st.n_hab=0;st.n_hunt=0;st.n_census=0;st.n_mkt=0;st.t_antl=0;st.t_wght=0;st.t_herd=0;st.t_terr=0;st.t_age=0;for(int i=0;i<N;i++)buckps[i].active=0;for(int i=0;i<N-2;i++)buckhs[i].active=0;for(int i=0;i<N-4;i++)buckhunts[i].active=0;for(int i=0;i<N-6;i++)buckcs[i].active=0;for(int i=0;i<N-6;i++)buckms[i].active=0;init=1;ps("[BUCK] Buck initialized\n");return 0;}
int buck_population(int lc,int ap,int wk,int hs,int ta,int ay,int rs){return add(buckps,&st.n_pop,&st.t_antl,N,lc,ap,wk,hs,ta,ay,rs);}
int buck_habitat(int lc,int ap,int wk,int hs,int ta,int ay,int rs){return add(buckhs,&st.n_hab,&st.t_wght,N-2,lc,ap,wk,hs,ta,ay,rs);}
int buck_hunting(int lc,int ap,int wk,int hs,int ta,int ay,int rs){return add(buckhunts,&st.n_hunt,&st.t_herd,N-4,lc,ap,wk,hs,ta,ay,rs);}
int buck_census(int lc,int ap,int wk,int hs,int ta,int ay,int rs){return add(buckcs,&st.n_census,&st.t_terr,N-6,lc,ap,wk,hs,ta,ay,rs);}
int buck_market(int lc,int ap,int wk,int hs,int ta,int ay,int rs){return add(buckms,&st.n_mkt,&st.t_age,N-6,lc,ap,wk,hs,ta,ay,rs);}
void buck_report(void){ps("[BUCK] Pop: ");pi(st.n_pop);ps(" Antl=");pi(st.t_antl);ps("\nHab: ");pi(st.n_hab);ps(" Wght=");pi(st.t_wght);ps("\nHunt: ");pi(st.n_hunt);ps(" Herd=");pi(st.t_herd);ps("\nCensus: ");pi(st.n_census);ps(" Terr=");pi(st.t_terr);ps("\nMkt: ");pi(st.n_mkt);ps(" Age=");pi(st.t_age);ps("\n");}
void buck_state(void){ps("[BUCK] Pop=");pi(st.n_pop);ps(" Hab=");pi(st.n_hab);ps(" Hunt=");pi(st.n_hunt);ps(" Census=");pi(st.n_census);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Buck Admin Demo ===\n\n");buck_init();
/* 1=woodland 2=prairie 3=wetland 4=mountain 5=farmland */
ps("Buck population...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ap=4+(i%8),wk=60+(i*10),hs=5+(i*3),ta=20+(i*10),ay=2+(i%12),rs=(i%4)+1;buck_population(lc,ap,wk,hs,ta,ay,rs);}
ps("\nBuck habitat...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ap=6+(i%6),wk=80+(i*8),hs=8+(i*2),ta=30+(i*8),ay=3+(i%10),rs=(i%4)+1;buck_habitat(lc,ap,wk,hs,ta,ay,rs);}
ps("\nBuck hunting...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ap=8+(i%5),wk=100+(i*6),hs=10+(i*2),ta=40+(i*6),ay=4+(i%8),rs=(i%3)+2;buck_hunting(lc,ap,wk,hs,ta,ay,rs);}
ps("\nBuck census...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ap=5+(i%7),wk=70+(i*9),hs=6+(i*3),ta=25+(i*9),ay=2+(i%11),rs=(i%4)+1;buck_census(lc,ap,wk,hs,ta,ay,rs);}
ps("\nBuck market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ap=10+(i%4),wk=120+(i*5),hs=12+(i*2),ta=50+(i*5),ay=5+(i%6),rs=(i%3)+2;buck_market(lc,ap,wk,hs,ta,ay,rs);}
ps("\n");buck_report();buck_state();ps("\n=== Demo Complete ===\n");return 0;}
