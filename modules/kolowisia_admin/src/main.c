/* kolowisia_admin: Kolowisia management technology administration (v1.0)
 * Kolowisia planning, kolowisia execution, kolowisia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kol_t;
typedef struct{int n_kolp,n_kole,n_kol2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kol_state_t;
static kol_t kolps[N],koless[N-2],koloss[N-4],kolas[N-6],kolmks[N-6]; static kol_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(kol_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kol_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KOL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kol_init(void){if(init)return -1;st.n_kolp=0;st.n_kole=0;st.n_kol2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kolps[i].active=0;for(int i=0;i<N-2;i++)koless[i].active=0;for(int i=0;i<N-4;i++)koloss[i].active=0;for(int i=0;i<N-6;i++)kolas[i].active=0;for(int i=0;i<N-6;i++)kolmks[i].active=0;init=1;ps("[KOL] Kolowisia initialized\n");return 0;}
int kol_planning(int t,int c,int a,int b,int d,int e,int y){return add(kolps,&st.n_kolp,&st.t_f1,N,t,c,a,b,d,e,y);}
int kol_execution(int t,int c,int a,int b,int d,int e,int y){return add(koless,&st.n_kole,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kol_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(koloss,&st.n_kol2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kol_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kolas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kol_market(int t,int c,int a,int b,int d,int e,int y){return add(kolmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kol_report(void){ps("[KOL] Kolp: ");pi(st.n_kolp);ps(" PCS=");pi(st.t_f1);ps("\nKole: ");pi(st.n_kole);ps(" PCS=");pi(st.t_f2);ps("\nKolv: ");pi(st.n_kol2);ps(" PCS=");pi(st.t_f3);ps("\nKolic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kol_state(void){ps("[KOL] Kolp=");pi(st.n_kolp);ps(" Kole=");pi(st.n_kole);ps(" Kolv=");pi(st.n_kol2);ps(" Kolic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kolowisia Admin Demo ===\n\n");kol_init();
ps("Kolowisia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kol_planning(t,c,1308+(i*17),1297+(i*14),1277+(i*10),1259+(i*6),2020+(i%5));}
ps("\nKolowisia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kol_execution(t,c,1297+(i*15),1286+(i*12),1268+(i*8),1255+(i*5),2021+(i%4));}
ps("\nKolowisia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kol_evaluation(t,c,1289+(i*13),1278+(i*10),1262+(i*7),1251+(i*4),2022+(i%3));}
ps("\nKolowisia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kol_accessory(t,c,1281+(i*11),1272+(i*9),1258+(i*6),1248+(i*3),2023+(i%2));}
ps("\nKolowisia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kol_market(t,c,1275+(i*9),1266+(i*7),1253+(i*5),1245+(i*3),2024);}
ps("\n");kol_report();kol_state();ps("\n=== Demo Complete ===\n");return 0;}
