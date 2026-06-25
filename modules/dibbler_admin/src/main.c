/* dibbler_admin: Dibbler (Parantechinus) small marsupials (v1.0)
 * Dibbler forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, pouch_ct, tail_cm, db_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,pouch_ct,tail_cm,db_idx,age_yr,active;} dibb_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_pouch,t_tail,t_db;} dibb_state_t;
static dibb_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static dibb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dibb_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int pc,int tc,int db,int ay){if(*cnt>=mx)return -1;dibb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->pouch_ct=pc;x->tail_cm=tc;x->db_idx=db;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DIBB] Dibbler ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" pc=");pi(pc);ps(" tc=");pi(tc);ps(" db=");pi(db);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dibbler_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_pouch=0;st.t_tail=0;st.t_db=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[DIBB] Dibbler initialized\n");return 0;}
int dibbler_forest(int lc,int bl,int bw,int pc,int tc,int db,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,pc,tc,db,ay);}
int dibbler_feeding(int lc,int bl,int bw,int pc,int tc,int db,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,pc,tc,db,ay);}
int dibbler_breeding(int lc,int bl,int bw,int pc,int tc,int db,int ay){return add(svb,&st.n_breed,&st.t_pouch,N-4,lc,bl,bw,pc,tc,db,ay);}
int dibbler_health(int lc,int bl,int bw,int pc,int tc,int db,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,pc,tc,db,ay);}
int dibbler_market(int lc,int bl,int bw,int pc,int tc,int db,int ay){return add(svm,&st.n_mkt,&st.t_db,N-6,lc,bl,bw,pc,tc,db,ay);}
void dibbler_report(void){ps("[DIBB] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Pouch=");pi(st.t_pouch);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Db=");pi(st.t_db);ps("\n");}
void dibbler_state(void){ps("[DIBB] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dibbler Admin Demo ===\n\n");dibbler_init();
ps("Dibbler forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=20+(i*1),bw=70+(i*5),pc=6+(i%4),tc=16+(i*1),db=(i%8)+1,ay=(i%5)+1;dibbler_forest(lc,bl,bw,pc,tc,db,ay);}
ps("\nDibbler feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=21+(i*1),bw=75+(i*4),pc=6+(i%4),tc=17+(i*1),db=(i%6)+1,ay=(i%4)+1;dibbler_feeding(lc,bl,bw,pc,tc,db,ay);}
ps("\nDibbler breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=22+(i*1),bw=80+(i*3),pc=7+(i%4),tc=17+(i*1),db=(i%5)+1,ay=(i%3)+1;dibbler_breeding(lc,bl,bw,pc,tc,db,ay);}
ps("\nDibbler health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=19+(i*2),bw=65+(i*6),pc=5+(i%4),tc=15+(i*1),db=(i%10)+1,ay=(i%5)+1;dibbler_health(lc,bl,bw,pc,tc,db,ay);}
ps("\nDibbler market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=23+(i*1),bw=85+(i*3),pc=7+(i%4),tc=18+(i*1),db=(i%4)+1,ay=(i%3)+1;dibbler_market(lc,bl,bw,pc,tc,db,ay);}
ps("\n");dibbler_report();dibbler_state();ps("\n=== Demo Complete ===\n");return 0;}
