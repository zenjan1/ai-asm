/* spectech_admin: Spectacles technology administration (v1.0)
 * Optical glasses, sunglasses, protective glasses, fashion frames, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spe_t;
typedef struct{int n_og,n_sg,n_pg,n_fg,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spe_state_t;
static spe_t ogs[N],sgs[N-2],pgs[N-4],fgs[N-6],mks[N-6]; static spe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spe_init(void){if(init)return -1;st.n_og=0;st.n_sg=0;st.n_pg=0;st.n_fg=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ogs[i].active=0;for(int i=0;i<N-2;i++)sgs[i].active=0;for(int i=0;i<N-4;i++)pgs[i].active=0;for(int i=0;i<N-6;i++)fgs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SPE] Spectech initialized\n");return 0;}
int spe_optical(int t,int c,int a,int b,int d,int e,int y){return add(ogs,&st.n_og,&st.t_f1,N,t,c,a,b,d,e,y);}
int spe_sunglass(int t,int c,int a,int b,int d,int e,int y){return add(sgs,&st.n_sg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spe_protective(int t,int c,int a,int b,int d,int e,int y){return add(pgs,&st.n_pg,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spe_fashion(int t,int c,int a,int b,int d,int e,int y){return add(fgs,&st.n_fg,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spe_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spe_report(void){ps("[SPE] Optical: ");pi(st.n_og);ps(" PCS=");pi(st.t_f1);ps("\nSun: ");pi(st.n_sg);ps(" PCS=");pi(st.t_f2);ps("\nProt: ");pi(st.n_pg);ps(" PCS=");pi(st.t_f3);ps("\nFashion: ");pi(st.n_fg);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void spe_state(void){ps("[SPE] Og=");pi(st.n_og);ps(" Sg=");pi(st.n_sg);ps(" Pg=");pi(st.n_pg);ps(" Fg=");pi(st.n_fg);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Spec Tech Admin Demo ===\n\n");spe_init();
ps("Optical glasses...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spe_optical(t,c,201+(i*17),186+(i*14),166+(i*10),148+(i*6),2020+(i%5));}
ps("\nSunglasses...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spe_sunglass(t,c,190+(i*15),176+(i*12),158+(i*8),145+(i*5),2021+(i%4));}
ps("\nProtective glasses...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spe_protective(t,c,182+(i*13),168+(i*10),152+(i*7),141+(i*4),2022+(i%3));}
ps("\nFashion frames...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spe_fashion(t,c,174+(i*11),162+(i*9),148+(i*6),138+(i*3),2023+(i%2));}
ps("\nSpec marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spe_market(t,c,168+(i*9),157+(i*7),144+(i*5),136+(i*3),2024);}
ps("\n");spe_report();spe_state();ps("\n=== Demo Complete ===\n");return 0;}
