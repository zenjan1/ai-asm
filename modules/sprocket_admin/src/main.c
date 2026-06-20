/* sprocket_admin: Sprocket technology administration (v1.0)
 * Standard sprocket, special sprocket, sprocket assembly, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spk_t;
typedef struct{int n_st,n_sp,n_co,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spk_state_t;
static spk_t sts[N],sps[N-2],cos[N-4],acs[N-6],mks[N-6]; static spk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spk_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spk_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spk_init(void){if(init)return -1;st.n_st=0;st.n_sp=0;st.n_co=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sts[i].active=0;for(int i=0;i<N-2;i++)sps[i].active=0;for(int i=0;i<N-4;i++)cos[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SPK] Sprocket initialized\n");return 0;}
int spk_standard(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f1,N,t,c,a,b,d,e,y);}
int spk_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spk_assembly(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spk_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spk_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spk_report(void){ps("[SPK] St: ");pi(st.n_st);ps(" PCS=");pi(st.t_f1);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f2);ps("\nCo: ");pi(st.n_co);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void spk_state(void){ps("[SPK] St=");pi(st.n_st);ps(" Sp=");pi(st.n_sp);ps(" Co=");pi(st.n_co);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sprocket Admin Demo ===\n\n");spk_init();
ps("Standard sprockets...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spk_standard(t,c,351+(i*17),336+(i*14),316+(i*10),298+(i*6),2020+(i%5));}
ps("\nSpecial sprockets...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spk_special(t,c,340+(i*15),326+(i*12),308+(i*8),295+(i*5),2021+(i%4));}
ps("\nSprocket assemblies...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spk_assembly(t,c,332+(i*13),318+(i*10),302+(i*7),291+(i*4),2022+(i%3));}
ps("\nSprocket accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spk_accessory(t,c,324+(i*11),312+(i*9),298+(i*6),288+(i*3),2023+(i%2));}
ps("\nSprocket marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spk_market(t,c,318+(i*9),307+(i*7),294+(i*5),286+(i*3),2024);}
ps("\n");spk_report();spk_state();ps("\n=== Demo Complete ===\n");return 0;}
