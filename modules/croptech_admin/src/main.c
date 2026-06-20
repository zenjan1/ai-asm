/* croptech_admin: Crop and agriculture technology administration (v1.0)
 * Crop farming, livestock, agricultural machinery, fertilizer and pesticide, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cpt_t;
typedef struct{int n_cr,n_ls,n_am,n_fp,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cpt_state_t;
static cpt_t crs[N],lss[N-2],ams[N-4],fps[N-6],mks[N-6]; static cpt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cpt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cpt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CPT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cpt_init(void){if(init)return -1;st.n_cr=0;st.n_ls=0;st.n_am=0;st.n_fp=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)crs[i].active=0;for(int i=0;i<N-2;i++)lss[i].active=0;for(int i=0;i<N-4;i++)ams[i].active=0;for(int i=0;i<N-6;i++)fps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CPT] Croptech initialized\n");return 0;}
int cpt_crop(int t,int c,int a,int b,int d,int e,int y){return add(crs,&st.n_cr,&st.t_f1,N,t,c,a,b,d,e,y);}
int cpt_livestock(int t,int c,int a,int b,int d,int e,int y){return add(lss,&st.n_ls,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cpt_machinery(int t,int c,int a,int b,int d,int e,int y){return add(ams,&st.n_am,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cpt_fertilizer(int t,int c,int a,int b,int d,int e,int y){return add(fps,&st.n_fp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cpt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cpt_report(void){ps("[CPT] Cr: ");pi(st.n_cr);ps(" Ton=");pi(st.t_f1);ps("\nLs: ");pi(st.n_ls);ps(" Head=");pi(st.t_f2);ps("\nAm: ");pi(st.n_am);ps(" PCS=");pi(st.t_f3);ps("\nFp: ");pi(st.n_fp);ps(" kg=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cpt_state(void){ps("[CPT] Cr=");pi(st.n_cr);ps(" Ls=");pi(st.n_ls);ps(" Am=");pi(st.n_am);ps(" Fp=");pi(st.n_fp);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Crop Tech Admin Demo ===\n\n");cpt_init();
ps("Crop farming...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cpt_crop(t,c,226+(i*17),211+(i*14),191+(i*10),173+(i*6),2020+(i%5));}
ps("\nLivestock...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cpt_livestock(t,c,215+(i*15),201+(i*12),183+(i*8),170+(i*5),2021+(i%4));}
ps("\nAgricultural machinery...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cpt_machinery(t,c,207+(i*13),193+(i*10),177+(i*7),166+(i*4),2022+(i%3));}
ps("\nFertilizer and pesticide...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpt_fertilizer(t,c,199+(i*11),187+(i*9),173+(i*6),163+(i*3),2023+(i%2));}
ps("\nAgri marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpt_market(t,c,193+(i*9),182+(i*7),169+(i*5),161+(i*3),2024);}
ps("\n");cpt_report();cpt_state();ps("\n=== Demo Complete ===\n");return 0;}
