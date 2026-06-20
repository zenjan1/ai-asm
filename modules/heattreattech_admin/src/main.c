/* heattreattech_admin: Heat treatment technology administration (v1.0)
 * Annealing, quenching/tempering, chemical treatment, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} htr_t;
typedef struct{int n_an,n_qt,n_ch,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} htr_state_t;
static htr_t ans[N],qts[N-2],chs[N-4],acs[N-6],mks[N-6]; static htr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(htr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;htr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HTR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int htr_init(void){if(init)return -1;st.n_an=0;st.n_qt=0;st.n_ch=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ans[i].active=0;for(int i=0;i<N-2;i++)qts[i].active=0;for(int i=0;i<N-4;i++)chs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HTR] Heattreattech initialized\n");return 0;}
int htr_annealing(int t,int c,int a,int b,int d,int e,int y){return add(ans,&st.n_an,&st.t_f1,N,t,c,a,b,d,e,y);}
int htr_quenching(int t,int c,int a,int b,int d,int e,int y){return add(qts,&st.n_qt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int htr_chemical(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int htr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int htr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void htr_report(void){ps("[HTR] An: ");pi(st.n_an);ps(" PCS=");pi(st.t_f1);ps("\nQt: ");pi(st.n_qt);ps(" PCS=");pi(st.t_f2);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void htr_state(void){ps("[HTR] An=");pi(st.n_an);ps(" Qt=");pi(st.n_qt);ps(" Ch=");pi(st.n_ch);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Heat Treatment Tech Admin Demo ===\n\n");htr_init();
ps("Annealing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;htr_annealing(t,c,270+(i*17),255+(i*14),235+(i*10),217+(i*6),2020+(i%5));}
ps("\nQuenching & tempering...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;htr_quenching(t,c,259+(i*15),245+(i*12),227+(i*8),214+(i*5),2021+(i%4));}
ps("\nChemical treatment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;htr_chemical(t,c,251+(i*13),237+(i*10),221+(i*7),210+(i*4),2022+(i%3));}
ps("\nHeat treatment accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;htr_accessory(t,c,243+(i*11),231+(i*9),217+(i*6),207+(i*3),2023+(i%2));}
ps("\nHeat treatment marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;htr_market(t,c,237+(i*9),226+(i*7),213+(i*5),205+(i*3),2024);}
ps("\n");htr_report();htr_state();ps("\n=== Demo Complete ===\n");return 0;}
