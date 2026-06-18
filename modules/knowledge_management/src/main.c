/* knowledge_management: Knowledge management system (v1.0)
 * Knowledge base, expert network, best practices, learning paths, smart search
 */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_ARTICLES     20
#define MAX_EXPERTS      10
#define MAX_PRACTICES    10
#define MAX_COURSES      8
#define MAX_SKILLS       16
#define MAX_TAGS         16

typedef struct {
    int    article_id;
    int    category;
    int    author_id;
    int    views;
    int    rating;
    int    n_versions;
    int    n_tags;
    int    active;
} article_t;

typedef struct {
    int    expert_id;
    int    n_skills;
    int    skill_ids[MAX_SKILLS];
    int    consultation_count;
    int    rating;
    int    active;
} expert_t;

typedef struct {
    int    practice_id;
    int    category;
    int    adoption_count;
    int    effectiveness;
    int    n_steps;
    int    active;
} practice_t;

typedef struct {
    int    course_id;
    int    category;
    int    duration_hours;
    int    n_modules;
    int    n_enrolled;
    int    completion_rate;
    int    active;
} course_t;

typedef struct {
    int    skill_id;
    int    n_experts;
    int    n_articles;
    int    demand_level;
    int    active;
} skill_t;

typedef struct {
    int    tag_id;
    int    n_articles;
    int    n_experts;
    int    active;
} tag_t;

typedef struct {
    int    n_articles;
    int    n_experts;
    int    n_practices;
    int    n_courses;
    int    n_skills;
    int    n_tags;
    int    total_views;
    int    total_consultations;
    int    total_enrollments;
    int    search_queries;
} km_state_t;

static article_t articles[MAX_ARTICLES];
static expert_t experts[MAX_EXPERTS];
static practice_t practices[MAX_PRACTICES];
static course_t courses[MAX_COURSES];
static skill_t skills[MAX_SKILLS];
static tag_t tags[MAX_TAGS];
static km_state_t km;

static int initialized = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}

int km_init(void) {
    if (initialized) return -1;
    km.n_articles = 0; km.n_experts = 0; km.n_practices = 0;
    km.n_courses = 0; km.n_skills = 0; km.n_tags = 0;
    km.total_views = 0; km.total_consultations = 0;
    km.total_enrollments = 0; km.search_queries = 0;
    for (int i = 0; i < MAX_ARTICLES; i++) articles[i].active = 0;
    for (int i = 0; i < MAX_EXPERTS; i++) experts[i].active = 0;
    for (int i = 0; i < MAX_PRACTICES; i++) practices[i].active = 0;
    for (int i = 0; i < MAX_COURSES; i++) courses[i].active = 0;
    for (int i = 0; i < MAX_SKILLS; i++) skills[i].active = 0;
    for (int i = 0; i < MAX_TAGS; i++) tags[i].active = 0;
    initialized = 1;
    print_str("[KM] Knowledge management initialized\n");
    return 0;
}

int km_create_skill(int demand_level) {
    if (km.n_skills >= MAX_SKILLS) return -1;
    skill_t* s = &skills[km.n_skills];
    s->skill_id = km.n_skills;
    s->n_experts = 0;
    s->n_articles = 0;
    s->demand_level = demand_level;
    s->active = 1;
    km.n_skills++;
    print_str("[KM] Skill "); print_int(km.n_skills - 1);
    print_str(" demand="); print_int(demand_level); print_str("\n");
    return km.n_skills - 1;
}

int km_create_tag(void) {
    if (km.n_tags >= MAX_TAGS) return -1;
    tag_t* t = &tags[km.n_tags];
    t->tag_id = km.n_tags;
    t->n_articles = 0;
    t->n_experts = 0;
    t->active = 1;
    km.n_tags++;
    print_str("[KM] Tag "); print_int(km.n_tags - 1); print_str(" created\n");
    return km.n_tags - 1;
}

int km_add_expert(int n_skills, int rating) {
    if (km.n_experts >= MAX_EXPERTS) return -1;
    expert_t* e = &experts[km.n_experts];
    e->expert_id = km.n_experts;
    e->n_skills = n_skills;
    for (int i = 0; i < n_skills && i < MAX_SKILLS; i++) {
        int skill_idx = (km.n_experts * 3 + i) % km.n_skills;
        e->skill_ids[i] = skill_idx;
        if (skill_idx < km.n_skills) skills[skill_idx].n_experts++;
    }
    e->consultation_count = 0;
    e->rating = rating;
    e->active = 1;
    km.n_experts++;
    print_str("[KM] Expert "); print_int(km.n_experts - 1);
    print_str(" skills="); print_int(n_skills);
    print_str(" rating="); print_int(rating); print_str("\n");
    return km.n_experts - 1;
}

