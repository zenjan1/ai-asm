/* tiarella_admin: Tiarella management technology administration (v1.0)
 * Tiarella planning, tiarella execution, tiarella evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tia_t;
typedef struct{int n_tiap,n_tiae,n_tia2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tia_state_t;
static tia_t tiaps[N],tiass[N-2],tiavss[N-4],tiaas[N-6],tiamks[N-6]; static tia_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tia_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tia_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TIA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tia_init(void){if(init)return -1;st.n_tiap=0;st.n_tiae=0;st.n_tia2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tiaps[i].active=0;for(int i=0;i<N-2;i++)tiass[i].active=0;for(int i=0;i<N-4;i++)tiavss[i].active=0;for(int i=0;i<N-6;i++)tiaas[i].active=0;for(int i=0;i<N-6;i++)tiamks[i].active=0;init=1;ps("[TIA] Tiarella initialized\n");return 0;}
int tia_planning(int t,int c,int a,int b,int d,int e,int y){return add(tiaps,&st.n_tiap,&st.t_f1,N,t,c,a,b,d,e,y);}
int tia_execution(int t,int c,int a,int b,int d,int e,int y){return add(tiass,&st.n_tiae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tia_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tiavss,&st.n_tia2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tia_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tiaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tia_market(int t,int c,int a,int b,int d,int e,int y){return add(tiamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tia_report(void){ps("[TIA] Tiap: ");pi(st.n_tiap);ps(" PCS=");pi(st.t_f1);ps("\nTiae: ");pi(st.n_tiae);ps(" PCS=");pi(st.t_f2);ps("\nTiav: ");pi(st.n_tia2);ps(" PCS=");pi(st.t_f3);ps("\nTiac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tia_state(void){ps("[TIA] Tiap=");pi(st.n_tiap);ps(" Tiae=");pi(st.n_tiae);ps(" Tiav=");pi(st.n_tia2);ps(" Tiac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tiarella Admin Demo ===\n\n");tia_init();
ps("Tiarella planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tia_planning(t,c,947+(i*17),936+(i*14),916+(i*10),898+(i*6),2020+(i%5));}
ps("\nTiarella execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tia_execution(t,c,936+(i*15),925+(i*12),907+(i*8),894+(i*5),2021+(i%4));}
ps("\nTiarella evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tia_evaluation(t,c,928+(i*13),917+(i*10),901+(i*7),890+(i*4),2022+(i%3));}
ps("\nTiarella accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tia_accessory(t,c,920+(i*11),911+(i*9),897+(i*6),887+(i*3),2023+(i%2));}
ps("\nTiarella marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tia_market(t,c,914+(i*9),905+(i*7),892+(i*5),884+(i*3),2024);}
ps("\n");tia_report();tia_state();ps("\n=== Demo Complete ===\n");return 0;}
