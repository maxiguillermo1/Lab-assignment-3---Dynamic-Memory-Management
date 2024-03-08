// Standard library includes for input/output and dynamic memory allocation
#include <stdio.h>
#include <stdlib.h>

// Definition of a Block structure for managing dynamic memory allocation
struct Block
{
    int block_size;           // Size of the data portion of the block
    struct Block *next_block; // Pointer to the next block in a linked list
};

// Constants representing the size of a Block structure and the size of a pointer
const int OVERHEAD_SIZE = sizeof(struct Block); // Size of the metadata (Block structure)
const int POINTER_SIZE = sizeof(void *);        // Size of a pointer, used to align allocations
struct Block *free_head;                        // Global variable pointing to the head of the free list

// Function to initialize the heap (dynamic memory area managed by this allocator)
void my_initialize_heap(int size)
{
    // Allocate memory for the heap, including space for the Block structure itself
    free_head = (struct Block *)malloc(size + sizeof(struct Block));
    if (free_head != NULL) // Check if allocation was successful
    {
        // Initialize the first block in the heap
        free_head->block_size = size; // Set block size
        free_head->next_block = NULL; // Currently, there is no next block
    }
}

// Function to allocate memory from the heap
void *my_alloc(int size)
{
    if (size <= 0) // Ensure requested size is positive
    {
        printf("Size must be greater than 0.\n");
        return NULL; // Return NULL for invalid size requests
    }

    // Adjust the requested size for alignment and add overhead for the block metadata
    int alignedSize = (size + POINTER_SIZE - 1) & ~(POINTER_SIZE - 1); // Align size up to nearest pointer size
    int requiredSize = alignedSize + OVERHEAD_SIZE;                    // Total size required including overhead

    struct Block *curr = free_head; // Start at the head of the free list
    struct Block *prev = NULL;      // Previous block pointer for traversal

    // Traverse the free list to find a suitable block
    while (curr != NULL)
    {
        if (curr->block_size >= requiredSize) // Check if the current block is large enough
        {
            // Determine if there's enough space in the current block to split it
            if (curr->block_size >= requiredSize + OVERHEAD_SIZE + POINTER_SIZE)
            {
                // Split the block
                struct Block *newBlock = (struct Block *)((char *)curr + requiredSize); // Calculate new block's address
                newBlock->block_size = curr->block_size - requiredSize;                 // Set new block's size
                newBlock->next_block = curr->next_block;                                // Link new block to the next block

                curr->block_size = alignedSize; // Update current block's size

                // Update the free list
                if (prev == NULL) // If splitting the first block in the list
                {
                    free_head = newBlock; // Set free_head to point to the new block
                }
                else // If not the first block
                {
                    prev->next_block = newBlock; // Update previous block to point to the new block
                }
            }
            else // If not enough space to split, allocate the entire block
            {
                if (prev == NULL) // If allocating the first block
                {
                    free_head = curr->next_block; // Update free_head to skip the allocated block
                }
                else // If not the first block
                {
                    prev->next_block = curr->next_block; // Remove the current block from the list by updating previous block's next pointer
                }
            }

            // Return a pointer to the allocated memory (data portion of the block)
            return (void *)((char *)curr + OVERHEAD_SIZE);
        }

        // Move to the next block in the list
        prev = curr;
        curr = curr->next_block;
    }

    // If no suitable block was found, return NULL
    return NULL;
}

// Function to free allocated memory and add it back to the free list
void my_free(void *ptr)
{
    if (ptr == NULL) // Do nothing if NULL pointer is passed
        return;

    // Calculate the address of the block structure based on the given data pointer
    struct Block *blockToFree = (struct Block *)((char *)ptr - OVERHEAD_SIZE);

    // Add the block back to the start of the free list
    blockToFree->next_block = free_head;
    free_head = blockToFree;
}

// The menuOption functions demonstrate different scenarios for allocating and freeing memory
// using the custom memory allocator. They showcase how the allocatorhandles different sizes and patterns of allocation and deallocation, aiming to test the functionality and efficiency of the custom memory management implementation.

// First test case: Allocate and then free an integer, followed by allocating another integer
void menuOptionOne()
{
    // Allocate memory for an integer and print its address
    int *numOne = my_alloc(sizeof(int));
    printf("Address of int A: %p\n", numOne);
    // Free the previously allocated memory
    my_free(numOne);

    // Allocate memory for another integer and print its address
    // This demonstrates that the allocator reuses freed memory
    int *numTwo = my_alloc(sizeof(int));
    printf("Address of int B: %p\n", numTwo);
};

