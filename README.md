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
  [Progress](#Progress)
- Not all typedefs are implemented
- Not all undefined behaviour works the same way. For instance,
  iterating past the end causes a loop back to the start.

## Progress

### Basic use

- [x] Empty constructor
- [x] Count constructor
- [x] Range constructor
- [x] Assignment operators
- [x] Push back/push front
- [ ] Emplace back/emplace front
- [x] Pop back/pop front
- [x] Iterators
- [x] Const iterators
- [ ] Reverse iterators
- [ ] Const reverse iterators
- [ ] Insert at iterator
- [ ] Delete at iterator
- [ ] Sizing
- [ ] Resizing

### Comparisons

- [ ] Equality/inequality
- [ ] Greater than/less than

### Algorithmic use

- [ ] sort
- [ ] merge
- [ ] splice
- [ ] remove (if)
- [ ] unique

## Why would you do this?

I learned about C++ smart pointers but since the main things I write
code for are university assignments (not a lot of memory management in
general) and programming contest submissions (no need for readable,
maintainable code) so I never looked into it.

For the summer I wanted a programming challenge to do, so here it is.
