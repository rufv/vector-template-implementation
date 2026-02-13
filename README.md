# vector-template-implementation
A template-based C++ Vector class implementation featuring dynamic resizing, move semantics, and random-access iterators. Developed for educational purposes.
---

# Custom C++ Vector Implementation

A high-performance, template-based dynamic array implementation in C++, designed as a custom alternative to `std::vector`. This project focuses on manual memory management, template programming, and iterator logic.

## Key Features
- **Dynamic Memory Management:** Uses `std::unique_ptr` for safe RAII-based memory handling.
- **Template Core:** Supports any data type via C++ Templates.
- **STL Compatibility:** Full support for `Iterator` and `ConstIterator`, allowing the use of range-based for loops and `<algorithm>` library.
- **Exception Safety:** Boundary checking on element access.

## Technical Specification

The class manages three internal states: `sz` (current size), `max_sz` (total capacity), and a `values` array.

### Constructors & Destructor
* `Vector()`: Initializes an empty vector with a default minimum capacity (5).
* `Vector(size_type n)`: Initializes a vector with a specific capacity `n`.
* `Vector(initializer_list)`: Allows elegant initialization like `Vector v = {1, 2, 3}`.

### Capacity Management
* `reserve(n)`: Pre-allocates memory for at least `n` elements to prevent frequent reallocations.
* `shrink_to_fit()`: Reduces memory usage by freeing unused capacity to match the current size.
* `push_back(val)`: Adds an element to the end. If the capacity is full, it automatically doubles the internal storage.
* `pop_back()`: Removes the last element from the container.

### Element Access
* `operator[]`: Provides fast, direct access to elements (no bounds checking).
* `at(index)`: Provides safe access. Throws `std::out_of_range` if the index is invalid.
* `front()` / `back()`: Access the first and last elements respectively.

### Iterators
The implementation includes `begin()` and `end()` methods that return:
- `Iterator`: For read-write access.
- `ConstIterator`: For read-only access, ensuring `const` correctness for the container.

## Usage Example
```cpp
#include "vector.h"
#include <iostream>

int main() {
    // Initialization using initializer_list
    Vector<int> vec = {1, 2, 3, 4, 5};

    // Adding elements (triggers dynamic resizing if needed)
    vec.push_back(6);

    // Using range-based for loop (thanks to Iterators)
    for (const auto& element : vec) {
        std::cout << element << " ";
    }

    // Safe access
    try {
        int value = vec.at(10); 
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

## Testing
Unit testing is performed using the [doctest](https://github.com/doctest/doctest) framework. The suite covers:
- Copy semantics (copy constructor and assignment operator).
- Memory growth and reallocation logic.
- Iterator arithmetic and validity.

---
*This is an educational project developed for the "Programmierung 2" course (SS25).*