// Second test case: Allocate two integers and check their addresses
void menuOptionTwo()
{
    // Allocate memory for the first integer and check for allocation failure
    int *numOne = my_alloc(sizeof(int));
    if (numOne == NULL)
    {
        printf("Allocation for int A failed.\n");
        return; // Return early if allocation failed
    }
    printf("Address of int A: %p\n", (void *)numOne);

    // Allocate memory for the second integer and check for allocation failure
    int *numTwo = my_alloc(sizeof(int));
    if (numTwo == NULL)
    {
        printf("Allocation for int B failed.\n");
        return; // Return early if allocation failed
    }
    printf("Address of int B: %p\n", (void *)numTwo);

    // Verify and print the distance between the two allocated integers
    printf("Verifying Results...\n");
    int overheadPlusLarger = OVERHEAD_SIZE + (sizeof(int) > POINTER_SIZE ? sizeof(int) : POINTER_SIZE);
    printf("Size of overhead + larger of (the size of an integer; the minimum block size): %d bytes\n", overheadPlusLarger);
    int distance = (char *)numTwo - (char *)numOne;
    printf("Address B - Address A: %d bytes\n", distance);
}

// Third test case: Allocate three integers, free the second, and allocate other data types
void menuOptionThree()
{
    // Allocate memory for three integers and print their addresses
    int *numOne = my_alloc(sizeof(int));
    printf("Address of int A: %p\n", numOne);
    int *numTwo = my_alloc(sizeof(int));
    printf("Address of int B: %p\n", numTwo);
    int *numThree = my_alloc(sizeof(int));
    printf("Address of int C: %p\n", numThree);

    // Free the memory of the second integer
    my_free(numTwo);

    // Allocate memory for an array of 2 double values and print its address
    // This tests the allocator's ability to handle requests of different sizes and types
    printf("After freeing int B...\n");
    double *arr = my_alloc(2 * sizeof(double));
    printf("Address of array of 2 double values: %p\n", arr);

    // Allocate memory for another integer and print its address
    // The address should be the same as the one freed earlier if the allocator reuses freed space efficiently
    int *numFour = my_alloc(sizeof(int));
    printf("Address of int D (should be the int B): %p\n", numFour);
};

// Fourth test case: Allocate a char and an int, and compare their addresses
void menuOptionFour()
{
    // Allocate memory for a char and print its address
    char *charOne = my_alloc(sizeof(char));
    printf("Address of char A: %p\n", charOne);

    // Allocate memory for an integer and print its address
    // This tests how the allocator manages different data sizes and alignment requirements
    int *numTwo = my_alloc(sizeof(int));
    printf("Address of int B: %p\n", numTwo);
};

// Fifth test case: Allocate a large array and verify the allocator's behavior
void menuOptionFive()
{
    // Allocate memory for an 80-element integer array and print its address
    int *arr = my_alloc(80 * sizeof(int));
    printf("Address of array: %p\n", arr);

    // Allocate memory for an additional integer and print its address and value
    // This tests the allocator's ability to handle subsequent allocations after a large allocation
    int *numOne = my_alloc(sizeof(int));
    printf("Address of int A: %p\n", numOne);
    printf("Value of int A: %d\n", *numOne);

    // Calculate and print the difference between the start of the array and the additional integer
    // This helps verify the correct calculation of addresses after a large allocation
    printf("Difference between array start and int A: %ld bytes\n", (char *)numOne - (char *)arr - 80 * sizeof(int));

    // Free the array to test the allocator's ability to reclaim and reuse freed memory
    my_free(arr);

    // After freeing the array, verify and print the address and value of the additional integer
    // This checks if the address and value remain consistent after freeing adjacent memory
    printf("After freeing array...\n");
    printf("Address of int value: %p\n", numOne);
    printf("Value of int A: %d\n", *numOne);
}

// Main function to run the allocator tests
int main()
{
    int menuChoice = 0; // Variable to store the user's menu choice
    int runAgain = 1;   // Flag to control the menu loop

    // Initialize the custom heap with a specific size before running tests
    my_initialize_heap(1000);

    // Loop to repeatedly show the menu and process user input until the user chooses to quit
    while (runAgain == 1)
    {
        // Display menu options to the user
        printf("\n1. Allocate an int \n2. Allocate two ints \n3. Allocate three ints \n4. Allocate one char \n5. Allocate space for an 80-element int array \n6. Quit \nChoose a menu option: ");
        // Read the user's menu choice
        scanf("%d", &menuChoice);
        // Announce the selected test case
        printf("\n---Test Case %d---\n", menuChoice);

        // Switch statement to execute the appropriate test case based on the user's choice
        if (menuChoice == 1)
        {
            menuOptionOne(); // Run the first test case
        }
        else if (menuChoice == 2)
        {
            menuOptionTwo(); // Run the second test case
        }
        else if (menuChoice == 3)
        {
            menuOptionThree(); // Run the third test case
        }
        else if (menuChoice == 4)
        {
            menuOptionFour(); // Run the fourth test case
        }
        else if (menuChoice == 5)
        {
            menuOptionFive(); // Run the fifth test case
        }
        else if (menuChoice == 6)
        {
            // If the user chooses to quit, print a message and exit the loop
            printf("Done!");
            runAgain = 0; // Set flag to exit the loop
        }
    }
    return 0; // End of program
}
