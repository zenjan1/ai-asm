/* proptech_admin: Proptech administration (v1.0)
 * Smart property, property management, real estate finance, construction tech, space operations
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pta_t;
typedef struct{int n_sp,n_pm,n_rf,n_ct,n_so,t_f1,t_f2,t_f3,t_f4,t_f5;} pta_state_t;
static pta_t sps[N],pms[N-2],rfs[N-4],cts[N-6],sos[N-6]; static pta_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pta_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pta_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PTA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pta_init(void){if(init)return -1;st.n_sp=0;st.n_pm=0;st.n_rf=0;st.n_ct=0;st.n_so=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sps[i].active=0;for(int i=0;i<N-2;i++)pms[i].active=0;for(int i=0;i<N-4;i++)rfs[i].active=0;for(int i=0;i<N-6;i++)cts[i].active=0;for(int i=0;i<N-6;i++)sos[i].active=0;init=1;ps("[PTA] Proptech initialized\n");return 0;}
int pta_smart(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pta_property(int t,int c,int a,int b,int d,int e,int y){return add(pms,&st.n_pm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pta_finance(int t,int c,int a,int b,int d,int e,int y){return add(rfs,&st.n_rf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pta_construction(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pta_space(int t,int c,int a,int b,int d,int e,int y){return add(sos,&st.n_so,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pta_report(void){ps("[PTA] Smart: ");pi(st.n_sp);ps(" VR=");pi(st.t_f1);ps("\nProp: ");pi(st.n_pm);ps(" IoT=");pi(st.t_f2);ps("\nFin: ");pi(st.n_rf);ps(" REIT=");pi(st.t_f3);ps("\nConst: ");pi(st.n_ct);ps(" BIM=");pi(st.t_f4);ps("\nSpace: ");pi(st.n_so);ps(" SaaS=");pi(st.t_f5);ps("\n");}
void pta_state(void){ps("[PTA] Sp=");pi(st.n_sp);ps(" Pm=");pi(st.n_pm);ps(" Rf=");pi(st.n_rf);ps(" Ct=");pi(st.n_ct);ps(" So=");pi(st.n_so);ps("\n");}
int main(void){
ps("=== Proptech Admin Demo ===\n\n");pta_init();
ps("Smart property...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pta_smart(t,c,137+(i*17),122+(i*14),102+(i*10),85+(i*6),2020+(i%5));}
ps("\nProperty management...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pta_property(t,c,126+(i*15),112+(i*12),94+(i*8),81+(i*5),2021+(i%4));}
ps("\nReal estate finance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pta_finance(t,c,118+(i*13),104+(i*10),88+(i*7),77+(i*4),2022+(i%3));}
ps("\nConstruction tech...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pta_construction(t,c,110+(i*11),98+(i*9),84+(i*6),74+(i*3),2023+(i%2));}
ps("\nSpace operations...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pta_space(t,c,104+(i*9),93+(i*7),80+(i*5),72+(i*3),2024);}
ps("\n");pta_report();pta_state();ps("\n=== Demo Complete ===\n");return 0;}
