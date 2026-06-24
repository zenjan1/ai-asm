/* century_admin: Century plant (Agave americana) succulent management (v1.0)
 * Century cultivation, offset_division, fiber_harvest, landscape, market
 * Features: rosette_diameter_cm, leaf_count, leaf_width_cm, flower_spike_m, pup_count, harvest_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,ros_dia,leaf_ct,leaf_wd,flr_spk,pup_ct,harv_yr,active;} centu_t;
typedef struct{int n_cult,n_offset,n_fiber,n_land,n_mkt,t_ros,t_leaf,t_width,t_spike,t_pup;} centu_state_t;
static centu_t centucs[N],centuos[N-2],centufs[N-4],centuls[N-6],centums[N-6]; static centu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(centu_t*a,int*cnt,int*sum,int mx,int lc,int rd,int lct,int lw,int fs,int pc,int hy){if(*cnt>=mx)return -1;centu_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->ros_dia=rd;x->leaf_ct=lct;x->leaf_wd=lw;x->flr_spk=fs;x->pup_ct=pc;x->harv_yr=hy;x->active=1;*sum+=rd;(*cnt)++;ps("[CENTU] Century ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" rd=");pi(rd);ps(" lct=");pi(lct);ps(" lw=");pi(lw);ps(" fs=");pi(fs);ps(" pc=");pi(pc);ps(" hy=");pi(hy);ps("\n");return *cnt-1;}
int centu_init(void){if(init)return -1;st.n_cult=0;st.n_offset=0;st.n_fiber=0;st.n_land=0;st.n_mkt=0;st.t_ros=0;st.t_leaf=0;st.t_width=0;st.t_spike=0;st.t_pup=0;for(int i=0;i<N;i++)centucs[i].active=0;for(int i=0;i<N-2;i++)centuos[i].active=0;for(int i=0;i<N-4;i++)centufs[i].active=0;for(int i=0;i<N-6;i++)centuls[i].active=0;for(int i=0;i<N-6;i++)centums[i].active=0;init=1;ps("[CENTU] Century initialized\n");return 0;}
int centu_cultivation(int lc,int rd,int lct,int lw,int fs,int pc,int hy){return add(centucs,&st.n_cult,&st.t_ros,N,lc,rd,lct,lw,fs,pc,hy);}
int centu_offset_division(int lc,int rd,int lct,int lw,int fs,int pc,int hy){return add(centuos,&st.n_offset,&st.t_leaf,N-2,lc,rd,lct,lw,fs,pc,hy);}
int centu_fiber_harvest(int lc,int rd,int lct,int lw,int fs,int pc,int hy){return add(centufs,&st.n_fiber,&st.t_width,N-4,lc,rd,lct,lw,fs,pc,hy);}
int centu_landscape(int lc,int rd,int lct,int lw,int fs,int pc,int hy){return add(centuls,&st.n_land,&st.t_spike,N-6,lc,rd,lct,lw,fs,pc,hy);}
int centu_market(int lc,int rd,int lct,int lw,int fs,int pc,int hy){return add(centums,&st.n_mkt,&st.t_pup,N-6,lc,rd,lct,lw,fs,pc,hy);}
void centu_report(void){ps("[CENTU] Cult: ");pi(st.n_cult);ps(" Ros=");pi(st.t_ros);ps("\nOffset: ");pi(st.n_offset);ps(" Leaf=");pi(st.t_leaf);ps("\nFiber: ");pi(st.n_fiber);ps(" Width=");pi(st.t_width);ps("\nLand: ");pi(st.n_land);ps(" Spike=");pi(st.t_spike);ps("\nMkt: ");pi(st.n_mkt);ps(" Pup=");pi(st.t_pup);ps("\n");}
void centu_state(void){ps("[CENTU] Cult=");pi(st.n_cult);ps(" Offset=");pi(st.n_offset);ps(" Fiber=");pi(st.n_fiber);ps(" Land=");pi(st.n_land);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Century Admin Demo ===\n\n");centu_init();
/* 1=xeriscape 2=desert_garden 3=container 4=rock_garden 5=mediterranean */
ps("Century cultivation...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,rd=50+(i*10),lct=20+(i*5),lw=5+(i*2),fs=3+(i%5),pc=2+(i%4),hy=5+(i*2);centu_cultivation(lc,rd,lct,lw,fs,pc,hy);}
ps("\nCentury offset division...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,rd=60+(i*8),lct=25+(i*4),lw=6+(i*2),fs=4+(i%4),pc=3+(i%3),hy=6+(i*2);centu_offset_division(lc,rd,lct,lw,fs,pc,hy);}
ps("\nCentury fiber harvest...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,rd=70+(i*6),lct=30+(i*3),lw=7+(i*2),fs=5+(i%3),pc=4+(i*2),hy=7+(i*2);centu_fiber_harvest(lc,rd,lct,lw,fs,pc,hy);}
ps("\nCentury landscape...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,rd=40+(i*12),lct=15+(i*6),lw=4+(i*3),fs=2+(i%5),pc=1+(i%5),hy=4+(i*3);centu_landscape(lc,rd,lct,lw,fs,pc,hy);}
ps("\nCentury market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,rd=80+(i*5),lct=35+(i*3),lw=8+(i*2),fs=6+(i%2),pc=5+(i*2),hy=8+(i*2);centu_market(lc,rd,lct,lw,fs,pc,hy);}
ps("\n");centu_report();centu_state();ps("\n=== Demo Complete ===\n");return 0;}
