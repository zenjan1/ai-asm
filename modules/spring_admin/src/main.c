/* spring_admin: Spring technology administration (v1.0)
 * Compression spring, extension spring, special spring, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spr_t;
typedef struct{int n_cm,n_ex,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spr_state_t;
static spr_t cms[N],exs[N-2],sps[N-4],acs[N-6],mks[N-6]; static spr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spr_init(void){if(init)return -1;st.n_cm=0;st.n_ex=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cms[i].active=0;for(int i=0;i<N-2;i++)exs[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SPR] Spring initialized\n");return 0;}
int spr_compression(int t,int c,int a,int b,int d,int e,int y){return add(cms,&st.n_cm,&st.t_f1,N,t,c,a,b,d,e,y);}
int spr_extension(int t,int c,int a,int b,int d,int e,int y){return add(exs,&st.n_ex,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spr_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spr_report(void){ps("[SPR] Cm: ");pi(st.n_cm);ps(" PCS=");pi(st.t_f1);ps("\nEx: ");pi(st.n_ex);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void spr_state(void){ps("[SPR] Cm=");pi(st.n_cm);ps(" Ex=");pi(st.n_ex);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Spring Admin Demo ===\n\n");spr_init();
ps("Compression springs...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spr_compression(t,c,383+(i*17),368+(i*14),348+(i*10),330+(i*6),2020+(i%5));}
ps("\nExtension springs...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spr_extension(t,c,372+(i*15),358+(i*12),340+(i*8),327+(i*5),2021+(i%4));}
ps("\nSpecial springs...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spr_special(t,c,364+(i*13),350+(i*10),334+(i*7),323+(i*4),2022+(i%3));}
ps("\nSpring accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spr_accessory(t,c,356+(i*11),344+(i*9),330+(i*6),320+(i*3),2023+(i%2));}
ps("\nSpring marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spr_market(t,c,350+(i*9),339+(i*7),326+(i*5),318+(i*3),2024);}
ps("\n");spr_report();spr_state();ps("\n=== Demo Complete ===\n");return 0;}
