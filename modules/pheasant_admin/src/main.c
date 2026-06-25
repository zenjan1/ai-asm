/* pheasant_admin: Pheasant (Phasianus colchicus) ring-necked pheasant game bird (v1.0)
 * Pheasant field, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_len_cm, call_vol, plumage_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,cl_vl,pl_idx,age_yr,active;} phst_t;
typedef struct{int n_field,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_cl,t_pl;} phst_state_t;
static phst_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static phst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(phst_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int cv,int pi2,int ay){if(*cnt>=mx)return -1;phst_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->cl_vl=cv;x->pl_idx=pi2;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PHST] Pheasant ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" cv=");pi(cv);ps(" pi=");pi(pi2);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int phst_init(void){if(init)return -1;st.n_field=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_cl=0;st.t_pl=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[PHST] Pheasant initialized\n");return 0;}
int phst_field(int lc,int bl,int bw,int tl,int cv,int pi2,int ay){return add(apl,&st.n_field,&st.t_ln,N,lc,bl,bw,tl,cv,pi2,ay);}
int phst_feeding(int lc,int bl,int bw,int tl,int cv,int pi2,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,cv,pi2,ay);}
int phst_breeding(int lc,int bl,int bw,int tl,int cv,int pi2,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,cv,pi2,ay);}
int phst_health(int lc,int bl,int bw,int tl,int cv,int pi2,int ay){return add(ahd,&st.n_health,&st.t_cl,N-6,lc,bl,bw,tl,cv,pi2,ay);}
int phst_market(int lc,int bl,int bw,int tl,int cv,int pi2,int ay){return add(amd,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,tl,cv,pi2,ay);}
void phst_report(void){ps("[PHST] Field: ");pi(st.n_field);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void phst_state(void){ps("[PHST] Field=");pi(st.n_field);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Pheasant Admin Demo ===\n\n");phst_init();
ps("Pheasant field...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*4),bw=1+(i*1),tl=25+(i*3),cv=30+(i*5),pi2=(i%6)+1,ay=1+(i%6);phst_field(lc,bl,bw,tl,cv,pi2,ay);}
ps("\nPheasant feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=52+(i*4),bw=1+(i*1),tl=26+(i*3),cv=32+(i*5),pi2=(i%5)+2,ay=1+(i%5);phst_feeding(lc,bl,bw,tl,cv,pi2,ay);}
ps("\nPheasant breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=55+(i*4),bw=2+(i*1),tl=28+(i*3),cv=35+(i*5),pi2=(i%4)+1,ay=2+(i%4);phst_breeding(lc,bl,bw,tl,cv,pi2,ay);}
ps("\nPheasant health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=48+(i*5),bw=1+(i*1),tl=23+(i*4),cv=28+(i*6),pi2=(i%3)+3,ay=2+(i%3);phst_health(lc,bl,bw,tl,cv,pi2,ay);}
ps("\nPheasant market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=60+(i*4),bw=2+(i*1),tl=30+(i*3),cv=40+(i*5),pi2=(i%6)+1,ay=3+(i%3);phst_market(lc,bl,bw,tl,cv,pi2,ay);}
ps("\n");phst_report();phst_state();ps("\n=== Demo Complete ===\n");return 0;}
