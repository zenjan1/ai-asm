/* chameleon_admin: Chameleon reptile terrarium management (v1.0)
 * Chameleon habitat, feeding, health, breeding, market
 * Features: body_length_cm, weight_g, color_change, tongue_len_cm, eye_rotation, shedding_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,body_ln,wght_g,clr_chg,tongue_ln,eye_rot,shed_wk,active;} cham_t;
typedef struct{int n_hab,n_feed,n_health,n_breed,n_mkt,t_body,t_wt,t_clr,t_tongue,t_eye;} cham_state_t;
static cham_t chamhs[N],chamfs[N-2],chamhes[N-4],chambrs[N-6],chamms[N-6]; static cham_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cham_t*a,int*cnt,int*sum,int mx,int lc,int bl,int wg,int cc,int tl,int er,int sw){if(*cnt>=mx)return -1;cham_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->body_ln=bl;x->wght_g=wg;x->clr_chg=cc;x->tongue_ln=tl;x->eye_rot=er;x->shed_wk=sw;x->active=1;*sum+=bl;(*cnt)++;ps("[CHAM] Chameleon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" wg=");pi(wg);ps(" cc=");pi(cc);ps(" tl=");pi(tl);ps(" er=");pi(er);ps(" sw=");pi(sw);ps("\n");return *cnt-1;}
int cham_init(void){if(init)return -1;st.n_hab=0;st.n_feed=0;st.n_health=0;st.n_breed=0;st.n_mkt=0;st.t_body=0;st.t_wt=0;st.t_clr=0;st.t_tongue=0;st.t_eye=0;for(int i=0;i<N;i++)chamhs[i].active=0;for(int i=0;i<N-2;i++)chamfs[i].active=0;for(int i=0;i<N-4;i++)chamhes[i].active=0;for(int i=0;i<N-6;i++)chambrs[i].active=0;for(int i=0;i<N-6;i++)chamms[i].active=0;init=1;ps("[CHAM] Chameleon initialized\n");return 0;}
int cham_habitat(int lc,int bl,int wg,int cc,int tl,int er,int sw){return add(chamhs,&st.n_hab,&st.t_body,N,lc,bl,wg,cc,tl,er,sw);}
int cham_feeding(int lc,int bl,int wg,int cc,int tl,int er,int sw){return add(chamfs,&st.n_feed,&st.t_wt,N-2,lc,bl,wg,cc,tl,er,sw);}
int cham_health(int lc,int bl,int wg,int cc,int tl,int er,int sw){return add(chamhes,&st.n_health,&st.t_clr,N-4,lc,bl,wg,cc,tl,er,sw);}
int cham_breeding(int lc,int bl,int wg,int cc,int tl,int er,int sw){return add(chambrs,&st.n_breed,&st.t_tongue,N-6,lc,bl,wg,cc,tl,er,sw);}
int cham_market(int lc,int bl,int wg,int cc,int tl,int er,int sw){return add(chamms,&st.n_mkt,&st.t_eye,N-6,lc,bl,wg,cc,tl,er,sw);}
void cham_report(void){ps("[CHAM] Hab: ");pi(st.n_hab);ps(" Body=");pi(st.t_body);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nHealth: ");pi(st.n_health);ps(" Clr=");pi(st.t_clr);ps("\nBreed: ");pi(st.n_breed);ps(" Tongue=");pi(st.t_tongue);ps("\nMkt: ");pi(st.n_mkt);ps(" Eye=");pi(st.t_eye);ps("\n");}
void cham_state(void){ps("[CHAM] Hab=");pi(st.n_hab);ps(" Feed=");pi(st.n_feed);ps(" Health=");pi(st.n_health);ps(" Breed=");pi(st.n_breed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chameleon Admin Demo ===\n\n");cham_init();
/* 1=terrarium 2=greenhouse 3=outdoor 4=breeding_room 5=show */
ps("Chameleon habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*3),wg=50+(i*20),cc=(i%8)+1,tl=5+(i*2),er=30+(i%60),sw=4+(i%8);cham_habitat(lc,bl,wg,cc,tl,er,sw);}
ps("\nChameleon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=12+(i*2),wg=60+(i*15),cc=(i%7)+1,tl=6+(i*2),er=40+(i%50),sw=5+(i%6);cham_feeding(lc,bl,wg,cc,tl,er,sw);}
ps("\nChameleon health...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=14+(i*2),wg=70+(i*12),cc=(i%6)+1,tl=7+(i*2),er=50+(i%40),sw=6+(i%4);cham_health(lc,bl,wg,cc,tl,er,sw);}
ps("\nChameleon breeding...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=8+(i*4),wg=40+(i*25),cc=(i%9)+1,tl=4+(i*3),er=20+(i%70),sw=3+(i%9);cham_breeding(lc,bl,wg,cc,tl,er,sw);}
ps("\nChameleon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=16+(i*2),wg=80+(i*10),cc=(i%5)+1,tl=8+(i*2),er=60+(i%30),sw=7+(i%3);cham_market(lc,bl,wg,cc,tl,er,sw);}
ps("\n");cham_report();cham_state();ps("\n=== Demo Complete ===\n");return 0;}
