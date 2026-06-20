/* cuttingtech_admin: Cutting technology administration (v1.0)
 * Turning cutting, milling cutting, drilling/boring, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cut_t;
typedef struct{int n_tn,n_ml,n_dr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cut_state_t;
static cut_t tns[N],mls[N-2],drs[N-4],acs[N-6],mks[N-6]; static cut_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cut_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cut_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CUT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cut_init(void){if(init)return -1;st.n_tn=0;st.n_ml=0;st.n_dr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tns[i].active=0;for(int i=0;i<N-2;i++)mls[i].active=0;for(int i=0;i<N-4;i++)drs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CUT] Cuttingtech initialized\n");return 0;}
int cut_turning(int t,int c,int a,int b,int d,int e,int y){return add(tns,&st.n_tn,&st.t_f1,N,t,c,a,b,d,e,y);}
int cut_milling(int t,int c,int a,int b,int d,int e,int y){return add(mls,&st.n_ml,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cut_drilling(int t,int c,int a,int b,int d,int e,int y){return add(drs,&st.n_dr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cut_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cut_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cut_report(void){ps("[CUT] Tn: ");pi(st.n_tn);ps(" PCS=");pi(st.t_f1);ps("\nMl: ");pi(st.n_ml);ps(" PCS=");pi(st.t_f2);ps("\nDr: ");pi(st.n_dr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cut_state(void){ps("[CUT] Tn=");pi(st.n_tn);ps(" Ml=");pi(st.n_ml);ps(" Dr=");pi(st.n_dr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cutting Tech Admin Demo ===\n\n");cut_init();
ps("Turning cutting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cut_turning(t,c,301+(i*17),286+(i*14),266+(i*10),248+(i*6),2020+(i%5));}
ps("\nMilling cutting...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cut_milling(t,c,290+(i*15),276+(i*12),258+(i*8),245+(i*5),2021+(i%4));}
ps("\nDrilling/boring...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cut_drilling(t,c,282+(i*13),268+(i*10),252+(i*7),241+(i*4),2022+(i%3));}
ps("\nCutting accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cut_accessory(t,c,274+(i*11),262+(i*9),248+(i*6),238+(i*3),2023+(i%2));}
ps("\nCutting marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cut_market(t,c,268+(i*9),257+(i*7),244+(i*5),236+(i*3),2024);}
ps("\n");cut_report();cut_state();ps("\n=== Demo Complete ===\n");return 0;}
