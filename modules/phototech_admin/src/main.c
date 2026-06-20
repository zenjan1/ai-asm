/* phototech_admin: Photo-electric technology administration (v1.0)
 * Optical elements, camera manufacturing, imaging equipment, optical instruments, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pht_t;
typedef struct{int n_opt,n_cam,n_img,n_ins,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pht_state_t;
static pht_t opts[N],cams[N-2],imgs[N-4],inss[N-6],mks[N-6]; static pht_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pht_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pht_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PHT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pht_init(void){if(init)return -1;st.n_opt=0;st.n_cam=0;st.n_img=0;st.n_ins=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)opts[i].active=0;for(int i=0;i<N-2;i++)cams[i].active=0;for(int i=0;i<N-4;i++)imgs[i].active=0;for(int i=0;i<N-6;i++)inss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PHT] Phototech initialized\n");return 0;}
int pht_optical(int t,int c,int a,int b,int d,int e,int y){return add(opts,&st.n_opt,&st.t_f1,N,t,c,a,b,d,e,y);}
int pht_camera(int t,int c,int a,int b,int d,int e,int y){return add(cams,&st.n_cam,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pht_image(int t,int c,int a,int b,int d,int e,int y){return add(imgs,&st.n_img,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pht_instrument(int t,int c,int a,int b,int d,int e,int y){return add(inss,&st.n_ins,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pht_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pht_report(void){ps("[PHT] Opt: ");pi(st.n_opt);ps(" PCS=");pi(st.t_f1);ps("\nCam: ");pi(st.n_cam);ps(" PCS=");pi(st.t_f2);ps("\nImg: ");pi(st.n_img);ps(" PCS=");pi(st.t_f3);ps("\nIns: ");pi(st.n_ins);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pht_state(void){ps("[PHT] Opt=");pi(st.n_opt);ps(" Cam=");pi(st.n_cam);ps(" Img=");pi(st.n_img);ps(" Ins=");pi(st.n_ins);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Photo Tech Admin Demo ===\n\n");pht_init();
ps("Optical elements...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pht_optical(t,c,215+(i*17),200+(i*14),180+(i*10),162+(i*6),2020+(i%5));}
ps("\nCamera manufacturing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pht_camera(t,c,204+(i*15),190+(i*12),172+(i*8),159+(i*5),2021+(i%4));}
ps("\nImaging equipment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pht_image(t,c,196+(i*13),182+(i*10),166+(i*7),155+(i*4),2022+(i%3));}
ps("\nOptical instruments...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pht_instrument(t,c,188+(i*11),176+(i*9),162+(i*6),152+(i*3),2023+(i%2));}
ps("\nPhoto marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pht_market(t,c,182+(i*9),171+(i*7),158+(i*5),150+(i*3),2024);}
ps("\n");pht_report();pht_state();ps("\n=== Demo Complete ===\n");return 0;}
