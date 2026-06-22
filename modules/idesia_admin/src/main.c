/* idesia_admin: Idesia management technology administration (v1.0)
 * Idesia planning, idesia execution, idesia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} idi_t;
typedef struct{int n_idip,n_idie,n_idi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} idi_state_t;
static idi_t idips[N],idiess[N-2],idivss[N-4],idias[N-6],idimks[N-6]; static idi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(idi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;idi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IDI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int idi_init(void){if(init)return -1;st.n_idip=0;st.n_idie=0;st.n_idi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)idips[i].active=0;for(int i=0;i<N-2;i++)idiess[i].active=0;for(int i=0;i<N-4;i++)idivss[i].active=0;for(int i=0;i<N-6;i++)idias[i].active=0;for(int i=0;i<N-6;i++)idimks[i].active=0;init=1;ps("[IDI] Idesia initialized\n");return 0;}
int idi_planning(int t,int c,int a,int b,int d,int e,int y){return add(idips,&st.n_idip,&st.t_f1,N,t,c,a,b,d,e,y);}
int idi_execution(int t,int c,int a,int b,int d,int e,int y){return add(idiess,&st.n_idie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int idi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(idivss,&st.n_idi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int idi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(idias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int idi_market(int t,int c,int a,int b,int d,int e,int y){return add(idimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void idi_report(void){ps("[IDI] Idip: ");pi(st.n_idip);ps(" PCS=");pi(st.t_f1);ps("\nIdie: ");pi(st.n_idie);ps(" PCS=");pi(st.t_f2);ps("\nIdiv: ");pi(st.n_idi2);ps(" PCS=");pi(st.t_f3);ps("\nIdic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void idi_state(void){ps("[IDI] Idip=");pi(st.n_idip);ps(" Idie=");pi(st.n_idie);ps(" Idiv=");pi(st.n_idi2);ps(" Idic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Idesia Admin Demo ===\n\n");idi_init();
ps("Idesia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;idi_planning(t,c,1302+(i*17),1291+(i*14),1271+(i*10),1253+(i*6),2020+(i%5));}
ps("\nIdesia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;idi_execution(t,c,1291+(i*15),1280+(i*12),1262+(i*8),1249+(i*5),2021+(i%4));}
ps("\nIdesia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;idi_evaluation(t,c,1283+(i*13),1272+(i*10),1256+(i*7),1245+(i*4),2022+(i%3));}
ps("\nIdesia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;idi_accessory(t,c,1275+(i*11),1266+(i*9),1252+(i*6),1242+(i*3),2023+(i%2));}
ps("\nIdesia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;idi_market(t,c,1269+(i*9),1260+(i*7),1247+(i*5),1239+(i*3),2024);}
ps("\n");idi_report();idi_state();ps("\n=== Demo Complete ===\n");return 0;}
