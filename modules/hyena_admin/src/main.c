/* hyena_admin: Hyena (Crocuta crocuta) spotted scavenger canid (v1.0)
 * Hyena savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, jaw_cm, run_speed, spot_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,jaw_cm,rn_sp,sp_idx,age_yr,active;} hyen_t;
typedef struct{int n_savan,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_jaw,t_rn,t_sp;} hyen_state_t;
static hyen_t svn[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static hyen_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hyen_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int jc,int rs,int si,int ay){if(*cnt>=mx)return -1;hyen_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->jaw_cm=jc;x->rn_sp=rs;x->sp_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HYEN] Hyena ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" jc=");pi(jc);ps(" rs=");pi(rs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hyena_init(void){if(init)return -1;st.n_savan=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_jaw=0;st.t_rn=0;st.t_sp=0;for(int i=0;i<N;i++)svn[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[HYEN] Hyena initialized\n");return 0;}
int hyena_savanna(int lc,int bl,int bw,int jc,int rs,int si,int ay){return add(svn,&st.n_savan,&st.t_ln,N,lc,bl,bw,jc,rs,si,ay);}
int hyena_feeding(int lc,int bl,int bw,int jc,int rs,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,jc,rs,si,ay);}
int hyena_breeding(int lc,int bl,int bw,int jc,int rs,int si,int ay){return add(svb,&st.n_breed,&st.t_jaw,N-4,lc,bl,bw,jc,rs,si,ay);}
int hyena_health(int lc,int bl,int bw,int jc,int rs,int si,int ay){return add(svh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,jc,rs,si,ay);}
int hyena_market(int lc,int bl,int bw,int jc,int rs,int si,int ay){return add(svm,&st.n_mkt,&st.t_sp,N-6,lc,bl,bw,jc,rs,si,ay);}
void hyena_report(void){ps("[HYEN] Savan: ");pi(st.n_savan);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Jaw=");pi(st.t_jaw);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Sp=");pi(st.t_sp);ps("\n");}
void hyena_state(void){ps("[HYEN] Savan=");pi(st.n_savan);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hyena Admin Demo ===\n\n");hyena_init();
ps("Hyena savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=110+(i*6),bw=55+(i*5),jc=15+(i*2),rs=12+(i*3),si=(i%10)+1,ay=(i%15)+1;hyena_savanna(lc,bl,bw,jc,rs,si,ay);}
ps("\nHyena feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=113+(i*5),bw=57+(i*4),jc=16+(i*2),rs=13+(i*2),si=(i%9)+1,ay=(i%12)+1;hyena_feeding(lc,bl,bw,jc,rs,si,ay);}
ps("\nHyena breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=116+(i*4),bw=59+(i*3),jc=17+(i*1),rs=11+(i*4),si=(i%8)+1,ay=(i%10)+1;hyena_breeding(lc,bl,bw,jc,rs,si,ay);}
ps("\nHyena health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=108+(i*7),bw=53+(i*6),jc=14+(i*3),rs=10+(i*5),si=(i%11)+1,ay=(i%8)+1;hyena_health(lc,bl,bw,jc,rs,si,ay);}
ps("\nHyena market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=118+(i*4),bw=61+(i*3),jc=18+(i*1),rs=14+(i*2),si=(i%6)+1,ay=(i%7)+1;hyena_market(lc,bl,bw,jc,rs,si,ay);}
ps("\n");hyena_report();hyena_state();ps("\n=== Demo Complete ===\n");return 0;}
