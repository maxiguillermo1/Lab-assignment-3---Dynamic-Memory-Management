
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Block {
	int block_size;
	struct Block* next_block;
};

const int OVERHEAD_SIZE = sizeof(struct Block);
const int POINTER_SIZE = sizeof(void*);
struct Block* free_head;

void my_initialize_heap(int size) {
    // Allocate a large memory block to serve as the heap
    free_head = (struct Block*)malloc(size + sizeof(struct Block));
    if (free_head != NULL) {
        // Initialize the first (and initially the only) block in the free list
        free_head->block_size = size;
        free_head->next_block = NULL; // No next block yet
    }
}

void* my_alloc(int size) {
    if (size <= 0) {
        printf("Size must be greater than 0.\n");
        return NULL;
    }

    // Ensure the requested size is aligned to POINTER_SIZE
    int alignedSize = (size + POINTER_SIZE - 1) & ~(POINTER_SIZE - 1);
    int totalSize = alignedSize + OVERHEAD_SIZE;

    struct Block* curr = free_head;
    struct Block* prev = NULL;

    while (curr != NULL) {
        if (curr->block_size >= totalSize) { // Check if the current block can satisfy the request
            if (curr->block_size >= totalSize + sizeof(struct Block) + POINTER_SIZE) { // Check if splittable
                // Split block code here
                struct Block* newBlock = (struct Block*)((char*)curr + totalSize);
                newBlock->block_size = curr->block_size - totalSize;
                newBlock->next_block = curr->next_block;
                
                curr->block_size = alignedSize;
                if (prev) {
                    prev->next_block = newBlock;
                } else {
                    free_head = newBlock;
                }
            } else { // Not splittable
                if (prev) {
                    prev->next_block = curr->next_block;
                } else {
                    free_head = curr->next_block;
                }
            }
			printf("Considering block at %p with size %d for allocation\n", (void*)curr, curr->block_size);

            return (void*)((char*)curr + OVERHEAD_SIZE); // Return the allocated memory, skipping the Block overhead
        }
        prev = curr;
        curr = curr->next_block;
    }
    return NULL; // No suitable block found
}


void my_free(void* ptr) {
    if (ptr == NULL) return;

    // Convert ptr back to a Block pointer (considering the overhead)
    struct Block* blockToFree = (struct Block*)((char*)ptr - OVERHEAD_SIZE);
    blockToFree->next_block = free_head; // Insert the block at the beginning of the free list
    free_head = blockToFree;
	printf("Freeing block at %p, new free_head is %p\n", (void*)blockToFree, (void*)free_head);


    // Optional: merge adjacent free blocks to reduce fragmentation
}


void menuOptionOne() {
   int *numOne = my_alloc(sizeof(int));
   printf("Address of int A: %p\n", numOne); 
   my_free(numOne); 

   int *numTwo = my_alloc(sizeof(int));
   printf("Address of int B: %p\n", numTwo); 
};

//Allocate two ints and print their addresses; they should be exactly the size of your overhead plus the
//larger of (the size of an integer; the minimum block size) apart.
void menuOptionTwo() {
   int *numOne = my_alloc(sizeof(int));
   printf("Address of int A: %p\n", numOne); 
   int *numTwo = my_alloc(sizeof(int));
   printf("Address of int B: %p\n", numTwo); 
   printf("Verifying Results...\n");
   int overheadPlusLarger = OVERHEAD_SIZE + sizeof(void*); 
   printf("Size of overhead + larger of (the size of an integer; the minimum block size): %d bytes\n", overheadPlusLarger); 
   printf("Address B - Address A: %d bytes \n", (int)numTwo - (int)numOne); 
};

//Allocate three ints and print their addresses, then free the second of the three. Allocate an array of 2
//double values and print its address (to allocate an array in C, allocate (2 * sizeof(double)); verify
//that the address is correct. Allocate another int and print its address; verify that the address is the
//same as the int that you freed.
void menuOptionThree() {
   int *numOne = my_alloc(sizeof(int));
   printf("Address of int A: %p\n", numOne); 
   int *numTwo = my_alloc(sizeof(int));
   printf("Address of int B: %p\n", numTwo); 
   int *numThree = my_alloc(sizeof(int));
   printf("Address of int C: %p\n", numThree); 
   my_free(numTwo); 

   printf("After freeing int B...\n"); 
   double *arr = my_alloc(2 * sizeof(double));
   printf("Address of array of 2 double values: %p\n", arr); 

   int *numFour = my_alloc(sizeof(int));
   printf("Address of int D (should be the int B): %p\n", numFour); 
};

//Allocate one char, then allocate one int, and print their addresses. They should be exactly the same
//distance apart as in test #2.
void menuOptionFour() {
   int check = 0; 

   char *charOne = my_alloc(sizeof(char));
   printf("Address of char A: %p\n", charOne); 
   int *numTwo = my_alloc(sizeof(int));
   printf("Address of int B: %p\n", numTwo); 
   
   int overheadPlusLarger = OVERHEAD_SIZE + sizeof(void*); 
   printf("Size of overhead + larger of (the size of an integer; the minimum block size): %d\n", overheadPlusLarger); 

}; 

//Allocate space for a 80-element int array, then for one more int value. Verify that the address of
//the int value is 80 * sizeof(int) + the size of your header after the array's address. Free the array.
//Verify that the int's address and value has not changed.
void menuOptionFive() {
  int *arr = my_alloc(80 * sizeof(int));
  int *numOne = my_alloc(sizeof(int));
  printf("Address of array: %p\n", arr); 
  printf("Address of int A: %p\n", numOne);
  printf("Address of int value: %p\n", arr + 80 + sizeof(int));
  printf("Value of int A: %d\n", *numOne); 

  printf("Difference betwween array and int A: %d\n", (int)numOne - (int)arr); 

  my_free(arr); 

  printf("After freeing array...\n"); 
  printf("Address of int value: %p\n", numOne);
  printf("Value of int A: %d\n", *numOne); 

}
int main() {
     int menuChoice = 0;
   int runAgain = 1;
   while (runAgain == 1) {
      printf("\n1. Allocate an int \n2. Allocate two ints \n3. Allocate three ints \n4. Allocate one char \n5. Allocate space for an 80-element int array \n6. Quit \nChoose a menu option: ");
      scanf("%d", &menuChoice); 
      printf("\n---Test Case %d---\n", menuChoice); 
      my_initialize_heap(1000); 

      if (menuChoice == 1) {
         menuOptionOne(); 
      }

      else if (menuChoice == 2) {
         menuOptionTwo(); 
      }

      else if (menuChoice == 3) {
         menuOptionThree(); 
      }

      else if (menuChoice == 4) {
         menuOptionFour();
      }

      else if (menuChoice == 5) {
         menuOptionFive(); 
      } 

      else if (menuChoice == 6) {
         printf("Done!");
         runAgain = 0; 
      }
   } 
   return 0;
}
