/* crow_admin: Crow (Corvus corax) corvid bird husbandry (v1.0)
 * Crow housing, feeding, training, grooming, market
 * Features: body_len_cm, body_wt_g, wing_span_cm, vocal_idx, food_cache, feather_idx
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,vocal_idx,food_cc,fthr_idx,active;} crw_t;
typedef struct{int n_house,n_feed,n_train,n_groom,n_mkt,t_ln,t_wt,t_wing,t_vocal,t_food;} crw_state_t;
static crw_t crwhs[N],crwfd[N-2],crwtr[N-4],crwgr[N-6],crwmt[N-6]; static crw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crw_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int vi,int fc,int fi){if(*cnt>=mx)return -1;crw_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->vocal_idx=vi;x->food_cc=fc;x->fthr_idx=fi;x->active=1;*sum+=bl;(*cnt)++;ps("[CRW] Crow ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" vi=");pi(vi);ps(" fc=");pi(fc);ps(" fi=");pi(fi);ps("\n");return *cnt-1;}
int crw_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_train=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_vocal=0;st.t_food=0;for(int i=0;i<N;i++)crwhs[i].active=0;for(int i=0;i<N-2;i++)crwfd[i].active=0;for(int i=0;i<N-4;i++)crwtr[i].active=0;for(int i=0;i<N-6;i++)crwgr[i].active=0;for(int i=0;i<N-6;i++)crwmt[i].active=0;init=1;ps("[CRW] Crow initialized\n");return 0;}
int crw_housing(int lc,int bl,int bw,int ws,int vi,int fc,int fi){return add(crwhs,&st.n_house,&st.t_ln,N,lc,bl,bw,ws,vi,fc,fi);}
int crw_feeding(int lc,int bl,int bw,int ws,int vi,int fc,int fi){return add(crwfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,vi,fc,fi);}
int crw_training(int lc,int bl,int bw,int ws,int vi,int fc,int fi){return add(crwtr,&st.n_train,&st.t_wing,N-4,lc,bl,bw,ws,vi,fc,fi);}
int crw_grooming(int lc,int bl,int bw,int ws,int vi,int fc,int fi){return add(crwgr,&st.n_groom,&st.t_vocal,N-6,lc,bl,bw,ws,vi,fc,fi);}
int crw_market(int lc,int bl,int bw,int ws,int vi,int fc,int fi){return add(crwmt,&st.n_mkt,&st.t_food,N-6,lc,bl,bw,ws,vi,fc,fi);}
void crw_report(void){ps("[CRW] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nTrain: ");pi(st.n_train);ps(" Wing=");pi(st.t_wing);ps("\nGroom: ");pi(st.n_groom);ps(" Vocal=");pi(st.t_vocal);ps("\nMkt: ");pi(st.n_mkt);ps(" Food=");pi(st.t_food);ps("\n");}
void crw_state(void){ps("[CRW] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Train=");pi(st.n_train);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Crow Admin Demo ===\n\n");crw_init();
/* 1=aviary 2=urban 3=sanctuary 4=forest 5=market */
ps("Crow housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=45+(i*4),bw=350+(i*40),ws=100+(i*8),vi=(i%7)+1,fc=5+(i*3),fi=(i%5)+1;crw_housing(lc,bl,bw,ws,vi,fc,fi);}
ps("\nCrow feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=48+(i*3),bw=370+(i*35),ws=105+(i*7),vi=(i%6)+1,fc=6+(i*2),fi=(i%4)+1;crw_feeding(lc,bl,bw,ws,vi,fc,fi);}
ps("\nCrow training...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=50+(i*3),bw=390+(i*30),ws=108+(i*6),vi=(i%5)+1,fc=7+(i*2),fi=(i%3)+1;crw_training(lc,bl,bw,ws,vi,fc,fi);}
ps("\nCrow grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=42+(i*5),bw=330+(i*50),ws=95+(i*10),vi=(i%8)+1,fc=4+(i*4),fi=(i%6)+1;crw_grooming(lc,bl,bw,ws,vi,fc,fi);}
ps("\nCrow market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=52+(i*3),bw=410+(i*25),ws=112+(i*5),vi=(i%4)+4,fc=8+(i*2),fi=(i%3)+3;crw_market(lc,bl,bw,ws,vi,fc,fi);}
ps("\n");crw_report();crw_state();ps("\n=== Demo Complete ===\n");return 0;}
