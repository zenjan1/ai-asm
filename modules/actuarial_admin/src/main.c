/* actuarial_admin: Actuarial administration (v1.0)
 * Life contingency, pension mathematics, risk theory, ruin theory, credibility theory
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} act_t;
typedef struct{int n_lc,n_pm,n_rt,n_ru,n_cr,t_f1,t_f2,t_f3,t_f4,t_f5;} act_state_t;
static act_t lcs[N],pms[N-2],rts[N-4],rus[N-6],crs[N-6]; static act_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(act_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;act_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int act_init(void){if(init)return -1;st.n_lc=0;st.n_pm=0;st.n_rt=0;st.n_ru=0;st.n_cr=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lcs[i].active=0;for(int i=0;i<N-2;i++)pms[i].active=0;for(int i=0;i<N-4;i++)rts[i].active=0;for(int i=0;i<N-6;i++)rus[i].active=0;for(int i=0;i<N-6;i++)crs[i].active=0;init=1;ps("[ACT] Actuarial initialized\n");return 0;}
int act_life(int t,int c,int a,int b,int d,int e,int y){return add(lcs,&st.n_lc,&st.t_f1,N,t,c,a,b,d,e,y);}
int act_pension(int t,int c,int a,int b,int d,int e,int y){return add(pms,&st.n_pm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int act_risk(int t,int c,int a,int b,int d,int e,int y){return add(rts,&st.n_rt,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int act_ruin(int t,int c,int a,int b,int d,int e,int y){return add(rus,&st.n_ru,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int act_cred(int t,int c,int a,int b,int d,int e,int y){return add(crs,&st.n_cr,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void act_report(void){ps("[ACT] Life: ");pi(st.n_lc);ps(" Mort=");pi(st.t_f1);ps("\nPens: ");pi(st.n_pm);ps(" DB=");pi(st.t_f2);ps("\nRisk: ");pi(st.n_rt);ps(" EVT=");pi(st.t_f3);ps("\nRuin: ");pi(st.n_ru);ps(" RC=");pi(st.t_f4);ps("\nCred: ");pi(st.n_cr);ps(" Z=");pi(st.t_f5);ps("\n");}
void act_state(void){ps("[ACT] Lc=");pi(st.n_lc);ps(" Pm=");pi(st.n_pm);ps(" Rt=");pi(st.n_rt);ps(" Ru=");pi(st.n_ru);ps(" Cr=");pi(st.n_cr);ps("\n");}
int main(void){
ps("=== Actuarial Admin Demo ===\n\n");act_init();
ps("Life contingency...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;act_life(t,c,120+(i*17),105+(i*14),85+(i*10),68+(i*6),2020+(i%5));}
ps("\nPension mathematics...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;act_pension(t,c,109+(i*15),95+(i*12),77+(i*8),64+(i*5),2021+(i%4));}
ps("\nRisk theory...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;act_risk(t,c,101+(i*13),87+(i*10),71+(i*7),60+(i*4),2022+(i%3));}
ps("\nRuin theory...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;act_ruin(t,c,93+(i*11),81+(i*9),67+(i*6),57+(i*3),2023+(i%2));}
ps("\nCredibility theory...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;act_cred(t,c,87+(i*9),76+(i*7),63+(i*5),55+(i*3),2024);}
ps("\n");act_report();act_state();ps("\n=== Demo Complete ===\n");return 0;}
