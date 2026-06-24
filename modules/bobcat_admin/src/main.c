/* bobcat_admin: Bobcat (Lynx rufus) wildlife monitoring and tracking (v1.0)
 * Bobcat tracking, habitat assessment, hunting analysis, breeding survey, market
 * Features: weight kg, territory km, prey count, sightings, den depth, activity hr
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,wght_kg,territor_km,prey_ct,sight,den_dp,activ_hr,active;} bob_t;
typedef struct{int n_track,n_habitat,n_hunt,n_breed,n_mkt,t_wght,t_terr,t_prey,t_sight,t_den;} bob_state_t;
static bob_t bobps[N],bobhs[N-2],bobhunts[N-4],bobbrs[N-6],bobms[N-6]; static bob_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bob_t*a,int*cnt,int*sum,int mx,int lc,int wg,int tr,int pc,int si,int dd,int ah){if(*cnt>=mx)return -1;bob_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->wght_kg=wg;x->territor_km=tr;x->prey_ct=pc;x->sight=si;x->den_dp=dd;x->activ_hr=ah;x->active=1;*sum+=wg;(*cnt)++;ps("[BOB] Bobcat ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" wg=");pi(wg);ps(" tr=");pi(tr);ps(" pc=");pi(pc);ps(" si=");pi(si);ps(" dd=");pi(dd);ps(" ah=");pi(ah);ps("\n");return *cnt-1;}
int bob_init(void){if(init)return -1;st.n_track=0;st.n_habitat=0;st.n_hunt=0;st.n_breed=0;st.n_mkt=0;st.t_wght=0;st.t_terr=0;st.t_prey=0;st.t_sight=0;st.t_den=0;for(int i=0;i<N;i++)bobps[i].active=0;for(int i=0;i<N-2;i++)bobhs[i].active=0;for(int i=0;i<N-4;i++)bobhunts[i].active=0;for(int i=0;i<N-6;i++)bobbrs[i].active=0;for(int i=0;i<N-6;i++)bobms[i].active=0;init=1;ps("[BOB] Bobcat initialized\n");return 0;}
int bob_tracking(int lc,int wg,int tr,int pc,int si,int dd,int ah){return add(bobps,&st.n_track,&st.t_wght,N,lc,wg,tr,pc,si,dd,ah);}
int bob_habitat(int lc,int wg,int tr,int pc,int si,int dd,int ah){return add(bobhs,&st.n_habitat,&st.t_terr,N-2,lc,wg,tr,pc,si,dd,ah);}
int bob_hunting(int lc,int wg,int tr,int pc,int si,int dd,int ah){return add(bobhunts,&st.n_hunt,&st.t_prey,N-4,lc,wg,tr,pc,si,dd,ah);}
int bob_breeding(int lc,int wg,int tr,int pc,int si,int dd,int ah){return add(bobbrs,&st.n_breed,&st.t_sight,N-6,lc,wg,tr,pc,si,dd,ah);}
int bob_market(int lc,int wg,int tr,int pc,int si,int dd,int ah){return add(bobms,&st.n_mkt,&st.t_den,N-6,lc,wg,tr,pc,si,dd,ah);}
void bob_report(void){ps("[BOB] Track: ");pi(st.n_track);ps(" Wght=");pi(st.t_wght);ps("\nHabitat: ");pi(st.n_habitat);ps(" Terr=");pi(st.t_terr);ps("\nHunt: ");pi(st.n_hunt);ps(" Prey=");pi(st.t_prey);ps("\nBreed: ");pi(st.n_breed);ps(" Sight=");pi(st.t_sight);ps("\nMkt: ");pi(st.n_mkt);ps(" Den=");pi(st.t_den);ps("\n");}
void bob_state(void){ps("[BOB] Track=");pi(st.n_track);ps(" Habitat=");pi(st.n_habitat);ps(" Hunt=");pi(st.n_hunt);ps(" Breed=");pi(st.n_breed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bobcat Admin Demo ===\n\n");bob_init();
/* 1=forest 2=desert 3=swamp 4=mountain 5=grassland */
ps("Bobcat tracking...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,wg=8+(i*3),tr=5+(i*2),pc=3+(i%4),si=2+(i%5),dd=20+(i*10),ah=18+(i%6);bob_tracking(lc,wg,tr,pc,si,dd,ah);}
ps("\nBobcat habitat...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,wg=10+(i*2),tr=8+(i*3),pc=4+(i%3),si=3+(i%4),dd=25+(i*8),ah=20+(i%4);bob_habitat(lc,wg,tr,pc,si,dd,ah);}
ps("\nBobcat hunting...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,wg=12+(i*2),tr=6+(i*2),pc=5+(i%4),si=4+(i%3),dd=15+(i*12),ah=22+(i%2);bob_hunting(lc,wg,tr,pc,si,dd,ah);}
ps("\nBobcat breeding...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,wg=6+(i*3),tr=4+(i*2),pc=2+(i%3),si=1+(i%4),dd=30+(i*5),ah=19+(i%5);bob_breeding(lc,wg,tr,pc,si,dd,ah);}
ps("\nBobcat market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,wg=14+(i*2),tr=10+(i*2),pc=6+(i%3),si=5+(i%3),dd=22+(i*10),ah=21+(i%3);bob_market(lc,wg,tr,pc,si,dd,ah);}
ps("\n");bob_report();bob_state();ps("\n=== Demo Complete ===\n");return 0;}
