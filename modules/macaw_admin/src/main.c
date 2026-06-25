/* macaw_admin: Macaw (Ara macao) scarlet macaw tropical parrot (v1.0)
 * Macaw aviary, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_span_cm, call_vol, plumage_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,cl_vl,pl_idx,age_yr,active;} macaw_t;
typedef struct{int n_aviary,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_cl,t_pl;} macaw_state_t;
static macaw_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static macaw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(macaw_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int cv,int pi2,int ay){if(*cnt>=mx)return -1;macaw_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->cl_vl=cv;x->pl_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MACAW] Macaw ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" cv=");pi(cv);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int macaw_init(void){if(init)return -1;st.n_aviary=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_cl=0;st.t_pl=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MACAW] Macaw initialized\n");return 0;}
int macaw_aviary(int lc,int bl,int bw,int ws,int cv,int pi2,int ay){return add(apl,&st.n_aviary,&st.t_ln,N,lc,bl,bw,ws,cv,pi2,ay);}
int macaw_feeding(int lc,int bl,int bw,int ws,int cv,int pi2,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,cv,pi2,ay);}
int macaw_breeding(int lc,int bl,int bw,int ws,int cv,int pi2,int ay){return add(abd,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,cv,pi2,ay);}
int macaw_health(int lc,int bl,int bw,int ws,int cv,int pi2,int ay){return add(ahd,&st.n_health,&st.t_cl,N-6,lc,bl,bw,ws,cv,pi2,ay);}
int macaw_market(int lc,int bl,int bw,int ws,int cv,int pi2,int ay){return add(amd,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,ws,cv,pi2,ay);}
void macaw_report(void){ps("[MACAW] Aviary: ");pi(st.n_aviary);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void macaw_state(void){ps("[MACAW] Aviary=");pi(st.n_aviary);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Macaw Admin Demo ===\n\n");macaw_init();
ps("Macaw aviary...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=900+(i*50),ws=80+(i*5),cv=60+(i*4),pi2=(i%6)+1,ay=1+(i%15);macaw_aviary(lc,bl,bw,ws,cv,pi2,ay);}
ps("\nMacaw feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*3),bw=950+(i*50),ws=82+(i*5),cv=62+(i*4),pi2=(i%5)+2,ay=2+(i%12);macaw_feeding(lc,bl,bw,ws,cv,pi2,ay);}
ps("\nMacaw breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=45+(i*3),bw=1000+(i*50),ws=85+(i*5),cv=65+(i*4),pi2=(i%4)+1,ay=3+(i%10);macaw_breeding(lc,bl,bw,ws,cv,pi2,ay);}
ps("\nMacaw health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=880+(i*60),ws=78+(i*6),cv=58+(i*5),pi2=(i%3)+3,ay=4+(i%8);macaw_health(lc,bl,bw,ws,cv,pi2,ay);}
ps("\nMacaw market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=50+(i*3),bw=1100+(i*50),ws=90+(i*5),cv=70+(i*4),pi2=(i%6)+1,ay=5+(i%7);macaw_market(lc,bl,bw,ws,cv,pi2,ay);}
ps("\n");macaw_report();macaw_state();ps("\n=== Demo Complete ===\n");return 0;}
