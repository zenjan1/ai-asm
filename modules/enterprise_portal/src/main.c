/* enterprise_portal: Enterprise portal system (v1.0)
 * Unified entry, notifications, calendar, collaboration tools, analytics
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

#define MAX_USERS        12
#define MAX_APPS         10
#define MAX_NOTIFICATIONS 20
#define MAX_EVENTS       12
#define MAX_MESSAGES     16
#define MAX_DISCUSSIONS  8
#define MAX_POLLS        6
#define MAX_SURVEYS      6

typedef struct {
    int    user_id;
    int    role;
    int    n_apps;
    int    n_notifications;
    int    n_events;
    int    active;
} user_t;

typedef struct {
    int    app_id;
    int    type;
    int    n_users;
    int    n_sessions;
    double avg_duration;
    int    active;
} app_t;

typedef struct {
    int    notif_id;
    int    user_id;
    int    type;
    int    day;
    int    read_status;
    int    priority;
    int    active;
} notification_t;

typedef struct {
    int    event_id;
    int    organizer_id;
    int    type;
    int    day;
    int    hour;
    int    duration;
    int    attendees;
    int    active;
} event_t;

typedef struct {
    int    msg_id;
    int    sender_id;
    int    receiver_id;
    int    day;
    int    read_status;
    int    active;
} message_t;

typedef struct {
    int    disc_id;
    int    creator_id;
    int    n_posts;
    int    n_views;
    int    active;
} discussion_t;

typedef struct {
    int    poll_id;
    int    creator_id;
    int    n_options;
    int    n_votes;
    int    active;
} poll_t;

typedef struct {
    int    survey_id;
    int    creator_id;
    int    n_questions;
    int    n_responses;
    double avg_rating;
    int    active;
} survey_t;

typedef struct {
    int    n_users;
    int    n_apps;
    int    n_notifications;
    int    n_events;
    int    n_messages;
    int    n_discussions;
    int    n_polls;
    int    n_surveys;
    int    total_sessions;
    int    total_login_events;
    int    unread_notifications;
} ep_state_t;

static user_t users[MAX_USERS];
static app_t apps[MAX_APPS];
static notification_t notifications[MAX_NOTIFICATIONS];
static event_t events[MAX_EVENTS];
static message_t messages[MAX_MESSAGES];
static discussion_t discussions[MAX_DISCUSSIONS];
static poll_t polls[MAX_POLLS];
static survey_t surveys[MAX_SURVEYS];
static ep_state_t ep;

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

int ep_init(void) {
    if (initialized) return -1;
    ep.n_users = 0; ep.n_apps = 0; ep.n_notifications = 0;
    ep.n_events = 0; ep.n_messages = 0; ep.n_discussions = 0;
    ep.n_polls = 0; ep.n_surveys = 0;
    ep.total_sessions = 0; ep.total_login_events = 0;
    ep.unread_notifications = 0;
    for (int i = 0; i < MAX_USERS; i++) users[i].active = 0;
    for (int i = 0; i < MAX_APPS; i++) apps[i].active = 0;
    for (int i = 0; i < MAX_NOTIFICATIONS; i++) notifications[i].active = 0;
    for (int i = 0; i < MAX_EVENTS; i++) events[i].active = 0;
    for (int i = 0; i < MAX_MESSAGES; i++) messages[i].active = 0;
    for (int i = 0; i < MAX_DISCUSSIONS; i++) discussions[i].active = 0;
    for (int i = 0; i < MAX_POLLS; i++) polls[i].active = 0;
    for (int i = 0; i < MAX_SURVEYS; i++) surveys[i].active = 0;
    initialized = 1;
    print_str("[EP] Enterprise portal initialized\n");
    return 0;
}

int ep_register_user(int role) {
    if (ep.n_users >= MAX_USERS) return -1;
    user_t* u = &users[ep.n_users];
    u->user_id = ep.n_users;
    u->role = role;
    u->n_apps = 0;
    u->n_notifications = 0;
    u->n_events = 0;
    u->active = 1;
    ep.n_users++;
    ep.total_login_events++;
    print_str("[EP] User "); print_int(ep.n_users - 1);
    print_str(" role="); print_int(role); print_str(" registered\n");
    return ep.n_users - 1;
}

int ep_register_app(int type) {
    if (ep.n_apps >= MAX_APPS) return -1;
    app_t* a = &apps[ep.n_apps];
    a->app_id = ep.n_apps;
    a->type = type;
    a->n_users = 0;
    a->n_sessions = 0;
    a->avg_duration = 0.0;
    a->active = 1;
    ep.n_apps++;
    print_str("[EP] App "); print_int(ep.n_apps - 1);
    print_str(" type="); print_int(type); print_str(" registered\n");
    return ep.n_apps - 1;
}

void ep_grant_app_access(int user_id, int app_id) {
    if (user_id >= ep.n_users || app_id >= ep.n_apps) return;
    users[user_id].n_apps++;
    apps[app_id].n_users++;
    print_str("[EP] User "); print_int(user_id);
    print_str(" granted access to App "); print_int(app_id); print_str("\n");
}

void ep_launch_app(int app_id) {
    if (app_id >= ep.n_apps) return;
    apps[app_id].n_sessions++;
    ep.total_sessions++;
    print_str("[EP] Launched App "); print_int(app_id);
    print_str(" (sessions: "); print_int(apps[app_id].n_sessions); print_str(")\n");
}

int ep_send_notification(int user_id, int type, int priority) {
    if (ep.n_notifications >= MAX_NOTIFICATIONS || user_id >= ep.n_users) return -1;
    notification_t* n = &notifications[ep.n_notifications];
    n->notif_id = ep.n_notifications;
    n->user_id = user_id;
    n->type = type;
    n->day = 1;
    n->read_status = 0;
    n->priority = priority;
    n->active = 1;
    ep.n_notifications++;
    users[user_id].n_notifications++;
    ep.unread_notifications++;
    print_str("[EP] Notification "); print_int(ep.n_notifications - 1);
    print_str(" to U"); print_int(user_id);
    print_str(" type="); print_int(type);
    print_str(" P"); print_int(priority); print_str("\n");
    return ep.n_notifications - 1;
}

void ep_read_notification(int notif_id) {
    if (notif_id >= ep.n_notifications) return;
    if (notifications[notif_id].read_status == 0) {
        notifications[notif_id].read_status = 1;
        ep.unread_notifications--;
    }
    print_str("[EP] Notification "); print_int(notif_id); print_str(" read\n");
}

int ep_schedule_event(int organizer_id, int type, int day, int hour, int duration, int attendees) {
    if (ep.n_events >= MAX_EVENTS || organizer_id >= ep.n_users) return -1;
    event_t* e = &events[ep.n_events];
    e->event_id = ep.n_events;
    e->organizer_id = organizer_id;
    e->type = type;
    e->day = day;
    e->hour = hour;
    e->duration = duration;
    e->attendees = attendees;
    e->active = 1;
    ep.n_events++;
    users[organizer_id].n_events++;
    print_str("[EP] Event "); print_int(ep.n_events - 1);
    print_str(" day="); print_int(day);
    print_str(" "); print_int(hour); print_str(":00");
    print_str(" dur="); print_int(duration); print_str("h");
    print_str(" att="); print_int(attendees); print_str("\n");
    return ep.n_events - 1;
}

int ep_send_message(int sender_id, int receiver_id) {
    if (ep.n_messages >= MAX_MESSAGES) return -1;
    if (sender_id >= ep.n_users || receiver_id >= ep.n_users) return -1;
    message_t* m = &messages[ep.n_messages];
    m->msg_id = ep.n_messages;
    m->sender_id = sender_id;
    m->receiver_id = receiver_id;
    m->day = 1;
    m->read_status = 0;
    m->active = 1;
    ep.n_messages++;
    print_str("[EP] Message "); print_int(ep.n_messages - 1);
    print_str(" U"); print_int(sender_id); print_str("->U"); print_int(receiver_id); print_str("\n");
    return ep.n_messages - 1;
}

void ep_read_message(int msg_id) {
    if (msg_id >= ep.n_messages) return;
    if (messages[msg_id].read_status == 0) {
        messages[msg_id].read_status = 1;
    }
    print_str("[EP] Message "); print_int(msg_id); print_str(" read\n");
}

int ep_create_discussion(int creator_id) {
    if (ep.n_discussions >= MAX_DISCUSSIONS || creator_id >= ep.n_users) return -1;
    discussion_t* d = &discussions[ep.n_discussions];
    d->disc_id = ep.n_discussions;
    d->creator_id = creator_id;
    d->n_posts = 1;
    d->n_views = 0;
    d->active = 1;
    ep.n_discussions++;
    print_str("[EP] Discussion "); print_int(ep.n_discussions - 1);
    print_str(" by U"); print_int(creator_id); print_str("\n");
    return ep.n_discussions - 1;
}

void ep_post_to_discussion(int disc_id) {
    if (disc_id >= ep.n_discussions) return;
    discussions[disc_id].n_posts++;
    discussions[disc_id].n_views++;
    print_str("[EP] Discussion "); print_int(disc_id);
    print_str(" post (total: "); print_int(discussions[disc_id].n_posts); print_str(")\n");
}

int ep_create_poll(int creator_id, int options) {
    if (ep.n_polls >= MAX_POLLS || creator_id >= ep.n_users) return -1;
    poll_t* p = &polls[ep.n_polls];
    p->poll_id = ep.n_polls;
    p->creator_id = creator_id;
    p->n_options = options;
    p->n_votes = 0;
    p->active = 1;
    ep.n_polls++;
    print_str("[EP] Poll "); print_int(ep.n_polls - 1);
    print_str(" by U"); print_int(creator_id);
    print_str(" options="); print_int(options); print_str("\n");
    return ep.n_polls - 1;
}

void ep_vote(int poll_id) {
    if (poll_id >= ep.n_polls) return;
    polls[poll_id].n_votes++;
    print_str("[EP] Poll "); print_int(poll_id);
    print_str(" vote (total: "); print_int(polls[poll_id].n_votes); print_str(")\n");
}

int ep_create_survey(int creator_id, int questions) {
    if (ep.n_surveys >= MAX_SURVEYS || creator_id >= ep.n_users) return -1;
    survey_t* s = &surveys[ep.n_surveys];
    s->survey_id = ep.n_surveys;
    s->creator_id = creator_id;
    s->n_questions = questions;
    s->n_responses = 0;
    s->avg_rating = 0.0;
    s->active = 1;
    ep.n_surveys++;
    print_str("[EP] Survey "); print_int(ep.n_surveys - 1);
    print_str(" by U"); print_int(creator_id);
    print_str(" questions="); print_int(questions); print_str("\n");
    return ep.n_surveys - 1;
}

void ep_respond_survey(int survey_id, int rating) {
    if (survey_id >= ep.n_surveys) return;
    survey_t* s = &surveys[survey_id];
    double total = s->avg_rating * s->n_responses + rating;
    s->n_responses++;
    s->avg_rating = total / s->n_responses;
    print_str("[EP] Survey "); print_int(survey_id);
    print_str(" response (n="); print_int(s->n_responses);
    print_str(" avg="); print_int((int)s->avg_rating); print_str(")\n");
}

void ep_portal_analytics(void) {
    print_str("[EP] Portal analytics:\n");
    print_str("  Total users: "); print_int(ep.n_users); print_str("\n");
    print_str("  Total apps: "); print_int(ep.n_apps); print_str("\n");
    print_str("  Total sessions: "); print_int(ep.total_sessions); print_str("\n");
    print_str("  Login events: "); print_int(ep.total_login_events); print_str("\n");
    print_str("  Unread notifications: "); print_int(ep.unread_notifications); print_str("\n");
    for (int i = 0; i < ep.n_apps; i++) {
        print_str("  App "); print_int(i);
        print_str(" type="); print_int(apps[i].type);
        print_str(" users="); print_int(apps[i].n_users);
        print_str(" sessions="); print_int(apps[i].n_sessions); print_str("\n");
    }
}

void ep_print_state(void) {
    print_str("[EP] Users="); print_int(ep.n_users);
    print_str(" Apps="); print_int(ep.n_apps);
    print_str(" Notifs="); print_int(ep.n_notifications);
    print_str(" Events="); print_int(ep.n_events);
    print_str(" Msgs="); print_int(ep.n_messages);
    print_str("\n");
    print_str("  Discussions: "); print_int(ep.n_discussions); print_str("\n");
    print_str("  Polls: "); print_int(ep.n_polls); print_str("\n");
    print_str("  Surveys: "); print_int(ep.n_surveys); print_str("\n");
    print_str("  Sessions: "); print_int(ep.total_sessions); print_str("\n");
}

int main(void) {
    print_str("=== Enterprise Portal System Demo ===\n\n");
    ep_init();

    print_str("Registering users...\n");
    for (int i = 0; i < 8; i++) ep_register_user(1 + i % 3);

    print_str("\nRegistering apps...\n");
    ep_register_app(1);
    ep_register_app(2);
    ep_register_app(3);
    ep_register_app(4);
    ep_register_app(5);
    ep_register_app(1);

    print_str("\nGranting app access...\n");
    ep_grant_app_access(0, 0);
    ep_grant_app_access(0, 1);
    ep_grant_app_access(0, 2);
    ep_grant_app_access(1, 0);
    ep_grant_app_access(1, 3);
    ep_grant_app_access(2, 0);
    ep_grant_app_access(2, 4);
    ep_grant_app_access(3, 1);
    ep_grant_app_access(3, 2);
    ep_grant_app_access(4, 5);

    print_str("\nLaunching apps...\n");
    ep_launch_app(0);
    ep_launch_app(0);
    ep_launch_app(1);
    ep_launch_app(2);
    ep_launch_app(3);
    ep_launch_app(4);
    ep_launch_app(5);
    ep_launch_app(0);

    print_str("\nSending notifications...\n");
    ep_send_notification(0, 1, 2);
    ep_send_notification(1, 2, 1);
    ep_send_notification(2, 1, 3);
    ep_send_notification(3, 3, 2);
    ep_send_notification(0, 2, 1);
    ep_send_notification(4, 1, 2);
    ep_send_notification(5, 2, 1);

    print_str("\nReading notifications...\n");
    ep_read_notification(0);
    ep_read_notification(2);
    ep_read_notification(4);
    ep_read_notification(5);

    print_str("\nScheduling events...\n");
    ep_schedule_event(0, 1, 5, 10, 1, 5);
    ep_schedule_event(1, 2, 7, 14, 2, 8);
    ep_schedule_event(2, 1, 10, 9, 1, 4);
    ep_schedule_event(3, 3, 12, 16, 1, 6);
    ep_schedule_event(0, 2, 15, 11, 2, 10);

    print_str("\nSending messages...\n");
    ep_send_message(0, 1);
    ep_send_message(1, 2);
    ep_send_message(2, 3);
    ep_send_message(3, 0);
    ep_send_message(0, 4);
    ep_send_message(4, 5);
    ep_send_message(5, 6);
    ep_send_message(6, 0);

    print_str("\nReading messages...\n");
    ep_read_message(0);
    ep_read_message(1);
    ep_read_message(3);
    ep_read_message(5);

    print_str("\nCreating discussions...\n");
    ep_create_discussion(0);
    ep_create_discussion(2);
    ep_create_discussion(4);

    print_str("\nPosting to discussions...\n");
    ep_post_to_discussion(0);
    ep_post_to_discussion(0);
    ep_post_to_discussion(0);
    ep_post_to_discussion(1);
    ep_post_to_discussion(1);
    ep_post_to_discussion(2);

    print_str("\nCreating polls...\n");
    ep_create_poll(0, 4);
    ep_create_poll(1, 3);
    ep_create_poll(3, 5);

    print_str("\nVoting in polls...\n");
    ep_vote(0); ep_vote(0); ep_vote(0);
    ep_vote(1); ep_vote(1);
    ep_vote(2); ep_vote(2); ep_vote(2); ep_vote(2);

    print_str("\nCreating surveys...\n");
    ep_create_survey(0, 10);
    ep_create_survey(2, 8);
    ep_create_survey(4, 12);

    print_str("\nResponding to surveys...\n");
    ep_respond_survey(0, 4);
    ep_respond_survey(0, 5);
    ep_respond_survey(0, 3);
    ep_respond_survey(1, 5);
    ep_respond_survey(1, 4);
    ep_respond_survey(2, 3);
    ep_respond_survey(2, 4);
    ep_respond_survey(2, 5);
    ep_respond_survey(2, 4);

    print_str("\nPortal analytics...\n");
    ep_portal_analytics();

    print_str("\nFinal state...\n");
    ep_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
