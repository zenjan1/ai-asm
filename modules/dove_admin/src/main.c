/* dove_admin: Dove (Columba livia) dove pigeon bird husbandry (v1.0)
 * Dove housing, feeding, breeding, grooming, market
 * Features: body_len_cm, body_wt_g, wing_span_cm, plumage_idx, coo_vol, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,plm_idx,coo_vl,age_yr,active;} dove_t;
typedef struct{int n_house,n_feed,n_breed,n_groom,n_mkt,t_ln,t_wt,t_wg,t_pl,t_co;} dove_state_t;
static dove_t dhs[N],dfd[N-2],dbr[N-4],dgr[N-6],dmk[N-6]; static dove_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dove_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int px,int cv,int ay){if(*cnt>=mx)return -1;dove_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->plm_idx=px;x->coo_vl=cv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DOVE] Dove ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" px=");pi(px);ps(" cv=");pi(cv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dove_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wg=0;st.t_pl=0;st.t_co=0;for(int i=0;i<N;i++)dhs[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dbr[i].active=0;for(int i=0;i<N-6;i++)dgr[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DOVE] Dove initialized\n");return 0;}
int dove_housing(int lc,int bl,int bw,int ws,int px,int cv,int ay){return add(dhs,&st.n_house,&st.t_ln,N,lc,bl,bw,ws,px,cv,ay);}
int dove_feeding(int lc,int bl,int bw,int ws,int px,int cv,int ay){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,px,cv,ay);}
int dove_breeding(int lc,int bl,int bw,int ws,int px,int cv,int ay){return add(dbr,&st.n_breed,&st.t_wg,N-4,lc,bl,bw,ws,px,cv,ay);}
int dove_grooming(int lc,int bl,int bw,int ws,int px,int cv,int ay){return add(dgr,&st.n_groom,&st.t_pl,N-6,lc,bl,bw,ws,px,cv,ay);}
int dove_market(int lc,int bl,int bw,int ws,int px,int cv,int ay){return add(dmk,&st.n_mkt,&st.t_co,N-6,lc,bl,bw,ws,px,cv,ay);}
void dove_report(void){ps("[DOVE] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wg=");pi(st.t_wg);ps("\nGroom: ");pi(st.n_groom);ps(" Pl=");pi(st.t_pl);ps("\nMkt: ");pi(st.n_mkt);ps(" Co=");pi(st.t_co);ps("\n");}
void dove_state(void){ps("[DOVE] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dove Admin Demo ===\n\n");dove_init();
/* 1=cote 2=aviary 3=garden 4=park 5=market */
ps("Dove housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*3),bw=200+(i*20),ws=50+(i*4),px=(i%6)+1,cv=40+(i*5),ay=1+(i%8);dove_housing(lc,bl,bw,ws,px,cv,ay);}
ps("\nDove feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*2),bw=220+(i*15),ws=52+(i*3),px=(i%5)+1,cv=45+(i*4),ay=2+(i%6);dove_feeding(lc,bl,bw,ws,px,cv,ay);}
ps("\nDove breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=34+(i*2),bw=240+(i*12),ws=54+(i*3),px=(i%4)+1,cv=50+(i*3),ay=1+(i%5);dove_breeding(lc,bl,bw,ws,px,cv,ay);}
ps("\nDove grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*4),bw=180+(i*25),ws=48+(i*5),px=(i%7)+1,cv=35+(i*6),ay=3+(i%7);dove_grooming(lc,bl,bw,ws,px,cv,ay);}
ps("\nDove market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*2),bw=260+(i*10),ws=56+(i*2),px=(i%3)+4,cv=55+(i*3),ay=2+(i%4);dove_market(lc,bl,bw,ws,px,cv,ay);}
ps("\n");dove_report();dove_state();ps("\n=== Demo Complete ===\n");return 0;}
