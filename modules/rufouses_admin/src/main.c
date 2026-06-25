/* rufouses_admin: Rufouses (Aepyprymnus) rufous bettongs (v1.0)
 * Rufouses forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, fur_cm, tail_cm, rf_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fur_cm,tail_cm,rf_idx,age_yr,active;} rufs_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fur,t_tail,t_rf;} rufs_state_t;
static rufs_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static rufs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rufs_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int tc,int rf,int ay){if(*cnt>=mx)return -1;rufs_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fur_cm=fc;x->tail_cm=tc;x->rf_idx=rf;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[RUFS] Rufous ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" tc=");pi(tc);ps(" rf=");pi(rf);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int rufouses_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fur=0;st.t_tail=0;st.t_rf=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[RUFS] Rufouses initialized\n");return 0;}
int rufouses_forest(int lc,int bl,int bw,int fc,int tc,int rf,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,fc,tc,rf,ay);}
int rufouses_feeding(int lc,int bl,int bw,int fc,int tc,int rf,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,tc,rf,ay);}
int rufouses_breeding(int lc,int bl,int bw,int fc,int tc,int rf,int ay){return add(svb,&st.n_breed,&st.t_fur,N-4,lc,bl,bw,fc,tc,rf,ay);}
int rufouses_health(int lc,int bl,int bw,int fc,int tc,int rf,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,fc,tc,rf,ay);}
int rufouses_market(int lc,int bl,int bw,int fc,int tc,int rf,int ay){return add(svm,&st.n_mkt,&st.t_rf,N-6,lc,bl,bw,fc,tc,rf,ay);}
void rufouses_report(void){ps("[RUFS] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fur=");pi(st.t_fur);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Rf=");pi(st.t_rf);ps("\n");}
void rufouses_state(void){ps("[RUFS] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Rufouses Admin Demo ===\n\n");rufouses_init();
ps("Rufouses forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*2),bw=3+(i*1),fc=2+(i%2),tc=20+(i*2),rf=(i%8)+1,ay=(i%5)+1;rufouses_forest(lc,bl,bw,fc,tc,rf,ay);}
ps("\nRufouses feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*1),bw=4+(i*1),fc=2+(i%2),tc=21+(i%1),rf=(i%6)+1,ay=(i%4)+1;rufouses_feeding(lc,bl,bw,fc,tc,rf,ay);}
ps("\nRufouses breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=45+(i*1),bw=4+(i*1),fc=3+(i%2),tc=23+(i*1),rf=(i%5)+1,ay=(i%3)+1;rufouses_breeding(lc,bl,bw,fc,tc,rf,ay);}
ps("\nRufouses health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*3),bw=2+(i*1),fc=1+(i%2),tc=18+(i*2),rf=(i%10)+1,ay=(i%5)+1;rufouses_health(lc,bl,bw,fc,tc,rf,ay);}
ps("\nRufouses market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=48+(i*1),bw=5+(i*1),fc=3+(i%2),tc=25+(i*1),rf=(i%4)+1,ay=(i%3)+1;rufouses_market(lc,bl,bw,fc,tc,rf,ay);}
ps("\n");rufouses_report();rufouses_state();ps("\n=== Demo Complete ===\n");return 0;}
