/* flange_admin: Flange technology administration (v1.0)
 * Plate welding flange, butt-weld flange, loose flange, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} flg_t;
typedef struct{int n_pl,n_bt,n_ls,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} flg_state_t;
static flg_t pls[N],bts[N-2],lss[N-4],acs[N-6],mks[N-6]; static flg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(flg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;flg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FLG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int flg_init(void){if(init)return -1;st.n_pl=0;st.n_bt=0;st.n_ls=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pls[i].active=0;for(int i=0;i<N-2;i++)bts[i].active=0;for(int i=0;i<N-4;i++)lss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FLG] Flange initialized\n");return 0;}
int flg_plate(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f1,N,t,c,a,b,d,e,y);}
int flg_butt(int t,int c,int a,int b,int d,int e,int y){return add(bts,&st.n_bt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int flg_loose(int t,int c,int a,int b,int d,int e,int y){return add(lss,&st.n_ls,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int flg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int flg_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void flg_report(void){ps("[FLG] Pl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f1);ps("\nBt: ");pi(st.n_bt);ps(" PCS=");pi(st.t_f2);ps("\nLs: ");pi(st.n_ls);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void flg_state(void){ps("[FLG] Pl=");pi(st.n_pl);ps(" Bt=");pi(st.n_bt);ps(" Ls=");pi(st.n_ls);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Flange Admin Demo ===\n\n");flg_init();
ps("Plate welding flanges...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;flg_plate(t,c,358+(i*17),343+(i*14),323+(i*10),305+(i*6),2020+(i%5));}
ps("\nButt-weld flanges...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;flg_butt(t,c,347+(i*15),333+(i*12),315+(i*8),302+(i*5),2021+(i%4));}
ps("\nLoose flanges...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;flg_loose(t,c,339+(i*13),325+(i*10),309+(i*7),298+(i*4),2022+(i%3));}
ps("\nFlange accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flg_accessory(t,c,331+(i*11),319+(i*9),305+(i*6),295+(i*3),2023+(i%2));}
ps("\nFlange marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flg_market(t,c,325+(i*9),314+(i*7),301+(i*5),293+(i*3),2024);}
ps("\n");flg_report();flg_state();ps("\n=== Demo Complete ===\n");return 0;}
