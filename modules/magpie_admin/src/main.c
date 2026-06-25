/* magpie_admin: Magpie (Pica pica) Eurasian magpie corvid bird (v1.0)
 * Magpie habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_len_cm, call_vol, cache_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,cl_vl,ch_idx,age_yr,active;} magp_t;
typedef struct{int n_habitat,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_cl,t_ch;} magp_state_t;
static magp_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static magp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(magp_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int cv,int ci,int ay){if(*cnt>=mx)return -1;magp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->cl_vl=cv;x->ch_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MAGP] Magpie ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" cv=");pi(cv);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int magp_init(void){if(init)return -1;st.n_habitat=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_cl=0;st.t_ch=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MAGP] Magpie initialized\n");return 0;}
int magp_habitat(int lc,int bl,int bw,int tl,int cv,int ci,int ay){return add(apl,&st.n_habitat,&st.t_ln,N,lc,bl,bw,tl,cv,ci,ay);}
int magp_feeding(int lc,int bl,int bw,int tl,int cv,int ci,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,cv,ci,ay);}
int magp_breeding(int lc,int bl,int bw,int tl,int cv,int ci,int ay){return add(abd,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tl,cv,ci,ay);}
int magp_health(int lc,int bl,int bw,int tl,int cv,int ci,int ay){return add(ahd,&st.n_health,&st.t_cl,N-6,lc,bl,bw,tl,cv,ci,ay);}
int magp_market(int lc,int bl,int bw,int tl,int cv,int ci,int ay){return add(amd,&st.n_mkt,&st.t_ch,N-6,lc,bl,bw,tl,cv,ci,ay);}
void magp_report(void){ps("[MAGP] Habitat: ");pi(st.n_habitat);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Ch=");pi(st.t_ch);ps("\n");}
void magp_state(void){ps("[MAGP] Habitat=");pi(st.n_habitat);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Magpie Admin Demo ===\n\n");magp_init();
ps("Magpie habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*2),bw=150+(i*20),tl=20+(i*3),cv=40+(i*5),ci=(i%7)+1,ay=1+(i%12);magp_habitat(lc,bl,bw,tl,cv,ci,ay);}
ps("\nMagpie feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*2),bw=160+(i*20),tl=22+(i*3),cv=42+(i*5),ci=(i%6)+2,ay=2+(i%10);magp_feeding(lc,bl,bw,tl,cv,ci,ay);}
ps("\nMagpie breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=40+(i*2),bw=170+(i*20),tl=24+(i*3),cv=45+(i*5),ci=(i%5)+1,ay=3+(i%8);magp_breeding(lc,bl,bw,tl,cv,ci,ay);}
ps("\nMagpie health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=33+(i*3),bw=140+(i*25),tl=18+(i*4),cv=38+(i*6),ci=(i%4)+3,ay=4+(i%6);magp_health(lc,bl,bw,tl,cv,ci,ay);}
ps("\nMagpie market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=45+(i*2),bw=180+(i*20),tl=26+(i*3),cv=50+(i*5),ci=(i%3)+4,ay=5+(i%5);magp_market(lc,bl,bw,tl,cv,ci,ay);}
ps("\n");magp_report();magp_state();ps("\n=== Demo Complete ===\n");return 0;}
