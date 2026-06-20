/* annealingtech_admin: Annealing technology administration (v1.0)
 * Full annealing, spheroidizing, stress relieving, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ann_t;
typedef struct{int n_fl,n_sp,n_sr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ann_state_t;
static ann_t fls[N],sps[N-2],srs[N-4],acs[N-6],mks[N-6]; static ann_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ann_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ann_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ann_init(void){if(init)return -1;st.n_fl=0;st.n_sp=0;st.n_sr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fls[i].active=0;for(int i=0;i<N-2;i++)sps[i].active=0;for(int i=0;i<N-4;i++)srs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ANN] Annealingtech initialized\n");return 0;}
int ann_full(int t,int c,int a,int b,int d,int e,int y){return add(fls,&st.n_fl,&st.t_f1,N,t,c,a,b,d,e,y);}
int ann_spheroidize(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ann_stress(int t,int c,int a,int b,int d,int e,int y){return add(srs,&st.n_sr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ann_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ann_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ann_report(void){ps("[ANN] Fl: ");pi(st.n_fl);ps(" PCS=");pi(st.t_f1);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f2);ps("\nSr: ");pi(st.n_sr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ann_state(void){ps("[ANN] Fl=");pi(st.n_fl);ps(" Sp=");pi(st.n_sp);ps(" Sr=");pi(st.n_sr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Annealing Tech Admin Demo ===\n\n");ann_init();
ps("Full annealing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ann_full(t,c,326+(i*17),311+(i*14),291+(i*10),273+(i*6),2020+(i%5));}
ps("\nSpheroidizing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ann_spheroidize(t,c,315+(i*15),301+(i*12),283+(i*8),270+(i*5),2021+(i%4));}
ps("\nStress relieving...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ann_stress(t,c,307+(i*13),293+(i*10),277+(i*7),266+(i*4),2022+(i%3));}
ps("\nAnnealing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ann_accessory(t,c,299+(i*11),287+(i*9),273+(i*6),263+(i*3),2023+(i%2));}
ps("\nAnnealing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ann_market(t,c,293+(i*9),282+(i*7),269+(i*5),261+(i*3),2024);}
ps("\n");ann_report();ann_state();ps("\n=== Demo Complete ===\n");return 0;}
