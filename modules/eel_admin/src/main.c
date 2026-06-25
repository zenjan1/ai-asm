/* eel_admin: Eel (Anguilla anguilla) European elongated fish (v1.0)
 * Eel river, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fin_cm, swim_speed, scale_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,sc_idx,age_yr,active;} eel_t;
typedef struct{int n_river,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_sc;} eel_state_t;
static eel_t rvl[N],rvf[N-2],rvb[N-4],rvh[N-6],rvm[N-6]; static eel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eel_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int si,int ay){if(*cnt>=mx)return -1;eel_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[EEL] Eel ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int eel_init(void){if(init)return -1;st.n_river=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_sc=0;for(int i=0;i<N;i++)rvl[i].active=0;for(int i=0;i<N-2;i++)rvf[i].active=0;for(int i=0;i<N-4;i++)rvb[i].active=0;for(int i=0;i<N-6;i++)rvh[i].active=0;for(int i=0;i<N-6;i++)rvm[i].active=0;init=1;ps("[EEL] Eel initialized\n");return 0;}
int eel_river(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(rvl,&st.n_river,&st.t_ln,N,lc,bl,bw,fc,ss,si,ay);}
int eel_feeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(rvf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,si,ay);}
int eel_breeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(rvb,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,si,ay);}
int eel_health(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(rvh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,si,ay);}
int eel_market(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(rvm,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,fc,ss,si,ay);}
void eel_report(void){ps("[EEL] River: ");pi(st.n_river);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void eel_state(void){ps("[EEL] River=");pi(st.n_river);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Eel Admin Demo ===\n\n");eel_init();
ps("Eel river...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*5),bw=30+(i*10),fc=5+(i*2),ss=3+(i*2),si=(i%8)+1,ay=(i%10)+1;eel_river(lc,bl,bw,fc,ss,si,ay);}
ps("\nEel feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=53+(i*4),bw=35+(i*8),fc=6+(i*1),ss=4+(i*1),si=(i%6)+1,ay=(i%8)+1;eel_feeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nEel breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=56+(i*3),bw=40+(i*6),fc=7+(i%2),ss=2+(i*3),si=(i%5)+1,ay=(i%7)+1;eel_breeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nEel health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=48+(i*6),bw=28+(i*12),fc=4+(i*3),ss=2+(i*4),si=(i%10)+1,ay=(i%6)+1;eel_health(lc,bl,bw,fc,ss,si,ay);}
ps("\nEel market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=58+(i*3),bw=42+(i*5),fc=8+(i%2),ss=5+(i*2),si=(i%4)+1,ay=(i%5)+1;eel_market(lc,bl,bw,fc,ss,si,ay);}
ps("\n");eel_report();eel_state();ps("\n=== Demo Complete ===\n");return 0;}
