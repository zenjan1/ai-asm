/* cod_admin: Cod (Gadus morhua) Atlantic marine fish (v1.0)
 * Cod ocean, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, fin_cm, swim_speed, scale_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,sc_idx,age_yr,active;} cod_t;
typedef struct{int n_ocean,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_sc;} cod_state_t;
static cod_t ocl[N],ocf[N-2],ocb[N-4],och[N-6],ocm[N-6]; static cod_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cod_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int si,int ay){if(*cnt>=mx)return -1;cod_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[COD] Cod ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int cod_init(void){if(init)return -1;st.n_ocean=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_sc=0;for(int i=0;i<N;i++)ocl[i].active=0;for(int i=0;i<N-2;i++)ocf[i].active=0;for(int i=0;i<N-4;i++)ocb[i].active=0;for(int i=0;i<N-6;i++)och[i].active=0;for(int i=0;i<N-6;i++)ocm[i].active=0;init=1;ps("[COD] Cod initialized\n");return 0;}
int cod_ocean(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(ocl,&st.n_ocean,&st.t_ln,N,lc,bl,bw,fc,ss,si,ay);}
int cod_feeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(ocf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,si,ay);}
int cod_breeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(ocb,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,si,ay);}
int cod_health(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(och,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,si,ay);}
int cod_market(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(ocm,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,fc,ss,si,ay);}
void cod_report(void){ps("[COD] Ocean: ");pi(st.n_ocean);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void cod_state(void){ps("[COD] Ocean=");pi(st.n_ocean);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cod Admin Demo ===\n\n");cod_init();
ps("Cod ocean...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=60+(i*5),bw=3+(i*2),fc=10+(i*2),ss=5+(i*2),si=(i%8)+1,ay=(i%10)+1;cod_ocean(lc,bl,bw,fc,ss,si,ay);}
ps("\nCod feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=63+(i*4),bw=4+(i*2),fc=11+(i*2),ss=6+(i*1),si=(i%6)+1,ay=(i%8)+1;cod_feeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nCod breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=66+(i*3),bw=5+(i*1),fc=12+(i*1),ss=4+(i*3),si=(i%5)+1,ay=(i%7)+1;cod_breeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nCod health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=58+(i*6),bw=3+(i*3),fc=9+(i*3),ss=3+(i*4),si=(i%10)+1,ay=(i%6)+1;cod_health(lc,bl,bw,fc,ss,si,ay);}
ps("\nCod market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=68+(i*3),bw=6+(i*1),fc=13+(i*1),ss=7+(i*2),si=(i%4)+1,ay=(i%5)+1;cod_market(lc,bl,bw,fc,ss,si,ay);}
ps("\n");cod_report();cod_state();ps("\n=== Demo Complete ===\n");return 0;}
