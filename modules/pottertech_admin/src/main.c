/* pottertech_admin: Potter technology administration (v1.0)
 * Ceramic raw materials, ceramic forming, ceramic firing, ceramic products, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ptt_t;
typedef struct{int n_rm,n_fm,n_fr,n_pp,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ptt_state_t;
static ptt_t rms[N],fms[N-2],frs[N-4],pps[N-6],mks[N-6]; static ptt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ptt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ptt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PTT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ptt_init(void){if(init)return -1;st.n_rm=0;st.n_fm=0;st.n_fr=0;st.n_pp=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rms[i].active=0;for(int i=0;i<N-2;i++)fms[i].active=0;for(int i=0;i<N-4;i++)frs[i].active=0;for(int i=0;i<N-6;i++)pps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PTT] Pottertech initialized\n");return 0;}
int ptt_material(int t,int c,int a,int b,int d,int e,int y){return add(rms,&st.n_rm,&st.t_f1,N,t,c,a,b,d,e,y);}
int ptt_form(int t,int c,int a,int b,int d,int e,int y){return add(fms,&st.n_fm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ptt_fire(int t,int c,int a,int b,int d,int e,int y){return add(frs,&st.n_fr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ptt_product(int t,int c,int a,int b,int d,int e,int y){return add(pps,&st.n_pp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ptt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ptt_report(void){ps("[PTT] Material: ");pi(st.n_rm);ps(" Ton=");pi(st.t_f1);ps("\nForm: ");pi(st.n_fm);ps(" PCS=");pi(st.t_f2);ps("\nFire: ");pi(st.n_fr);ps(" PCS=");pi(st.t_f3);ps("\nProd: ");pi(st.n_pp);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ptt_state(void){ps("[PTT] Rm=");pi(st.n_rm);ps(" Fm=");pi(st.n_fm);ps(" Fr=");pi(st.n_fr);ps(" Pp=");pi(st.n_pp);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Potter Tech Admin Demo ===\n\n");ptt_init();
ps("Ceramic materials...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ptt_material(t,c,210+(i*17),195+(i*14),175+(i*10),157+(i*6),2020+(i%5));}
ps("\nCeramic forming...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ptt_form(t,c,199+(i*15),185+(i*12),167+(i*8),154+(i*5),2021+(i%4));}
ps("\nCeramic firing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ptt_fire(t,c,191+(i*13),177+(i*10),161+(i*7),150+(i*4),2022+(i%3));}
ps("\nCeramic products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ptt_product(t,c,183+(i*11),171+(i*9),157+(i*6),147+(i*3),2023+(i%2));}
ps("\nPotter marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ptt_market(t,c,177+(i*9),166+(i*7),153+(i*5),145+(i*3),2024);}
ps("\n");ptt_report();ptt_state();ps("\n=== Demo Complete ===\n");return 0;}
