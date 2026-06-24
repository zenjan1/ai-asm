/* musa_admin: Musa banana cultivation management (v1.0)
 * Banana fruit tracking, sucker propagation, ripening conversion, disease resistance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mus_t;
typedef struct{int n_musc,n_muse,n_mu2,n_ms,n_fr,t_f1,t_f2,t_f3,t_f4,t_f5;} mus_state_t;
static mus_t muscs[N],muses[N-2],mu2s[N-4],mss[N-6],frs[N-6]; static mus_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mus_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mus_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MUS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mus_init(void){if(init)return -1;st.n_musc=0;st.n_muse=0;st.n_mu2=0;st.n_ms=0;st.n_fr=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)muscs[i].active=0;for(int i=0;i<N-2;i++)muses[i].active=0;for(int i=0;i<N-4;i++)mu2s[i].active=0;for(int i=0;i<N-6;i++)mss[i].active=0;for(int i=0;i<N-6;i++)frs[i].active=0;init=1;ps("[MUS] Musa initialized\n");return 0;}
int mus_cultivar(int t,int c,int a,int b,int d,int e,int y){return add(muscs,&st.n_musc,&st.t_f1,N,t,c,a,b,d,e,y);}
int mus_bunch(int t,int c,int a,int b,int d,int e,int y){return add(muses,&st.n_muse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mus_sucker(int t,int c,int a,int b,int d,int e,int y){return add(mu2s,&st.n_mu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mus_resistance(int t,int c,int a,int b,int d,int e,int y){return add(mss,&st.n_ms,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mus_ripening(int t,int c,int a,int b,int d,int e,int y){return add(frs,&st.n_fr,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mus_report(void){ps("[MUS] Cultivar: ");pi(st.n_musc);ps(" PCS=");pi(st.t_f1);ps("\nBunch: ");pi(st.n_muse);ps(" PCS=");pi(st.t_f2);ps("\nSucker: ");pi(st.n_mu2);ps(" PCS=");pi(st.t_f3);ps("\nResistance: ");pi(st.n_ms);ps(" PCS=");pi(st.t_f4);ps("\nRipening: ");pi(st.n_fr);ps(" USD=");pi(st.t_f5);ps("\n");}
void mus_state(void){ps("[MUS] Cul=");pi(st.n_musc);ps(" Bun=");pi(st.n_muse);ps(" Suk=");pi(st.n_mu2);ps(" Res=");pi(st.n_ms);ps(" Rip=");pi(st.n_fr);ps("\n");}
int main(void){
ps("=== Musa Admin Demo ===\n\n");mus_init();
ps("Banana cultivar registration...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mus_cultivar(t,c,913+(i*17),902+(i*14),882+(i*10),864+(i*6),2020+(i%5));}
ps("\nFruit bunch tracking...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mus_bunch(t,c,902+(i*15),891+(i*12),873+(i*8),860+(i*5),2021+(i%4));}
ps("\nSucker propagation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mus_sucker(t,c,894+(i*13),883+(i*10),867+(i*7),856+(i*4),2022+(i%3));}
ps("\nDisease resistance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mus_resistance(t,c,886+(i*11),877+(i*9),863+(i*6),853+(i*3),2023+(i%2));}
ps("\nRipening conversion...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mus_ripening(t,c,880+(i*9),871+(i*7),858+(i*5),850+(i*3),2024);}
ps("\n");mus_report();mus_state();ps("\n=== Demo Complete ===\n");return 0;}
