# CLAUDE.md - Atomic Module

This file provides guidance to Claude Code when working with the Atomic module of Malterlib.

## Module Overview

The Atomic module provides thread-safe atomic operations and memory ordering utilities for concurrent programming in Malterlib. It wraps standard C++ atomic operations with additional functionality and consistent naming conventions across the framework.

## Module Structure

```
Atomic/
├── Include/Mib/Atomic/
│   └── Atomic              # Main include file for atomic functionality
├── Source/
│   ├── Malterlib_Atomic.h  # Core atomic implementation headers
│   └── Malterlib_Atomic.cpp # Implementation file
├── Test/
│   └── Test_Malterlib_Atomic.cpp # Unit tests for atomic operations
└── Documentation/
    └── Malterlib_Atomic.dox # Doxygen documentation
```

## Key Components

### TCAtomic Template
- **Purpose**: Main atomic type wrapper around std::atomic with Malterlib naming conventions
- **Location**: `Source/Malterlib_Atomic.h`
- **Features**:
  - Inherits from std::atomic
  - Provides all atomic operations with Malterlib-style method names
  - Supports constexpr construction (modern C++)
  - Full set of atomic operations: load, store, exchange, compare-exchange
  - Arithmetic operations: fetch_add, fetch_sub, increment/decrement operators
  - Bitwise operations: fetch_and, fetch_or, fetch_xor
  - Compound assignment operators: +=, -=, &=, |=, ^=
  - Lock-free operations when supported by hardware
  - Memory ordering control
  - Volatile support for memory-mapped I/O

### CAtomicFlag
- **Purpose**: Main atomic flag wrapper around std::atomic_flag
- **Features**:
  - Inherits from std::atomic_flag
  - Test-and-set and clear operations
  - Always lock-free by specification

### Memory Ordering
The module provides the following memory ordering options:
- `gc_MemoryOrder_Relaxed` - No synchronization or ordering constraints
- `gc_MemoryOrder_Consume` - Data dependency ordering for reads
- `gc_MemoryOrder_Acquire` - Synchronize with release operations
- `gc_MemoryOrder_Release` - Synchronize with acquire operations
- `gc_MemoryOrder_AcquireRelease` - Both acquire and release semantics
- `gc_MemoryOrder_SequentiallyConsistent` - Total ordering (default)

## Usage Guidelines

### Including the Module
```cpp
#include <Mib/Atomic/Atomic>
```

### Basic Atomic Operations
```cpp
using namespace NMib::NAtomic;

// Modern C++ with constinit for compile-time initialization of globals
constinit TCAtomic<int32> g_Counter(0);  // Global/static with constinit
TCAtomic<int32> LocalCounter(42);        // Local variable

// Store and load values
LocalCounter.f_Store(100);
int32 Value = LocalCounter.f_Load();

// Assignment operator
LocalCounter = 200;

// Arithmetic operations (for integral types)
LocalCounter.f_FetchAdd(5);   // Returns old value
LocalCounter.f_FetchSub(3);   // Returns old value

// Bitwise operations
LocalCounter.f_FetchAnd(0xFF);  // Returns old value
LocalCounter.f_FetchOr(0x01);   // Returns old value
LocalCounter.f_FetchXor(0xAA);  // Returns old value

// Exchange operation
int32 OldValue = LocalCounter.f_Exchange(500);

// Compare and exchange
int32 Expected = 500;
bool Success = LocalCounter.f_CompareExchangeWeak(Expected, 600);
// If failed, 'Expected' is updated with the actual value

Expected = 600;
Success = LocalCounter.f_CompareExchangeStrong(Expected, 700);

// Increment/decrement operators
++LocalCounter;  // Pre-increment (returns new value)
LocalCounter++;  // Post-increment (returns old value)
--LocalCounter;  // Pre-decrement (returns new value)
LocalCounter--;  // Post-decrement (returns old value)

// Compound assignment operators
LocalCounter += 10;  // Returns old value
LocalCounter -= 5;   // Returns old value
LocalCounter &= 0xFF; // Returns old value
LocalCounter |= 0x10; // Returns old value
LocalCounter ^= 0xF0; // Returns old value
```

### Memory Ordering
```cpp
// Relaxed ordering for statistics/counters
Counter.f_FetchAdd(1, gc_MemoryOrder_Relaxed);

// Acquire-release for synchronization
Flag.f_Store(true, gc_MemoryOrder_Release);
if (Flag.f_Load(gc_MemoryOrder_Acquire)) {
    // Synchronized access to shared data
}
```

### Atomic Flags
```cpp
CAtomicFlag Flag;  // Default initialization

// Test and set
if (!Flag.f_TestAndSet()) {
    // Successfully acquired flag
    // ... critical section ...
    Flag.f_Clear();
}
```

## Build Integration

### Build Properties
- **MalterlibLibrary_Atomic**: Controls inclusion of Atomic library
- **MalterlibSubLibraries**: Automatically includes "Atomic" when enabled
- **Include Path**: `Include/` directory is added to include roots

### Testing
Tests are located in `Test/Test_Malterlib_Atomic.cpp` and are included in the Tests workspace:
```bash
MalterlibBuildShowProgress=false ./mib test --paths '["Malterlib/Atomic/*"]'
```

## Dependencies

### Internal Dependencies
- **Core**: Platform definitions and type traits
- **Type**: Type traits and utilities

### External Dependencies
- Standard C++ `<atomic>` header

## Important Notes

- Modern C++ code should use `TCAtomic` with `constinit` for global/static atomic variables
- All atomic types support volatile qualifiers for memory-mapped I/O scenarios
- Lock-free status can be checked at compile-time (`mc_bIsAlwaysLockFree`) or runtime (`f_IsLockFree()`)
- The module is header-only for most functionality to ensure inlining of atomic operations

## Common Patterns

### Counter Pattern
```cpp
constinit TCAtomic<uint64> g_RequestCount(0);

void HandleRequest() {
    g_RequestCount.f_FetchAdd(1, gc_MemoryOrder_Relaxed);
    // Process request...
}
```

### Producer-Consumer Pattern
```cpp
constinit TCAtomic<bool> g_DataReady(false);
constinit TCAtomic<int> g_Data(0);

// Producer
g_Data.f_Store(42, gc_MemoryOrder_Relaxed);
g_DataReady.f_Store(true, gc_MemoryOrder_Release);

// Consumer
while (!g_DataReady.f_Load(gc_MemoryOrder_Acquire)) {
    // Wait for data
}
int Data = g_Data.f_Load(gc_MemoryOrder_Relaxed);
```

### Spinlock Pattern
```cpp
CAtomicFlag g_Lock;

void CriticalSection() {
    while (g_Lock.f_TestAndSet(gc_MemoryOrder_Acquire)) {
        // Spin
    }

    // Critical section code...

    g_Lock.f_Clear(gc_MemoryOrder_Release);
}
```