int km_publish_article(int category, int author_id, int rating) {
    if (km.n_articles >= MAX_ARTICLES) return -1;
    article_t* a = &articles[km.n_articles];
    a->article_id = km.n_articles;
    a->category = category;
    a->author_id = author_id;
    a->views = 0;
    a->rating = rating;
    a->n_versions = 1;
    a->n_tags = 0;
    a->active = 1;
    km.n_articles++;
    print_str("[KM] Article "); print_int(km.n_articles - 1);
    print_str(" cat="); print_int(category);
    print_str(" author="); print_int(author_id);
    print_str(" rating="); print_int(rating); print_str("\n");
    return km.n_articles - 1;
}

void km_tag_article(int article_id, int tag_id) {
    if (article_id >= km.n_articles || tag_id >= km.n_tags) return;
    articles[article_id].n_tags++;
    tags[tag_id].n_articles++;
    print_str("[KM] Article "); print_int(article_id);
    print_str(" tagged T"); print_int(tag_id); print_str("\n");
}

void km_view_article(int article_id) {
    if (article_id >= km.n_articles) return;
    articles[article_id].views++;
    km.total_views++;
}

void km_consult_expert(int expert_id) {
    if (expert_id >= km.n_experts) return;
    experts[expert_id].consultation_count++;
    km.total_consultations++;
    print_str("[KM] Consulted expert "); print_int(expert_id);
    print_str(" (total: "); print_int(experts[expert_id].consultation_count); print_str(")\n");
}

int km_create_practice(int category, int effectiveness, int steps) {
    if (km.n_practices >= MAX_PRACTICES) return -1;
    practice_t* p = &practices[km.n_practices];
    p->practice_id = km.n_practices;
    p->category = category;
    p->adoption_count = 0;
    p->effectiveness = effectiveness;
    p->n_steps = steps;
    p->active = 1;
    km.n_practices++;
    print_str("[KM] Practice "); print_int(km.n_practices - 1);
    print_str(" cat="); print_int(category);
    print_str(" eff="); print_int(effectiveness); print_str("%");
    print_str(" steps="); print_int(steps); print_str("\n");
    return km.n_practices - 1;
}

void km_adopt_practice(int practice_id) {
    if (practice_id >= km.n_practices) return;
    practices[practice_id].adoption_count++;
    print_str("[KM] Practice "); print_int(practice_id);
    print_str(" adopted (total: "); print_int(practices[practice_id].adoption_count); print_str(")\n");
}

int km_create_course(int category, int hours, int modules) {
    if (km.n_courses >= MAX_COURSES) return -1;
    course_t* c = &courses[km.n_courses];
    c->course_id = km.n_courses;
    c->category = category;
    c->duration_hours = hours;
    c->n_modules = modules;
    c->n_enrolled = 0;
    c->completion_rate = 0;
    c->active = 1;
    km.n_courses++;
    print_str("[KM] Course "); print_int(km.n_courses - 1);
    print_str(" cat="); print_int(category);
    print_str(" hrs="); print_int(hours);
    print_str(" modules="); print_int(modules); print_str("\n");
    return km.n_courses - 1;
}

void km_enroll_course(int course_id, int students) {
    if (course_id >= km.n_courses) return;
    courses[course_id].n_enrolled += students;
    km.total_enrollments += students;
    print_str("[KM] Course "); print_int(course_id);
    print_str(" +"); print_int(students); print_str(" enrolled (total: ");
    print_int(courses[course_id].n_enrolled); print_str(")\n");
}

void km_complete_course(int course_id, int rate) {
    if (course_id >= km.n_courses) return;
    courses[course_id].completion_rate = rate;
    print_str("[KM] Course "); print_int(course_id);
    print_str(" completion rate: "); print_int(rate); print_str("%\n");
}

void km_search(int category_filter, int tag_filter) {
    km.search_queries++;
    print_str("[KM] Search");
    if (category_filter >= 0) { print_str(" cat="); print_int(category_filter); }
    if (tag_filter >= 0) { print_str(" tag="); print_int(tag_filter); }
    print_str(":\n");
    int matches = 0;
    for (int i = 0; i < km.n_articles; i++) {
        int match = 1;
        if (category_filter >= 0 && articles[i].category != category_filter) match = 0;
        if (tag_filter >= 0 && articles[i].n_tags == 0) match = 0;
        if (match) {
            print_str("  A"); print_int(i);
            print_str(" rating="); print_int(articles[i].rating);
            print_str(" views="); print_int(articles[i].views); print_str("\n");
            matches++;
        }
    }
    print_str("  Matches: "); print_int(matches); print_str("\n");
}

