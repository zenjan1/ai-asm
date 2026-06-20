/* extrudingtech_admin: Extrusion technology administration (v1.0)
 * Forward extrusion, backward extrusion, special extrusion, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ext_t;
typedef struct{int n_fw,n_bk,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ext_state_t;
static ext_t fws[N],bks[N-2],sps[N-4],acs[N-6],mks[N-6]; static ext_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ext_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ext_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EXT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ext_init(void){if(init)return -1;st.n_fw=0;st.n_bk=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fws[i].active=0;for(int i=0;i<N-2;i++)bks[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[EXT] Extrudingtech initialized\n");return 0;}
int ext_forward(int t,int c,int a,int b,int d,int e,int y){return add(fws,&st.n_fw,&st.t_f1,N,t,c,a,b,d,e,y);}
int ext_backward(int t,int c,int a,int b,int d,int e,int y){return add(bks,&st.n_bk,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ext_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ext_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ext_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ext_report(void){ps("[EXT] Fw: ");pi(st.n_fw);ps(" PCS=");pi(st.t_f1);ps("\nBk: ");pi(st.n_bk);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ext_state(void){ps("[EXT] Fw=");pi(st.n_fw);ps(" Bk=");pi(st.n_bk);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Extrusion Tech Admin Demo ===\n\n");ext_init();
ps("Forward extrusion...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ext_forward(t,c,278+(i*17),263+(i*14),243+(i*10),225+(i*6),2020+(i%5));}
ps("\nBackward extrusion...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ext_backward(t,c,267+(i*15),253+(i*12),235+(i*8),222+(i*5),2021+(i%4));}
ps("\nSpecial extrusion...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ext_special(t,c,259+(i*13),245+(i*10),229+(i*7),218+(i*4),2022+(i%3));}
ps("\nExtrusion accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ext_accessory(t,c,251+(i*11),239+(i*9),225+(i*6),215+(i*3),2023+(i%2));}
ps("\nExtrusion marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ext_market(t,c,245+(i*9),234+(i*7),221+(i*5),213+(i*3),2024);}
ps("\n");ext_report();ext_state();ps("\n=== Demo Complete ===\n");return 0;}
