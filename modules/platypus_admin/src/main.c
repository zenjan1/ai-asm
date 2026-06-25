/* platypus_admin: Platypus (Ornithorhynchus anatinus) duck-billed platypus (v1.0)
 * Platypus burrow, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, bill_cm, dive_sec, venom_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,bill_cm,dv_sc,vm_idx,age_yr,active;} pltp_t;
typedef struct{int n_burrow,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_bill,t_dv,t_vm;} pltp_state_t;
static pltp_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static pltp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pltp_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int ds,int vi,int ay){if(*cnt>=mx)return -1;pltp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->bill_cm=bc;x->dv_sc=ds;x->vm_idx=vi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PLTP] Platypus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" ds=");pi(ds);ps(" vi=");pi(vi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int pltp_init(void){if(init)return -1;st.n_burrow=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_bill=0;st.t_dv=0;st.t_vm=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[PLTP] Platypus initialized\n");return 0;}
int pltp_burrow(int lc,int bl,int bw,int bc,int ds,int vi,int ay){return add(apl,&st.n_burrow,&st.t_ln,N,lc,bl,bw,bc,ds,vi,ay);}
int pltp_feeding(int lc,int bl,int bw,int bc,int ds,int vi,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,ds,vi,ay);}
int pltp_breeding(int lc,int bl,int bw,int bc,int ds,int vi,int ay){return add(abd,&st.n_breed,&st.t_bill,N-4,lc,bl,bw,bc,ds,vi,ay);}
int pltp_health(int lc,int bl,int bw,int bc,int ds,int vi,int ay){return add(ahd,&st.n_health,&st.t_dv,N-6,lc,bl,bw,bc,ds,vi,ay);}
int pltp_market(int lc,int bl,int bw,int bc,int ds,int vi,int ay){return add(amd,&st.n_mkt,&st.t_vm,N-6,lc,bl,bw,bc,ds,vi,ay);}
void pltp_report(void){ps("[PLTP] Burrow: ");pi(st.n_burrow);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Bill=");pi(st.t_bill);ps("\nHealth: ");pi(st.n_health);ps(" Dv=");pi(st.t_dv);ps("\nMkt: ");pi(st.n_mkt);ps(" Vm=");pi(st.t_vm);ps("\n");}
void pltp_state(void){ps("[PLTP] Burrow=");pi(st.n_burrow);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Platypus Admin Demo ===\n\n");pltp_init();
ps("Platypus burrow...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=1+(i*1),bc=6+(i*2),ds=30+(i*5),vi=(i%6)+1,ay=2+(i%10);pltp_burrow(lc,bl,bw,bc,ds,vi,ay);}
ps("\nPlatypus feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*3),bw=1+(i*1),bc=7+(i*2),ds=32+(i*5),vi=(i%5)+2,ay=3+(i%8);pltp_feeding(lc,bl,bw,bc,ds,vi,ay);}
ps("\nPlatypus breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=45+(i*3),bw=2+(i*1),bc=7+(i*2),ds=35+(i*5),vi=(i%4)+1,ay=4+(i%6);pltp_breeding(lc,bl,bw,bc,ds,vi,ay);}
ps("\nPlatypus health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=1+(i*1),bc=5+(i*3),ds=28+(i*6),vi=(i%3)+3,ay=5+(i%5);pltp_health(lc,bl,bw,bc,ds,vi,ay);}
ps("\nPlatypus market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=50+(i*3),bw=2+(i*1),bc=8+(i*2),ds=40+(i*5),vi=(i%6)+1,ay=6+(i%4);pltp_market(lc,bl,bw,bc,ds,vi,ay);}
ps("\n");pltp_report();pltp_state();ps("\n=== Demo Complete ===\n");return 0;}
