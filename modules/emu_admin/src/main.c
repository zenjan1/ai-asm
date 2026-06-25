/* emu_admin: Emu (Dromaius novaehollandiae) Australian large flightless bird (v1.0)
 * Emu outback, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, leg_cm, run_speed, plum_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_cm,rn_sp,pl_idx,age_yr,active;} emu_t;
typedef struct{int n_outback,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_leg,t_rn,t_pl;} emu_state_t;
static emu_t out[N],otf[N-2],otb[N-4],oth[N-6],otm[N-6]; static emu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(emu_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int lg,int rs,int pl,int ay){if(*cnt>=mx)return -1;emu_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_cm=lg;x->rn_sp=rs;x->pl_idx=pl;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[EMU] Emu ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" lg=");pi(lg);ps(" rs=");pi(rs);ps(" pl=");pi(pl);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int emu_init(void){if(init)return -1;st.n_outback=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_rn=0;st.t_pl=0;for(int i=0;i<N;i++)out[i].active=0;for(int i=0;i<N-2;i++)otf[i].active=0;for(int i=0;i<N-4;i++)otb[i].active=0;for(int i=0;i<N-6;i++)oth[i].active=0;for(int i=0;i<N-6;i++)otm[i].active=0;init=1;ps("[EMU] Emu initialized\n");return 0;}
int emu_outback(int lc,int bl,int bw,int lg,int rs,int pl,int ay){return add(out,&st.n_outback,&st.t_ln,N,lc,bl,bw,lg,rs,pl,ay);}
int emu_feeding(int lc,int bl,int bw,int lg,int rs,int pl,int ay){return add(otf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,lg,rs,pl,ay);}
int emu_breeding(int lc,int bl,int bw,int lg,int rs,int pl,int ay){return add(otb,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,lg,rs,pl,ay);}
int emu_health(int lc,int bl,int bw,int lg,int rs,int pl,int ay){return add(oth,&st.n_health,&st.t_rn,N-6,lc,bl,bw,lg,rs,pl,ay);}
int emu_market(int lc,int bl,int bw,int lg,int rs,int pl,int ay){return add(otm,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,lg,rs,pl,ay);}
void emu_report(void){ps("[EMU] Outback: ");pi(st.n_outback);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void emu_state(void){ps("[EMU] Outback=");pi(st.n_outback);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Emu Admin Demo ===\n\n");emu_init();
ps("Emu outback...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*8),bw=40+(i*5),lg=30+(i*4),rs=15+(i*3),pl=(i%8)+1,ay=(i%15)+1;emu_outback(lc,bl,bw,lg,rs,pl,ay);}
ps("\nEmu feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=155+(i*6),bw=42+(i*4),lg=32+(i*3),rs=16+(i*2),pl=(i%7)+1,ay=(i%12)+1;emu_feeding(lc,bl,bw,lg,rs,pl,ay);}
ps("\nEmu breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=160+(i*5),bw=45+(i*3),lg=34+(i*2),rs=14+(i*4),pl=(i%6)+1,ay=(i%10)+1;emu_breeding(lc,bl,bw,lg,rs,pl,ay);}
ps("\nEmu health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=148+(i*9),bw=38+(i*6),lg=29+(i*5),rs=13+(i*5),pl=(i%9)+1,ay=(i%8)+1;emu_health(lc,bl,bw,lg,rs,pl,ay);}
ps("\nEmu market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=165+(i*4),bw=48+(i*3),lg=36+(i*2),rs=17+(i*2),pl=(i%5)+1,ay=(i%7)+1;emu_market(lc,bl,bw,lg,rs,pl,ay);}
ps("\n");emu_report();emu_state();ps("\n=== Demo Complete ===\n");return 0;}
