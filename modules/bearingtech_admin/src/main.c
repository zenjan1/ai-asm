/* bearingtech_admin: Bearing technology administration (v1.0)
 * Rolling bearings, sliding bearings, special bearings, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} brg_t;
typedef struct{int n_rl,n_sl,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} brg_state_t;
static brg_t rls[N],sls[N-2],sps[N-4],acs[N-6],mks[N-6]; static brg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(brg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;brg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BRG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int brg_init(void){if(init)return -1;st.n_rl=0;st.n_sl=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rls[i].active=0;for(int i=0;i<N-2;i++)sls[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BRG] Bearingtech initialized\n");return 0;}
int brg_rolling(int t,int c,int a,int b,int d,int e,int y){return add(rls,&st.n_rl,&st.t_f1,N,t,c,a,b,d,e,y);}
int brg_sliding(int t,int c,int a,int b,int d,int e,int y){return add(sls,&st.n_sl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int brg_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int brg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int brg_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void brg_report(void){ps("[BRG] Rl: ");pi(st.n_rl);ps(" PCS=");pi(st.t_f1);ps("\nSl: ");pi(st.n_sl);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void brg_state(void){ps("[BRG] Rl=");pi(st.n_rl);ps(" Sl=");pi(st.n_sl);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bearing Tech Admin Demo ===\n\n");brg_init();
ps("Rolling bearings...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;brg_rolling(t,c,261+(i*17),246+(i*14),226+(i*10),208+(i*6),2020+(i%5));}
ps("\nSliding bearings...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;brg_sliding(t,c,250+(i*15),236+(i*12),218+(i*8),205+(i*5),2021+(i%4));}
ps("\nSpecial bearings...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;brg_special(t,c,242+(i*13),228+(i*10),212+(i*7),201+(i*4),2022+(i%3));}
ps("\nBearing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brg_accessory(t,c,234+(i*11),222+(i*9),208+(i*6),198+(i*3),2023+(i%2));}
ps("\nBearing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brg_market(t,c,228+(i*9),217+(i*7),204+(i*5),196+(i*3),2024);}
ps("\n");brg_report();brg_state();ps("\n=== Demo Complete ===\n");return 0;}