void km_expert_recommendation(int skill_id) {
    if (skill_id >= km.n_skills) return;
    print_str("[KM] Expert recommendation for skill "); print_int(skill_id); print_str(":\n");
    int count = 0;
    for (int i = 0; i < km.n_experts; i++) {
        for (int j = 0; j < experts[i].n_skills; j++) {
            if (experts[i].skill_ids[j] == skill_id) {
                print_str("  Expert "); print_int(i);
                print_str(" rating="); print_int(experts[i].rating);
                print_str(" consults="); print_int(experts[i].consultation_count); print_str("\n");
                count++;
                break;
            }
        }
        if (count >= 3) break;
    }
    if (count == 0) print_str("  No experts found\n");
}

void km_print_state(void) {
    print_str("[KM] Articles="); print_int(km.n_articles);
    print_str(" Experts="); print_int(km.n_experts);
    print_str(" Practices="); print_int(km.n_practices);
    print_str(" Courses="); print_int(km.n_courses);
    print_str(" Skills="); print_int(km.n_skills);
    print_str(" Tags="); print_int(km.n_tags);
    print_str("\n");
    print_str("  Total views: "); print_int(km.total_views); print_str("\n");
    print_str("  Consultations: "); print_int(km.total_consultations); print_str("\n");
    print_str("  Enrollments: "); print_int(km.total_enrollments); print_str("\n");
    print_str("  Searches: "); print_int(km.search_queries); print_str("\n");
}

int main(void) {
    print_str("=== Knowledge Management System Demo ===\n\n");
    km_init();

    print_str("Creating skills...\n");
    km_create_skill(3);
    km_create_skill(2);
    km_create_skill(3);
    km_create_skill(1);
    km_create_skill(2);
    km_create_skill(3);

    print_str("\nCreating tags...\n");
    km_create_tag();
    km_create_tag();
    km_create_tag();
    km_create_tag();
    km_create_tag();

    print_str("\nAdding experts...\n");
    km_add_expert(3, 90);
    km_add_expert(2, 85);
    km_add_expert(4, 95);
    km_add_expert(2, 80);
    km_add_expert(3, 88);

    print_str("\nPublishing articles...\n");
    km_publish_article(1, 0, 85);
    km_publish_article(2, 1, 78);
    km_publish_article(1, 2, 92);
    km_publish_article(3, 3, 70);
    km_publish_article(2, 4, 88);
    km_publish_article(1, 0, 80);
    km_publish_article(3, 2, 75);
    km_publish_article(2, 1, 82);

    print_str("\nTagging articles...\n");
    km_tag_article(0, 0);
    km_tag_article(0, 1);
    km_tag_article(1, 2);
    km_tag_article(2, 0);
    km_tag_article(2, 3);
    km_tag_article(3, 4);
    km_tag_article(4, 1);
    km_tag_article(4, 2);
    km_tag_article(5, 0);
    km_tag_article(6, 3);
    km_tag_article(7, 1);

    print_str("\nViewing articles...\n");
    for (int i = 0; i < 10; i++) km_view_article(i % km.n_articles);
    km_view_article(0); km_view_article(0); km_view_article(2);

    print_str("\nConsulting experts...\n");
    km_consult_expert(0);
    km_consult_expert(2);
    km_consult_expert(2);
    km_consult_expert(4);
    km_consult_expert(0);

    print_str("\nCreating best practices...\n");
    km_create_practice(1, 85, 5);
    km_create_practice(2, 90, 4);
    km_create_practice(1, 78, 6);
    km_create_practice(3, 92, 3);
    km_create_practice(2, 80, 5);

    print_str("\nAdopting practices...\n");
    km_adopt_practice(0);
    km_adopt_practice(0);
    km_adopt_practice(1);
    km_adopt_practice(1);
    km_adopt_practice(1);
    km_adopt_practice(3);

    print_str("\nCreating courses...\n");
    km_create_course(1, 20, 5);
    km_create_course(2, 15, 4);
    km_create_course(1, 30, 8);
    km_create_course(3, 10, 3);
    km_create_course(2, 25, 6);

    print_str("\nEnrolling students...\n");
    km_enroll_course(0, 25);
    km_enroll_course(1, 18);
    km_enroll_course(2, 30);
    km_enroll_course(3, 12);
    km_enroll_course(4, 20);

    print_str("\nCompleting courses...\n");
    km_complete_course(0, 85);
    km_complete_course(1, 72);
    km_complete_course(2, 90);

    print_str("\nSearching knowledge base...\n");
    km_search(1, -1);
    km_search(-1, 0);
    km_search(2, 1);

    print_str("\nExpert recommendations...\n");
    km_expert_recommendation(0);
    km_expert_recommendation(2);
    km_expert_recommendation(4);

    print_str("\nFinal state...\n");
    km_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
