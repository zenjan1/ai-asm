/* possum_admin: Possum (Trichosurus vulpecula) common brushtail possum marsupial (v1.0)
 * Possum woodland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, climb_speed, fr_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,cl_sp,fr_idx,age_yr,active;} poss_t;
typedef struct{int n_woodland,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_cl,t_fr;} poss_state_t;
static poss_t arr_w[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static poss_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(poss_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int cs,int fri,int ay){if(*cnt>=mx)return -1;poss_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->cl_sp=cs;x->fr_idx=fri;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[POSS] Possum ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" cs=");pi(cs);ps(" fri=");pi(fri);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int possum_init(void){if(init)return -1;st.n_woodland=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_cl=0;st.t_fr=0;for(int i=0;i<N;i++)arr_w[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[POSS] Possum initialized\n");return 0;}
int possum_woodland(int lc,int bl,int bw,int tc,int cs,int fri,int ay){return add(arr_w,&st.n_woodland,&st.t_ln,N,lc,bl,bw,tc,cs,fri,ay);}
int possum_feeding(int lc,int bl,int bw,int tc,int cs,int fri,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,cs,fri,ay);}
int possum_breeding(int lc,int bl,int bw,int tc,int cs,int fri,int ay){return add(arr_b,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,cs,fri,ay);}
int possum_health(int lc,int bl,int bw,int tc,int cs,int fri,int ay){return add(arr_h,&st.n_health,&st.t_cl,N-6,lc,bl,bw,tc,cs,fri,ay);}
int possum_market(int lc,int bl,int bw,int tc,int cs,int fri,int ay){return add(arr_m,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,cs,fri,ay);}
void possum_report(void){ps("[POSS] Woodland: ");pi(st.n_woodland);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void possum_state(void){ps("[POSS] Woodland=");pi(st.n_woodland);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Possum Admin Demo ===\n\n");possum_init();
ps("Possum woodland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=45+(i*3),bw=2+(i*1),tc=35+(i*2),cs=3+(i*1),fri=(i%6)+1,ay=(i%8)+1;possum_woodland(lc,bl,bw,tc,cs,fri,ay);}
ps("\nPossum feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=47+(i*2),bw=2+(i*1),tc=37+(i*2),cs=4+(i*1),fri=(i%5)+1,ay=(i%7)+1;possum_feeding(lc,bl,bw,tc,cs,fri,ay);}
ps("\nPossum breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=50+(i*2),bw=3+(i*1),tc=39+(i*1),cs=2+(i*2),fri=(i%4)+1,ay=(i%6)+1;possum_breeding(lc,bl,bw,tc,cs,fri,ay);}
ps("\nPossum health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=42+(i*4),bw=2+(i*1),tc=33+(i*2),cs=2+(i*3),fri=(i%7)+1,ay=(i%5)+1;possum_health(lc,bl,bw,tc,cs,fri,ay);}
ps("\nPossum market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=53+(i*2),bw=3+(i*1),tc=41+(i*1),cs=5+(i*1),fri=(i%3)+1,ay=(i%4)+1;possum_market(lc,bl,bw,tc,cs,fri,ay);}
ps("\n");possum_report();possum_state();ps("\n=== Demo Complete ===\n");return 0;}
