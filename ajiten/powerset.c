#include <stdio.h>
int count=0;
struct node {
    char *s;
    struct node* prev;
};
char *arrv[] = {"a", "b", "c", '\0'};
int arrc = sizeof(arrv)/sizeof(arrv[0]) - 1;
int call_no =0;
void powerset(char **v, int n, struct node *up, int call_no, int val)
{
    struct node me = {NULL, NULL};     
    printf("\n---(1)char **v:%s, int n:%d, node *up:%p, call_no:%d, val:%d", *v,n,up,call_no,val);                                      
    if (!n) {
        printf("  count> %d", ++count);
        putchar('[');
        while (up) {
            printf("%s", up->s);
            up = up->prev;
            printf(" (2)node *up: %p", up); 
        }
        puts("]");
    } else {
        me.s = *v;
        me.prev = up; 
        printf(" (3)me:%p", (void*)&me);   
        powerset(v + 1, n - 1, up, ++call_no, 1);
        powerset(v + 1, n - 1, &me, ++call_no, 0);
    }
}
    
int main(int argc, char **argv)
{
    printf("size of char: %ld, size of char*: %ld, size of 'struct node': %ld, size of 'struct node pointer':%ld", sizeof(char), sizeof(char *), sizeof (struct node), sizeof(struct node *()));
    powerset(arrv, arrc, 0, ++call_no,1);
    return 0;
}
