/* artemisia_admin: Artemisia management technology administration (v1.0)
 * Artemisia planning, artemisia execution, artemisia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} art_t;
typedef struct{int n_artp,n_arte,n_art2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} art_state_t;
static art_t artps[N],artss[N-2],artvss[N-4],artas[N-6],artmks[N-6]; static art_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(art_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;art_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ART] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int art_init(void){if(init)return -1;st.n_artp=0;st.n_arte=0;st.n_art2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)artps[i].active=0;for(int i=0;i<N-2;i++)artss[i].active=0;for(int i=0;i<N-4;i++)artvss[i].active=0;for(int i=0;i<N-6;i++)artas[i].active=0;for(int i=0;i<N-6;i++)artmks[i].active=0;init=1;ps("[ART] Artemisia initialized\n");return 0;}
int art_planning(int t,int c,int a,int b,int d,int e,int y){return add(artps,&st.n_artp,&st.t_f1,N,t,c,a,b,d,e,y);}
int art_execution(int t,int c,int a,int b,int d,int e,int y){return add(artss,&st.n_arte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int art_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(artvss,&st.n_art2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int art_accessory(int t,int c,int a,int b,int d,int e,int y){return add(artas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int art_market(int t,int c,int a,int b,int d,int e,int y){return add(artmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void art_report(void){ps("[ART] Artp: ");pi(st.n_artp);ps(" PCS=");pi(st.t_f1);ps("\nArte: ");pi(st.n_arte);ps(" PCS=");pi(st.t_f2);ps("\nArtv: ");pi(st.n_art2);ps(" PCS=");pi(st.t_f3);ps("\nArtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void art_state(void){ps("[ART] Artp=");pi(st.n_artp);ps(" Arte=");pi(st.n_arte);ps(" Artv=");pi(st.n_art2);ps(" Artc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Artemisia Admin Demo ===\n\n");art_init();
ps("Artemisia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;art_planning(t,c,898+(i*17),887+(i*14),867+(i*10),849+(i*6),2020+(i%5));}
ps("\nArtemisia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;art_execution(t,c,887+(i*15),876+(i*12),858+(i*8),845+(i*5),2021+(i%4));}
ps("\nArtemisia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;art_evaluation(t,c,879+(i*13),868+(i*10),852+(i*7),841+(i*4),2022+(i%3));}
ps("\nArtemisia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;art_accessory(t,c,871+(i*11),862+(i*9),848+(i*6),838+(i*3),2023+(i%2));}
ps("\nArtemisia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;art_market(t,c,865+(i*9),856+(i*7),843+(i*5),835+(i*3),2024);}
ps("\n");art_report();art_state();ps("\n=== Demo Complete ===\n");return 0;}
