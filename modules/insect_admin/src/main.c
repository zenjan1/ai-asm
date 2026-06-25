/* insect_admin: Insect (Insecta) common arthropod (v1.0)
 * Insect field, feeding, breeding, health, market
 * Features: body_len_mm, body_wt_mg, wing_mm, fly_speed, segment_idx, age_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wng_mm,fl_sp,sg_idx,age_wk,active;} ins_t;
typedef struct{int n_field,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wng,t_fl,t_sg;} ins_state_t;
static ins_t fld[N],flf[N-2],flb[N-4],flh[N-6],flm[N-6]; static ins_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ins_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wm,int fs,int si,int aw){if(*cnt>=mx)return -1;ins_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wng_mm=wm;x->fl_sp=fs;x->sg_idx=si;x->age_wk=aw;x->active=1;*sum+=bl;(*cnt)++;ps("[INS] Insect ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wm=");pi(wm);ps(" fs=");pi(fs);ps(" si=");pi(si);ps(" aw=");pi(aw);ps("\n");return *cnt-1;}
int insect_init(void){if(init)return -1;st.n_field=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wng=0;st.t_fl=0;st.t_sg=0;for(int i=0;i<N;i++)fld[i].active=0;for(int i=0;i<N-2;i++)flf[i].active=0;for(int i=0;i<N-4;i++)flb[i].active=0;for(int i=0;i<N-6;i++)flh[i].active=0;for(int i=0;i<N-6;i++)flm[i].active=0;init=1;ps("[INS] Insect initialized\n");return 0;}
int insect_field(int lc,int bl,int bw,int wm,int fs,int si,int aw){return add(fld,&st.n_field,&st.t_ln,N,lc,bl,bw,wm,fs,si,aw);}
int insect_feeding(int lc,int bl,int bw,int wm,int fs,int si,int aw){return add(flf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wm,fs,si,aw);}
int insect_breeding(int lc,int bl,int bw,int wm,int fs,int si,int aw){return add(flb,&st.n_breed,&st.t_wng,N-4,lc,bl,bw,wm,fs,si,aw);}
int insect_health(int lc,int bl,int bw,int wm,int fs,int si,int aw){return add(flh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wm,fs,si,aw);}
int insect_market(int lc,int bl,int bw,int wm,int fs,int si,int aw){return add(flm,&st.n_mkt,&st.t_sg,N-6,lc,bl,bw,wm,fs,si,aw);}
void insect_report(void){ps("[INS] Field: ");pi(st.n_field);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wng=");pi(st.t_wng);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Sg=");pi(st.t_sg);ps("\n");}
void insect_state(void){ps("[INS] Field=");pi(st.n_field);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Insect Admin Demo ===\n\n");insect_init();
ps("Insect field...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*2),bw=50+(i*10),wm=8+(i*2),fs=5+(i*3),si=(i%6)+1,aw=(i%8)+1;insect_field(lc,bl,bw,wm,fs,si,aw);}
ps("\nInsect feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i*2),bw=55+(i*8),wm=9+(i*2),fs=6+(i*2),si=(i%5)+1,aw=(i%7)+1;insect_feeding(lc,bl,bw,wm,fs,si,aw);}
ps("\nInsect breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i*1),bw=60+(i*6),wm=10+(i*1),fs=4+(i*4),si=(i%4)+1,aw=(i%6)+1;insect_breeding(lc,bl,bw,wm,fs,si,aw);}
ps("\nInsect health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*3),bw=45+(i*12),wm=7+(i*3),fs=3+(i*5),si=(i%7)+1,aw=(i%5)+1;insect_health(lc,bl,bw,wm,fs,si,aw);}
ps("\nInsect market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i*1),bw=65+(i*5),wm=11+(i*1),fs=7+(i*2),si=(i%3)+1,aw=(i%4)+1;insect_market(lc,bl,bw,wm,fs,si,aw);}
ps("\n");insect_report();insect_state();ps("\n=== Demo Complete ===\n");return 0;}
