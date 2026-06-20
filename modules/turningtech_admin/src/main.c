/* turningtech_admin: Turning technology administration (v1.0)
 * OD turning, ID turning, face turning, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} trn_t;
typedef struct{int n_od,n_id,n_fc,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} trn_state_t;
static trn_t ods[N],ids[N-2],fcs[N-4],acs[N-6],mks[N-6]; static trn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(trn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;trn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int trn_init(void){if(init)return -1;st.n_od=0;st.n_id=0;st.n_fc=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ods[i].active=0;for(int i=0;i<N-2;i++)ids[i].active=0;for(int i=0;i<N-4;i++)fcs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TRN] Turningtech initialized\n");return 0;}
int trn_od(int t,int c,int a,int b,int d,int e,int y){return add(ods,&st.n_od,&st.t_f1,N,t,c,a,b,d,e,y);}
int trn_id(int t,int c,int a,int b,int d,int e,int y){return add(ids,&st.n_id,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int trn_face(int t,int c,int a,int b,int d,int e,int y){return add(fcs,&st.n_fc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int trn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int trn_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void trn_report(void){ps("[TRN] Od: ");pi(st.n_od);ps(" PCS=");pi(st.t_f1);ps("\nId: ");pi(st.n_id);ps(" PCS=");pi(st.t_f2);ps("\nFc: ");pi(st.n_fc);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void trn_state(void){ps("[TRN] Od=");pi(st.n_od);ps(" Id=");pi(st.n_id);ps(" Fc=");pi(st.n_fc);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Turning Tech Admin Demo ===\n\n");trn_init();
ps("OD turning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;trn_od(t,c,318+(i*17),303+(i*14),283+(i*10),265+(i*6),2020+(i%5));}
ps("\nID turning...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;trn_id(t,c,307+(i*15),293+(i*12),275+(i*8),262+(i*5),2021+(i%4));}
ps("\nFace turning...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;trn_face(t,c,299+(i*13),285+(i*10),269+(i*7),258+(i*4),2022+(i%3));}
ps("\nTurning accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trn_accessory(t,c,291+(i*11),279+(i*9),265+(i*6),255+(i*3),2023+(i%2));}
ps("\nTurning marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trn_market(t,c,285+(i*9),274+(i*7),261+(i*5),253+(i*3),2024);}
ps("\n");trn_report();trn_state();ps("\n=== Demo Complete ===\n");return 0;}
