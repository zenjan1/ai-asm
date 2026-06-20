/* blankingtech_admin: Blanking/cutting technology administration (v1.0)
 * Saw cutting, shear cutting, flame cutting, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} blk_t;
typedef struct{int n_sw,n_sh,n_fl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} blk_state_t;
static blk_t sws[N],shs[N-2],fls[N-4],acs[N-6],mks[N-6]; static blk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blk_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;blk_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BLK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int blk_init(void){if(init)return -1;st.n_sw=0;st.n_sh=0;st.n_fl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sws[i].active=0;for(int i=0;i<N-2;i++)shs[i].active=0;for(int i=0;i<N-4;i++)fls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BLK] Blankingtech initialized\n");return 0;}
int blk_saw(int t,int c,int a,int b,int d,int e,int y){return add(sws,&st.n_sw,&st.t_f1,N,t,c,a,b,d,e,y);}
int blk_shear(int t,int c,int a,int b,int d,int e,int y){return add(shs,&st.n_sh,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int blk_flame(int t,int c,int a,int b,int d,int e,int y){return add(fls,&st.n_fl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int blk_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int blk_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void blk_report(void){ps("[BLK] Sw: ");pi(st.n_sw);ps(" PCS=");pi(st.t_f1);ps("\nSh: ");pi(st.n_sh);ps(" PCS=");pi(st.t_f2);ps("\nFl: ");pi(st.n_fl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void blk_state(void){ps("[BLK] Sw=");pi(st.n_sw);ps(" Sh=");pi(st.n_sh);ps(" Fl=");pi(st.n_fl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Blanking Tech Admin Demo ===\n\n");blk_init();
ps("Saw cutting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;blk_saw(t,c,285+(i*17),270+(i*14),250+(i*10),232+(i*6),2020+(i%5));}
ps("\nShear cutting...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;blk_shear(t,c,274+(i*15),260+(i*12),242+(i*8),229+(i*5),2021+(i%4));}
ps("\nFlame cutting...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;blk_flame(t,c,266+(i*13),252+(i*10),236+(i*7),225+(i*4),2022+(i%3));}
ps("\nBlanking accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blk_accessory(t,c,258+(i*11),246+(i*9),232+(i*6),222+(i*3),2023+(i%2));}
ps("\nBlanking marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blk_market(t,c,252+(i*9),241+(i*7),228+(i*5),220+(i*3),2024);}
ps("\n");blk_report();blk_state();ps("\n=== Demo Complete ===\n");return 0;}
