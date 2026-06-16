/* community_hub: Community center for forums, code sharing, collaboration, and knowledge (v47.0) */

#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ===== Constants ===== */
#define MAX_USERS          64
#define MAX_TOPICS         128
#define MAX_REPLIES        512
#define MAX_SNIPPETS       128
#define MAX_SNIPPET_VER    16
#define MAX_PROJECTS       32
#define MAX_TASKS          128
#define MAX_WIKI           128
#define MAX_NOTIFICATIONS  256
#define MAX_SUBSCRIPTIONS  128

#define MAX_NAME           32
#define MAX_EMAIL          64
#define MAX_PASSWORD       32
#define MAX_TITLE          80
#define MAX_BODY           512
#define MAX_CODE           512
#define MAX_TAG            24
#define MAX_TAGS           8
#define MAX_CATEGORY       32
#define MAX_DESC           128
#define MAX_MSG            128

/* Permission levels */
#define PERM_GUEST         0
#define PERM_MEMBER        1
#define PERM_MODERATOR     2
#define PERM_ADMIN         3

/* Topic status */
#define TOPIC_OPEN         0
#define TOPIC_PINNED       1
#define TOPIC_LOCKED       2
#define TOPIC_CLOSED       3

/* Task status */
#define TASK_TODO          0
#define TASK_IN_PROGRESS   1
#define TASK_REVIEW        2
#define TASK_DONE          3

/* Notification types */
#define NOTIFY_REPLY       0
#define NOTIFY_MENTION     1
#define NOTIFY_ASSIGN      2
#define NOTIFY_UPDATE      3
#define NOTIFY_SYSTEM      4

/* ===== Structs ===== */

typedef struct {
    int   user_id;
    char  name[MAX_NAME];
    char  email[MAX_EMAIL];
    char  password[MAX_PASSWORD];
    int   permission;
    int   active;
    int   created_at;
    int   post_count;
    int   reputation;
} user_t;

typedef struct {
    int   reply_id;
    int   topic_id;
    int   author_id;
    char  body[MAX_BODY];
    int   created_at;
    int   upvotes;
    int   deleted;
} reply_t;

typedef struct {
    int   topic_id;
    int   author_id;
    char  title[MAX_TITLE];
    char  body[MAX_BODY];
    char  category[MAX_CATEGORY];
    char  tags[MAX_TAGS][MAX_TAG];
    int   tag_count;
    int   status;
    int   reply_count;
    int   views;
    int   created_at;
    int   deleted;
} topic_t;

typedef struct {
    int   version;
    char  code[MAX_CODE];
    char  message[MAX_DESC];
    int   committed_at;
} snippet_ver_t;

typedef struct {
    int   snippet_id;
    int   author_id;
    char  title[MAX_TITLE];
    char  language[MAX_NAME];
    char  tags[MAX_TAGS][MAX_TAG];
    int   tag_count;
    int   current_ver;
    int   version_count;
    snippet_ver_t versions[MAX_SNIPPET_VER];
    int   forks;
    int   stars;
    int   created_at;
    int   deleted;
} snippet_t;

typedef struct {
    int   task_id;
    int   project_id;
    int   assignee_id;
    char  title[MAX_TITLE];
    char  desc[MAX_DESC];
    int   status;
    int   priority;
    int   created_at;
    int   updated_at;
} task_t;

typedef struct {
    int   project_id;
    int   owner_id;
    char  name[MAX_NAME];
    char  desc[MAX_DESC];
    int   members[MAX_USERS];
    int   member_count;
    int   task_count;
    int   created_at;
    int   active;
} project_t;

typedef struct {
    int   wiki_id;
    int   author_id;
    char  title[MAX_TITLE];
    char  body[MAX_BODY];
    char  category[MAX_CATEGORY];
    int   views;
    int   created_at;
    int   updated_at;
    int   deleted;
} wiki_t;

typedef struct {
    int   notif_id;
    int   user_id;
    int   type;
    char  message[MAX_MSG];
    int   read_flag;
    int   created_at;
} notification_t;

typedef struct {
    int   sub_id;
    int   user_id;
    int   topic_id;
    int   project_id;
    int   active;
} subscription_t;

/* ===== Global state ===== */
static user_t          users[MAX_USERS];
static int             user_count = 0;
static int             next_user_id = 1;

static topic_t         topics[MAX_TOPICS];
static int             topic_count = 0;
static int             next_topic_id = 1;

static reply_t         replies[MAX_REPLIES];
static int             reply_count = 0;
static int             next_reply_id = 1;

static snippet_t       snippets[MAX_SNIPPETS];
static int             snippet_count = 0;
static int             next_snippet_id = 1;

static project_t       projects[MAX_PROJECTS];
static int             project_count = 0;
static int             next_project_id = 1;

static task_t          tasks[MAX_TASKS];
static int             task_count = 0;
static int             next_task_id = 1;

