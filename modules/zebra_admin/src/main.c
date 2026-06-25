/* zebra_admin: Zebra (Equus quagga) striped equine (v1.0)
 * Zebra savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, hoof_cm, run_speed, stripe_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hoof_cm,run_sp,st_idx,age_yr,active;} zbra_t;
typedef struct{int n_savn,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hf,t_run,t_st;} zbra_state_t;
static zbra_t zpl[N],zfd[N-2],zbr[N-4],zhl[N-6],zmk[N-6]; static zbra_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zbra_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int si,int ay){if(*cnt>=mx)return -1;zbra_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hoof_cm=hc;x->run_sp=rs;x->st_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ZBRA] Zebra ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int zbra_init(void){if(init)return -1;st.n_savn=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hf=0;st.t_run=0;st.t_st=0;for(int i=0;i<N;i++)zpl[i].active=0;for(int i=0;i<N-2;i++)zfd[i].active=0;for(int i=0;i<N-4;i++)zbr[i].active=0;for(int i=0;i<N-6;i++)zhl[i].active=0;for(int i=0;i<N-6;i++)zmk[i].active=0;init=1;ps("[ZBRA] Zebra initialized\n");return 0;}
int zbra_savanna(int lc,int bl,int bw,int hc,int rs,int si,int ay){return add(zpl,&st.n_savn,&st.t_ln,N,lc,bl,bw,hc,rs,si,ay);}
int zbra_feeding(int lc,int bl,int bw,int hc,int rs,int si,int ay){return add(zfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,si,ay);}
int zbra_breeding(int lc,int bl,int bw,int hc,int rs,int si,int ay){return add(zbr,&st.n_breed,&st.t_hf,N-4,lc,bl,bw,hc,rs,si,ay);}
int zbra_health(int lc,int bl,int bw,int hc,int rs,int si,int ay){return add(zhl,&st.n_health,&st.t_run,N-6,lc,bl,bw,hc,rs,si,ay);}
int zbra_market(int lc,int bl,int bw,int hc,int rs,int si,int ay){return add(zmk,&st.n_mkt,&st.t_st,N-6,lc,bl,bw,hc,rs,si,ay);}
void zbra_report(void){ps("[ZBRA] Savanna: ");pi(st.n_savn);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hf=");pi(st.t_hf);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" St=");pi(st.t_st);ps("\n");}
void zbra_state(void){ps("[ZBRA] Savn=");pi(st.n_savn);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Zebra Admin Demo ===\n\n");zbra_init();
ps("Zebra savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*12),bw=300+(i*30),hc=15+(i*2),rs=40+(i*5),st=(i%6)+1,ay=(i%5)+1;zbra_savanna(lc,bl,bw,hc,rs,st,ay);}
ps("\nZebra feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=220+(i*10),bw=320+(i*25),hc=16+(i%3),rs=45+(i*4),st=(i%5)+1,ay=(i%4)+1;zbra_feeding(lc,bl,bw,hc,rs,st,ay);}
ps("\nZebra breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=180+(i*15),bw=280+(i*35),hc=14+(i*3),rs=35+(i*6),st=(i%4)+1,ay=(i%3)+1;zbra_breeding(lc,bl,bw,hc,rs,st,ay);}
ps("\nZebra health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=240+(i*8),bw=350+(i*20),hc=17+(i%2),rs=50+(i*3),st=(i%7)+1,ay=(i%6)+2;zbra_health(lc,bl,bw,hc,rs,st,ay);}
ps("\nZebra market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=260+(i*6),bw=380+(i*15),hc=18+(i%2),rs=30+(i*4),st=(i%3)+3,ay=(i%4)+3;zbra_market(lc,bl,bw,hc,rs,st,ay);}
ps("\n");zbra_report();zbra_state();ps("\n=== Demo Complete ===\n");return 0;}
