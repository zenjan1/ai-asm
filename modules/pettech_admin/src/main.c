/* pettech_admin: Pet technology administration (v1.0)
 * Pet food, pet supplies, pet medical, pet services, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pet_t;
typedef struct{int n_pfd,n_psp,n_pmd,n_psv,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pet_state_t;
static pet_t pfds[N],psps[N-2],pmds[N-4],psvs[N-6],mks[N-6]; static pet_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pet_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pet_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PET] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pet_init(void){if(init)return -1;st.n_pfd=0;st.n_psp=0;st.n_pmd=0;st.n_psv=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pfds[i].active=0;for(int i=0;i<N-2;i++)psps[i].active=0;for(int i=0;i<N-4;i++)pmds[i].active=0;for(int i=0;i<N-6;i++)psvs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PET] Pettech initialized\n");return 0;}
int pet_food(int t,int c,int a,int b,int d,int e,int y){return add(pfds,&st.n_pfd,&st.t_f1,N,t,c,a,b,d,e,y);}
int pet_supply(int t,int c,int a,int b,int d,int e,int y){return add(psps,&st.n_psp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pet_medical(int t,int c,int a,int b,int d,int e,int y){return add(pmds,&st.n_pmd,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pet_service(int t,int c,int a,int b,int d,int e,int y){return add(psvs,&st.n_psv,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pet_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pet_report(void){ps("[PET] Pfd: ");pi(st.n_pfd);ps(" kg=");pi(st.t_f1);ps("\nPsp: ");pi(st.n_psp);ps(" PCS=");pi(st.t_f2);ps("\nPmd: ");pi(st.n_pmd);ps(" PCS=");pi(st.t_f3);ps("\nPsv: ");pi(st.n_psv);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pet_state(void){ps("[PET] Pfd=");pi(st.n_pfd);ps(" Psp=");pi(st.n_psp);ps(" Pmd=");pi(st.n_pmd);ps(" Psv=");pi(st.n_psv);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pet Tech Admin Demo ===\n\n");pet_init();
ps("Pet food...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pet_food(t,c,233+(i*17),218+(i*14),198+(i*10),180+(i*6),2020+(i%5));}
ps("\nPet supplies...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pet_supply(t,c,222+(i*15),208+(i*12),190+(i*8),177+(i*5),2021+(i%4));}
ps("\nPet medical...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pet_medical(t,c,214+(i*13),200+(i*10),184+(i*7),173+(i*4),2022+(i%3));}
ps("\nPet services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pet_service(t,c,206+(i*11),194+(i*9),180+(i*6),170+(i*3),2023+(i%2));}
ps("\nPet marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pet_market(t,c,200+(i*9),189+(i*7),176+(i*5),168+(i*3),2024);}
ps("\n");pet_report();pet_state();ps("\n=== Demo Complete ===\n");return 0;}
