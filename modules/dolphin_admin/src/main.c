/* dolphin_admin: Dolphin (Delphinus delphis) common marine mammal (v1.0)
 * Dolphin ocean, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, fin_cm, swim_speed, skin_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,sk_idx,age_yr,active;} dlph_t;
typedef struct{int n_ocean,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_sk;} dlph_state_t;
static dlph_t ocl[N],ocf[N-2],ocb[N-4],och[N-6],ocm[N-6]; static dlph_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dlph_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int si,int ay){if(*cnt>=mx)return -1;dlph_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->sk_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DLPH] Dolphin ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dolphin_init(void){if(init)return -1;st.n_ocean=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_sk=0;for(int i=0;i<N;i++)ocl[i].active=0;for(int i=0;i<N-2;i++)ocf[i].active=0;for(int i=0;i<N-4;i++)ocb[i].active=0;for(int i=0;i<N-6;i++)och[i].active=0;for(int i=0;i<N-6;i++)ocm[i].active=0;init=1;ps("[DLPH] Dolphin initialized\n");return 0;}
int dolphin_ocean(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(ocl,&st.n_ocean,&st.t_ln,N,lc,bl,bw,fc,ss,si,ay);}
int dolphin_feeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(ocf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,si,ay);}
int dolphin_breeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(ocb,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,si,ay);}
int dolphin_health(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(och,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,si,ay);}
int dolphin_market(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(ocm,&st.n_mkt,&st.t_sk,N-6,lc,bl,bw,fc,ss,si,ay);}
void dolphin_report(void){ps("[DLPH] Ocean: ");pi(st.n_ocean);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sk=");pi(st.t_sk);ps("\n");}
void dolphin_state(void){ps("[DLPH] Ocean=");pi(st.n_ocean);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dolphin Admin Demo ===\n\n");dolphin_init();
ps("Dolphin ocean...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*10),bw=60+(i*15),fc=20+(i*3),ss=25+(i*4),si=(i%8)+1,ay=(i%20)+1;dolphin_ocean(lc,bl,bw,fc,ss,si,ay);}
ps("\nDolphin feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=155+(i*8),bw=65+(i*12),fc=22+(i*2),ss=27+(i*3),si=(i%6)+1,ay=(i%18)+1;dolphin_feeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nDolphin breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=160+(i*6),bw=70+(i*10),fc=24+(i*2),ss=23+(i*5),si=(i%5)+1,ay=(i%15)+1;dolphin_breeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nDolphin health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=145+(i*12),bw=55+(i*20),fc=18+(i*4),ss=20+(i*6),si=(i%10)+1,ay=(i%12)+1;dolphin_health(lc,bl,bw,fc,ss,si,ay);}
ps("\nDolphin market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=165+(i*5),bw=75+(i*8),fc=26+(i%3),ss=30+(i*3),si=(i%4)+1,ay=(i%10)+1;dolphin_market(lc,bl,bw,fc,ss,si,ay);}
ps("\n");dolphin_report();dolphin_state();ps("\n=== Demo Complete ===\n");return 0;}
