#include <stdio.h>
#include <stdlib.h>
struct node
{
   int data;
   struct node *left;
   struct node *right;
   int height;
};

struct node *tree=NULL;

void create_tree(struct node *);
struct node *insertElement(struct node *, int);
void print(struct node *, int);
int max(int, int);

int nodecount=0;
int *count= &nodecount;

// A utility function to get the height of the tree
int height(struct node *node)
{
    int leftheight= 0, rightheight= 0;
    if(node==NULL)
       return 0;

    if(node->left)   leftheight = height(node->left);
    if (node->right) rightheight = height(node->right);

    return max(leftheight, rightheight)+1;
}

//get maximum of two integers
int max(int a, int b)
{
    return (a >= b)? a : b;
}

int main()
{
  int val;
  create_tree(tree);
  if (isatty(0)) printf("enter elements of the new nodes : ");
  int i=0, n, c;
  int countval=0, *count = &countval;
  while(c= getchar(), c!=EOF && c!='\n')
   {
      ungetc(c, stdin);
      int nscan= scanf("%d", &val);
      if (nscan==1)
      {
         ++i;
         tree = insertElement(tree, val);
      }
      else
        break;
    }
    n= i;

    print(tree, 0);
    return 0;
}

void create_tree(struct node *tree)
{
   tree = NULL;
}

struct node *insertElement(struct node *tree, int val)
{
   struct node *ptr, *nodeptr, *parentptr;
   ptr = (struct node*)malloc(sizeof(struct node));
   ptr->data = val;
   ptr->left = NULL;
   ptr->right = NULL;
   ptr->height = 1;
   if(tree==NULL)
   {
      tree=ptr;
      nodecount+=1;
   }
   else
   {
      nodeptr=tree;
      while(nodeptr!=NULL)
      {
         parentptr=nodeptr;
         if(val<nodeptr->data)
         {
            nodeptr=nodeptr->left;
         }
         else if(val>nodeptr->data)
         {
            nodeptr = nodeptr->right;
         }
         else if(val==nodeptr->data)
         {
            break;
         }
      }
      if(val<parentptr->data)
      {
         parentptr->left = ptr;
         parentptr->left->height = height(parentptr)+1;
         nodecount+=1;
      }
      else if(val>parentptr->data)
      {
         parentptr->right = ptr;
         parentptr->right->height = height(parentptr)+1;
         nodecount++;
      }
      else
      {
         free(ptr);
      }
   }
   return tree;
}

void print(struct node *t, int depth) {
    int i;
    if (t == NULL)
        return;
    print(t->right, depth + 1);
    for (i = 0; i < 4 * depth; ++i)
        printf(" ");
    printf(" %d [height=%d]\n", t->data, t->height);
    print(t->left, depth + 1);
}
