/* chicken_admin: Chicken (Gallus gallus domesticus) domestic poultry (v1.0)
 * Chicken coop, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, beak_cm, walk_speed, feather_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,wk_sp,ft_idx,age_yr,active;} chk_t;
typedef struct{int n_coop,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_wk,t_ft;} chk_state_t;
static chk_t cpl[N],cpf[N-2],cpb[N-4],cph[N-6],cpm[N-6]; static chk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int ws,int fi,int ay){if(*cnt>=mx)return -1;chk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->wk_sp=ws;x->ft_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CHK] Chicken ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" ws=");pi(ws);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int chicken_init(void){if(init)return -1;st.n_coop=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_wk=0;st.t_ft=0;for(int i=0;i<N;i++)cpl[i].active=0;for(int i=0;i<N-2;i++)cpf[i].active=0;for(int i=0;i<N-4;i++)cpb[i].active=0;for(int i=0;i<N-6;i++)cph[i].active=0;for(int i=0;i<N-6;i++)cpm[i].active=0;init=1;ps("[CHK] Chicken initialized\n");return 0;}
int chicken_coop(int lc,int bl,int bw,int bc,int ws,int fi,int ay){return add(cpl,&st.n_coop,&st.t_ln,N,lc,bl,bw,bc,ws,fi,ay);}
int chicken_feeding(int lc,int bl,int bw,int bc,int ws,int fi,int ay){return add(cpf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,ws,fi,ay);}
int chicken_breeding(int lc,int bl,int bw,int bc,int ws,int fi,int ay){return add(cpb,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,ws,fi,ay);}
int chicken_health(int lc,int bl,int bw,int bc,int ws,int fi,int ay){return add(cph,&st.n_health,&st.t_wk,N-6,lc,bl,bw,bc,ws,fi,ay);}
int chicken_market(int lc,int bl,int bw,int bc,int ws,int fi,int ay){return add(cpm,&st.n_mkt,&st.t_ft,N-6,lc,bl,bw,bc,ws,fi,ay);}
void chicken_report(void){ps("[CHK] Coop: ");pi(st.n_coop);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Ft=");pi(st.t_ft);ps("\n");}
void chicken_state(void){ps("[CHK] Coop=");pi(st.n_coop);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chicken Admin Demo ===\n\n");chicken_init();
ps("Chicken coop...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*3),bw=1+(i*1),bc=2+(i%2),ws=2+(i%2),fi=(i%6)+1,ay=(i%5)+1;chicken_coop(lc,bl,bw,bc,ws,fi,ay);}
ps("\nChicken feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=27+(i*2),bw=2+(i*1),bc=2+(i%3),ws=3+(i*1),fi=(i%5)+1,ay=(i%4)+1;chicken_feeding(lc,bl,bw,bc,ws,fi,ay);}
ps("\nChicken breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=29+(i*2),bw=2+(i*1),bc=3+(i%2),ws=2+(i*2),fi=(i%4)+1,ay=(i%3)+1;chicken_breeding(lc,bl,bw,bc,ws,fi,ay);}
ps("\nChicken health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=24+(i*4),bw=1+(i*2),bc=2+(i%3),ws=1+(i*3),fi=(i%8)+1,ay=(i%3)+1;chicken_health(lc,bl,bw,bc,ws,fi,ay);}
ps("\nChicken market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=30+(i*2),bw=3+(i*1),bc=3+(i%2),ws=3+(i*2),fi=(i%3)+1,ay=(i%2)+1;chicken_market(lc,bl,bw,bc,ws,fi,ay);}
ps("\n");chicken_report();chicken_state();ps("\n=== Demo Complete ===\n");return 0;}
