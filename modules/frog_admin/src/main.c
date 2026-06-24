/* frog_admin: Frog (Rana temporaria) common frog amphibian management (v1.0)
 * Frog habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, jump_cm, skin_idx, croak_vol, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,jmp_cm,sk_idx,crk_vl,age_yr,active;} frog_t;
typedef struct{int n_hab,n_feed,n_breed,n_hlth,n_mkt,t_ln,t_wt,t_jm,t_sk,t_cr;} frog_state_t;
static frog_t fhab[N],ffd[N-2],fbr[N-4],fhl[N-6],fmk[N-6]; static frog_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(frog_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int jc,int si,int cv,int ay){if(*cnt>=mx)return -1;frog_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->jmp_cm=jc;x->sk_idx=si;x->crk_vl=cv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[FROG] Frog ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" jc=");pi(jc);ps(" si=");pi(si);ps(" cv=");pi(cv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int frog_init(void){if(init)return -1;st.n_hab=0;st.n_feed=0;st.n_breed=0;st.n_hlth=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_jm=0;st.t_sk=0;st.t_cr=0;for(int i=0;i<N;i++)fhab[i].active=0;for(int i=0;i<N-2;i++)ffd[i].active=0;for(int i=0;i<N-4;i++)fbr[i].active=0;for(int i=0;i<N-6;i++)fhl[i].active=0;for(int i=0;i<N-6;i++)fmk[i].active=0;init=1;ps("[FROG] Frog initialized\n");return 0;}
int frog_habitat(int lc,int bl,int bw,int jc,int si,int cv,int ay){return add(fhab,&st.n_hab,&st.t_ln,N,lc,bl,bw,jc,si,cv,ay);}
int frog_feeding(int lc,int bl,int bw,int jc,int si,int cv,int ay){return add(ffd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,jc,si,cv,ay);}
int frog_breeding(int lc,int bl,int bw,int jc,int si,int cv,int ay){return add(fbr,&st.n_breed,&st.t_jm,N-4,lc,bl,bw,jc,si,cv,ay);}
int frog_health(int lc,int bl,int bw,int jc,int si,int cv,int ay){return add(fhl,&st.n_hlth,&st.t_sk,N-6,lc,bl,bw,jc,si,cv,ay);}
int frog_market(int lc,int bl,int bw,int jc,int si,int cv,int ay){return add(fmk,&st.n_mkt,&st.t_cr,N-6,lc,bl,bw,jc,si,cv,ay);}
void frog_report(void){ps("[FROG] Hab: ");pi(st.n_hab);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Jm=");pi(st.t_jm);ps("\nHlth: ");pi(st.n_hlth);ps(" Sk=");pi(st.t_sk);ps("\nMkt: ");pi(st.n_mkt);ps(" Cr=");pi(st.t_cr);ps("\n");}
void frog_state(void){ps("[FROG] Hab=");pi(st.n_hab);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_hlth);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Frog Admin Demo ===\n\n");frog_init();
/* 1=pond 2=marsh 3=stream 4=tank 5=market */
ps("Frog habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=8+(i%3),bw=30+(i*5),jc=20+(i*4),si=(i%6)+1,cv=40+(i*5),ay=1+(i%5);frog_habitat(lc,bl,bw,jc,si,cv,ay);}
ps("\nFrog feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=9+(i%2),bw=35+(i*4),jc=22+(i*3),si=(i%5)+1,cv=45+(i*4),ay=1+(i%4);frog_feeding(lc,bl,bw,jc,si,cv,ay);}
ps("\nFrog breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=7+(i%3),bw=25+(i*6),jc=18+(i*5),si=(i%4)+1,cv=50+(i*3),ay=2+(i%4);frog_breeding(lc,bl,bw,jc,si,cv,ay);}
ps("\nFrog health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=10+(i%2),bw=40+(i*4),jc=25+(i*3),si=(i%7)+1,cv=35+(i*6),ay=2+(i%6);frog_health(lc,bl,bw,jc,si,cv,ay);}
ps("\nFrog market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=9+(i%2),bw=45+(i*3),jc=28+(i*2),si=(i%3)+4,cv=55+(i*3),ay=3+(i%3);frog_market(lc,bl,bw,jc,si,cv,ay);}
ps("\n");frog_report();frog_state();ps("\n=== Demo Complete ===\n");return 0;}
