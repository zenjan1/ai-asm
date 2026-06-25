/* macropods_admin: Macropods (Macropodidae) large marsupeds (v1.0)
 * Macropods forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, hind_cm, tail_cm, mp_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hind_cm,tail_cm,mp_idx,age_yr,active;} mcpd_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hind,t_tail,t_mp;} mcpd_state_t;
static mcpd_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static mcpd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mcpd_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int tc,int mp,int ay){if(*cnt>=mx)return -1;mcpd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hind_cm=hc;x->tail_cm=tc;x->mp_idx=mp;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MCPD] Macropod ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" tc=");pi(tc);ps(" mp=");pi(mp);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int macropods_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hind=0;st.t_tail=0;st.t_mp=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[MCPD] Macropods initialized\n");return 0;}
int macropods_forest(int lc,int bl,int bw,int hc,int tc,int mp,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,hc,tc,mp,ay);}
int macropods_feeding(int lc,int bl,int bw,int hc,int tc,int mp,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,tc,mp,ay);}
int macropods_breeding(int lc,int bl,int bw,int hc,int tc,int mp,int ay){return add(svb,&st.n_breed,&st.t_hind,N-4,lc,bl,bw,hc,tc,mp,ay);}
int macropods_health(int lc,int bl,int bw,int hc,int tc,int mp,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,hc,tc,mp,ay);}
int macropods_market(int lc,int bl,int bw,int hc,int tc,int mp,int ay){return add(svm,&st.n_mkt,&st.t_mp,N-6,lc,bl,bw,hc,tc,mp,ay);}
void macropods_report(void){ps("[MCPD] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hind=");pi(st.t_hind);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Mp=");pi(st.t_mp);ps("\n");}
void macropods_state(void){ps("[MCPD] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Macropods Admin Demo ===\n\n");macropods_init();
ps("Macropods forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=90+(i*3),bw=25+(i*2),hc=40+(i%4),tc=70+(i*3),mp=(i%8)+1,ay=(i%5)+1;macropods_forest(lc,bl,bw,hc,tc,mp,ay);}
ps("\nMacropods feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=92+(i*2),bw=26+(i*2),hc=41+(i%3),tc=72+(i*2),mp=(i%6)+1,ay=(i%4)+1;macropods_feeding(lc,bl,bw,hc,tc,mp,ay);}
ps("\nMacropods breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=95+(i*2),bw=28+(i*1),hc=42+(i%3),tc=75+(i*2),mp=(i%5)+1,ay=(i%3)+1;macropods_breeding(lc,bl,bw,hc,tc,mp,ay);}
ps("\nMacropods health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=88+(i*4),bw=23+(i*2),hc=39+(i%4),tc=68+(i*3),mp=(i%10)+1,ay=(i%5)+1;macropods_health(lc,bl,bw,hc,tc,mp,ay);}
ps("\nMacropods market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=98+(i*2),bw=30+(i*1),hc=43+(i%3),tc=78+(i*2),mp=(i%4)+1,ay=(i%3)+1;macropods_market(lc,bl,bw,hc,tc,mp,ay);}
ps("\n");macropods_report();macropods_state();ps("\n=== Demo Complete ===\n");return 0;}
