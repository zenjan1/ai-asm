/* tilia_admin: Tilia management technology administration (v1.0)
 * Tilia planning, tilia execution, tilia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} til_t;
typedef struct{int n_tlp,n_tle,n_tlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} til_state_t;
static til_t tilps[N],tilss[N-2],tilvss[N-4],tilas[N-6],tilmks[N-6]; static til_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(til_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;til_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TIL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int til_init(void){if(init)return -1;st.n_tlp=0;st.n_tle=0;st.n_tlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tilps[i].active=0;for(int i=0;i<N-2;i++)tilss[i].active=0;for(int i=0;i<N-4;i++)tilvss[i].active=0;for(int i=0;i<N-6;i++)tilas[i].active=0;for(int i=0;i<N-6;i++)tilmks[i].active=0;init=1;ps("[TIL] Tilia initialized\n");return 0;}
int til_planning(int t,int c,int a,int b,int d,int e,int y){return add(tilps,&st.n_tlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int til_execution(int t,int c,int a,int b,int d,int e,int y){return add(tilss,&st.n_tle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int til_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tilvss,&st.n_tlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int til_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tilas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int til_market(int t,int c,int a,int b,int d,int e,int y){return add(tilmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void til_report(void){ps("[TIL] Tlp: ");pi(st.n_tlp);ps(" PCS=");pi(st.t_f1);ps("\nTle: ");pi(st.n_tle);ps(" PCS=");pi(st.t_f2);ps("\nTlv: ");pi(st.n_tlv);ps(" PCS=");pi(st.t_f3);ps("\nTlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void til_state(void){ps("[TIL] Tlp=");pi(st.n_tlp);ps(" Tle=");pi(st.n_tle);ps(" Tlv=");pi(st.n_tlv);ps(" Tlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tilia Admin Demo ===\n\n");til_init();
ps("Tilia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;til_planning(t,c,867+(i*17),856+(i*14),836+(i*10),818+(i*6),2020+(i%5));}
ps("\nTilia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;til_execution(t,c,856+(i*15),845+(i*12),827+(i*8),814+(i*5),2021+(i%4));}
ps("\nTilia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;til_evaluation(t,c,848+(i*13),837+(i*10),821+(i*7),810+(i*4),2022+(i%3));}
ps("\nTilia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;til_accessory(t,c,840+(i*11),831+(i*9),817+(i*6),807+(i*3),2023+(i%2));}
ps("\nTilia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;til_market(t,c,834+(i*9),825+(i*7),812+(i*5),804+(i*3),2024);}
ps("\n");til_report();til_state();ps("\n=== Demo Complete ===\n");return 0;}
