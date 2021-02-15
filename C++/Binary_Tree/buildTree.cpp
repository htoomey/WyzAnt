#include <iostream>
#include <string>
#include <stdio.h>
#include "buildTree.h"
using namespace std;

#define SIZE 100

// Function prototypes
void generatePostOrder(int, int[], int[]);
int createTreeFromSegment(int, int, int, int[], int[], struct btreeNode *);
struct btreeNode *createNewNode (int);
void postOrderStuffArray (struct btreeNode *, int post[]);
void printBtreeArrays(int, int[], int[], int[]);
void postorderDelete(struct btreeNode *);


int main(void)
{
	int i;
	int numberOfNodes;
    int preOrder[SIZE];
    int inOrder[SIZE];

	for(i = 0; i < SIZE; i++)
	{
		preOrder[i] = 0;
		inOrder[i] = 0;
	}
	cin >> numberOfNodes;

	for(i = 0; i < numberOfNodes; i++)
		cin >> inOrder[i];
	for(i = 0; i < numberOfNodes; i++)
		cin >> preOrder[i];

	generatePostOrder(numberOfNodes,inOrder,preOrder);
    return (0);
}


// ************************************************************
// Build the binary tree one node at a time from pre and in order arrays.
// ************************************************************

void generatePostOrder(int numberNodes, int in[SIZE], int pre[SIZE])
{
   struct btreeNode *treeRoot = NULL;
   int post[SIZE];

   // Error Checking: Exit if number of nodes is zero or negative
   if (numberNodes <= 0) return;

   // Start with entire set of arrays, then recursively call the
   // function to process the left and right sides until done.
   createTreeFromSegment(0, numberNodes, 0, pre, in, treeRoot);

   // Traverse tree recursively in post-order and stuff values into post array
   postOrderStuffArray (treeRoot, post);

   // Print out pre, in and post arrays
   printBtreeArrays(numberNodes, pre, in, post);

   // Delete binary tree before exiting
   postorderDelete(treeRoot);
}


// ************************************************************
// Call recursively to build the binary tree
// ************************************************************

int createTreeFromSegment(	int segmentStartIndex,
							int segmentLength,
							int inOffset,
						  	int pre[SIZE],
						  	int in[SIZE],
						  	struct btreeNode *parentNode)
{
   int leftSegmentStartIndex, leftSegmentLength;
   int rightSegmentStartIndex, rightSegmentLength;


   // Exit if there is no segment remaining
   if (segmentLength == 0) return (0);

   // Create and initialize the root node
   if (parentNode == NULL)
      parentNode = createNewNode (pre[0]);



   // Split array into left and right segments
   leftSegmentStartIndex = segmentStartIndex;

   // Find left segment
   leftSegmentLength = 0;
   while ((pre[leftSegmentStartIndex] != in[leftSegmentStartIndex - inOffset + leftSegmentLength]) &&
   		  (leftSegmentLength < segmentLength))
   {
   	   leftSegmentLength++;
   }

   // Left segment found.  Now calculate right segment.
   rightSegmentStartIndex = leftSegmentStartIndex + leftSegmentLength;
   rightSegmentLength = segmentLength - leftSegmentLength;


   // Now add new left and right nodes, then recursively call this same function to
   // process the left and right segments until done.

   // Scenario 1: [L,R] Non-zero length left and non-zero length right segments found
   if (leftSegmentLength && rightSegmentLength)
   {
	  // Add node to left side of the binary tree
	  parentNode->left = createNewNode (pre[leftSegmentStartIndex]);
      createTreeFromSegment(leftSegmentStartIndex, leftSegmentLength, inOffset+1, pre, in, parentNode->left);

      // Add node to right side of the binary tree
      parentNode->right = createNewNode (pre[rightSegmentStartIndex]);
      createTreeFromSegment(rightSegmentStartIndex, rightSegmentLength, inOffset+1, pre, in, parentNode->right);
   }


