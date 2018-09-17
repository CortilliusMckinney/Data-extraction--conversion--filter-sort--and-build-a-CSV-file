
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LAT 0
#define LOCID 1 

// Required functions.
float sexag2decimal(char *degreeString);

// ***************************
//    AVL TREE
// 
//    - copied from lab 3 with
//      modifications for this hw
// ***************************
/*
 * This code is provided by M. McAlpin and can be freely used for solving
 * problems in COP3502 (aka CS1).
 * 
 * In the event you need to use it for some other application or course
 * PLEASE confirm with the appropriate parties. At the minimum, if using
 * this code for another course, please check with the author 
 * at the following email address:
 *          michael.mcalpin@ucf.edu
 * 
 */
/* Recap left & right rotations (simple case)
T1, T2 and T3 are subtrees of the tree rooted with y (on left side) 
or x (on right side)           
                y                               x
               / \     Right Rotation          /  \
              x   T3   â€“ â€“ â€“ â€“ â€“ â€“ â€“ >        T1   y 
             / \       < - - - - - - -            / \
            T1  T2     Left Rotation            T2  T3
Keys in both of the above trees follow the following order 
      keys(T1) < key(x) < keys(T2) < key(y) < keys(T3)
So BST property is not violated anywhere.
*/

 
// An AVL tree node
struct node
{
    airPdata * key;
    struct node *left;
    struct node *right;
    int height;
};

// A utility function to get the maximum of two integers.
int max(int a, int b);

// A utility function to get the maximum of two airPdata objects,
// depending on the specified sortOrder (LAT or LOCID)
airPdata* maxAirPdata(char sortOrder, airPdata * a, airPdata * b);
 
