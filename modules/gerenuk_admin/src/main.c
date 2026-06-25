/* gerenuk_admin: Gerenuk (Litocranius walleri) long-necked antelope (v1.0)
 * Gerenuk thicket, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, neck_cm, jump_cm, ge_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,neck_cm,jp_cm,ge_idx,age_yr,active;} gern_t;
typedef struct{int n_thick,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_neck,t_jp,t_ge;} gern_state_t;
static gern_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static gern_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gern_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nc,int jc,int gi,int ay){if(*cnt>=mx)return -1;gern_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->neck_cm=nc;x->jp_cm=jc;x->ge_idx=gi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GERN] Gerenuk ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nc=");pi(nc);ps(" jc=");pi(jc);ps(" gi=");pi(gi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gerenuk_init(void){if(init)return -1;st.n_thick=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_neck=0;st.t_jp=0;st.t_ge=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[GERN] Gerenuk initialized\n");return 0;}
int gerenuk_thicket(int lc,int bl,int bw,int nc,int jc,int gi,int ay){return add(svl,&st.n_thick,&st.t_ln,N,lc,bl,bw,nc,jc,gi,ay);}
int gerenuk_feeding(int lc,int bl,int bw,int nc,int jc,int gi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nc,jc,gi,ay);}
int gerenuk_breeding(int lc,int bl,int bw,int nc,int jc,int gi,int ay){return add(svb,&st.n_breed,&st.t_neck,N-4,lc,bl,bw,nc,jc,gi,ay);}
int gerenuk_health(int lc,int bl,int bw,int nc,int jc,int gi,int ay){return add(svh,&st.n_health,&st.t_jp,N-6,lc,bl,bw,nc,jc,gi,ay);}
int gerenuk_market(int lc,int bl,int bw,int nc,int jc,int gi,int ay){return add(svm,&st.n_mkt,&st.t_ge,N-6,lc,bl,bw,nc,jc,gi,ay);}
void gerenuk_report(void){ps("[GERN] Thick: ");pi(st.n_thick);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Neck=");pi(st.t_neck);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Ge=");pi(st.t_ge);ps("\n");}
void gerenuk_state(void){ps("[GERN] Thick=");pi(st.n_thick);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gerenuk Admin Demo ===\n\n");gerenuk_init();
ps("Gerenuk thicket...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*5),bw=25+(i*3),nc=40+(i*3),jc=90+(i*6),gi=(i%8)+1,ay=(i%10)+1;gerenuk_thicket(lc,bl,bw,nc,jc,gi,ay);}
ps("\nGerenuk feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=103+(i*4),bw=27+(i*2),nc=42+(i*2),jc=95+(i*5),gi=(i%6)+1,ay=(i%9)+1;gerenuk_feeding(lc,bl,bw,nc,jc,gi,ay);}
ps("\nGerenuk breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=106+(i*3),bw=29+(i*2),nc=44+(i*2),jc=100+(i*4),gi=(i%5)+1,ay=(i%8)+1;gerenuk_breeding(lc,bl,bw,nc,jc,gi,ay);}
ps("\nGerenuk health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=98+(i*6),bw=23+(i*4),nc=38+(i*4),jc=88+(i*8),gi=(i%10)+1,ay=(i%6)+1;gerenuk_health(lc,bl,bw,nc,jc,gi,ay);}
ps("\nGerenuk market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=110+(i*2),bw=31+(i*1),nc=46+(i*1),jc=105+(i*3),gi=(i%4)+1,ay=(i%5)+1;gerenuk_market(lc,bl,bw,nc,jc,gi,ay);}
ps("\n");gerenuk_report();gerenuk_state();ps("\n=== Demo Complete ===\n");return 0;}