   // Scenario 2: [L,0] Non-zero length left and zero length right segments found
   // Example 2a: [2 4 7]  shape: /     Example 2b: [1 2 4]  shape: /
   //             [4 7 2]         \                 [4 2 1]        /
   else if (leftSegmentLength && !rightSegmentLength)
   {
      // Add node to left side of the binary tree
	  parentNode->left = createNewNode (pre[leftSegmentStartIndex]);

      //  If we are down to a 2x2, then finish, otherwise recursively call again
      if (leftSegmentLength == 2)
      {
         parentNode->left->left = createNewNode (pre[leftSegmentStartIndex+1]);

         if (pre[leftSegmentStartIndex + 1] == in[leftSegmentStartIndex - inOffset + 1])	// pre = in
            parentNode->left->left->right = createNewNode (pre[leftSegmentStartIndex+2]);
		 else	// pre != in
            parentNode->left->left->left = createNewNode (pre[leftSegmentStartIndex+2]);
      }
      else
          createTreeFromSegment(leftSegmentStartIndex, leftSegmentLength, inOffset+1, pre, in, parentNode->left);
   }


   // Scenario 3: [0,R] Zero length left and non-zero length right segments found
   // Example 3a: [8 10 11]  shape: \    Example 3b: [6 9 9b]  shape: \
   //             [8 11 10]         /                [6 9 9b]          \
   else if (!leftSegmentLength && rightSegmentLength)
   {
      // Add node to right side of the binary tree
      parentNode->right = createNewNode (pre[rightSegmentStartIndex]);

      //  If we are down to a 2x2, then finish, otherwise recursively call again
      if (rightSegmentLength == 2)
      {
         parentNode->right->right = createNewNode (pre[leftSegmentStartIndex+1]);

         if (pre[leftSegmentStartIndex + 1] == in[leftSegmentStartIndex - inOffset + 1])	// pre = in
            parentNode->right->right->right = createNewNode (pre[leftSegmentStartIndex+2]);
		 else	// pre != in
            parentNode->right->right->left = createNewNode (pre[leftSegmentStartIndex+2]);
      }
      else
         createTreeFromSegment(rightSegmentStartIndex, rightSegmentLength, inOffset+1, pre, in, parentNode->right);
   }

   // Scenario 4: [0,0] Zero length left and zero length right segments found
#if 0
   else
   {
      return (0);	// Exit if there are no segments remaining
   }
#endif

   return (0);

}


// ************************************************************
// Create a new node for our binary tree.
// ************************************************************

struct btreeNode *createNewNode (int num)
{
   struct btreeNode *newBtreeNode = NULL;

   // Create the new binary tree node
   newBtreeNode = new struct btreeNode;

   // Initialize the new binary tree node
   newBtreeNode->data = num;
   newBtreeNode->left = NULL;
   newBtreeNode->right = NULL;
}


// ************************************************************
// Put values of binary tree into post order array
// ************************************************************
void postOrderStuffArray (struct btreeNode *nodePtr, int post[SIZE])
{
   static int i = 0;

   if (nodePtr == NULL) return;

   if (nodePtr->left != NULL)
      postOrderStuffArray(nodePtr->left, post);

   if (nodePtr->right != NULL)
      postOrderStuffArray(nodePtr->right, post);

   post[i] = nodePtr->data;
   i++;
}


// ************************************************************
// Print out pre, in and post arrays
// ************************************************************

void printBtreeArrays(int numberNodes, int pre[SIZE], int in[SIZE], int post[SIZE])
{
   int i;

   // Print out PreOrder array
   cout<<endl<<endl;
   cout<<"Preorder traversal"<<endl;
   for(i=0; i<numberNodes; i++)
      cout<<pre[i]<<", ";

   // Print out InOrder array
   cout<<endl<<endl;
   cout<<"Inorder traversal"<<endl;
   for(i=0; i<numberNodes; i++)
      cout<<in[i]<<", ";

   // Print out PostOrder array
   cout<<endl<<endl;
   cout<<"Postorder traversal"<<endl;
   for(i=0; i<numberNodes; i++)
      cout<<post[i]<<", ";

   cout<<endl<<endl;
}


// ************************************************************
// Give back the memory used by the binary tree
// ************************************************************

void postorderDelete(struct btreeNode *nodePtr)
{
   if (nodePtr == NULL) return;

   if (nodePtr->left != NULL)
      postorderDelete(nodePtr->left);

   if (nodePtr->right != NULL)
      postorderDelete(nodePtr->right);

   // left and right both are NULL, so it is safe to delete
   delete nodePtr;
}



// ************************************************************
//
// Sample Input Data:
// Pre =  1  2  4  7  3  5  8  10 11 6  9
// In  =  4  7  2  1  8 11 10  5  3  6  9
//
// Expected output data:
// Post = 7  4  2 11 10  8  5  9  6  3  1
//
// ************************************************************

