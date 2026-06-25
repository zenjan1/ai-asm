/* grysbok_admin: Grysbok (Raphicerus sharpei) small shy nocturnal antelope (v1.0)
 * Grysbok thicket, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, sprint_speed, gr_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,sp_sp,gr_idx,age_yr,active;} grsb_t;
typedef struct{int n_thick,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_sp,t_gr;} grsb_state_t;
static grsb_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static grsb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(grsb_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int ss,int gi,int ay){if(*cnt>=mx)return -1;grsb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->sp_sp=ss;x->gr_idx=gi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GRSB] Grysbok ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" ss=");pi(ss);ps(" gi=");pi(gi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int grysbok_init(void){if(init)return -1;st.n_thick=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_sp=0;st.t_gr=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[GRSB] Grysbok initialized\n");return 0;}
int grysbok_thicket(int lc,int bl,int bw,int ec,int ss,int gi,int ay){return add(svl,&st.n_thick,&st.t_ln,N,lc,bl,bw,ec,ss,gi,ay);}
int grysbok_feeding(int lc,int bl,int bw,int ec,int ss,int gi,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,ss,gi,ay);}
int grysbok_breeding(int lc,int bl,int bw,int ec,int ss,int gi,int ay){return add(svb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,ss,gi,ay);}
int grysbok_health(int lc,int bl,int bw,int ec,int ss,int gi,int ay){return add(svh,&st.n_health,&st.t_sp,N-6,lc,bl,bw,ec,ss,gi,ay);}
int grysbok_market(int lc,int bl,int bw,int ec,int ss,int gi,int ay){return add(svm,&st.n_mkt,&st.t_gr,N-6,lc,bl,bw,ec,ss,gi,ay);}
void grysbok_report(void){ps("[GRSB] Thick: ");pi(st.n_thick);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Sp=");pi(st.t_sp);ps("\nMkt: ");pi(st.n_mkt);ps(" Gr=");pi(st.t_gr);ps("\n");}
void grysbok_state(void){ps("[GRSB] Thick=");pi(st.n_thick);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Grysbok Admin Demo ===\n\n");grysbok_init();
ps("Grysbok thicket...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=75+(i*3),bw=11+(i*1),ec=5+(i*1),ss=14+(i*2),gi=(i%8)+1,ay=(i%10)+1;grysbok_thicket(lc,bl,bw,ec,ss,gi,ay);}
ps("\nGrysbok feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=77+(i*2),bw=12+(i*1),ec=6+(i*1),ss=15+(i*1),gi=(i%6)+1,ay=(i%8)+1;grysbok_feeding(lc,bl,bw,ec,ss,gi,ay);}
ps("\nGrysbok breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=79+(i*2),bw=13+(i*1),ec=6+(i*1),ss=13+(i*2),gi=(i%5)+1,ay=(i%7)+1;grysbok_breeding(lc,bl,bw,ec,ss,gi,ay);}
ps("\nGrysbok health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=73+(i*4),bw=10+(i*2),ec=4+(i*1),ss=12+(i*3),gi=(i%10)+1,ay=(i%6)+1;grysbok_health(lc,bl,bw,ec,ss,gi,ay);}
ps("\nGrysbok market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=81+(i*1),bw=14+(i*1),ec=7+(i*1),ss=17+(i*1),gi=(i%4)+1,ay=(i%5)+1;grysbok_market(lc,bl,bw,ec,ss,gi,ay);}
ps("\n");grysbok_report();grysbok_state();ps("\n=== Demo Complete ===\n");return 0;}
