/* embothium_admin: Embothium management technology administration (v1.0)
 * Embothium planning, embothium execution, embothium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} emb_t;
typedef struct{int n_embp,n_embe,n_emb2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} emb_state_t;
static emb_t embps[N],embss[N-2],embvss[N-4],embas[N-6],embmks[N-6]; static emb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(emb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;emb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EMB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int emb_init(void){if(init)return -1;st.n_embp=0;st.n_embe=0;st.n_emb2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)embps[i].active=0;for(int i=0;i<N-2;i++)embss[i].active=0;for(int i=0;i<N-4;i++)embvss[i].active=0;for(int i=0;i<N-6;i++)embas[i].active=0;for(int i=0;i<N-6;i++)embmks[i].active=0;init=1;ps("[EMB] Embothium initialized\n");return 0;}
int emb_planning(int t,int c,int a,int b,int d,int e,int y){return add(embps,&st.n_embp,&st.t_f1,N,t,c,a,b,d,e,y);}
int emb_execution(int t,int c,int a,int b,int d,int e,int y){return add(embss,&st.n_embe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int emb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(embvss,&st.n_emb2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int emb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(embas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int emb_market(int t,int c,int a,int b,int d,int e,int y){return add(embmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void emb_report(void){ps("[EMB] Embp: ");pi(st.n_embp);ps(" PCS=");pi(st.t_f1);ps("\nEmbe: ");pi(st.n_embe);ps(" PCS=");pi(st.t_f2);ps("\nEmbv: ");pi(st.n_emb2);ps(" PCS=");pi(st.t_f3);ps("\nEmbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void emb_state(void){ps("[EMB] Embp=");pi(st.n_embp);ps(" Embe=");pi(st.n_embe);ps(" Embv=");pi(st.n_emb2);ps(" Embc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Embothium Admin Demo ===\n\n");emb_init();
ps("Embothium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;emb_planning(t,c,1275+(i*17),1264+(i*14),1244+(i*10),1226+(i*6),2020+(i%5));}
ps("\nEmbothium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;emb_execution(t,c,1264+(i*15),1253+(i*12),1235+(i*8),1222+(i*5),2021+(i%4));}
ps("\nEmbothium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;emb_evaluation(t,c,1256+(i*13),1245+(i*10),1229+(i*7),1218+(i*4),2022+(i%3));}
ps("\nEmbothium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;emb_accessory(t,c,1248+(i*11),1239+(i*9),1225+(i*6),1215+(i*3),2023+(i%2));}
ps("\nEmbothium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;emb_market(t,c,1242+(i*9),1233+(i*7),1220+(i*5),1212+(i*3),2024);}
ps("\n");emb_report();emb_state();ps("\n=== Demo Complete ===\n");return 0;}
