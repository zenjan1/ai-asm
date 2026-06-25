/* raven_admin: Raven (Corvus corax) common raven large corvid (v1.0)
 * Raven cliff, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_span_cm, call_vol, cache_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,cl_vl,ch_idx,age_yr,active;} rav_t;
typedef struct{int n_cliff,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_cl,t_ch;} rav_state_t;
static rav_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static rav_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rav_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int cv,int ci,int ay){if(*cnt>=mx)return -1;rav_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->cl_vl=cv;x->ch_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[RAVN] Raven ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" cv=");pi(cv);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int rav_init(void){if(init)return -1;st.n_cliff=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_cl=0;st.t_ch=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[RAVN] Raven initialized\n");return 0;}
int rav_cliff(int lc,int bl,int bw,int ws,int cv,int ci,int ay){return add(apl,&st.n_cliff,&st.t_ln,N,lc,bl,bw,ws,cv,ci,ay);}
int rav_feeding(int lc,int bl,int bw,int ws,int cv,int ci,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,cv,ci,ay);}
int rav_breeding(int lc,int bl,int bw,int ws,int cv,int ci,int ay){return add(abd,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,cv,ci,ay);}
int rav_health(int lc,int bl,int bw,int ws,int cv,int ci,int ay){return add(ahd,&st.n_health,&st.t_cl,N-6,lc,bl,bw,ws,cv,ci,ay);}
int rav_market(int lc,int bl,int bw,int ws,int cv,int ci,int ay){return add(amd,&st.n_mkt,&st.t_ch,N-6,lc,bl,bw,ws,cv,ci,ay);}
void rav_report(void){ps("[RAVN] Cliff: ");pi(st.n_cliff);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Ch=");pi(st.t_ch);ps("\n");}
void rav_state(void){ps("[RAVN] Cliff=");pi(st.n_cliff);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Raven Admin Demo ===\n\n");rav_init();
ps("Raven cliff...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=55+(i*4),bw=1+(i*1),ws=120+(i*8),cv=40+(i*5),ci=(i%6)+1,ay=2+(i%15);rav_cliff(lc,bl,bw,ws,cv,ci,ay);}
ps("\nRaven feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=57+(i*4),bw=1+(i*1),ws=125+(i*8),cv=42+(i*5),ci=(i%5)+2,ay=3+(i%12);rav_feeding(lc,bl,bw,ws,cv,ci,ay);}
ps("\nRaven breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=60+(i*4),bw=2+(i*1),ws=130+(i*8),cv=45+(i*5),ci=(i%4)+1,ay=4+(i%10);rav_breeding(lc,bl,bw,ws,cv,ci,ay);}
ps("\nRaven health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=53+(i*5),bw=1+(i*1),ws=115+(i*10),cv=38+(i*6),ci=(i%3)+3,ay=5+(i%8);rav_health(lc,bl,bw,ws,cv,ci,ay);}
ps("\nRaven market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=65+(i*4),bw=2+(i*1),ws=140+(i*8),cv=50+(i*5),ci=(i%6)+1,ay=6+(i%6);rav_market(lc,bl,bw,ws,cv,ci,ay);}
ps("\n");rav_report();rav_state();ps("\n=== Demo Complete ===\n");return 0;}
