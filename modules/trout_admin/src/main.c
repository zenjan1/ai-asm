/* trout_admin: Trout (Salmo trutta) brown trout freshwater fish (v1.0)
 * Trout stream, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fin_cm, swim_speed, sc_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,sc_idx,age_yr,active;} trut_t;
typedef struct{int n_stream,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_sc;} trut_state_t;
static trut_t arr_s[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static trut_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(trut_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int sci,int ay){if(*cnt>=mx)return -1;trut_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->sc_idx=sci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[TRUT] Trout ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" sci=");pi(sci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int trout_init(void){if(init)return -1;st.n_stream=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_sc=0;for(int i=0;i<N;i++)arr_s[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[TRUT] Trout initialized\n");return 0;}
int trout_stream(int lc,int bl,int bw,int fc,int ss,int sci,int ay){return add(arr_s,&st.n_stream,&st.t_ln,N,lc,bl,bw,fc,ss,sci,ay);}
int trout_feeding(int lc,int bl,int bw,int fc,int ss,int sci,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,sci,ay);}
int trout_breeding(int lc,int bl,int bw,int fc,int ss,int sci,int ay){return add(arr_b,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,sci,ay);}
int trout_health(int lc,int bl,int bw,int fc,int ss,int sci,int ay){return add(arr_h,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,sci,ay);}
int trout_market(int lc,int bl,int bw,int fc,int ss,int sci,int ay){return add(arr_m,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,fc,ss,sci,ay);}
void trout_report(void){ps("[TRUT] Stream: ");pi(st.n_stream);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void trout_state(void){ps("[TRUT] Stream=");pi(st.n_stream);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Trout Admin Demo ===\n\n");trout_init();
ps("Trout stream...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*3),bw=300+(i*30),fc=6+(i*1),ss=15+(i*2),sci=(i%6)+1,ay=(i%8)+1;trout_stream(lc,bl,bw,fc,ss,sci,ay);}
ps("\nTrout feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=33+(i*2),bw=320+(i*25),fc=7+(i*1),ss=17+(i*2),sci=(i%5)+1,ay=(i%7)+1;trout_feeding(lc,bl,bw,fc,ss,sci,ay);}
ps("\nTrout breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=36+(i*2),bw=340+(i*20),fc=7+(i*1),ss=13+(i*3),sci=(i%4)+1,ay=(i%6)+1;trout_breeding(lc,bl,bw,fc,ss,sci,ay);}
ps("\nTrout health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=27+(i*4),bw=280+(i*35),fc=5+(i*1),ss=10+(i*4),sci=(i%7)+1,ay=(i%5)+1;trout_health(lc,bl,bw,fc,ss,sci,ay);}
ps("\nTrout market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=39+(i*2),bw=360+(i*15),fc=8+(i*1),ss=20+(i*2),sci=(i%3)+1,ay=(i%4)+1;trout_market(lc,bl,bw,fc,ss,sci,ay);}
ps("\n");trout_report();trout_state();ps("\n=== Demo Complete ===\n");return 0;}