static wiki_t          wiki_pages[MAX_WIKI];
static int             wiki_count = 0;
static int             next_wiki_id = 1;

static notification_t  notifications[MAX_NOTIFICATIONS];
static int             notif_count = 0;
static int             next_notif_id = 1;

static subscription_t  subscriptions[MAX_SUBSCRIPTIONS];
static int             sub_count = 0;
static int             next_sub_id = 1;

static int             clock = 0;
static int             current_user = -1;

/* ===== String utilities ===== */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strstr(const char *haystack, const char *needle) {
    int hl = my_strlen(haystack), nl = my_strlen(needle);
    if (nl == 0 || nl > hl) return 0;
    for (int i = 0; i <= hl - nl; i++) {
        int j = 0; while (j < nl && haystack[i+j] == needle[j]) j++;
        if (j == nl) return 1;
    }
    return 0;
}

/* ===== Print helpers ===== */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}
static void print_sep(void) {
    print_str("  ==========================================================\n");
}

/* ===== User system ===== */
int user_register(const char *name, const char *email, const char *password) {
    if (user_count >= MAX_USERS) return -1;
    for (int i = 0; i < user_count; i++) {
        if (users[i].active && my_strcmp(users[i].email, email) == 0) return -2;
    }
    int idx = user_count++;
    users[idx].user_id = next_user_id++;
    my_strncpy(users[idx].name, name, MAX_NAME - 1);
    my_strncpy(users[idx].email, email, MAX_EMAIL - 1);
    my_strncpy(users[idx].password, password, MAX_PASSWORD - 1);
    users[idx].permission = PERM_MEMBER;
    users[idx].active = 1;
    users[idx].created_at = ++clock;
    users[idx].post_count = 0;
    users[idx].reputation = 0;
    return users[idx].user_id;
}

int user_authenticate(const char *email, const char *password) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].active &&
            my_strcmp(users[i].email, email) == 0 &&
            my_strcmp(users[i].password, password) == 0) {
            current_user = users[i].user_id;
            return users[i].user_id;
        }
    }
    return -1;
}

int user_set_permission(int user_id, int permission) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id && users[i].active) {
            if (users[i].permission >= permission) return -1;
            users[i].permission = permission;
            return 0;
        }
    }
    return -1;
}

int user_find_by_id(int user_id, user_t *out) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == user_id && users[i].active) {
            *out = users[i];
            return 0;
        }
    }
    return -1;
}

/* ===== Notification system ===== */
int notify_push(int user_id, int type, const char *message) {
    if (notif_count >= MAX_NOTIFICATIONS) return -1;
    int idx = notif_count++;
    notifications[idx].notif_id = next_notif_id++;
    notifications[idx].user_id = user_id;
    notifications[idx].type = type;
    my_strncpy(notifications[idx].message, message, MAX_MSG - 1);
    notifications[idx].read_flag = 0;
    notifications[idx].created_at = ++clock;
    return notifications[idx].notif_id;
}

int notify_get_unread(int user_id, int *ids, int max_results) {
    int found = 0;
    for (int i = 0; i < notif_count && found < max_results; i++) {
        if (notifications[i].user_id == user_id && !notifications[i].read_flag) {
            ids[found++] = notifications[i].notif_id;
        }
    }
    return found;
}

int notify_mark_read(int notif_id) {
    for (int i = 0; i < notif_count; i++) {
        if (notifications[i].notif_id == notif_id) {
            notifications[i].read_flag = 1;
            return 0;
        }
    }
    return -1;
}

int subscribe(int user_id, int topic_id, int project_id) {
    if (sub_count >= MAX_SUBSCRIPTIONS) return -1;
    int idx = sub_count++;
    subscriptions[idx].sub_id = next_sub_id++;
    subscriptions[idx].user_id = user_id;
    subscriptions[idx].topic_id = topic_id;
    subscriptions[idx].project_id = project_id;
    subscriptions[idx].active = 1;
    return subscriptions[idx].sub_id;
}

/* ===== Discussion forums ===== */
int forum_create_topic(int author_id, const char *title, const char *body,
                       const char *category, const char *tag1, const char *tag2) {
    if (topic_count >= MAX_TOPICS) return -1;
    int idx = topic_count++;
    topics[idx].topic_id = next_topic_id++;
    topics[idx].author_id = author_id;
    my_strncpy(topics[idx].title, title, MAX_TITLE - 1);
    my_strncpy(topics[idx].body, body, MAX_BODY - 1);
    my_strncpy(topics[idx].category, category, MAX_CATEGORY - 1);
    topics[idx].tag_count = 0;
    if (tag1 && tag1[0]) {
        my_strncpy(topics[idx].tags[topics[idx].tag_count], tag1, MAX_TAG - 1);
        topics[idx].tag_count++;
    }
    if (tag2 && tag2[0]) {
        my_strncpy(topics[idx].tags[topics[idx].tag_count], tag2, MAX_TAG - 1);
        topics[idx].tag_count++;
    }
    topics[idx].status = TOPIC_OPEN;
    topics[idx].reply_count = 0;
    topics[idx].views = 0;
    topics[idx].created_at = ++clock;
    topics[idx].deleted = 0;
    /* Increment author post count */
    for (int i = 0; i < user_count; i++) {
        if (users[i].user_id == author_id) { users[i].post_count++; break; }
    }
    notify_push(author_id, NOTIFY_SYSTEM, "Topic created successfully");
    return topics[idx].topic_id;
}

