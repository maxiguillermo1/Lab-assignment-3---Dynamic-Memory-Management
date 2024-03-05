# CECS 342 Lab Assignment 3: Custom Dynamic Memory Allocator

## Due Date

**Wednesday, March 13**

## Total Points

30 points

## Overview

In this lab, you will develop your own dynamic memory allocator (heap manager) in C. The implementation will utilize a free list with a first-fit block selection strategy to manage memory allocations and deallocations.

## Implementation Guidelines

Your task is to follow these guidelines closely to implement the custom memory allocator:

1. **Struct Definition**: Define a `struct Block` to represent an allocation block with two members: `block_size` indicating the number of bytes in the data section, and `next_block`, a pointer to the next block in the free list.

   ```c
   struct Block {
       int block_size;
       struct Block *next_block;
   };
