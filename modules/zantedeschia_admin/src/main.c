/* zantedeschia_admin: Zantedeschia management technology administration (v1.0)
 * Zantedeschia planning, zantedeschia execution, zantedeschia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zt_t;
typedef struct{int n_ztp,n_zte,n_ztv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zt_state_t;
static zt_t ztps[N],ztss[N-2],ztvss[N-4],ztas[N-6],ztmks[N-6]; static zt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZTD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ztd_init(void){if(init)return -1;st.n_ztp=0;st.n_zte=0;st.n_ztv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ztps[i].active=0;for(int i=0;i<N-2;i++)ztss[i].active=0;for(int i=0;i<N-4;i++)ztvss[i].active=0;for(int i=0;i<N-6;i++)ztas[i].active=0;for(int i=0;i<N-6;i++)ztmks[i].active=0;init=1;ps("[ZTD] Zantedeschia initialized\n");return 0;}
int ztd_planning(int t,int c,int a,int b,int d,int e,int y){return add(ztps,&st.n_ztp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ztd_execution(int t,int c,int a,int b,int d,int e,int y){return add(ztss,&st.n_zte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ztd_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ztvss,&st.n_ztv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ztd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ztas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ztd_market(int t,int c,int a,int b,int d,int e,int y){return add(ztmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ztd_report(void){ps("[ZTD] Ztp: ");pi(st.n_ztp);ps(" PCS=");pi(st.t_f1);ps("\nZte: ");pi(st.n_zte);ps(" PCS=");pi(st.t_f2);ps("\nZtv: ");pi(st.n_ztv);ps(" PCS=");pi(st.t_f3);ps("\nZtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ztd_state(void){ps("[ZTD] Ztp=");pi(st.n_ztp);ps(" Zte=");pi(st.n_zte);ps(" Ztv=");pi(st.n_ztv);ps(" Ztc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zantedeschia Admin Demo ===\n\n");ztd_init();
ps("Zantedeschia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ztd_planning(t,c,756+(i*17),745+(i*14),725+(i*10),707+(i*6),2020+(i%5));}
ps("\nZantedeschia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ztd_execution(t,c,745+(i*15),734+(i*12),716+(i*8),703+(i*5),2021+(i%4));}
ps("\nZantedeschia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ztd_evaluation(t,c,737+(i*13),726+(i*10),712+(i*7),701+(i*4),2022+(i%3));}
ps("\nZantedeschia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ztd_accessory(t,c,729+(i*11),720+(i*9),706+(i*6),696+(i*3),2023+(i%2));}
ps("\nZantedeschia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ztd_market(t,c,723+(i*9),714+(i*7),701+(i*5),693+(i*3),2024);}
ps("\n");ztd_report();ztd_state();ps("\n=== Demo Complete ===\n");return 0;}
