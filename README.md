# safelist -- An memory-safe linked list in C++

[![Build Status](https://travis-ci.org/bertptrs/safelist.svg?branch=master)](https://travis-ci.org/bertptrs/safelist)

This repository contains a header-only reimplementation of `std::list`
using the memory constructs introduced in C++11. The goals of this
project is as follows:

- Have a delete-free implementation of a linked list
- Prevent use-after-free memory errors
- Prevent dangling pointers
- Prevent memory leaks

Other problems, such as null-pointer dereferencing are still very much
possible and will still occur, but will be more predictable.

## Usage

The `safelist` is designed as a drop-in replacement for `std::list`, so
replacing it should not be a problem. Include [safelist.hpp](safelist.hpp)
wherever you would normally include `list`, and change your types
accordingly.

Not everything is implemented exactly as it is in `std::list`. Key
differences are:

- Allocator is not specifiable, and is the default allocator
- Features are lacking according to what it says in
  [Progress](#progress)
- Not all typedefs are implemented
- Not all undefined behaviour works the same way. For instance,
  iterating past the end causes a loop back to the start.
- The complexity of `sort()` is `O(n^2)` rather than the `O(n log n)`
  specified by the stl. Feel free to send a PR with an improvement.

## Progress

### Basic use

- [x] Empty constructor
- [x] Count constructor
- [x] Range constructor
- [x] Assignment operators
- [x] Push back/push front
- [x] Emplace back/emplace front
- [x] Pop back/pop front
- [x] Iterators
- [x] Const iterators
- [ ] Reverse iterators
- [ ] Const reverse iterators
- [ ] Insert at iterator
- [x] Emplace at iterator
- [x] Delete at iterator
- [x] Sizing
- [x] Resizing

### Comparisons

- [x] Equality/inequality
- [x] Greater than/less than

### Algorithmic use

- [x] sort
- [ ] merge
- [ ] splice
- [x] remove (if)
- [x] reverse
- [x] unique

## Why would you do this?

I learned about C++ smart pointers but since the main things I write
code for are university assignments (not a lot of memory management in
general) and programming contest submissions (no need for readable,
maintainable code) so I never looked into it.

For the summer I wanted a programming challenge to do, so here it is.
