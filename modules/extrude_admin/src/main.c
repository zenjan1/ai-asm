/* extrude_admin: Extrusion technology administration (v1.0)
 * Direct extrusion, indirect extrusion, special extrusion, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ext_t;
typedef struct{int n_dr,n_in,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ext_state_t;
static ext_t drs[N],ins[N-2],sps[N-4],acs[N-6],mks[N-6]; static ext_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ext_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ext_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EXT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ext_init(void){if(init)return -1;st.n_dr=0;st.n_in=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)drs[i].active=0;for(int i=0;i<N-2;i++)ins[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[EXT] Extrude initialized\n");return 0;}
int ext_direct(int t,int c,int a,int b,int d,int e,int y){return add(drs,&st.n_dr,&st.t_f1,N,t,c,a,b,d,e,y);}
int ext_indirect(int t,int c,int a,int b,int d,int e,int y){return add(ins,&st.n_in,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ext_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ext_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ext_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ext_report(void){ps("[EXT] Dr: ");pi(st.n_dr);ps(" PCS=");pi(st.t_f1);ps("\nIn: ");pi(st.n_in);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ext_state(void){ps("[EXT] Dr=");pi(st.n_dr);ps(" In=");pi(st.n_in);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Extrude Admin Demo ===\n\n");ext_init();
ps("Direct extrusion...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ext_direct(t,c,404+(i*17),389+(i*14),369+(i*10),351+(i*6),2020+(i%5));}
ps("\nIndirect extrusion...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ext_indirect(t,c,393+(i*15),379+(i*12),361+(i*8),348+(i*5),2021+(i%4));}
ps("\nSpecial extrusion...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ext_special(t,c,385+(i*13),371+(i*10),355+(i*7),344+(i*4),2022+(i%3));}
ps("\nExtrusion accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ext_accessory(t,c,377+(i*11),365+(i*9),351+(i*6),341+(i*3),2023+(i%2));}
ps("\nExtrusion marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ext_market(t,c,371+(i*9),360+(i*7),347+(i*5),339+(i*3),2024);}
ps("\n");ext_report();ext_state();ps("\n=== Demo Complete ===\n");return 0;}
