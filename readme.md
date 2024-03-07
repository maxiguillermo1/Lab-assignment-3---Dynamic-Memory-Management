#CECS 342 Lab 3 Simplified Guide

#Due Date

    Wednesday, March 13
    Points: 30

# Dynamic Memory Allocator Guide

## Introduction

This guide provides an overview of how to implement a dynamic memory allocator in C, focusing on the key concepts of "Overhead Size" and "Pointer Size". These are fundamental to managing memory efficiently in a custom allocator using a free list and first-fit block selection strategy.

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

## Conclusion

Understanding and implementing the concepts of "Overhead Size" and "Pointer Size" are crucial for developing an efficient dynamic memory allocator. These principles help manage memory blocks effectively, ensuring that your allocator performs well and utilizes memory resources wisely.