int forum_add_reply(int topic_id, int author_id, const char *body) {
    if (reply_count >= MAX_REPLIES) return -1;
    for (int i = 0; i < topic_count; i++) {
        if (topics[i].topic_id == topic_id && !topics[i].deleted &&
            topics[i].status != TOPIC_LOCKED) {
            int idx = reply_count++;
            replies[idx].reply_id = next_reply_id++;
            replies[idx].topic_id = topic_id;
            replies[idx].author_id = author_id;
            my_strncpy(replies[idx].body, body, MAX_BODY - 1);
            replies[idx].created_at = ++clock;
            replies[idx].upvotes = 0;
            replies[idx].deleted = 0;
            topics[i].reply_count++;
            for (int u = 0; u < user_count; u++) {
                if (users[u].user_id == author_id) { users[u].post_count++; break; }
            }
            /* Notify topic author */
            char msg[MAX_MSG];
            my_strncpy(msg, "New reply on: ", MAX_MSG - 1);
            int ml = my_strlen(msg);
            my_strncpy(msg + ml, topics[i].title, MAX_MSG - ml - 1);
            notify_push(topics[i].author_id, NOTIFY_REPLY, msg);
            /* Notify subscribers */
            for (int s = 0; s < sub_count; s++) {
                if (subscriptions[s].topic_id == topic_id &&
                    subscriptions[s].user_id != author_id &&
                    subscriptions[s].active) {
                    notify_push(subscriptions[s].user_id, NOTIFY_REPLY, msg);
                }
            }
            return replies[idx].reply_id;
        }
    }
    return -1;
}

int forum_reply_upvote(int reply_id) {
    for (int i = 0; i < reply_count; i++) {
        if (replies[i].reply_id == reply_id && !replies[i].deleted) {
            replies[i].upvotes++;
            for (int u = 0; u < user_count; u++) {
                if (users[u].user_id == replies[i].author_id) {
                    users[u].reputation++;
                    break;
                }
            }
            return 0;
        }
    }
    return -1;
}

int forum_set_status(int topic_id, int status) {
    for (int i = 0; i < topic_count; i++) {
        if (topics[i].topic_id == topic_id && !topics[i].deleted) {
            topics[i].status = status;
            return 0;
        }
    }
    return -1;
}

int forum_search_topics(const char *keyword, int *result_ids, int max_results) {
    int found = 0;
    for (int i = 0; i < topic_count && found < max_results; i++) {
        if (topics[i].deleted) continue;
        if (my_strstr(topics[i].title, keyword) ||
            my_strstr(topics[i].body, keyword) ||
            my_strstr(topics[i].category, keyword)) {
            result_ids[found++] = topics[i].topic_id;
        } else {
            for (int t = 0; t < topics[i].tag_count; t++) {
                if (my_strstr(topics[i].tags[t], keyword)) {
                    result_ids[found++] = topics[i].topic_id;
                    break;
                }
            }
        }
    }
    return found;
}

/* ===== Code sharing ===== */
int code_publish(int author_id, const char *title, const char *language,
                 const char *code, const char *tag1) {
    if (snippet_count >= MAX_SNIPPETS) return -1;
    int idx = snippet_count++;
    snippets[idx].snippet_id = next_snippet_id++;
    snippets[idx].author_id = author_id;
    my_strncpy(snippets[idx].title, title, MAX_TITLE - 1);
    my_strncpy(snippets[idx].language, language, MAX_NAME - 1);
    snippets[idx].tag_count = 0;
    if (tag1 && tag1[0]) {
        my_strncpy(snippets[idx].tags[0], tag1, MAX_TAG - 1);
        snippets[idx].tag_count = 1;
    }
    snippets[idx].current_ver = 0;
    snippets[idx].version_count = 1;
    snippets[idx].versions[0].version = 1;
    my_strncpy(snippets[idx].versions[0].code, code, MAX_CODE - 1);
    my_strncpy(snippets[idx].versions[0].message, "Initial commit", MAX_DESC - 1);
    snippets[idx].versions[0].committed_at = ++clock;
    snippets[idx].forks = 0;
    snippets[idx].stars = 0;
    snippets[idx].created_at = clock;
    snippets[idx].deleted = 0;
    return snippets[idx].snippet_id;
}

