/* ambrosia_admin: Ambrosia management technology administration (v1.0)
 * Ambrosia planning, ambrosia execution, ambrosia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ambr_t;
typedef struct{int n_ambrp,n_ambr,n_ambr2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ambr_state_t;
static ambr_t ambrs[N],ambre[N-2],ambrv[N-4],ambrac[N-6],ambrm[N-6]; static ambr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ambr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ambr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AMBR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ambr_init(void){if(init)return -1;st.n_ambrp=0;st.n_ambr=0;st.n_ambr2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ambrs[i].active=0;for(int i=0;i<N-2;i++)ambre[i].active=0;for(int i=0;i<N-4;i++)ambrv[i].active=0;for(int i=0;i<N-6;i++)ambrac[i].active=0;for(int i=0;i<N-6;i++)ambrm[i].active=0;init=1;ps("[AMBR] Ambrosia initialized\n");return 0;}
int ambr_planning(int t,int c,int a,int b,int d,int e,int y){return add(ambrs,&st.n_ambrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ambr_execution(int t,int c,int a,int b,int d,int e,int y){return add(ambre,&st.n_ambr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ambr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ambrv,&st.n_ambr2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ambr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ambrac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ambr_market(int t,int c,int a,int b,int d,int e,int y){return add(ambrm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ambr_report(void){ps("[AMBR] Mbrp: ");pi(st.n_ambrp);ps(" PCS=");pi(st.t_f1);ps("\nMbre: ");pi(st.n_ambr);ps(" PCS=");pi(st.t_f2);ps("\nMbrv: ");pi(st.n_ambr2);ps(" PCS=");pi(st.t_f3);ps("\nMbrac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ambr_state(void){ps("[AMBR] Mbrp=");pi(st.n_ambrp);ps(" Mbre=");pi(st.n_ambr);ps(" Mbrv=");pi(st.n_ambr2);ps(" Mbrac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ambrosia Admin Demo ===\n\n");ambr_init();
ps("Ambrosia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ambr_planning(t,c,1401+(i*17),1390+(i*14),1370+(i*10),1352+(i*6),2020+(i%5));}
ps("\nAmbrosia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ambr_execution(t,c,1390+(i*15),1379+(i*12),1361+(i*8),1348+(i*5),2021+(i%4));}
ps("\nAmbrosia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ambr_evaluation(t,c,1382+(i*13),1371+(i*10),1355+(i*7),1344+(i*4),2022+(i%3));}
ps("\nAmbrosia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ambr_accessory(t,c,1374+(i*11),1365+(i*9),1351+(i*6),1341+(i*3),2023+(i%2));}
ps("\nAmbrosia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ambr_market(t,c,1368+(i*9),1359+(i*7),1346+(i*5),1338+(i*3),2024);}
ps("\n");ambr_report();ambr_state();ps("\n=== Demo Complete ===\n");return 0;}
