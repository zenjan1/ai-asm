/* viola_admin: Viola management technology administration (v1.0)
 * Viola planning, viola execution, viola evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vla_t;
typedef struct{int n_vlp,n_vle,n_vlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vla_state_t;
static vla_t vlaps[N],vlass[N-2],vlavss[N-4],vlaas[N-6],vlamks[N-6]; static vla_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vla_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vla_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VLA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vla_init(void){if(init)return -1;st.n_vlp=0;st.n_vle=0;st.n_vlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vlaps[i].active=0;for(int i=0;i<N-2;i++)vlass[i].active=0;for(int i=0;i<N-4;i++)vlavss[i].active=0;for(int i=0;i<N-6;i++)vlaas[i].active=0;for(int i=0;i<N-6;i++)vlamks[i].active=0;init=1;ps("[VLA] Viola initialized\n");return 0;}
int vla_planning(int t,int c,int a,int b,int d,int e,int y){return add(vlaps,&st.n_vlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vla_execution(int t,int c,int a,int b,int d,int e,int y){return add(vlass,&st.n_vle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vla_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vlavss,&st.n_vlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vla_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vlaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vla_market(int t,int c,int a,int b,int d,int e,int y){return add(vlamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vla_report(void){ps("[VLA] Vlp: ");pi(st.n_vlp);ps(" PCS=");pi(st.t_f1);ps("\nVle: ");pi(st.n_vle);ps(" PCS=");pi(st.t_f2);ps("\nVlv: ");pi(st.n_vlv);ps(" PCS=");pi(st.t_f3);ps("\nVlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vla_state(void){ps("[VLA] Vlp=");pi(st.n_vlp);ps(" Vle=");pi(st.n_vle);ps(" Vlv=");pi(st.n_vlv);ps(" Vlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Viola Admin Demo ===\n\n");vla_init();
ps("Viola planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vla_planning(t,c,877+(i*17),866+(i*14),846+(i*10),828+(i*6),2020+(i%5));}
ps("\nViola execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vla_execution(t,c,866+(i*15),855+(i*12),837+(i*8),824+(i*5),2021+(i%4));}
ps("\nViola evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vla_evaluation(t,c,858+(i*13),847+(i*10),831+(i*7),820+(i*4),2022+(i%3));}
ps("\nViola accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vla_accessory(t,c,850+(i*11),841+(i*9),827+(i*6),817+(i*3),2023+(i%2));}
ps("\nViola marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vla_market(t,c,844+(i*9),835+(i*7),822+(i*5),814+(i*3),2024);}
ps("\n");vla_report();vla_state();ps("\n=== Demo Complete ===\n");return 0;}