int code_commit(int snippet_id, const char *code, const char *message) {
    for (int i = 0; i < snippet_count; i++) {
        if (snippets[i].snippet_id == snippet_id && !snippets[i].deleted) {
            if (snippets[i].version_count >= MAX_SNIPPET_VER) return -1;
            int v = snippets[i].version_count;
            snippets[i].versions[v].version = v + 1;
            my_strncpy(snippets[i].versions[v].code, code, MAX_CODE - 1);
            my_strncpy(snippets[i].versions[v].message, message, MAX_DESC - 1);
            snippets[i].versions[v].committed_at = ++clock;
            snippets[i].version_count++;
            snippets[i].current_ver = v;
            return v + 1;
        }
    }
    return -1;
}

int code_star(int snippet_id) {
    for (int i = 0; i < snippet_count; i++) {
        if (snippets[i].snippet_id == snippet_id && !snippets[i].deleted) {
            snippets[i].stars++;
            return 0;
        }
    }
    return -1;
}

int code_fork(int snippet_id, int new_author_id) {
    for (int i = 0; i < snippet_count; i++) {
        if (snippets[i].snippet_id == snippet_id && !snippets[i].deleted) {
            snippets[i].forks++;
            return code_publish(new_author_id, snippets[i].title,
                                snippets[i].language,
                                snippets[i].versions[snippets[i].current_ver].code,
                                "forked");
        }
    }
    return -1;
}

/* ===== Collaborative development ===== */
int project_create(int owner_id, const char *name, const char *desc) {
    if (project_count >= MAX_PROJECTS) return -1;
    int idx = project_count++;
    projects[idx].project_id = next_project_id++;
    projects[idx].owner_id = owner_id;
    my_strncpy(projects[idx].name, name, MAX_NAME - 1);
    my_strncpy(projects[idx].desc, desc, MAX_DESC - 1);
    projects[idx].members[0] = owner_id;
    projects[idx].member_count = 1;
    projects[idx].task_count = 0;
    projects[idx].created_at = ++clock;
    projects[idx].active = 1;
    return projects[idx].project_id;
}

int project_add_member(int project_id, int user_id) {
    for (int i = 0; i < project_count; i++) {
        if (projects[i].project_id == project_id && projects[i].active) {
            if (projects[i].member_count >= MAX_USERS) return -1;
            projects[i].members[projects[i].member_count++] = user_id;
            return 0;
        }
    }
    return -1;
}

int task_create(int project_id, int assignee_id, const char *title,
                const char *desc, int priority) {
    if (task_count >= MAX_TASKS) return -1;
    for (int i = 0; i < project_count; i++) {
        if (projects[i].project_id == project_id && projects[i].active) {
            int idx = task_count++;
            tasks[idx].task_id = next_task_id++;
            tasks[idx].project_id = project_id;
            tasks[idx].assignee_id = assignee_id;
            my_strncpy(tasks[idx].title, title, MAX_TITLE - 1);
            my_strncpy(tasks[idx].desc, desc, MAX_DESC - 1);
            tasks[idx].status = TASK_TODO;
            tasks[idx].priority = priority;
            tasks[idx].created_at = ++clock;
            tasks[idx].updated_at = clock;
            projects[i].task_count++;
            char msg[MAX_MSG];
            my_strncpy(msg, "Task assigned: ", MAX_MSG - 1);
            int ml = my_strlen(msg);
            my_strncpy(msg + ml, title, MAX_MSG - ml - 1);
            notify_push(assignee_id, NOTIFY_ASSIGN, msg);
            return tasks[idx].task_id;
        }
    }
    return -1;
}

int task_update_status(int task_id, int status) {
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].task_id == task_id) {
            tasks[i].status = status;
            tasks[i].updated_at = ++clock;
            char msg[MAX_MSG];
            my_strncpy(msg, "Task updated: ", MAX_MSG - 1);
            int ml = my_strlen(msg);
            my_strncpy(msg + ml, tasks[i].title, MAX_MSG - ml - 1);
            notify_push(tasks[i].assignee_id, NOTIFY_UPDATE, msg);
            return 0;
        }
    }
    return -1;
}

int project_progress(int project_id) {
    int total = 0, done = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].project_id == project_id) {
            total++;
            if (tasks[i].status == TASK_DONE) done++;
        }
    }
    if (total == 0) return 0;
    return (done * 100) / total;
}

/* ===== Knowledge base (wiki) ===== */
int wiki_create(int author_id, const char *title, const char *body,
                const char *category) {
    if (wiki_count >= MAX_WIKI) return -1;
    int idx = wiki_count++;
    wiki_pages[idx].wiki_id = next_wiki_id++;
    wiki_pages[idx].author_id = author_id;
    my_strncpy(wiki_pages[idx].title, title, MAX_TITLE - 1);
    my_strncpy(wiki_pages[idx].body, body, MAX_BODY - 1);
    my_strncpy(wiki_pages[idx].category, category, MAX_CATEGORY - 1);
    wiki_pages[idx].views = 0;
    wiki_pages[idx].created_at = ++clock;
    wiki_pages[idx].updated_at = clock;
    wiki_pages[idx].deleted = 0;
    return wiki_pages[idx].wiki_id;
}

