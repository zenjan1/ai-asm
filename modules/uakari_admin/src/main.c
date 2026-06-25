/* uakari_admin: Uakari (Cacajao uakarii) red-faced bald monkey (v1.0)
 * Uakari forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, climb_speed, fr_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,cl_sp,fr_idx,age_yr,active;} ukar_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_cl,t_fr;} ukar_state_t;
static ukar_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static ukar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ukar_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int cs,int fi,int ay){if(*cnt>=mx)return -1;ukar_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->cl_sp=cs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[UKAR] Uakari ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" cs=");pi(cs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int uakari_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_cl=0;st.t_fr=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[UKAR] Uakari initialized\n");return 0;}
int uakari_forest(int lc,int bl,int bw,int tc,int cs,int fi,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,tc,cs,fi,ay);}
int uakari_feeding(int lc,int bl,int bw,int tc,int cs,int fi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,cs,fi,ay);}
int uakari_breeding(int lc,int bl,int bw,int tc,int cs,int fi,int ay){return add(svb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,cs,fi,ay);}
int uakari_health(int lc,int bl,int bw,int tc,int cs,int fi,int ay){return add(svh,&st.n_health,&st.t_cl,N-6,lc,bl,bw,tc,cs,fi,ay);}
int uakari_market(int lc,int bl,int bw,int tc,int cs,int fi,int ay){return add(svm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,cs,fi,ay);}
void uakari_report(void){ps("[UKAR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void uakari_state(void){ps("[UKAR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Uakari Admin Demo ===\n\n");uakari_init();
ps("Uakari forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=45+(i*3),bw=3+(i*1),tc=15+(i*1),cs=2+(i*1),fi=(i%8)+1,ay=(i%20)+1;uakari_forest(lc,bl,bw,tc,cs,fi,ay);}
ps("\nUakari feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=47+(i*2),bw=3+(i*1),tc=16+(i*1),cs=3+(i*1),fi=(i%6)+1,ay=(i%18)+1;uakari_feeding(lc,bl,bw,tc,cs,fi,ay);}
ps("\nUakari breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=48+(i*2),bw=4+(i*1),tc=17+(i*1),cs=2+(i*2),fi=(i%5)+1,ay=(i%15)+1;uakari_breeding(lc,bl,bw,tc,cs,fi,ay);}
ps("\nUakari health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=44+(i*3),bw=3+(i*2),tc=14+(i*2),cs=2+(i*1),fi=(i%10)+1,ay=(i%12)+1;uakari_health(lc,bl,bw,tc,cs,fi,ay);}
ps("\nUakari market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=50+(i*1),bw=4+(i*1),tc=18+(i*1),cs=4+(i*1),fi=(i%4)+1,ay=(i%10)+1;uakari_market(lc,bl,bw,tc,cs,fi,ay);}
ps("\n");uakari_report();uakari_state();ps("\n=== Demo Complete ===\n");return 0;}
