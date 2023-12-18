#ifndef LIST_H
#define LIST_H

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

#define LIST_HEAD(name) \
    struct list_head name = { &{name}, &{name} }



static inline void list_init(struct list_head *head) {
    head = head->prev = head->next;
}

static inline void list_add(struct list_head *child, struct list_head *parent) {
    parent->next->prev = child;
    child->next = parent->next;
    parent->next = child;
    child->prev = parent;
}


static inline void list_add_tail(struct list_head *child, struct list_head *head) {
    head->prev->next = child;
    child->prev = head->prev;
    child->next = head;
    head->prev = child;
}

static inline void list_del(struct list_head *elem) {
    struct list_head *prev = elem->prev;
    struct list_head *next = elem->next;

    prev->next = next;
    next->prev = prev;
}

static inline int list_empty(struct list_head *head) {
    return head->next == head;
}

#define list_entry(ptr, type, member) \
            ((type *) ((char*) (ptr) - offsetof(type,member) ) )

#define list_first_entry(ptr, type, member) \
            list_entry((ptr)->next,type,member) 

#define list_for_each(pos, head) \
        for(pos = (head)->next; \
                pos != (head);  \
                pos = pos->next);

// lookahead
#define list_for_each_safe(pos, p, head) \
        for(pos = (head)->next, p = pos->next; \
                pos != (head); \
                pos = p, p  = pos->next) 

#endif

