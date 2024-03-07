#CECS 342 Lab 3 Simplified Guide

#Due Date

    Wednesday, March 13
    Points: 30

# Dynamic Memory Allocator Guide

## Introduction

This guide provides an overview of how to implement a dynamic memory allocator in C, focusing on the key concepts of "Overhead Size" and "Pointer Size". These are fundamental to managing memory efficiently in a custom allocator using a free list and first-fit block selection strategy.

## Basic Idea

Your dynamic memory allocator works like a smart system in a library. Instead of books, you're dealing with memory spaces (blocks). When someone (a program) needs space (like a book), they ask your system. Your system then checks if it has a space big enough and hands it over. If the requested space isn't needed anymore, it's returned back to be used by someone else later.

## Understanding `main.c`

This file acts as the interface for interacting with your memory allocator. It offers a menu to perform different actions, like borrowing space for integers, characters, or even arrays. After using the space, you can also return it. Each menu option is a different scenario of using this library system, showing how flexible and efficient it is.

- **menuOptionOne to Five**: These functions are specific scenarios, like asking for space to store one or more integers, a character, arrays, etc. They show how to allocate and deallocate memory, and print addresses to understand where each item is stored.
- **main function**: This is where your program starts. It presents a menu to choose different scenarios, initializes the memory allocator with a predefined size, and then executes the chosen scenario.

## Understanding `memoryhelp.c`

This file contains the logic of your memory allocator, handling the behind-the-scenes work.

- **Struct Block**: Think of this as the information tag on each book in the library, telling you how big it is and what the next book is.
- **my_initialize_heap**: This function sets up your library before anyone starts borrowing space. It's like setting up the shelves and deciding how much room you have.
- **my_alloc**: This is when someone asks for space. This function checks if there's enough room, finds the right spot, and might even split a large space into a smaller one to fit the request perfectly. It ensures that no space is wasted.
- **my_free**: This is when borrowed space is returned back. It makes sure the returned space is marked as available for someone else to use.

## Key Concepts

### Overhead Size

The "Overhead Size" refers to the additional memory required to store management information for each memory block (or node) managed by the allocator. This information typically includes the size of the block and a pointer to the next free block.

- **Purpose**: Allows the allocator to keep track of memory blocks and manage the free list effectively.
- **Calculation**: Use `sizeof(struct Block)` to determine the overhead size, where `struct Block` includes at least `block_size` and `next_block`.

### Pointer Size

The "Pointer Size" is the size required to store a memory address, which varies based on the system architecture (e.g., 4 bytes on a 32-bit system, 8 bytes on a 64-bit system).

- **Purpose**: Ensures that memory allocations are aligned correctly and that the allocator respects the minimum block size for efficient memory usage and access.
- **Importance**: Critical for aligning blocks in memory, determining the minimum usable block size, and preventing fragmentation.

## Simplified Example

Imagine you're organizing boxes (blocks) on a shelf (memory):

- **Overhead Size**: Each box needs a label (metadata) that tells you what's inside and where the next box is. The space the label takes is the overhead.
- **Pointer Size**: The shelf rules say each box must fit neatly in a space that's at least as wide as a label holder (the minimum block size, related to the pointer size). This ensures every box fits properly and can be easily found and accessed.

This analogy helps illustrate how "Overhead Size" is necessary for identifying and managing each block, while "Pointer Size" ensures that every block is placed correctly and efficiently on the "shelf" (memory).

## Implementation Guidelines

1. **Define a Block Structure**: Include necessary metadata for memory management (e.g., block size and next block pointer).

2. **Calculate Overhead Size**: Determine the size of your `Block` struct to account for the metadata in each memory block.

3. **Store Pointer Size**: Keep the size of a pointer in a global constant for memory alignment and block size calculations.

4. **Implement Allocation and Deallocation**: Use the first-fit strategy for allocating memory from the free list and properly manage adding blocks back to the free list upon deallocation.

5. **Test Thoroughly**: Ensure your allocator handles various allocation and deallocation scenarios correctly without causing memory leaks or fragmentation.

# Memory Allocator Testing Summary

## Simple Overview

We tested a program designed to organize memory efficiently, akin to neatly arranging items in a drawer. Our goal was to ensure various items (`int`, `char`, arrays) could be placed and retrieved without wasting space. Here's a breakdown of our tests and their outcomes, explained simply.

## The Tests

### Test 1: Reusing Space

- **What We Did**: Placed an `int` in memory, removed it, then placed another `int`.
- **Outcome**: The new `int` occupied the same spot as the first one, showing we can reuse space smartly.

### Test 2: Neat Arrangement

- **What We Did**: Placed two `int`s next to each other.
- **Outcome**: They fit well with a small, planned gap for organization, indicating our spacing rules work.

### Test 3: Efficient Space Management

- **What We Did**: Inserted three `int`s, removed the middle one, then added two doubles and another `int`.
- **Outcome**: The new items fit perfectly into the available space, demonstrating efficient use and reuse of space.

### Test 4: Handling Different Sizes

- **What We Did**: Placed a `char` and an `int` in memory.
- **Outcome**: Both had the appropriate amount of space between them, respecting our organizational rules.

### Test 5: Accommodating Large Items

- **What We Did**: Allocated space for an 80-element `int` array, then added one more `int`.
- **Outcome**: The large item and the new `int` were correctly spaced, showing we can manage items of varying sizes together.

## Conclusion

Our memory allocator passed all tests with flying colors, proving its capability to efficiently organize different types of data in memory. It's like having a perfectly organized drawer where everything fits just right and can be easily found when needed. Understanding and implementing the concepts of "Overhead Size" and "Pointer Size" are crucial for developing an efficient dynamic memory allocator. These principles help manage memory blocks effectively, ensuring that your allocator performs well and utilizes memory resources wisely.
