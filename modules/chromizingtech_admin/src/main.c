/* chromizingtech_admin: Chromizing technology administration (v1.0)
 * Solid chromizing, liquid chromizing, gas chromizing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} crm_t;
typedef struct{int n_so,n_lq,n_gs,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} crm_state_t;
static crm_t sos[N],lqs[N-2],gss[N-4],acs[N-6],mks[N-6]; static crm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;crm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CRM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int crm_init(void){if(init)return -1;st.n_so=0;st.n_lq=0;st.n_gs=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sos[i].active=0;for(int i=0;i<N-2;i++)lqs[i].active=0;for(int i=0;i<N-4;i++)gss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CRM] Chromizingtech initialized\n");return 0;}
int crm_solid(int t,int c,int a,int b,int d,int e,int y){return add(sos,&st.n_so,&st.t_f1,N,t,c,a,b,d,e,y);}
int crm_liquid(int t,int c,int a,int b,int d,int e,int y){return add(lqs,&st.n_lq,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int crm_gas(int t,int c,int a,int b,int d,int e,int y){return add(gss,&st.n_gs,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int crm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int crm_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void crm_report(void){ps("[CRM] So: ");pi(st.n_so);ps(" PCS=");pi(st.t_f1);ps("\nLq: ");pi(st.n_lq);ps(" PCS=");pi(st.t_f2);ps("\nGs: ");pi(st.n_gs);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void crm_state(void){ps("[CRM] So=");pi(st.n_so);ps(" Lq=");pi(st.n_lq);ps(" Gs=");pi(st.n_gs);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chromizing Tech Admin Demo ===\n\n");crm_init();
ps("Solid chromizing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;crm_solid(t,c,332+(i*17),317+(i*14),297+(i*10),279+(i*6),2020+(i%5));}
ps("\nLiquid chromizing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;crm_liquid(t,c,321+(i*15),307+(i*12),289+(i*8),276+(i*5),2021+(i%4));}
ps("\nGas chromizing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;crm_gas(t,c,313+(i*13),299+(i*10),283+(i*7),272+(i*4),2022+(i%3));}
ps("\nChromizing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crm_accessory(t,c,305+(i*11),293+(i*9),279+(i*6),269+(i*3),2023+(i%2));}
ps("\nChromizing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crm_market(t,c,299+(i*9),288+(i*7),275+(i*5),267+(i*3),2024);}
ps("\n");crm_report();crm_state();ps("\n=== Demo Complete ===\n");return 0;}
