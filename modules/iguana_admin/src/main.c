/* iguana_admin: Iguana (Iguana iguana) green large tree reptile (v1.0)
 * Iguana tree, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, climb_speed, scale_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,cl_sp,sc_idx,age_yr,active;} igua_t;
typedef struct{int n_tree,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_cl,t_sc;} igua_state_t;
static igua_t tre[N],trf[N-2],trb[N-4],trh[N-6],trm[N-6]; static igua_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(igua_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int cs,int si,int ay){if(*cnt>=mx)return -1;igua_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->cl_sp=cs;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[IGUA] Iguana ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" cs=");pi(cs);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int iguana_init(void){if(init)return -1;st.n_tree=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_cl=0;st.t_sc=0;for(int i=0;i<N;i++)tre[i].active=0;for(int i=0;i<N-2;i++)trf[i].active=0;for(int i=0;i<N-4;i++)trb[i].active=0;for(int i=0;i<N-6;i++)trh[i].active=0;for(int i=0;i<N-6;i++)trm[i].active=0;init=1;ps("[IGUA] Iguana initialized\n");return 0;}
int iguana_tree(int lc,int bl,int bw,int tc,int cs,int si,int ay){return add(tre,&st.n_tree,&st.t_ln,N,lc,bl,bw,tc,cs,si,ay);}
int iguana_feeding(int lc,int bl,int bw,int tc,int cs,int si,int ay){return add(trf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,cs,si,ay);}
int iguana_breeding(int lc,int bl,int bw,int tc,int cs,int si,int ay){return add(trb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,cs,si,ay);}
int iguana_health(int lc,int bl,int bw,int tc,int cs,int si,int ay){return add(trh,&st.n_health,&st.t_cl,N-6,lc,bl,bw,tc,cs,si,ay);}
int iguana_market(int lc,int bl,int bw,int tc,int cs,int si,int ay){return add(trm,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,tc,cs,si,ay);}
void iguana_report(void){ps("[IGUA] Tree: ");pi(st.n_tree);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void iguana_state(void){ps("[IGUA] Tree=");pi(st.n_tree);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Iguana Admin Demo ===\n\n");iguana_init();
ps("Iguana tree...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*6),bw=5+(i*1),tc=80+(i*5),cs=2+(i*1),si=(i%8)+1,ay=(i%15)+1;iguana_tree(lc,bl,bw,tc,cs,si,ay);}
ps("\nIguana feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=103+(i*5),bw=5+(i*1),tc=82+(i*4),cs=3+(i*1),si=(i%7)+1,ay=(i%12)+1;iguana_feeding(lc,bl,bw,tc,cs,si,ay);}
ps("\nIguana breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=106+(i*4),bw=6+(i*1),tc=84+(i*3),cs=2+(i*2),si=(i%6)+1,ay=(i%10)+1;iguana_breeding(lc,bl,bw,tc,cs,si,ay);}
ps("\nIguana health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=98+(i*7),bw=4+(i*1),tc=78+(i*6),cs=1+(i*2),si=(i%9)+1,ay=(i%8)+1;iguana_health(lc,bl,bw,tc,cs,si,ay);}
ps("\nIguana market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=108+(i*4),bw=6+(i*1),tc=86+(i*3),cs=3+(i*1),si=(i%5)+1,ay=(i%7)+1;iguana_market(lc,bl,bw,tc,cs,si,ay);}
ps("\n");iguana_report();iguana_state();ps("\n=== Demo Complete ===\n");return 0;}
