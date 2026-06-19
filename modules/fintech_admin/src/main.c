/* fintech_admin: Fintech administration (v1.0)
 * P2P lending, crowdfunding, robo-advisor, digital lending, neo-bank
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fta_t;
typedef struct{int n_pp,n_cf,n_ra,n_dl,n_nb,t_f1,t_f2,t_f3,t_f4,t_f5;} fta_state_t;
static fta_t pps[N],cfs[N-2],ras[N-4],dls[N-6],nbs[N-6]; static fta_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fta_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fta_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FTA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fta_init(void){if(init)return -1;st.n_pp=0;st.n_cf=0;st.n_ra=0;st.n_dl=0;st.n_nb=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pps[i].active=0;for(int i=0;i<N-2;i++)cfs[i].active=0;for(int i=0;i<N-4;i++)ras[i].active=0;for(int i=0;i<N-6;i++)dls[i].active=0;for(int i=0;i<N-6;i++)nbs[i].active=0;init=1;ps("[FTA] Fintech initialized\n");return 0;}
int fta_p2p(int t,int c,int a,int b,int d,int e,int y){return add(pps,&st.n_pp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fta_crowd(int t,int c,int a,int b,int d,int e,int y){return add(cfs,&st.n_cf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fta_robo(int t,int c,int a,int b,int d,int e,int y){return add(ras,&st.n_ra,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fta_digital(int t,int c,int a,int b,int d,int e,int y){return add(dls,&st.n_dl,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fta_neobank(int t,int c,int a,int b,int d,int e,int y){return add(nbs,&st.n_nb,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fta_report(void){ps("[FTA] P2P: ");pi(st.n_pp);ps(" NPL=");pi(st.t_f1);ps("\nCrowd: ");pi(st.n_cf);ps(" Fund=");pi(st.t_f2);ps("\nRobo: ");pi(st.n_ra);ps(" AUM=");pi(st.t_f3);ps("\nDL: ");pi(st.n_dl);ps(" Appr=");pi(st.t_f4);ps("\nNeo: ");pi(st.n_nb);ps(" Cust=");pi(st.t_f5);ps("\n");}
void fta_state(void){ps("[FTA] Pp=");pi(st.n_pp);ps(" Cf=");pi(st.n_cf);ps(" Ra=");pi(st.n_ra);ps(" Dl=");pi(st.n_dl);ps(" Nb=");pi(st.n_nb);ps("\n");}
int main(void){
ps("=== Fintech Admin Demo ===\n\n");fta_init();
ps("P2P lending...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fta_p2p(t,c,133+(i*17),118+(i*14),98+(i*10),81+(i*6),2020+(i%5));}
ps("\nCrowdfunding...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fta_crowd(t,c,122+(i*15),108+(i*12),90+(i*8),77+(i*5),2021+(i%4));}
ps("\nRobo-advisor...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fta_robo(t,c,114+(i*13),100+(i*10),84+(i*7),73+(i*4),2022+(i%3));}
ps("\nDigital lending...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fta_digital(t,c,106+(i*11),94+(i*9),80+(i*6),70+(i*3),2023+(i%2));}
ps("\nNeo-bank...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fta_neobank(t,c,100+(i*9),89+(i*7),76+(i*5),68+(i*3),2024);}
ps("\n");fta_report();fta_state();ps("\n=== Demo Complete ===\n");return 0;}
