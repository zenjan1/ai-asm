/* tubetech_admin: Tube fitting technology administration (v1.0)
 * Elbow fitting, tee fitting, flange fitting, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tbt_t;
typedef struct{int n_el,n_te,n_fl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tbt_state_t;
static tbt_t els[N],tes[N-2],fls[N-4],acs[N-6],mks[N-6]; static tbt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tbt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tbt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TBT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tbt_init(void){if(init)return -1;st.n_el=0;st.n_te=0;st.n_fl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)els[i].active=0;for(int i=0;i<N-2;i++)tes[i].active=0;for(int i=0;i<N-4;i++)fls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TBT] Tubetech initialized\n");return 0;}
int tbt_elbow(int t,int c,int a,int b,int d,int e,int y){return add(els,&st.n_el,&st.t_f1,N,t,c,a,b,d,e,y);}
int tbt_tee(int t,int c,int a,int b,int d,int e,int y){return add(tes,&st.n_te,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tbt_flange(int t,int c,int a,int b,int d,int e,int y){return add(fls,&st.n_fl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tbt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tbt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tbt_report(void){ps("[TBT] El: ");pi(st.n_el);ps(" PCS=");pi(st.t_f1);ps("\nTe: ");pi(st.n_te);ps(" PCS=");pi(st.t_f2);ps("\nFl: ");pi(st.n_fl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tbt_state(void){ps("[TBT] El=");pi(st.n_el);ps(" Te=");pi(st.n_te);ps(" Fl=");pi(st.n_fl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tube Tech Admin Demo ===\n\n");tbt_init();
ps("Elbow fittings...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tbt_elbow(t,c,365+(i*17),350+(i*14),330+(i*10),312+(i*6),2020+(i%5));}
ps("\nTee fittings...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tbt_tee(t,c,354+(i*15),340+(i*12),322+(i*8),309+(i*5),2021+(i%4));}
ps("\nFlange fittings...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tbt_flange(t,c,346+(i*13),332+(i*10),316+(i*7),305+(i*4),2022+(i%3));}
ps("\nTube accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tbt_accessory(t,c,338+(i*11),326+(i*9),312+(i*6),302+(i*3),2023+(i%2));}
ps("\nTube marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tbt_market(t,c,332+(i*9),321+(i*7),308+(i*5),300+(i*3),2024);}
ps("\n");tbt_report();tbt_state();ps("\n=== Demo Complete ===\n");return 0;}
