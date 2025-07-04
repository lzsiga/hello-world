#include <stdio.h>
#include <malloc.h>

struct node
{
   int data;
   struct node *left;
   struct node *right;
};

struct node *tree, *ptr;

void create_tree();
struct node *insertElement(struct node *, int);
void print(struct node *, int);
void preorderTraversal(struct node *);
void inorderTraversal(struct node *);
void postorderTraversal(struct node *);
struct node *findSmallestElement(struct node *);
struct node *findLargestElement(struct node *);
struct node *deleteElement(struct node *, int);
int totalNodes(struct node *);
int totalExternalNodes(struct node *);
int totalInternalNodes(struct node *);
int Height(struct node *);

int main(void)
{
   int val;
   create_tree();
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

    printf("\n The height of the tree = %d", Height(tree));
}


void create_tree(void)
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
         else if(val>nodeptr->data)
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
