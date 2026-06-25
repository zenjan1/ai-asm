/* rabbit_admin: Rabbit (Oryctolagus cuniculus) European rabbit small mammal (v1.0)
 * Rabbit burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, hop_speed, litter_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,hp_sp,lt_ct,age_yr,active;} rabt_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_hp,t_lt;} rabt_state_t;
static rabt_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static rabt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rabt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int hs,int lt,int ay){if(*cnt>=mx)return -1;rabt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->hp_sp=hs;x->lt_ct=lt;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[RABT] Rabbit ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" hs=");pi(hs);ps(" lt=");pi(lt);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int rabt_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_hp=0;st.t_lt=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[RABT] Rabbit initialized\n");return 0;}
int rabt_burrow(int lc,int bl,int bw,int ec,int hs,int lt,int ay){return add(apl,&st.n_burrow,&st.t_ln,N,lc,bl,bw,ec,hs,lt,ay);}
int rabt_feeding(int lc,int bl,int bw,int ec,int hs,int lt,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,hs,lt,ay);}
int rabt_breeding(int lc,int bl,int bw,int ec,int hs,int lt,int ay){return add(abd,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,hs,lt,ay);}
int rabt_health(int lc,int bl,int bw,int ec,int hs,int lt,int ay){return add(ahd,&st.n_health,&st.t_hp,N-6,lc,bl,bw,ec,hs,lt,ay);}
int rabt_market(int lc,int bl,int bw,int ec,int hs,int lt,int ay){return add(amd,&st.n_mkt,&st.t_lt,N-6,lc,bl,bw,ec,hs,lt,ay);}
void rabt_report(void){ps("[RABT] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Hp=");pi(st.t_hp);ps("\nMkt: ");pi(st.n_mkt);ps(" Lt=");pi(st.t_lt);ps("\n");}
void rabt_state(void){ps("[RABT] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Rabbit Admin Demo ===\n\n");rabt_init();
ps("Rabbit burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*3),bw=2+(i*1),ec=8+(i*2),hs=20+(i*5),lt=4+(i*2),ay=1+(i%6);rabt_burrow(lc,bl,bw,ec,hs,lt,ay);}
ps("\nRabbit feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*3),bw=2+(i*1),ec=9+(i*2),hs=22+(i*5),lt=5+(i*2),ay=1+(i%5);rabt_feeding(lc,bl,bw,ec,hs,lt,ay);}
ps("\nRabbit breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=40+(i*3),bw=3+(i*1),ec=10+(i*2),hs=25+(i*5),lt=6+(i*2),ay=2+(i%4);rabt_breeding(lc,bl,bw,ec,hs,lt,ay);}
ps("\nRabbit health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=33+(i*4),bw=1+(i*2),ec=7+(i*3),hs=18+(i*6),lt=3+(i*3),ay=2+(i%3);rabt_health(lc,bl,bw,ec,hs,lt,ay);}
ps("\nRabbit market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=45+(i*3),bw=3+(i*1),ec=12+(i*2),hs=30+(i*5),lt=7+(i*2),ay=3+(i%3);rabt_market(lc,bl,bw,ec,hs,lt,ay);}
ps("\n");rabt_report();rabt_state();ps("\n=== Demo Complete ===\n");return 0;}
