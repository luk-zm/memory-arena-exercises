# memory-arena-exercises
This repository contains solutions to exercises from the talk ["Enter The Arena: Simplifying Memory Management" by Ryan Fleury](https://www.rfleury.com/p/enter-the-arena-talk).

# Exercises
1. A Fixed-Capcity Arena
    - Write an arena implementation
    - This arena should operate on a fixed-capacity buffer, and not grow at all
    - It can obtain this buffer via **malloc** or equivalent
    - If the arena runs out of space, panic
2. The Chaining Upgrade
    - Extend your arena implementation from #1 by allowing growth via chaining
    - It can obtain each new chunk via **malloc** or equivalent
    - It should require no API changes - optionally, you may use this step to remove the capacity parameter in **ArenaAlloc**, or to extend **ArenaAlloc**'s parameters to include dynamic setting of this feature.
3. Using the MMU
    - Extend your arena implementation from #2 by allowing growth within a single chunk via initial virtual address space reservation, with physical memory backing pages only as necessary
    - Address space reservation cannot be done with **malloc** - it will require operating-system-specific APIs like **VirtualAlloc** (Windows) or **mmap** (Linux)
    - It should require no API changes - but you can also dynamically enable this feature
4. Humiliating std::vector
    - Use your arena from #3 to implement a growing dynamic array which never relocates
5. Supporting Sub-Lifetimes
    - Use your arena to implement a simple growing pool allocator, using a free list
    - This should be only for *N* known size, where *N* is small and statically known (like 1)
6. Visualization & Debugging
    - Extend your arena with optionally-enabled diagnostics logging, which - for each push & pop - logs timestamps, caller source code location, and arena allocation position
    - Use this logging in a visualizer
7. Convert Real Code
    - Use your arena allocator, and bag of techniques, in an existing codebase which uses non-arena, **malloc**/**free** style memory management
    - Conver the whole thing, or a reasonable subset of it
    - Measure performance before and after
    - Measure LOC before and after, if you're able to entirely elimineate dependance on **malloc**/**free**, in both usage/implementation code
