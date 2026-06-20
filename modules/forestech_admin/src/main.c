/* forestech_admin: Forestry technology administration (v1.0)
 * Forest cultivation, forest protection, wood processing, forest chemical, forest management
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} frt_t;
typedef struct{int n_cl,n_pt,n_wd,n_ch,n_mg,t_f1,t_f2,t_f3,t_f4,t_f5;} frt_state_t;
static frt_t cls[N],pts[N-2],wds[N-4],chs[N-6],mgs[N-6]; static frt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(frt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;frt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FRT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int frt_init(void){if(init)return -1;st.n_cl=0;st.n_pt=0;st.n_wd=0;st.n_ch=0;st.n_mg=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cls[i].active=0;for(int i=0;i<N-2;i++)pts[i].active=0;for(int i=0;i<N-4;i++)wds[i].active=0;for(int i=0;i<N-6;i++)chs[i].active=0;for(int i=0;i<N-6;i++)mgs[i].active=0;init=1;ps("[FRT] Forestech initialized\n");return 0;}
int frt_cultivate(int t,int c,int a,int b,int d,int e,int y){return add(cls,&st.n_cl,&st.t_f1,N,t,c,a,b,d,e,y);}
int frt_protect(int t,int c,int a,int b,int d,int e,int y){return add(pts,&st.n_pt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int frt_wood(int t,int c,int a,int b,int d,int e,int y){return add(wds,&st.n_wd,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int frt_chemical(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int frt_manage(int t,int c,int a,int b,int d,int e,int y){return add(mgs,&st.n_mg,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void frt_report(void){ps("[FRT] Cult: ");pi(st.n_cl);ps(" ha=");pi(st.t_f1);ps("\nProt: ");pi(st.n_pt);ps(" Fire=");pi(st.t_f2);ps("\nWood: ");pi(st.n_wd);ps(" m3=");pi(st.t_f3);ps("\nChem: ");pi(st.n_ch);ps(" Ton=");pi(st.t_f4);ps("\nMgmt: ");pi(st.n_mg);ps(" CO2=");pi(st.t_f5);ps("\n");}
void frt_state(void){ps("[FRT] Cl=");pi(st.n_cl);ps(" Pt=");pi(st.n_pt);ps(" Wd=");pi(st.n_wd);ps(" Ch=");pi(st.n_ch);ps(" Mg=");pi(st.n_mg);ps("\n");}
int main(void){
ps("=== Forestry Tech Admin Demo ===\n\n");frt_init();
ps("Forest cultivation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;frt_cultivate(t,c,167+(i*17),152+(i*14),132+(i*10),115+(i*6),2020+(i%5));}
ps("\nForest protection...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;frt_protect(t,c,156+(i*15),142+(i*12),124+(i*8),111+(i*5),2021+(i%4));}
ps("\nWood processing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;frt_wood(t,c,148+(i*13),134+(i*10),118+(i*7),107+(i*4),2022+(i%3));}
ps("\nForest chemical...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frt_chemical(t,c,140+(i*11),128+(i*9),114+(i*6),104+(i*3),2023+(i%2));}
ps("\nForest management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frt_manage(t,c,134+(i*9),123+(i*7),110+(i*5),102+(i*3),2024);}
ps("\n");frt_report();frt_state();ps("\n=== Demo Complete ===\n");return 0;}
