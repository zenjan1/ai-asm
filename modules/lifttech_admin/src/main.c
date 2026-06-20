/* lifttech_admin: Lift technology administration (v1.0)
 * Hydraulic lifts, scissor lifts, mast lifts, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lft_t;
typedef struct{int n_hy,n_sc,n_ma,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lft_state_t;
static lft_t hys[N],scs[N-2],mas[N-4],acs[N-6],mks[N-6]; static lft_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lft_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lft_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LFT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lft_init(void){if(init)return -1;st.n_hy=0;st.n_sc=0;st.n_ma=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hys[i].active=0;for(int i=0;i<N-2;i++)scs[i].active=0;for(int i=0;i<N-4;i++)mas[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[LFT] Lifttech initialized\n");return 0;}
int lft_hydraulic(int t,int c,int a,int b,int d,int e,int y){return add(hys,&st.n_hy,&st.t_f1,N,t,c,a,b,d,e,y);}
int lft_scissor(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lft_mast(int t,int c,int a,int b,int d,int e,int y){return add(mas,&st.n_ma,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lft_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lft_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lft_report(void){ps("[LFT] Hy: ");pi(st.n_hy);ps(" PCS=");pi(st.t_f1);ps("\nSc: ");pi(st.n_sc);ps(" PCS=");pi(st.t_f2);ps("\nMa: ");pi(st.n_ma);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lft_state(void){ps("[LFT] Hy=");pi(st.n_hy);ps(" Sc=");pi(st.n_sc);ps(" Ma=");pi(st.n_ma);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lift Tech Admin Demo ===\n\n");lft_init();
ps("Hydraulic lifts...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lft_hydraulic(t,c,256+(i*17),241+(i*14),221+(i*10),203+(i*6),2020+(i%5));}
ps("\nScissor lifts...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lft_scissor(t,c,245+(i*15),231+(i*12),213+(i*8),200+(i*5),2021+(i%4));}
ps("\nMast lifts...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lft_mast(t,c,237+(i*13),223+(i*10),207+(i*7),196+(i*4),2022+(i%3));}
ps("\nLift accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lft_accessory(t,c,229+(i*11),217+(i*9),203+(i*6),193+(i*3),2023+(i%2));}
ps("\nLift marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lft_market(t,c,223+(i*9),212+(i*7),199+(i*5),191+(i*3),2024);}
ps("\n");lft_report();lft_state();ps("\n=== Demo Complete ===\n");return 0;}
