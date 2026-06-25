/* sitatunga_admin: Sitatunga (Tragelaphus spekii) marsh-dwelling antelope (v1.0)
 * Sitatunga marsh, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, swim_speed, st_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hn_cm,sw_sp,st_idx,age_yr,active;} sitt_t;
typedef struct{int n_marsh,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hn,t_sw,t_st;} sitt_state_t;
static sitt_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static sitt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sitt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ss,int si,int ay){if(*cnt>=mx)return -1;sitt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hn_cm=hc;x->sw_sp=ss;x->st_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SITT] Sitatunga ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int sitatunga_init(void){if(init)return -1;st.n_marsh=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hn=0;st.t_sw=0;st.t_st=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[SITT] Sitatunga initialized\n");return 0;}
int sitatunga_marsh(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(svl,&st.n_marsh,&st.t_ln,N,lc,bl,bw,hc,ss,si,ay);}
int sitatunga_feeding(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ss,si,ay);}
int sitatunga_breeding(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(svb,&st.n_breed,&st.t_hn,N-4,lc,bl,bw,hc,ss,si,ay);}
int sitatunga_health(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(svh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,hc,ss,si,ay);}
int sitatunga_market(int lc,int bl,int bw,int hc,int ss,int si,int ay){return add(svm,&st.n_mkt,&st.t_st,N-6,lc,bl,bw,hc,ss,si,ay);}
void sitatunga_report(void){ps("[SITT] Marsh: ");pi(st.n_marsh);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hn=");pi(st.t_hn);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" St=");pi(st.t_st);ps("\n");}
void sitatunga_state(void){ps("[SITT] Marsh=");pi(st.n_marsh);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Sitatunga Admin Demo ===\n\n");sitatunga_init();
ps("Sitatunga marsh...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=130+(i*6),bw=60+(i*6),hc=50+(i*3),ss=8+(i*1),si=(i%8)+1,ay=(i%12)+1;sitatunga_marsh(lc,bl,bw,hc,ss,si,ay);}
ps("\nSitatunga feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=133+(i*5),bw=63+(i*5),hc=52+(i*2),ss=9+(i*1),si=(i%6)+1,ay=(i%10)+1;sitatunga_feeding(lc,bl,bw,hc,ss,si,ay);}
ps("\nSitatunga breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=136+(i*4),bw=66+(i*4),hc=54+(i*3),ss=7+(i*2),si=(i%5)+1,ay=(i%8)+1;sitatunga_breeding(lc,bl,bw,hc,ss,si,ay);}
ps("\nSitatunga health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=128+(i*8),bw=58+(i*8),hc=48+(i*4),ss=6+(i*2),si=(i%10)+1,ay=(i%7)+1;sitatunga_health(lc,bl,bw,hc,ss,si,ay);}
ps("\nSitatunga market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=140+(i*3),bw=68+(i*3),hc=56+(i*2),ss=10+(i*1),si=(i%4)+1,ay=(i%6)+1;sitatunga_market(lc,bl,bw,hc,ss,si,ay);}
ps("\n");sitatunga_report();sitatunga_state();ps("\n=== Demo Complete ===\n");return 0;}
