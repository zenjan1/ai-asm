/* chimpanzee_admin: Chimpanzee (Pan troglodytes) common great ape (v1.0)
 * Chimpanzee forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, arm_cm, climb_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,arm_cm,cl_sp,fr_idx,age_yr,active;} chmp_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_arm,t_cl,t_fr;} chmp_state_t;
static chmp_t frl[N],frf[N-2],frb[N-4],frh[N-6],frm[N-6]; static chmp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chmp_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int cs,int fi,int ay){if(*cnt>=mx)return -1;chmp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->arm_cm=ac;x->cl_sp=cs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CHMP] Chimp ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" cs=");pi(cs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int chimpanzee_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_arm=0;st.t_cl=0;st.t_fr=0;for(int i=0;i<N;i++)frl[i].active=0;for(int i=0;i<N-2;i++)frf[i].active=0;for(int i=0;i<N-4;i++)frb[i].active=0;for(int i=0;i<N-6;i++)frh[i].active=0;for(int i=0;i<N-6;i++)frm[i].active=0;init=1;ps("[CHMP] Chimpanzee initialized\n");return 0;}
int chimpanzee_forest(int lc,int bl,int bw,int ac,int cs,int fi,int ay){return add(frl,&st.n_forest,&st.t_ln,N,lc,bl,bw,ac,cs,fi,ay);}
int chimpanzee_feeding(int lc,int bl,int bw,int ac,int cs,int fi,int ay){return add(frf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,cs,fi,ay);}
int chimpanzee_breeding(int lc,int bl,int bw,int ac,int cs,int fi,int ay){return add(frb,&st.n_breed,&st.t_arm,N-4,lc,bl,bw,ac,cs,fi,ay);}
int chimpanzee_health(int lc,int bl,int bw,int ac,int cs,int fi,int ay){return add(frh,&st.n_health,&st.t_cl,N-6,lc,bl,bw,ac,cs,fi,ay);}
int chimpanzee_market(int lc,int bl,int bw,int ac,int cs,int fi,int ay){return add(frm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,ac,cs,fi,ay);}
void chimpanzee_report(void){ps("[CHMP] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Arm=");pi(st.t_arm);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void chimpanzee_state(void){ps("[CHMP] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chimpanzee Admin Demo ===\n\n");chimpanzee_init();
ps("Chimpanzee forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*6),bw=30+(i*8),ac=60+(i*5),cs=10+(i*3),fi=(i%8)+1,ay=(i%25)+1;chimpanzee_forest(lc,bl,bw,ac,cs,fi,ay);}
ps("\nChimpanzee feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=85+(i*5),bw=34+(i*6),ac=63+(i*4),cs=12+(i*2),fi=(i%6)+1,ay=(i%20)+1;chimpanzee_feeding(lc,bl,bw,ac,cs,fi,ay);}
ps("\nChimpanzee breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=90+(i*4),bw=38+(i*5),ac=66+(i*3),cs=8+(i*4),fi=(i%5)+1,ay=(i%15)+1;chimpanzee_breeding(lc,bl,bw,ac,cs,fi,ay);}
ps("\nChimpanzee health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=78+(i*7),bw=28+(i*10),ac=58+(i*6),cs=6+(i*5),fi=(i%10)+1,ay=(i%12)+1;chimpanzee_health(lc,bl,bw,ac,cs,fi,ay);}
ps("\nChimpanzee market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=95+(i*3),bw=42+(i*4),ac=68+(i*3),cs=15+(i*2),fi=(i%4)+1,ay=(i%10)+1;chimpanzee_market(lc,bl,bw,ac,cs,fi,ay);}
ps("\n");chimpanzee_report();chimpanzee_state();ps("\n=== Demo Complete ===\n");return 0;}