int wiki_update(int wiki_id, const char *body) {
    for (int i = 0; i < wiki_count; i++) {
        if (wiki_pages[i].wiki_id == wiki_id && !wiki_pages[i].deleted) {
            my_strncpy(wiki_pages[i].body, body, MAX_BODY - 1);
            wiki_pages[i].updated_at = ++clock;
            return 0;
        }
    }
    return -1;
}

int wiki_view(int wiki_id) {
    for (int i = 0; i < wiki_count; i++) {
        if (wiki_pages[i].wiki_id == wiki_id && !wiki_pages[i].deleted) {
            wiki_pages[i].views++;
            return wiki_pages[i].views;
        }
    }
    return -1;
}

int wiki_search(const char *keyword, int *result_ids, int max_results) {
    int found = 0;
    for (int i = 0; i < wiki_count && found < max_results; i++) {
        if (wiki_pages[i].deleted) continue;
        if (my_strstr(wiki_pages[i].title, keyword) ||
            my_strstr(wiki_pages[i].body, keyword) ||
            my_strstr(wiki_pages[i].category, keyword)) {
            result_ids[found++] = wiki_pages[i].wiki_id;
        }
    }
    return found;
}

/* ===== Display helpers ===== */
void display_topics(void) {
    print_str("  Discussion Forum Topics\n"); print_sep();
    for (int i = 0; i < topic_count; i++) {
        if (topics[i].deleted) continue;
        print_str("  #"); print_int(topics[i].topic_id);
        print_str(" ["); print_str(topics[i].category); print_str("] ");
        print_str(topics[i].title);
        print_str(" (replies="); print_int(topics[i].reply_count);
        print_str(", views="); print_int(topics[i].views);
        print_str(", tags="); print_int(topics[i].tag_count);
        print_str(")\n");
    }
}

void display_snippets(void) {
    print_str("  Code Snippets\n"); print_sep();
    for (int i = 0; i < snippet_count; i++) {
        if (snippets[i].deleted) continue;
        print_str("  #"); print_int(snippets[i].snippet_id);
        print_str(" ["); print_str(snippets[i].language); print_str("] ");
        print_str(snippets[i].title);
        print_str(" (v"); print_int(snippets[i].version_count);
        print_str(", stars="); print_int(snippets[i].stars);
        print_str(", forks="); print_int(snippets[i].forks);
        print_str(")\n");
    }
}

void display_projects(void) {
    print_str("  Projects\n"); print_sep();
    for (int i = 0; i < project_count; i++) {
        if (!projects[i].active) continue;
        int progress = project_progress(projects[i].project_id);
        print_str("  #"); print_int(projects[i].project_id);
        print_str(" "); print_str(projects[i].name);
        print_str(" (members="); print_int(projects[i].member_count);
        print_str(", tasks="); print_int(projects[i].task_count);
        print_str(", progress="); print_int(progress); print_str("%)\n");
    }
}

void display_wiki(void) {
    print_str("  Knowledge Base (Wiki)\n"); print_sep();
    for (int i = 0; i < wiki_count; i++) {
        if (wiki_pages[i].deleted) continue;
        print_str("  #"); print_int(wiki_pages[i].wiki_id);
        print_str(" ["); print_str(wiki_pages[i].category); print_str("] ");
        print_str(wiki_pages[i].title);
        print_str(" (views="); print_int(wiki_pages[i].views);
        print_str(")\n");
    }
}

void display_users(void) {
    print_str("  Registered Users\n"); print_sep();
    for (int i = 0; i < user_count; i++) {
        if (!users[i].active) continue;
        const char *role = "guest";
        if (users[i].permission == PERM_MEMBER) role = "member";
        else if (users[i].permission == PERM_MODERATOR) role = "moderator";
        else if (users[i].permission == PERM_ADMIN) role = "admin";
        print_str("  #"); print_int(users[i].user_id);
        print_str(" "); print_str(users[i].name);
        print_str(" <"); print_str(users[i].email); print_str(">");
        print_str(" ["); print_str(role); print_str("]");
        print_str(" (posts="); print_int(users[i].post_count);
        print_str(", rep="); print_int(users[i].reputation);
        print_str(")\n");
    }
}

