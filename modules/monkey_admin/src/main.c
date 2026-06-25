/* monkey_admin: Monkey (Macaca mulatta) rhesus macaque monkey primate (v1.0)
 * Monkey troop, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_len_cm, call_vol, troop_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,cl_vl,tr_idx,age_yr,active;} mnky_t;
typedef struct{int n_troop,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_cl,t_tr;} mnky_state_t;
static mnky_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static mnky_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mnky_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int cv,int ti,int ay){if(*cnt>=mx)return -1;mnky_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->cl_vl=cv;x->tr_idx=ti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MNKY] Monkey ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" cv=");pi(cv);ps(" ti=");pi(ti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mnky_init(void){if(init)return -1;st.n_troop=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_cl=0;st.t_tr=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MNKY] Monkey initialized\n");return 0;}
int mnky_troop(int lc,int bl,int bw,int tl,int cv,int ti,int ay){return add(apl,&st.n_troop,&st.t_ln,N,lc,bl,bw,tl,cv,ti,ay);}
int mnky_feeding(int lc,int bl,int bw,int tl,int cv,int ti,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,cv,ti,ay);}
int mnky_breeding(int lc,int bl,int bw,int tl,int cv,int ti,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,cv,ti,ay);}
int mnky_health(int lc,int bl,int bw,int tl,int cv,int ti,int ay){return add(ahd,&st.n_health,&st.t_cl,N-6,lc,bl,bw,tl,cv,ti,ay);}
int mnky_market(int lc,int bl,int bw,int tl,int cv,int ti,int ay){return add(amd,&st.n_mkt,&st.t_tr,N-6,lc,bl,bw,tl,cv,ti,ay);}
void mnky_report(void){ps("[MNKY] Troop: ");pi(st.n_troop);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Tr=");pi(st.t_tr);ps("\n");}
void mnky_state(void){ps("[MNKY] Troop=");pi(st.n_troop);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Monkey Admin Demo ===\n\n");mnky_init();
ps("Monkey troop...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=45+(i*3),bw=5000+(i*300),tl=18+(i*2),cv=50+(i*5),ti=(i%8)+1,ay=1+(i%15);mnky_troop(lc,bl,bw,tl,cv,ti,ay);}
ps("\nMonkey feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=47+(i*3),bw=5200+(i*300),tl=19+(i*2),cv=52+(i*5),ti=(i%7)+2,ay=2+(i%12);mnky_feeding(lc,bl,bw,tl,cv,ti,ay);}
ps("\nMonkey breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=50+(i*3),bw=5500+(i*300),tl=20+(i*2),cv=55+(i*5),ti=(i%6)+1,ay=3+(i%10);mnky_breeding(lc,bl,bw,tl,cv,ti,ay);}
ps("\nMonkey health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=43+(i*4),bw=4800+(i*400),tl=17+(i*3),cv=48+(i*6),ti=(i%5)+3,ay=4+(i%8);mnky_health(lc,bl,bw,tl,cv,ti,ay);}
ps("\nMonkey market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=55+(i*3),bw=6000+(i*300),tl=22+(i*2),cv=60+(i*5),ti=(i%4)+4,ay=5+(i%6);mnky_market(lc,bl,bw,tl,cv,ti,ay);}
ps("\n");mnky_report();mnky_state();ps("\n=== Demo Complete ===\n");return 0;}
