/* antelope_admin: Antelope (Antilope cervicapra) blackbuck swift ungulate (v1.0)
 * Antelope savanna, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, run_speed, hair_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,run_sp,hr_idx,age_yr,active;} antl_t;
typedef struct{int n_savan,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_run,t_hr;} antl_state_t;
static antl_t asl[N],asf[N-2],asb[N-4],ash[N-6],asm_arr[N-6]; static antl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(antl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int rs,int hi,int ay){if(*cnt>=mx)return -1;antl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->run_sp=rs;x->hr_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ANTL] Antelope ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" rs=");pi(rs);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int antelope_init(void){if(init)return -1;st.n_savan=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_run=0;st.t_hr=0;for(int i=0;i<N;i++)asl[i].active=0;for(int i=0;i<N-2;i++)asf[i].active=0;for(int i=0;i<N-4;i++)asb[i].active=0;for(int i=0;i<N-6;i++)ash[i].active=0;for(int i=0;i<N-6;i++)asm_arr[i].active=0;init=1;ps("[ANTL] Antelope initialized\n");return 0;}
int antelope_savanna(int lc,int bl,int bw,int hc,int rs,int hi,int ay){return add(asl,&st.n_savan,&st.t_ln,N,lc,bl,bw,hc,rs,hi,ay);}
int antelope_feeding(int lc,int bl,int bw,int hc,int rs,int hi,int ay){return add(asf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,rs,hi,ay);}
int antelope_breeding(int lc,int bl,int bw,int hc,int rs,int hi,int ay){return add(asb,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,rs,hi,ay);}
int antelope_health(int lc,int bl,int bw,int hc,int rs,int hi,int ay){return add(ash,&st.n_health,&st.t_run,N-6,lc,bl,bw,hc,rs,hi,ay);}
int antelope_market(int lc,int bl,int bw,int hc,int rs,int hi,int ay){return add(asm_arr,&st.n_mkt,&st.t_hr,N-6,lc,bl,bw,hc,rs,hi,ay);}
void antelope_report(void){ps("[ANTL] Savan: ");pi(st.n_savan);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Hr=");pi(st.t_hr);ps("\n");}
void antelope_state(void){ps("[ANTL] Savan=");pi(st.n_savan);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Antelope Admin Demo ===\n\n");antelope_init();
ps("Antelope savanna...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*5),bw=30+(i*5),hc=15+(i*3),rs=60+(i*4),hi=(i%10)+1,ay=(i%12)+1;antelope_savanna(lc,bl,bw,hc,rs,hi,ay);}
ps("\nAntelope feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=105+(i*4),bw=35+(i*4),hc=16+(i*2),rs=65+(i*3),hi=(i%8)+1,ay=(i%10)+1;antelope_feeding(lc,bl,bw,hc,rs,hi,ay);}
ps("\nAntelope breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=110+(i*3),bw=40+(i*3),hc=18+(i*2),rs=55+(i*5),hi=(i%6)+1,ay=(i%8)+1;antelope_breeding(lc,bl,bw,hc,rs,hi,ay);}
ps("\nAntelope health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=98+(i*6),bw=28+(i*6),hc=14+(i*4),rs=50+(i*6),hi=(i%12)+1,ay=(i%7)+1;antelope_health(lc,bl,bw,hc,rs,hi,ay);}
ps("\nAntelope market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=115+(i*3),bw=45+(i*3),hc=20+(i*2),rs=70+(i*3),hi=(i%5)+1,ay=(i%6)+1;antelope_market(lc,bl,bw,hc,rs,hi,ay);}
ps("\n");antelope_report();antelope_state();ps("\n=== Demo Complete ===\n");return 0;}
