#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *node, *safe = NULL;

    list_for_each_safe (node, safe, head) {
        element_t *entry = list_entry(node, element_t, list);

        if (entry->value)
            free(entry->value);

        free(entry);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new_node = malloc(sizeof(element_t));

    if (!new_node)
        return false;

    new_node->value = strdup(s);
    list_add(&new_node->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_node = malloc(sizeof(element_t));

    if (!new_node)
        return false;

    new_node->value = strdup(s);
    list_add_tail(&new_node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;

    element_t *entry = list_first_entry(head, element_t, list);
    list_del(&entry->list);

    if (sp) {
        strncpy(sp, entry->value, bufsize - 1);
    }

    return entry;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;

    element_t *entry = list_last_entry(head, element_t, list);
    list_del(&entry->list);

    if (sp) {
        strncpy(sp, entry->value, bufsize - 1);
    }

    return entry;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int count = 0;
    struct list_head *node;

    list_for_each (node, head) {
        count++;
    }

    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *front = head->next;
    struct list_head *back = head->prev;

    while (front != back && front->prev != back) {
        front = front->next;
        back = back->prev;
    }

    list_del(front);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    element_t *cur_entry, *next_entry, *safe = NULL;
    bool flag = false;

    list_for_each_entry_safe (cur_entry, safe, head, list) {
        next_entry = list_entry(cur_entry->list.next, element_t, list);

        if (&next_entry->list != head &&
            strcmp(cur_entry->value, next_entry->value) == 0) {
            flag = true;
            list_del(&cur_entry->list);
            free(cur_entry->value);
            free(cur_entry);
        } else if (flag) {
            flag = false;
            list_del(&cur_entry->list);
            free(cur_entry->value);
            free(cur_entry);
        }

    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    q_reverseK(head, 2);
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *cur, *safe;
    list_for_each_safe (cur, safe, head) {
        list_move(cur, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head))
        return;

    struct list_head *cur, *safe, tmp, tmp_rev;
    INIT_LIST_HEAD(&tmp);
    INIT_LIST_HEAD(&tmp_rev);
    int count = 1;

    list_for_each_safe (cur, safe, head) {

        if (count % k == 0 && cur->next == head) {
            list_cut_position(&tmp, head, cur);
            q_reverse(&tmp);
            list_splice_tail_init(&tmp, &tmp_rev);
            list_splice(&tmp_rev, head);
        } else if (count % k == 0) {
            list_cut_position(&tmp, head, cur);
            q_reverse(&tmp);
            list_splice_tail_init(&tmp, &tmp_rev);
        } else if (cur->next == head) {
            list_splice(&tmp_rev, head);
        }

        count++;
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
// 升冪排列，右邊的都要比左邊的大
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    element_t *cur_entry = list_last_entry(head, element_t, list);
    struct list_head *comp = head->prev->prev;
    int count = 1;

    while (cur_entry->list.prev != head) {
        element_t *comp_entry = list_entry(comp, element_t, list);

        if (strcmp(cur_entry->value, comp_entry->value) > 0) {
            comp = comp->prev;
            list_del(comp->next);
            q_release_element(comp_entry);
        } else {
            cur_entry = comp_entry;
            comp = comp->prev;
            count++;
        }
    }

    return count;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
