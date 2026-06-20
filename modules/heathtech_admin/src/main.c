/* heathtech_admin: Heat treatment technology administration (v1.0)
 * Annealing, quenching, tempering, chemical heat treatment, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hth_t;
typedef struct{int n_an,n_qu,n_tm,n_ch,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hth_state_t;
static hth_t ans[N],qus[N-2],tms[N-4],chs[N-6],mks[N-6]; static hth_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hth_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hth_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HTH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hth_init(void){if(init)return -1;st.n_an=0;st.n_qu=0;st.n_tm=0;st.n_ch=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ans[i].active=0;for(int i=0;i<N-2;i++)qus[i].active=0;for(int i=0;i<N-4;i++)tms[i].active=0;for(int i=0;i<N-6;i++)chs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HTH] Heathtech initialized\n");return 0;}
int hth_annealing(int t,int c,int a,int b,int d,int e,int y){return add(ans,&st.n_an,&st.t_f1,N,t,c,a,b,d,e,y);}
int hth_quenching(int t,int c,int a,int b,int d,int e,int y){return add(qus,&st.n_qu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hth_tempering(int t,int c,int a,int b,int d,int e,int y){return add(tms,&st.n_tm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hth_chemical(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hth_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hth_report(void){ps("[HTH] An: ");pi(st.n_an);ps(" PCS=");pi(st.t_f1);ps("\nQu: ");pi(st.n_qu);ps(" PCS=");pi(st.t_f2);ps("\nTm: ");pi(st.n_tm);ps(" PCS=");pi(st.t_f3);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hth_state(void){ps("[HTH] An=");pi(st.n_an);ps(" Qu=");pi(st.n_qu);ps(" Tm=");pi(st.n_tm);ps(" Ch=");pi(st.n_ch);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Heat Treatment Tech Admin Demo ===\n\n");hth_init();
ps("Annealing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hth_annealing(t,c,248+(i*17),233+(i*14),213+(i*10),195+(i*6),2020+(i%5));}
ps("\nQuenching...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hth_quenching(t,c,237+(i*15),223+(i*12),205+(i*8),192+(i*5),2021+(i%4));}
ps("\nTempering...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hth_tempering(t,c,229+(i*13),215+(i*10),199+(i*7),188+(i*4),2022+(i%3));}
ps("\nChemical heat treatment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hth_chemical(t,c,221+(i*11),209+(i*9),195+(i*6),185+(i*3),2023+(i%2));}
ps("\nHeat treatment marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hth_market(t,c,215+(i*9),204+(i*7),191+(i*5),183+(i*3),2024);}
ps("\n");hth_report();hth_state();ps("\n=== Demo Complete ===\n");return 0;}