void display_notifications(int user_id) {
    print_str("  Notifications\n"); print_sep();
    int unread = 0;
    for (int i = 0; i < notif_count; i++) {
        if (notifications[i].user_id != user_id) continue;
        const char *type_str = "unknown";
        if (notifications[i].type == NOTIFY_REPLY) type_str = "reply";
        else if (notifications[i].type == NOTIFY_MENTION) type_str = "mention";
        else if (notifications[i].type == NOTIFY_ASSIGN) type_str = "assign";
        else if (notifications[i].type == NOTIFY_UPDATE) type_str = "update";
        else if (notifications[i].type == NOTIFY_SYSTEM) type_str = "system";
        print_str("  #"); print_int(notifications[i].notif_id);
        print_str(" ["); print_str(type_str); print_str("] ");
        print_str(notifications[i].message);
        if (!notifications[i].read_flag) { print_str(" (UNREAD)"); unread++; }
        print_str("\n");
    }
    print_str("  Unread: "); print_int(unread); print_str("\n");
}

void display_tasks(int project_id) {
    print_str("  Tasks\n"); print_sep();
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].project_id != project_id) continue;
        const char *status = "TODO";
        if (tasks[i].status == TASK_IN_PROGRESS) status = "IN_PROGRESS";
        else if (tasks[i].status == TASK_REVIEW) status = "REVIEW";
        else if (tasks[i].status == TASK_DONE) status = "DONE";
        print_str("  #"); print_int(tasks[i].task_id);
        print_str(" ["); print_str(status); print_str("] ");
        print_str(tasks[i].title);
        print_str(" (priority="); print_int(tasks[i].priority);
        print_str(", assignee="); print_int(tasks[i].assignee_id);
        print_str(")\n");
    }
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("Community Hub v47.0 - Community Center for AI-ASM OS\n");

    if (help) {
        print_str("Usage: community_hub [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run full test suite\n\n");
        print_str("Features:\n");
        print_str("  - Discussion forums: topics, replies, tags, search\n");
        print_str("  - Code sharing: snippets, versioning, stars, forks\n");
        print_str("  - Collaboration: projects, tasks, progress tracking\n");
        print_str("  - Knowledge base: wiki, tutorials, FAQ\n");
        print_str("  - User system: registration, auth, permissions\n");
        print_str("  - Notifications: push, subscriptions, reminders\n");
        return;
    }

    if (test) {
        print_str("=== Community Hub Test Suite ===\n\n");

        /* --- User Registration --- */
        print_str("--- User System ---\n");
        int u1 = user_register("alice", "alice@aiasm.dev", "pass_alice_001");
        int u2 = user_register("bob", "bob@aiasm.dev", "pass_bob_002");
        int u3 = user_register("carol", "carol@aiasm.dev", "pass_carol_003");
        int u4 = user_register("dave", "dave@aiasm.dev", "pass_dave_004");
        print_str("Registered "); print_int(user_count); print_str(" users\n");
        print_str("  alice=#"); print_int(u1);
        print_str(" bob=#"); print_int(u2);
        print_str(" carol=#"); print_int(u3);
        print_str(" dave=#"); print_int(u4); print_str("\n");

        /* Authentication */
        int auth = user_authenticate("alice@aiasm.dev", "pass_alice_001");
        print_str("Auth alice: ");
        if (auth == u1) print_str("OK\n"); else print_str("FAIL\n");
        auth = user_authenticate("bob@aiasm.dev", "wrong_pass");
        print_str("Auth bob (bad pw): ");
        if (auth == -1) print_str("REJECTED OK\n"); else print_str("FAIL\n");

        /* Permissions */
        user_set_permission(u1, PERM_ADMIN);
        user_set_permission(u2, PERM_MODERATOR);
        print_str("Set alice=admin, bob=moderator\n\n");

        /* --- Discussion Forums --- */
        print_str("--- Discussion Forums ---\n");
        int t1 = forum_create_topic(u1, "Welcome to AI-ASM Community",
                                    "Introduce yourself and share your projects.",
                                    "general", "welcome", "intro");
        int t2 = forum_create_topic(u2, "WASM Optimization Techniques",
                                    "Let us discuss SIMD and memory optimization for WASM.",
                                    "technical", "wasm", "performance");
        int t3 = forum_create_topic(u3, "AI Model Integration Patterns",
                                    "Best practices for integrating LLMs into AI-ASM modules.",
                                    "technical", "ai", "architecture");
        int t4 = forum_create_topic(u4, "Bug: Memory leak in agent_runtime",
                                    "Found a memory leak when running long sessions.",
                                    "bugs", "bug", "memory");
        print_str("Created "); print_int(topic_count); print_str(" topics\n");

        /* Replies */
        forum_add_reply(t1, u2, "Hi everyone! Excited to be part of this community.");
        forum_add_reply(t1, u3, "Hello! Working on neural network modules.");
        forum_add_reply(t2, u1, "SIMD intrinsics gave us 4x speedup on matrix ops.");
        forum_add_reply(t2, u4, "Memory pooling is critical. Check out memory_pool module.");
        forum_add_reply(t3, u2, "We use a streaming approach with chunked responses.");
        forum_add_reply(t4, u1, "Looking into it. Likely related to the arena allocator.");
        print_str("Added "); print_int(reply_count); print_str(" replies\n");

        /* Upvotes */
        forum_reply_upvote(1); forum_reply_upvote(1); forum_reply_upvote(1);
        forum_reply_upvote(3); forum_reply_upvote(3);
        print_str("Upvoted top replies\n");

        /* Topic status */
        forum_set_status(t1, TOPIC_PINNED);
        forum_set_status(t4, TOPIC_LOCKED);
        print_str("Pinned topic #"); print_int(t1);
        print_str(", locked topic #"); print_int(t4); print_str("\n");

        /* Subscriptions */
        subscribe(u3, t1, -1);
        subscribe(u4, t2, -1);
        subscribe(u1, t3, -1);
        print_str("Subscriptions created: "); print_int(sub_count); print_str("\n\n");

        /* --- Code Sharing --- */
        print_str("--- Code Sharing ---\n");
        int s1 = code_publish(u1, "Matrix Multiply SIMD", "c",
                              "void mat_mul_simd(float *a, float *b, float *c, int n) {\n"
                              "  for (int i = 0; i < n; i++)\n"
                              "    for (int j = 0; j < n; j += 4) {\n"
                              "      v128 sum = v128_zero();\n"
                              "      for (int k = 0; k < n; k++)\n"
                              "        sum = v128_fma(a[i*n+k], b[k*n+j], sum);\n"
                              "      v128_store(&c[i*n+j], sum);\n"
                              "    }\n"
                              "}",
                              "simd");
        int s2 = code_publish(u2, "Arena Allocator", "c",
                              "typedef struct { char *buf; int used; int cap; } arena_t;\n"
                              "void *arena_alloc(arena_t *a, int size) {\n"
                              "  if (a->used + size > a->cap) return NULL;\n"
                              "  void *p = a->buf + a->used;\n"
                              "  a->used += size;\n"
                              "  return p;\n"
                              "}\n"
                              "void arena_reset(arena_t *a) { a->used = 0; }",
                              "memory");
        int s3 = code_publish(u3, "Neural Net Forward Pass", "python",
                              "def forward(x, weights, biases):\n"
                              "    for w, b in zip(weights, biases):\n"
                              "        x = relu(x @ w + b)\n"
                              "    return softmax(x)",
                              "ai");
        print_str("Published "); print_int(snippet_count); print_str(" snippets\n");

        /* Version commits */
        code_commit(s1,
                    "void mat_mul_simd_v2(float *a, float *b, float *c, int n) {\n"
                    "  /* Optimized with loop unrolling and prefetch */\n"
                    "  for (int i = 0; i < n; i++) {\n"
                    "    __builtin_prefetch(&a[(i+1)*n]);\n"
                    "    for (int j = 0; j < n; j += 8) {\n"
                    "      v128 s0 = v128_zero(), s1 = v128_zero();\n"
                    "      for (int k = 0; k < n; k++) {\n"
                    "        v128 bv = v128_load(&b[k*n+j]);\n"
                    "        s0 = v128_fma(a[i*n+k], bv, s0);\n"
                    "      }\n"
                    "      v128_store(&c[i*n+j], s0);\n"
                    "    }\n"
                    "  }\n"
                    "}",
                    "Add prefetching and 8-wide unrolling");
        print_str("Committed v2 for snippet #"); print_int(s1); print_str("\n");

        /* Stars and forks */
        code_star(s1); code_star(s1); code_star(s1);
        code_star(s2); code_star(s2);
        code_star(s3);
        int forked = code_fork(s2, u4);
        print_str("Stars and forks applied, fork created: #"); print_int(forked); print_str("\n\n");

        /* --- Collaborative Development --- */
        print_str("--- Collaborative Development ---\n");
        int p1 = project_create(u1, "ai-asm-core", "Core AI-ASM OS modules");
        int p2 = project_create(u2, "wasm-optimization", "WASM performance library");
        print_str("Created "); print_int(project_count); print_str(" projects\n");

        project_add_member(p1, u2);
        project_add_member(p1, u3);
        project_add_member(p1, u4);
        project_add_member(p2, u1);
        project_add_member(p2, u3);
        print_str("Added members to projects\n");

        /* Tasks */
        task_create(p1, u2, "Implement scheduler", "Build cooperative task scheduler", 3);
        task_create(p1, u3, "Memory manager", "Implement arena-based memory manager", 3);
        task_create(p1, u4, "Module loader", "WASM module loading and linking", 2);
        task_create(p1, u1, "Documentation", "Write API reference docs", 1);
        task_create(p2, u1, "SIMD wrappers", "Cross-platform SIMD abstraction", 3);
        task_create(p2, u3, "Benchmarks", "Performance benchmark suite", 2);
        print_str("Created "); print_int(task_count); print_str(" tasks\n");

        /* Update task statuses */
        task_update_status(1, TASK_IN_PROGRESS);
        task_update_status(2, TASK_IN_PROGRESS);
        task_update_status(3, TASK_REVIEW);
        task_update_status(5, TASK_DONE);
        task_update_status(2, TASK_DONE);
        print_str("Updated task statuses\n\n");

        /* --- Knowledge Base (Wiki) --- */
        print_str("--- Knowledge Base ---\n");
        int w1 = wiki_create(u1, "Getting Started with AI-ASM",
                             "AI-ASM is a WebAssembly-based operating system...\n"
                             "Modules are compiled with clang targeting wasm32.\n"
                             "Use host_alloc/host_print for I/O.",
                             "tutorial");
        int w2 = wiki_create(u2, "WASM Memory Model FAQ",
                             "Q: How does linear memory work?\n"
                             "A: WASM uses a contiguous byte-addressable memory.\n"
                             "Q: Can modules share memory?\n"
                             "A: Yes, through shared memory imports/exports.",
                             "faq");
        int w3 = wiki_create(u3, "Neural Network Integration Guide",
                             "To integrate a neural network module:\n"
                             "1. Define input/output tensor shapes\n"
                             "2. Implement forward pass in WASM\n"
                             "3. Use host_alloc for tensor buffers\n"
                             "4. Register with ai_router module",
                             "tutorial");
        int w4 = wiki_create(u4, "API Reference: host functions",
                             "host_alloc(size, align) -> ptr\n"
                             "host_print(str) -> void\n"
                             "host_exit(code) -> void\n"
                             "host_get_argv(buf, max) -> int",
                             "reference");
        print_str("Created "); print_int(wiki_count); print_str(" wiki pages\n");

        /* Wiki updates and views */
        wiki_update(w1, "AI-ASM v47.0 Getting Started Guide\n\n"
                        "1. Install clang with wasm32 target\n"
                        "2. Write module with _start entry point\n"
                        "3. Compile: clang --target=wasm32 -nostdlib ...\n"
                        "4. Deploy to modules/ directory");
        wiki_view(w1); wiki_view(w1); wiki_view(w1);
        wiki_view(w2); wiki_view(w2);
        wiki_view(w3);
        wiki_view(w4);
        print_str("Wiki pages updated and viewed\n\n");

        /* ===== Display All ===== */
        print_str("\n========== COMMUNITY HUB OVERVIEW ==========\n\n");
        display_users();
        print_str("\n");
        display_topics();
        print_str("\n");
        display_snippets();
        print_str("\n");
        display_projects();
        print_str("\n");
        display_tasks(p1);
        print_str("\n");
        display_wiki();
        print_str("\n");
        display_notifications(u1);

        /* Search */
        print_str("\n--- Search Results ---\n");
        int results[16];
        int found = forum_search_topics("WASM", results, 16);
        print_str("Forum search 'WASM': "); print_int(found); print_str(" topics\n");
        found = forum_search_topics("AI", results, 16);
        print_str("Forum search 'AI': "); print_int(found); print_str(" topics\n");
        found = wiki_search("WASM", results, 16);
        print_str("Wiki search 'WASM': "); print_int(found); print_str(" pages\n");
        found = wiki_search("memory", results, 16);
        print_str("Wiki search 'memory': "); print_int(found); print_str(" pages\n");

        /* Project progress */
        print_str("\n--- Project Progress ---\n");
        print_str("  ai-asm-core: "); print_int(project_progress(p1)); print_str("%\n");
        print_str("  wasm-optimization: "); print_int(project_progress(p2)); print_str("%\n");

        /* Notification summary for other users */
        print_str("\n--- Bob's Notifications ---\n");
        int bob_notifs[16];
        int bob_unread = notify_get_unread(u2, bob_notifs, 16);
        print_str("  Unread count: "); print_int(bob_unread); print_str("\n");

        /* Mark some as read */
        for (int i = 0; i < notif_count; i++) {
            if (notifications[i].user_id == u2 && !notifications[i].read_flag) {
                notify_mark_read(notifications[i].notif_id);
                break;
            }
        }
        bob_unread = notify_get_unread(u2, bob_notifs, 16);
        print_str("  After marking one read: "); print_int(bob_unread); print_str("\n");

        /* Summary statistics */
        print_str("\n========== HUB STATISTICS ==========\n");
        print_str("  Users:         "); print_int(user_count); print_str("\n");
        print_str("  Topics:        "); print_int(topic_count); print_str("\n");
        print_str("  Replies:       "); print_int(reply_count); print_str("\n");
        print_str("  Snippets:      "); print_int(snippet_count); print_str("\n");
        print_str("  Projects:      "); print_int(project_count); print_str("\n");
        print_str("  Tasks:         "); print_int(task_count); print_str("\n");
        print_str("  Wiki pages:    "); print_int(wiki_count); print_str("\n");
        print_str("  Notifications: "); print_int(notif_count); print_str("\n");
        print_str("  Subscriptions: "); print_int(sub_count); print_str("\n");
        print_str("\n=== Community Hub Test Suite Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
