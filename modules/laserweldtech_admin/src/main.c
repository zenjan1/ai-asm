/* laserweldtech_admin: Laser welding technology administration (v1.0)
 * Laser welding, laser cutting, laser cladding, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lsr_t;
typedef struct{int n_wd,n_ct,n_cl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lsr_state_t;
static lsr_t wds[N],cts[N-2],cls[N-4],acs[N-6],mks[N-6]; static lsr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lsr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lsr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LSR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lsr_init(void){if(init)return -1;st.n_wd=0;st.n_ct=0;st.n_cl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wds[i].active=0;for(int i=0;i<N-2;i++)cts[i].active=0;for(int i=0;i<N-4;i++)cls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[LSR] Laserweldtech initialized\n");return 0;}
int lsr_weld(int t,int c,int a,int b,int d,int e,int y){return add(wds,&st.n_wd,&st.t_f1,N,t,c,a,b,d,e,y);}
int lsr_cut(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lsr_clad(int t,int c,int a,int b,int d,int e,int y){return add(cls,&st.n_cl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lsr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lsr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lsr_report(void){ps("[LSR] Wd: ");pi(st.n_wd);ps(" PCS=");pi(st.t_f1);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f2);ps("\nCl: ");pi(st.n_cl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lsr_state(void){ps("[LSR] Wd=");pi(st.n_wd);ps(" Ct=");pi(st.n_ct);ps(" Cl=");pi(st.n_cl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Laser Weld Tech Admin Demo ===\n\n");lsr_init();
ps("Laser welding...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lsr_weld(t,c,289+(i*17),274+(i*14),254+(i*10),236+(i*6),2020+(i%5));}
ps("\nLaser cutting...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lsr_cut(t,c,278+(i*15),264+(i*12),246+(i*8),233+(i*5),2021+(i%4));}
ps("\nLaser cladding...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lsr_clad(t,c,270+(i*13),256+(i*10),240+(i*7),229+(i*4),2022+(i%3));}
ps("\nLaser accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lsr_accessory(t,c,262+(i*11),250+(i*9),236+(i*6),226+(i*3),2023+(i%2));}
ps("\nLaser marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lsr_market(t,c,256+(i*9),245+(i*7),232+(i*5),224+(i*3),2024);}
ps("\n");lsr_report();lsr_state();ps("\n=== Demo Complete ===\n");return 0;}
