/* kochia_admin: Kochia management technology administration (v1.0)
 * Kochia planning, kochia execution, kochia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} koc_t;
typedef struct{int n_kocp,n_koce,n_koc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} koc_state_t;
static koc_t kocps[N],kocss[N-2],kocvss[N-4],kocas[N-6],kocmks[N-6]; static koc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(koc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;koc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KOC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int koc_init(void){if(init)return -1;st.n_kocp=0;st.n_koce=0;st.n_koc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kocps[i].active=0;for(int i=0;i<N-2;i++)kocss[i].active=0;for(int i=0;i<N-4;i++)kocvss[i].active=0;for(int i=0;i<N-6;i++)kocas[i].active=0;for(int i=0;i<N-6;i++)kocmks[i].active=0;init=1;ps("[KOC] Kochia initialized\n");return 0;}
int koc_planning(int t,int c,int a,int b,int d,int e,int y){return add(kocps,&st.n_kocp,&st.t_f1,N,t,c,a,b,d,e,y);}
int koc_execution(int t,int c,int a,int b,int d,int e,int y){return add(kocss,&st.n_koce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int koc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(kocvss,&st.n_koc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int koc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kocas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int koc_market(int t,int c,int a,int b,int d,int e,int y){return add(kocmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void koc_report(void){ps("[KOC] Kocp: ");pi(st.n_kocp);ps(" PCS=");pi(st.t_f1);ps("\nKoce: ");pi(st.n_koce);ps(" PCS=");pi(st.t_f2);ps("\nKocv: ");pi(st.n_koc2);ps(" PCS=");pi(st.t_f3);ps("\nKocc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void koc_state(void){ps("[KOC] Kocp=");pi(st.n_kocp);ps(" Koce=");pi(st.n_koce);ps(" Kocv=");pi(st.n_koc2);ps(" Kocc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kochia Admin Demo ===\n\n");koc_init();
ps("Kochia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;koc_planning(t,c,932+(i*17),921+(i*14),901+(i*10),883+(i*6),2020+(i%5));}
ps("\nKochia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;koc_execution(t,c,921+(i*15),910+(i*12),892+(i*8),879+(i*5),2021+(i%4));}
ps("\nKochia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;koc_evaluation(t,c,913+(i*13),902+(i*10),886+(i*7),875+(i*4),2022+(i%3));}
ps("\nKochia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;koc_accessory(t,c,905+(i*11),896+(i*9),882+(i*6),872+(i*3),2023+(i%2));}
ps("\nKochia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;koc_market(t,c,899+(i*9),890+(i*7),877+(i*5),869+(i*3),2024);}
ps("\n");koc_report();koc_state();ps("\n=== Demo Complete ===\n");return 0;}
