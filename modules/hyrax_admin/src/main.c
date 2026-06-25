/* hyrax_admin: Hyrax (Procavia capensis) rock hyrax small mammal (v1.0)
 * Hyrax rocky, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, walk_speed, cl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,wk_sp,cl_idx,age_yr,active;} hyrx_t;
typedef struct{int n_rocky,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_wk,t_cl;} hyrx_state_t;
static hyrx_t arr_r[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static hyrx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hyrx_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int ws,int cli,int ay){if(*cnt>=mx)return -1;hyrx_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->wk_sp=ws;x->cl_idx=cli;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HYRX] Hyrax ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" ws=");pi(ws);ps(" cli=");pi(cli);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hyrax_init(void){if(init)return -1;st.n_rocky=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_wk=0;st.t_cl=0;for(int i=0;i<N;i++)arr_r[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[HYRX] Hyrax initialized\n");return 0;}
int hyrax_rocky(int lc,int bl,int bw,int ec,int ws,int cli,int ay){return add(arr_r,&st.n_rocky,&st.t_ln,N,lc,bl,bw,ec,ws,cli,ay);}
int hyrax_feeding(int lc,int bl,int bw,int ec,int ws,int cli,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,ws,cli,ay);}
int hyrax_breeding(int lc,int bl,int bw,int ec,int ws,int cli,int ay){return add(arr_b,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,ws,cli,ay);}
int hyrax_health(int lc,int bl,int bw,int ec,int ws,int cli,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,ec,ws,cli,ay);}
int hyrax_market(int lc,int bl,int bw,int ec,int ws,int cli,int ay){return add(arr_m,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,ec,ws,cli,ay);}
void hyrax_report(void){ps("[HYRX] Rocky: ");pi(st.n_rocky);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void hyrax_state(void){ps("[HYRX] Rocky=");pi(st.n_rocky);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hyrax Admin Demo ===\n\n");hyrax_init();
ps("Hyrax rocky...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=3+(i*1),ec=5+(i*1),ws=2+(i*1),cli=(i%6)+1,ay=(i%8)+1;hyrax_rocky(lc,bl,bw,ec,ws,cli,ay);}
ps("\nHyrax feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*2),bw=3+(i*1),ec=6+(i*1),ws=3+(i*1),cli=(i%5)+1,ay=(i%7)+1;hyrax_feeding(lc,bl,bw,ec,ws,cli,ay);}
ps("\nHyrax breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=45+(i*2),bw=4+(i*1),ec=6+(i*1),ws=2+(i*2),cli=(i%4)+1,ay=(i%6)+1;hyrax_breeding(lc,bl,bw,ec,ws,cli,ay);}
ps("\nHyrax health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=2+(i*1),ec=4+(i*1),ws=1+(i*3),cli=(i%7)+1,ay=(i%5)+1;hyrax_health(lc,bl,bw,ec,ws,cli,ay);}
ps("\nHyrax market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=48+(i*2),bw=4+(i*1),ec=7+(i*1),ws=4+(i*1),cli=(i%3)+1,ay=(i%4)+1;hyrax_market(lc,bl,bw,ec,ws,cli,ay);}
ps("\n");hyrax_report();hyrax_state();ps("\n=== Demo Complete ===\n");return 0;}