// A utility function to get height of the tree
int height(struct node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
 
// A utility function to get the maximum of two integers.
int max(int a, int b) {
	return a >= b ? a : b;
}

// A utility function to get the maximum of two airPdata
airPdata* maxAirPdata(char sortOrder, airPdata * a, airPdata * b)
{
    if (sortOrder == LAT) {
    	return (a->latitude > b->latitude)? a : b;
    } else {
		return (strcmp(a->LocID, b->LocID) > 0) ? a : b;
    }
}
 
/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct node* newNode(airPdata * key)
{
    struct node* node = (struct node*)
                        malloc(sizeof(struct node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}
 
// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct node *rightRotate(struct node *y)
{
    struct node *x = y->left;
    struct node *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
 
    // Return new root
    return x;
}
 
// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct node *leftRotate(struct node *x)
{
    struct node *y = x->right;
    struct node *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
 
    // Return new root
    return y;
}
 
/* 
 * RECAP Balance is based on Height
 *     Hn = Hl - Hr 
 * so 
 *    positive => LEFT HEAVY
 *    negative => RIGHT HEAVY
 */
// Get Balance factor of node N
int getBalance(struct node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
 
struct node* insert(struct node* node, airPdata * key, char sortOrder)
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));
 
    if (maxAirPdata(sortOrder, key, node->key) == node->key)
        node->left  = insert(node->left, key, sortOrder);
    else
        node->right = insert(node->right, key, sortOrder);
 
    /* 2. Update height of this ancestor node */
    node->height = max(height(node->left), height(node->right)) + 1;
 
    /* 3. Get the balance factor of this ancestor node to check whether
       this node became unbalanced */
    int balance = getBalance(node);
 
    // If this node becomes UNBALANCED, then there are 4 cases
    
    /* CASE # 1 => LEFT-LEFT aka left?
       T1, T2, T3 and T4 are subtrees.
         z                                      y 
        / \                                   /   \
       y   T4      Right Rotate (z)          x      z
      / \          - - - - - - - - ->      /  \    /  \ 
     x   T3                               T1  T2  T3  T4
    / \
  T1   T2
     */
    // Left Left Case in code
    if (balance > 1 && maxAirPdata(sortOrder, key, node->left->key) == node->left->key)
        return rightRotate(node);
    
    /* Case #2 => RIGHT-RIGHT aka right?

       z                                y
      /  \                            /   \ 
     T1   y     Left Rotate(z)       z      x
         / \   - - - - - - - -->    / \    / \
	T2  x                     T1  T2 T3  T4
       / \
     T3  T4

     */
    // Right Right Case in code
    if (balance < -1 && maxAirPdata(sortOrder, key, node->right->key) == key)
        return leftRotate(node);

    
    /* CASE # 3 => LEFT-RIGHT aka left-right?
     z                               z                           x
    / \                            /   \                        /  \ 
   y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
  / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
T1   x                          y    T3                    T1  T2 T3  T4
    / \                        / \
  T2   T3                    T1   T2

    */
    // Left Right Case in code
    if (balance > 1 && maxAirPdata(sortOrder, key, node->left->key) == key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
    /* CASE #4 = RIGHT-LEFT aka right-left?
        z                            z                            x
       / \                          / \                          /  \ 
      T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
      / \   - - - - - - - - ->     /  \      - - - - - - - ->  / \    /	\
    x   T4                       T2    y                      T1  T2  T3  T4
   / \                                /  \
 T2   T3                             T3   T4   
     */
    // Right Left Case in code
    if (balance < -1 && maxAirPdata(sortOrder, key, node->right->key) == node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
 
    /* return the (unchanged) node pointer */
    return node;
}
 

// *************************
//      airPdata output
// *************************

void print(airPdata * data) {
	printf("%s,", data->LocID);
	printf("%s,", data->fieldName);
	printf("%s,", data->city);
	printf("%0.4lf,", data->latitude);
	printf("%0.4lf\n", data->longitude);
}

// A utility function to print preorder traversal of the tree.
// The function also prints height of every node
void preOrder(struct node *root)
{
    if(root != NULL)
    {
      printf("%s/%1d ", root->key->LocID, root->height);
      preOrder(root->left);
      preOrder(root->right);
    }
}
void inOrder(struct node *root)
{
    if(root != NULL)
    {
      inOrder(root->left);
      print(root->key);
      inOrder(root->right);
    }
}
void postOrder(struct node *root)
{
    if(root != NULL)
    {
      postOrder(root->left);
      postOrder(root->right);
      printf("%s/%1d ", root->key->LocID, root->height);
    }
}
 

// ************************
//        file parsing
// ************************

// finds the requested token from the index line, copying it to
// the given target string.
//
// The "index" parameter tells the function which token to return
// next; for example, if "index" is 3, then it will return the third
// token, skipping the first and second.
void fetchToken(char ** line, int index, char ** target) {
	char * token = strsep(line, ",");
	while (index > 0) {
		token = strsep(line, ",");
		index--;
	}
	//printf("\ttoken: %s\n", token);
	*target = (char*) malloc((strlen(token)+1) * sizeof(char));
	strcpy(*target, token);
}

// The main parsing function. Reads from the given file pointer and allocates
// and populates an airPdata struct. Returns a pointer to the airPdata struct.
airPdata * read(FILE* fp) {
	airPdata * data;
	char * line;
	char * origLine;
	size_t lineSize = 256;
	size_t size;
	char * direction;

	line = (char*) malloc(lineSize * sizeof(char));
	origLine = line;
    
    direction = (char*) malloc(15 * sizeof(char));

	size = getline(&line, &lineSize, fp);
	//printf("READ (%d): %s -> [%c.%c.]\n", size, line, line[0], line[1]);

	if (size == 0 || size == -1 ) return NULL;

	data = (airPdata*) malloc(sizeof(airPdata));

	fetchToken(&line,1, &data->LocID);
	fetchToken(&line,0, &data->fieldName);
	fetchToken(&line,0, &data->city);
	fetchToken(&line,4, &direction);
    data->latitude = sexag2decimal(direction);
	fetchToken(&line,0, &direction);
    data->longitude = sexag2decimal(direction);

	free(origLine);
  free(direction);
	return data;		
}

// Frees the strings allocated for the given airPdata struct
void freeData(airPdata * data) {
	free(data->LocID);
	free(data->fieldName);
	free(data->city);
}

// Frees an avl subtree rooted at n.
void freeTree(struct node * n) {
	if (n == NULL) return;
	freeTree(n->left);
	freeTree(n->right);
	freeData(n->key);
	free(n);
}


// *****************************
//    converts base-60 degree string
//    to a float.
// *****************************
float sexag2decimal(char *degreeString) {
    
        float result = 0.0;
        int dd, mm, ss, mas;
        int dd2, mm2, ss2, mas2;
		char sign[2];
		char sign2[2];
		int conversions;
        
        if (degreeString == NULL) return 0.0;
        
		//printf("\t\treading direction:\n");
        conversions = sscanf(degreeString, "%d-%d-%d.%d%s", &dd, &mm, &ss, &mas, sign);
        if (conversions < 5) return 0.0;
        if (dd < 0 || dd > 180) return 0.0;
        if (mm < 0 || mm > 59) return 0.0;
        if (ss < 0 || ss > 59) return 0.0;
        if (mas < 0 || mas > 9999) return 0.0;
        if (sign[0] != 'S' && sign[0] != 'N' && sign[0] != 'W' && sign[0] != 'E') return 0.0;
        
        result += dd + mm/60.0 + (ss + mas/10000.0)/ 3600.0;
        if (sign[0] == 'S' || sign[0] == 'W') result *= -1;
        
        return result;
    
}

// function to check for valid LocID entries. Valid entries must be strings
// of length 3 or 4, containing alphabetic characters only.
char isValidLocID(airPdata * data) {
	
  int i;
	
	if (data == NULL) return 0;

	if (data->LocID == NULL) return 0;

	if (strlen(data->LocID) != 3 && strlen(data->LocID) != 4) return 0;

	for (i = 0; i < strlen(data->LocID); i++) {
		char c = data->LocID[i];
		if (c < 'A' ||  c > 'z' || (c > 'Z' && c < 'a')) return 0;
	}

	return 1;

};


// The main program, reading in airPdata from a file, putting them
// into an AVL tree, and then outputing them in the order requested.
int main(int argc, char * argv[]) {
	airPdata * data;
	char sortOrder;
	struct node * root;
	FILE* fp;

	if (argc < 2) {
		printf ("hw4Sort ERROR: No filename provided.\n");
		return 1;
	}

	if (argc < 3) {
		printf("hw4Sort ERROR: sortParameter invalid or not found.\n");
		return 1;
	}

	if (strcmp(argv[2], "a") == 0 || strcmp(argv[2], "A") == 0) sortOrder = LOCID;
	else if (strcmp(argv[2], "n") == 0 || strcmp(argv[2], "N") == 0) sortOrder = LAT;
	else {
		printf("hw4Sort ERROR: sortParameter invalid or not found.\n");
		return 1;
	}

	fp = fopen(argv[1], "r");
	
	if (fp == NULL) {
		printf("hw4Sort ERROR: File \"%s\" not found.\n", argv[1]);
		return 1;

	}

	// Read in the data items, adding valid items to the AVL tree.
	root = NULL;
	while ( !feof(fp) ) {

		data = read(fp);
		//printf("DATA READ\n");
		if (data != NULL) {
			// Only deal with "regular" airports, ignore helipads etc.
			//printf("Check data:\n");
			//print(data);
			if (isValidLocID(data)) {
				//printf("Valid\n\n");
				root = insert(root, data, sortOrder);
			} else {
				//printf("Invalid\n\n");
				freeData(data);
			}
		}
		//printf("DATA STORED\n");

	}

	// Print the result
	printf("code,name,city,lat,lon\n");
	inOrder(root);
	
	// Free dynamic memory allocations.
	freeTree(root);

	return 0;
}

