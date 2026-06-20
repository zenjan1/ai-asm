/* polish_admin: Polishing technology administration (v1.0)
 * Mechanical polishing, chemical polishing, grinding/polishing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} psh_t;
typedef struct{int n_mc,n_ch,n_gr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} psh_state_t;
static psh_t mcs[N],chs[N-2],grs[N-4],acs[N-6],mks[N-6]; static psh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(psh_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;psh_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PSH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int psh_init(void){if(init)return -1;st.n_mc=0;st.n_ch=0;st.n_gr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcs[i].active=0;for(int i=0;i<N-2;i++)chs[i].active=0;for(int i=0;i<N-4;i++)grs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PSH] Polish initialized\n");return 0;}
int psh_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f1,N,t,c,a,b,d,e,y);}
int psh_chemical(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int psh_grinding(int t,int c,int a,int b,int d,int e,int y){return add(grs,&st.n_gr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int psh_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int psh_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void psh_report(void){ps("[PSH] Mc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f1);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f2);ps("\nGr: ");pi(st.n_gr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void psh_state(void){ps("[PSH] Mc=");pi(st.n_mc);ps(" Ch=");pi(st.n_ch);ps(" Gr=");pi(st.n_gr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Polish Admin Demo ===\n\n");psh_init();
ps("Mechanical polishing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;psh_mechanical(t,c,412+(i*17),401+(i*14),381+(i*10),363+(i*6),2020+(i%5));}
ps("\nChemical polishing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;psh_chemical(t,c,401+(i*15),390+(i*12),372+(i*8),359+(i*5),2021+(i%4));}
ps("\nGrinding/polishing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;psh_grinding(t,c,393+(i*13),382+(i*10),366+(i*7),355+(i*4),2022+(i%3));}
ps("\nPolishing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;psh_accessory(t,c,385+(i*11),376+(i*9),362+(i*6),352+(i*3),2023+(i%2));}
ps("\nPolishing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;psh_market(t,c,379+(i*9),370+(i*7),357+(i*5),349+(i*3),2024);}
ps("\n");psh_report();psh_state();ps("\n=== Demo Complete ===\n");return 0;}
