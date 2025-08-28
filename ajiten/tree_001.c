#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct node
{
   int data;
   struct node *left;
   struct node *right;
};

struct node *create_tree(void);
struct node *insertElement(struct node *, int);
void print(struct node *, int);
void preorderTraversal(struct node *);
void inorderTraversal(struct node *);
void postorderTraversal(struct node *);
void levelorderTraversal(struct node *tree);
struct node *findSmallestElement(struct node *);
struct node *findLargestElement(struct node *);
struct node *deleteElement(struct node *, int);
int totalNodes(struct node *);
int totalExternalNodes(struct node *);
int totalInternalNodes(struct node *);
int Height(struct node *);

int main(void)
{
   struct node *tree;
   int val;

   tree= create_tree();
   printf("enter elements of the new nodes : ");  //5 1 3 16 8 12 11 2
   int i=0, n, c;
   while(c= getchar(), c!=EOF && c!='\n')
   {
      ungetc(c, stdin);
      int nscan= scanf("%d", &val);
      if (nscan==1)
      {
         ++i;
         tree = insertElement(tree, val);
         printf("After insert(%d) totalNodes=%d external=%d internal=%d\n",
                val, totalNodes(tree), totalExternalNodes(tree), totalInternalNodes(tree));
      }
      else
        break;
    }
    n= i;
    printf("\n Number of new nodes: %d\n\n\n", n);

    print(tree, 0);

    printf("\n The elements of the tree in in-order traversal are : \n");
    inorderTraversal(tree);

    printf("\n The elements of the tree in pre-order traversal are : \n");
    preorderTraversal(tree);

    printf("\n The elements of the tree in post-order traversal are  : \n");
    postorderTraversal(tree);

    printf("\n Total no. of nodes = %d", totalNodes(tree));

    printf("\n Total no. of external nodes = %d", totalExternalNodes(tree));

    printf("\n Total no. of internal nodes = %d", totalInternalNodes(tree));

    printf("\n The height of the tree = %d\n", Height(tree));

    levelorderTraversal(tree);
    return 0;
}


struct node *create_tree(void)
{
   return NULL;
}


struct node *insertElement(struct node *tree, int val)
{
   struct node *ptr, *nodeptr, *parentptr;
   ptr = (struct node*)malloc(sizeof(struct node));
   ptr->data = val;
   ptr->left = NULL;
   ptr->right = NULL;
   if(tree==NULL)
   {
      tree=ptr;
      tree->left=NULL;
      tree->right=NULL;
   }
   else
   {
      parentptr=NULL;
      nodeptr=tree;
      while(nodeptr!=NULL)
      {
         parentptr=nodeptr;
         if(val<nodeptr->data)
            nodeptr=nodeptr->left;
         else
            nodeptr = nodeptr->right;
      }
      if(val<parentptr->data)
         parentptr->left = ptr;
      else
         parentptr->right = ptr;
   }
   return tree;
}


void preorderTraversal(struct node *tree)
{
   if(tree != NULL)
   {
      printf("%d\t", tree->data);
      preorderTraversal(tree->left);
      preorderTraversal(tree->right);
   }
}


void inorderTraversal(struct node *tree)
{
   if(tree != NULL)
   {
      inorderTraversal(tree->left);
      printf("%d\t", tree->data);
      inorderTraversal(tree->right);
   }
}


void postorderTraversal(struct node *tree)
{
   if(tree != NULL)
   {
      postorderTraversal(tree->left);
      postorderTraversal(tree->right);
      printf("%d\t", tree->data);
   }
}

struct levelorderTraversalData {
    int depth;
    int offset;
    int data;
};

/* fills 'n' elements in 'arr' (starting from '*i')
   with depth/offset/data values;
   (where 'n' is the number of elements in the (sub)tree)
   'depth' (depth of this (sub)tree, 0 for the whole tree)
   'offs' (number of elements left to this (sub)tree)
   updates '*i'
   return value: 'n'
 */
int levelorderTraversal_core(struct node *tree, int depth, int offs, struct levelorderTraversalData *arr, int *i)
{
    int lnum=0, rnum= 0;
    if (tree->left)  lnum= levelorderTraversal_core(tree->left,  depth+1, offs,      arr, i);
    if (tree->right) rnum= levelorderTraversal_core(tree->right, depth+1, offs+lnum+1, arr, i);
    arr[*i].depth= depth;
    arr[*i].offset= offs+lnum;
    arr[*i].data= tree->data;
    ++*i;
    return lnum+1+rnum;
}

/* compare-rutin used by quicksort */
static int levelorderTraversal_compar(const void *pl, const void *pr)
{
    const struct levelorderTraversalData *l= pl;
    const struct levelorderTraversalData *r= pr;
    int cmp;

    cmp= (r->depth < l->depth) - (r->depth > l->depth);
    if (cmp==0) cmp= (r->offset < l->offset) - (r->offset > l->offset);
    return cmp;
}

void levelorderTraversal(struct node *tree)
{
    if (!tree) return;
    int n= totalNodes(tree);
    struct levelorderTraversalData *arr= calloc (n, sizeof arr[0]);
    int i= 0;
    levelorderTraversal_core(tree, 0, 0, arr, &i);
    qsort(arr, n, sizeof arr[0], levelorderTraversal_compar);
    for (i=0; i<n; ++i) {
        printf ("At depth %d, offset %d: data=%d\n", arr[i].depth, arr[i].offset, arr[i].data);
    }
    free(arr);
}

struct node *findSmallestElement(struct node *tree)
{
   if( (tree == NULL) || (tree->left == NULL))
      return tree;
   else
      return findSmallestElement(tree ->left);
}


struct node *findLargestElement(struct node *tree)
{
   if( (tree == NULL) || (tree->right == NULL))
      return tree;
   else
      return findLargestElement(tree->right);
}


int totalNodes(struct node *tree)
{
   if(tree==NULL)
      return 0;
   else
      return(totalNodes(tree->left) + totalNodes(tree->right) + 1);
}


int totalExternalNodes(struct node *tree)
{
   if(tree==NULL)
      return 0;
   else if((tree->left==NULL) && (tree->right==NULL))
      return 1;
   else
      return (totalExternalNodes(tree->left) + totalExternalNodes(tree->right));
}


int totalInternalNodes(struct node *tree)
{
   if( (tree==NULL) || ((tree->left==NULL) && (tree->right==NULL)))
      return 0;
   else
      return (totalInternalNodes(tree->left) + totalInternalNodes(tree->right) + 1);
}


int Height(struct node *tree)
{
   int leftheight, rightheight;
   if(tree==NULL)
      return 0;
   else
   {
      leftheight = Height(tree->left);
      rightheight = Height(tree->right);
      if(leftheight > rightheight)
         return (leftheight + 1);
      else
         return (rightheight + 1);
   }
}

void print(struct node *t, int depth) {
    int i;
    if (t == NULL)
        return;
    print(t->right, depth + 1);
    for (i = 0; i < 4 * depth; ++i)
        printf(" ");
    printf(" %d\n", t->data);
    print(t->left, depth + 1);
}
