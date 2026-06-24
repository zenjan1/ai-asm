/* melaleuca_admin: Melaleuca tea tree & paperbark management (v1.0)
 * Melaleuca cultivation, essential oil extraction, bark harvesting, wetland restoration
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mel_t;
typedef struct{int n_melc,n_mele,n_mel2,n_ml,n_hy,t_f1,t_f2,t_f3,t_f4,t_f5;} mel_state_t;
static mel_t melcs[N],meles[N-2],mel2s[N-4],mls[N-6],hys[N-6]; static mel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mel_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mel_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MEL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mel_init(void){if(init)return -1;st.n_melc=0;st.n_mele=0;st.n_mel2=0;st.n_ml=0;st.n_hy=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)melcs[i].active=0;for(int i=0;i<N-2;i++)meles[i].active=0;for(int i=0;i<N-4;i++)mel2s[i].active=0;for(int i=0;i<N-6;i++)mls[i].active=0;for(int i=0;i<N-6;i++)hys[i].active=0;init=1;ps("[MEL] Melaleuca initialized\n");return 0;}
int mel_cultivation(int t,int c,int a,int b,int d,int e,int y){return add(melcs,&st.n_melc,&st.t_f1,N,t,c,a,b,d,e,y);}
int mel_extraction(int t,int c,int a,int b,int d,int e,int y){return add(meles,&st.n_mele,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mel_bark(int t,int c,int a,int b,int d,int e,int y){return add(mel2s,&st.n_mel2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mel_wetland(int t,int c,int a,int b,int d,int e,int y){return add(mls,&st.n_ml,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mel_honey(int t,int c,int a,int b,int d,int e,int y){return add(hys,&st.n_hy,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mel_report(void){ps("[MEL] Cultivation: ");pi(st.n_melc);ps(" PCS=");pi(st.t_f1);ps("\nExtraction: ");pi(st.n_mele);ps(" PCS=");pi(st.t_f2);ps("\nBark: ");pi(st.n_mel2);ps(" PCS=");pi(st.t_f3);ps("\nWetland: ");pi(st.n_ml);ps(" PCS=");pi(st.t_f4);ps("\nHoney: ");pi(st.n_hy);ps(" USD=");pi(st.t_f5);ps("\n");}
void mel_state(void){ps("[MEL] Cult=");pi(st.n_melc);ps(" Extr=");pi(st.n_mele);ps(" Bark=");pi(st.n_mel2);ps(" Wet=");pi(st.n_ml);ps(" Hon=");pi(st.n_hy);ps("\n");}
int main(void){
ps("=== Melaleuca Admin Demo ===\n\n");mel_init();
ps("Melaleuca cultivation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mel_cultivation(t,c,901+(i*17),890+(i*14),870+(i*10),852+(i*6),2020+(i%5));}
ps("\nEssential oil extraction...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mel_extraction(t,c,890+(i*15),879+(i*12),861+(i*8),848+(i*5),2021+(i%4));}
ps("\nBark harvesting...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mel_bark(t,c,882+(i*13),871+(i*10),855+(i*7),844+(i*4),2022+(i%3));}
ps("\nWetland restoration...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mel_wetland(t,c,874+(i*11),865+(i*9),851+(i*6),841+(i*3),2023+(i%2));}
ps("\nHoney production...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mel_honey(t,c,868+(i*9),859+(i*7),846+(i*5),838+(i*3),2024);}
ps("\n");mel_report();mel_state();ps("\n=== Demo Complete ===\n");return 0;}
