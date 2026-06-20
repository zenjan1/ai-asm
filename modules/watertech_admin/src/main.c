/* watertech_admin: Water technology administration (v1.0)
 * Water source engineering, water supply, wastewater treatment, water environment, smart water
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wtt_t;
typedef struct{int n_sr,n_sp,n_dr,n_tv,n_sw,t_f1,t_f2,t_f3,t_f4,t_f5;} wtt_state_t;
static wtt_t srs[N],sps[N-2],drs[N-4],tvs[N-6],sws[N-6]; static wtt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wtt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wtt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WTT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wtt_init(void){if(init)return -1;st.n_sr=0;st.n_sp=0;st.n_dr=0;st.n_tv=0;st.n_sw=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)srs[i].active=0;for(int i=0;i<N-2;i++)sps[i].active=0;for(int i=0;i<N-4;i++)drs[i].active=0;for(int i=0;i<N-6;i++)tvs[i].active=0;for(int i=0;i<N-6;i++)sws[i].active=0;init=1;ps("[WTT] Watertech initialized\n");return 0;}
int wtt_source(int t,int c,int a,int b,int d,int e,int y){return add(srs,&st.n_sr,&st.t_f1,N,t,c,a,b,d,e,y);}
int wtt_supply(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wtt_drain(int t,int c,int a,int b,int d,int e,int y){return add(drs,&st.n_dr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wtt_treat(int t,int c,int a,int b,int d,int e,int y){return add(tvs,&st.n_tv,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wtt_smart(int t,int c,int a,int b,int d,int e,int y){return add(sws,&st.n_sw,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wtt_report(void){ps("[WTT] Src: ");pi(st.n_sr);ps(" Dam=");pi(st.t_f1);ps("\nSup: ");pi(st.n_sp);ps(" ppm=");pi(st.t_f2);ps("\nDrn: ");pi(st.n_dr);ps(" BOD=");pi(st.t_f3);ps("\nTreat: ");pi(st.n_tv);ps(" River=");pi(st.t_f4);ps("\nSmart: ");pi(st.n_sw);ps(" IoT=");pi(st.t_f5);ps("\n");}
void wtt_state(void){ps("[WTT] Sr=");pi(st.n_sr);ps(" Sp=");pi(st.n_sp);ps(" Dr=");pi(st.n_dr);ps(" Tv=");pi(st.n_tv);ps(" Sw=");pi(st.n_sw);ps("\n");}
int main(void){
ps("=== Water Tech Admin Demo ===\n\n");wtt_init();
ps("Water source engineering...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wtt_source(t,c,165+(i*17),150+(i*14),130+(i*10),113+(i*6),2020+(i%5));}
ps("\nWater supply...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wtt_supply(t,c,154+(i*15),140+(i*12),122+(i*8),109+(i*5),2021+(i%4));}
ps("\nWastewater treatment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wtt_drain(t,c,146+(i*13),132+(i*10),116+(i*7),105+(i*4),2022+(i%3));}
ps("\nWater environment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wtt_treat(t,c,138+(i*11),126+(i*9),112+(i*6),102+(i*3),2023+(i%2));}
ps("\nSmart water...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wtt_smart(t,c,132+(i*9),121+(i*7),108+(i*5),100+(i*3),2024);}
ps("\n");wtt_report();wtt_state();ps("\n=== Demo Complete ===\n");return 0;}
