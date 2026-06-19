/* insurtech_admin: Insurtech administration (v1.0)
 * Usage-based insurance, claims automation, digital underwriting, parametric insurance, mutual insurance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ita_t;
typedef struct{int n_ub,n_ca,n_du,n_pi,n_mi,t_f1,t_f2,t_f3,t_f4,t_f5;} ita_state_t;
static ita_t ubs[N],cas[N-2],dus[N-4],pis[N-6],mis[N-6]; static ita_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ita_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ita_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ITA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ita_init(void){if(init)return -1;st.n_ub=0;st.n_ca=0;st.n_du=0;st.n_pi=0;st.n_mi=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ubs[i].active=0;for(int i=0;i<N-2;i++)cas[i].active=0;for(int i=0;i<N-4;i++)dus[i].active=0;for(int i=0;i<N-6;i++)pis[i].active=0;for(int i=0;i<N-6;i++)mis[i].active=0;init=1;ps("[ITA] Insurtech initialized\n");return 0;}
int ita_usage(int t,int c,int a,int b,int d,int e,int y){return add(ubs,&st.n_ub,&st.t_f1,N,t,c,a,b,d,e,y);}
int ita_claims(int t,int c,int a,int b,int d,int e,int y){return add(cas,&st.n_ca,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ita_underwrite(int t,int c,int a,int b,int d,int e,int y){return add(dus,&st.n_du,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ita_parametric(int t,int c,int a,int b,int d,int e,int y){return add(pis,&st.n_pi,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ita_mutual(int t,int c,int a,int b,int d,int e,int y){return add(mis,&st.n_mi,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ita_report(void){ps("[ITA] UBI: ");pi(st.n_ub);ps(" Telem=");pi(st.t_f1);ps("\nClaims: ");pi(st.n_ca);ps(" AI=");pi(st.t_f2);ps("\nUW: ");pi(st.n_du);ps(" Auto=");pi(st.t_f3);ps("\nParam: ");pi(st.n_pi);ps(" Idx=");pi(st.t_f4);ps("\nMutual: ");pi(st.n_mi);ps(" Pool=");pi(st.t_f5);ps("\n");}
void ita_state(void){ps("[ITA] Ub=");pi(st.n_ub);ps(" Ca=");pi(st.n_ca);ps(" Du=");pi(st.n_du);ps(" Pi=");pi(st.n_pi);ps(" Mi=");pi(st.n_mi);ps("\n");}
int main(void){
ps("=== Insurtech Admin Demo ===\n\n");ita_init();
ps("Usage-based insurance...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ita_usage(t,c,134+(i*17),119+(i*14),99+(i*10),82+(i*6),2020+(i%5));}
ps("\nClaims automation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ita_claims(t,c,123+(i*15),109+(i*12),91+(i*8),78+(i*5),2021+(i%4));}
ps("\nDigital underwriting...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ita_underwrite(t,c,115+(i*13),101+(i*10),85+(i*7),74+(i*4),2022+(i%3));}
ps("\nParametric insurance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ita_parametric(t,c,107+(i*11),95+(i*9),81+(i*6),71+(i*3),2023+(i%2));}
ps("\nMutual insurance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ita_mutual(t,c,101+(i*9),90+(i*7),77+(i*5),69+(i*3),2024);}
ps("\n");ita_report();ita_state();ps("\n=== Demo Complete ===\n");return 0;}
