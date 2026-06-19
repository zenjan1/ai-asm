/* game_theory_admin: Game theory administration system (v1.0)
 * Complete info static games, complete info dynamic games, incomplete info, cooperative games, mechanism design
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gam_sub_t;
typedef struct{int n_cis,n_cid,n_inc,n_cop,n_mec,t_f1,t_f2,t_f3,t_f4,t_f5;} gam_state_t;
static gam_sub_t ciss[N],cids[N-2],incs[N-4],cops[N-6],mecs[N-6]; static gam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gam_sub_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gam_sub_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GAM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gam_init(void){if(init)return -1;st.n_cis=0;st.n_cid=0;st.n_inc=0;st.n_cop=0;st.n_mec=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ciss[i].active=0;for(int i=0;i<N-2;i++)cids[i].active=0;for(int i=0;i<N-4;i++)incs[i].active=0;for(int i=0;i<N-6;i++)cops[i].active=0;for(int i=0;i<N-6;i++)mecs[i].active=0;init=1;ps("[GAM] Game theory initialized\n");return 0;}
int gam_static(int t,int c,int a,int b,int d,int e,int y){return add(ciss,&st.n_cis,&st.t_f1,N,t,c,a,b,d,e,y);}
int gam_dynamic(int t,int c,int a,int b,int d,int e,int y){return add(cids,&st.n_cid,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gam_incomplete(int t,int c,int a,int b,int d,int e,int y){return add(incs,&st.n_inc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gam_coop(int t,int c,int a,int b,int d,int e,int y){return add(cops,&st.n_cop,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gam_mechanism(int t,int c,int a,int b,int d,int e,int y){return add(mecs,&st.n_mec,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gam_report(void){ps("[GAM] Static: ");pi(st.n_cis);ps(" Nash=");pi(st.t_f1);ps("\nDynamic: ");pi(st.n_cid);ps(" Sub=");pi(st.t_f2);ps("\nIncomplete: ");pi(st.n_inc);ps(" Bayes=");pi(st.t_f3);ps("\nCooperative: ");pi(st.n_cop);ps(" Core=");pi(st.t_f4);ps("\nMechanism: ");pi(st.n_mec);ps(" VCG=");pi(st.t_f5);ps("\n");}
void gam_state(void){ps("[GAM] Ci=");pi(st.n_cis);ps(" Cd=");pi(st.n_cid);ps(" Ic=");pi(st.n_inc);ps(" Co=");pi(st.n_cop);ps(" Me=");pi(st.n_mec);ps("\n");}
int main(void){
ps("=== Game Theory Admin Demo ===\n\n");gam_init();
ps("Complete info static...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gam_static(t,c,80+(i*19),66+(i*16),46+(i*12),29+(i*8),2020+(i%5));}
ps("\nComplete info dynamic...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gam_dynamic(t,c,70+(i*17),56+(i*14),38+(i*10),25+(i*7),2021+(i%4));}
ps("\nIncomplete info...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gam_incomplete(t,c,62+(i*15),48+(i*12),32+(i*8),21+(i*5),2022+(i%3));}
ps("\nCooperative games...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gam_coop(t,c,54+(i*13),42+(i*10),28+(i*6),19+(i*4),2023+(i%2));}
ps("\nMechanism design...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gam_mechanism(t,c,48+(i*11),37+(i*9),24+(i*6),16+(i*3),2024);}
ps("\n");gam_report();gam_state();ps("\n=== Demo Complete ===\n");return 0;}
