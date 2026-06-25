/* rooster_admin: Rooster (Gallus gallus domesticus) domestic rooster fowl (v1.0)
 * Rooster farmyard, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, comb_cm, walk_speed, cm_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,comb_cm,wk_sp,cm_idx,age_yr,active;} roos_t;
typedef struct{int n_farmyard,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_comb,t_wk,t_cm;} roos_state_t;
static roos_t arr_f[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static roos_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(roos_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int ws,int cmi,int ay){if(*cnt>=mx)return -1;roos_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->comb_cm=cc;x->wk_sp=ws;x->cm_idx=cmi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ROOS] Rooster ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" ws=");pi(ws);ps(" cmi=");pi(cmi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int rooster_init(void){if(init)return -1;st.n_farmyard=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_comb=0;st.t_wk=0;st.t_cm=0;for(int i=0;i<N;i++)arr_f[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[ROOS] Rooster initialized\n");return 0;}
int rooster_farmyard(int lc,int bl,int bw,int cc,int ws,int cmi,int ay){return add(arr_f,&st.n_farmyard,&st.t_ln,N,lc,bl,bw,cc,ws,cmi,ay);}
int rooster_feeding(int lc,int bl,int bw,int cc,int ws,int cmi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,ws,cmi,ay);}
int rooster_breeding(int lc,int bl,int bw,int cc,int ws,int cmi,int ay){return add(arr_b,&st.n_breed,&st.t_comb,N-4,lc,bl,bw,cc,ws,cmi,ay);}
int rooster_health(int lc,int bl,int bw,int cc,int ws,int cmi,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,cc,ws,cmi,ay);}
int rooster_market(int lc,int bl,int bw,int cc,int ws,int cmi,int ay){return add(arr_m,&st.n_mkt,&st.t_cm,N-6,lc,bl,bw,cc,ws,cmi,ay);}
void rooster_report(void){ps("[ROOS] Farmyard: ");pi(st.n_farmyard);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Comb=");pi(st.t_comb);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Cm=");pi(st.t_cm);ps("\n");}
void rooster_state(void){ps("[ROOS] Farmyard=");pi(st.n_farmyard);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Rooster Admin Demo ===\n\n");rooster_init();
ps("Rooster farmyard...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=3+(i*1),cc=8+(i*1),ws=3+(i*1),cmi=(i%6)+1,ay=(i%6)+1;rooster_farmyard(lc,bl,bw,cc,ws,cmi,ay);}
ps("\nRooster feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*2),bw=3+(i*1),cc=9+(i*1),ws=4+(i*1),cmi=(i%5)+1,ay=(i%5)+1;rooster_feeding(lc,bl,bw,cc,ws,cmi,ay);}
ps("\nRooster breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=45+(i*2),bw=4+(i*1),cc=10+(i*1),ws=2+(i*2),cmi=(i%4)+1,ay=(i%4)+1;rooster_breeding(lc,bl,bw,cc,ws,cmi,ay);}
ps("\nRooster health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=2+(i*1),cc=7+(i*1),ws=2+(i*3),cmi=(i%7)+1,ay=(i%3)+1;rooster_health(lc,bl,bw,cc,ws,cmi,ay);}
ps("\nRooster market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=48+(i*2),bw=4+(i*1),cc=11+(i*1),ws=5+(i*1),cmi=(i%3)+1,ay=(i%3)+1;rooster_market(lc,bl,bw,cc,ws,cmi,ay);}
ps("\n");rooster_report();rooster_state();ps("\n=== Demo Complete ===\n");return 0;}
