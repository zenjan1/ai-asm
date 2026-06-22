/* encephalartos_admin: Encephalartos management technology administration (v1.0)
 * Encephalartos planning, encephalartos execution, encephalartos evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} enc_t;
typedef struct{int n_encp,n_ence,n_enc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} enc_state_t;
static enc_t encps[N],encss[N-2],encvss[N-4],encas[N-6],encmks[N-6]; static enc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(enc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;enc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ENC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int enc_init(void){if(init)return -1;st.n_encp=0;st.n_ence=0;st.n_enc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)encps[i].active=0;for(int i=0;i<N-2;i++)encss[i].active=0;for(int i=0;i<N-4;i++)encvss[i].active=0;for(int i=0;i<N-6;i++)encas[i].active=0;for(int i=0;i<N-6;i++)encmks[i].active=0;init=1;ps("[ENC] Encephalartos initialized\n");return 0;}
int enc_planning(int t,int c,int a,int b,int d,int e,int y){return add(encps,&st.n_encp,&st.t_f1,N,t,c,a,b,d,e,y);}
int enc_execution(int t,int c,int a,int b,int d,int e,int y){return add(encss,&st.n_ence,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int enc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(encvss,&st.n_enc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int enc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(encas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int enc_market(int t,int c,int a,int b,int d,int e,int y){return add(encmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void enc_report(void){ps("[ENC] Encp: ");pi(st.n_encp);ps(" PCS=");pi(st.t_f1);ps("\nEnce: ");pi(st.n_ence);ps(" PCS=");pi(st.t_f2);ps("\nEncv: ");pi(st.n_enc2);ps(" PCS=");pi(st.t_f3);ps("\nEncc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void enc_state(void){ps("[ENC] Encp=");pi(st.n_encp);ps(" Ence=");pi(st.n_ence);ps(" Encv=");pi(st.n_enc2);ps(" Encc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Encephalartos Admin Demo ===\n\n");enc_init();
ps("Encephalartos planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;enc_planning(t,c,1273+(i*17),1262+(i*14),1242+(i*10),1224+(i*6),2020+(i%5));}
ps("\nEncephalartos execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;enc_execution(t,c,1262+(i*15),1251+(i*12),1233+(i*8),1220+(i*5),2021+(i%4));}
ps("\nEncephalartos evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;enc_evaluation(t,c,1254+(i*13),1243+(i*10),1227+(i*7),1216+(i*4),2022+(i%3));}
ps("\nEncephalartos accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;enc_accessory(t,c,1246+(i*11),1237+(i*9),1223+(i*6),1213+(i*3),2023+(i%2));}
ps("\nEncephalartos marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;enc_market(t,c,1240+(i*9),1231+(i*7),1218+(i*5),1210+(i*3),2024);}
ps("\n");enc_report();enc_state();ps("\n=== Demo Complete ===\n");return 0;}
