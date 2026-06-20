/* heat_admin: Heat treatment technology administration (v1.0)
 * Conventional heat treatment, surface heat treatment, chemical heat treatment, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} het_t;
typedef struct{int n_cv,n_sf,n_ch,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} het_state_t;
static het_t cvs[N],sfs[N-2],chs[N-4],acs[N-6],mks[N-6]; static het_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(het_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;het_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HET] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int het_init(void){if(init)return -1;st.n_cv=0;st.n_sf=0;st.n_ch=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cvs[i].active=0;for(int i=0;i<N-2;i++)sfs[i].active=0;for(int i=0;i<N-4;i++)chs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HET] Heat initialized\n");return 0;}
int het_conventional(int t,int c,int a,int b,int d,int e,int y){return add(cvs,&st.n_cv,&st.t_f1,N,t,c,a,b,d,e,y);}
int het_surface(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int het_chemical(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int het_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int het_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void het_report(void){ps("[HET] Cv: ");pi(st.n_cv);ps(" PCS=");pi(st.t_f1);ps("\nSf: ");pi(st.n_sf);ps(" PCS=");pi(st.t_f2);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void het_state(void){ps("[HET] Cv=");pi(st.n_cv);ps(" Sf=");pi(st.n_sf);ps(" Ch=");pi(st.n_ch);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Heat Admin Demo ===\n\n");het_init();
ps("Conventional heat treatment...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;het_conventional(t,c,414+(i*17),403+(i*14),383+(i*10),365+(i*6),2020+(i%5));}
ps("\nSurface heat treatment...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;het_surface(t,c,403+(i*15),392+(i*12),374+(i*8),361+(i*5),2021+(i%4));}
ps("\nChemical heat treatment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;het_chemical(t,c,395+(i*13),384+(i*10),368+(i*7),357+(i*4),2022+(i%3));}
ps("\nHeat treatment accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;het_accessory(t,c,387+(i*11),378+(i*9),364+(i*6),354+(i*3),2023+(i%2));}
ps("\nHeat treatment marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;het_market(t,c,381+(i*9),372+(i*7),359+(i*5),351+(i*3),2024);}
ps("\n");het_report();het_state();ps("\n=== Demo Complete ===\n");return 0;}
