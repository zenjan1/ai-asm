/* ngapa_admin: Ngapa burrowing marsupials (v1.0)
 * Ngapa forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, burrow_cm, tail_cm, ng_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,burrow_cm,tail_cm,ng_idx,age_yr,active;} ngap_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_burrow,t_tail,t_ng;} ngap_state_t;
static ngap_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static ngap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ngap_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int tc,int ng,int ay){if(*cnt>=mx)return -1;ngap_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->burrow_cm=bc;x->tail_cm=tc;x->ng_idx=ng;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[NGAP] Ngapa ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" tc=");pi(tc);ps(" ng=");pi(ng);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ngapa_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_burrow=0;st.t_tail=0;st.t_ng=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[NGAP] Ngapa initialized\n");return 0;}
int ngapa_forest(int lc,int bl,int bw,int bc,int tc,int ng,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,bc,tc,ng,ay);}
int ngapa_feeding(int lc,int bl,int bw,int bc,int tc,int ng,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,tc,ng,ay);}
int ngapa_breeding(int lc,int bl,int bw,int bc,int tc,int ng,int ay){return add(svb,&st.n_breed,&st.t_burrow,N-4,lc,bl,bw,bc,tc,ng,ay);}
int ngapa_health(int lc,int bl,int bw,int bc,int tc,int ng,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,bc,tc,ng,ay);}
int ngapa_market(int lc,int bl,int bw,int bc,int tc,int ng,int ay){return add(svm,&st.n_mkt,&st.t_ng,N-6,lc,bl,bw,bc,tc,ng,ay);}
void ngapa_report(void){ps("[NGAP] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Burrow=");pi(st.t_burrow);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Ng=");pi(st.t_ng);ps("\n");}
void ngapa_state(void){ps("[NGAP] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ngapa Admin Demo ===\n\n");ngapa_init();
ps("Ngapa forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*2),bw=500+(i*30),bc=40+(i*5),tc=18+(i*1),ng=(i%8)+1,ay=(i%5)+1;ngapa_forest(lc,bl,bw,bc,tc,ng,ay);}
ps("\nNgapa feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=27+(i*2),bw=530+(i*25),bc=42+(i*5),tc=19+(i*1),ng=(i%6)+1,ay=(i%4)+1;ngapa_feeding(lc,bl,bw,bc,tc,ng,ay);}
ps("\nNgapa breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=29+(i*2),bw=560+(i*20),bc=44+(i*5),tc=20+(i*1),ng=(i%5)+1,ay=(i%3)+1;ngapa_breeding(lc,bl,bw,bc,tc,ng,ay);}
ps("\nNgapa health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=23+(i*3),bw=470+(i*35),bc=38+(i*5),tc=17+(i*1),ng=(i%10)+1,ay=(i%5)+1;ngapa_health(lc,bl,bw,bc,tc,ng,ay);}
ps("\nNgapa market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=31+(i*2),bw=590+(i*20),bc=46+(i*5),tc=21+(i*1),ng=(i%4)+1,ay=(i%3)+1;ngapa_market(lc,bl,bw,bc,tc,ng,ay);}
ps("\n");ngapa_report();ngapa_state();ps("\n=== Demo Complete ===\n");return 0;}
