/*
 * Features:
 *   - asserts and sanity checks
 *   - logging, include file, line numbers, function names
 *   - structures/fields in existing structures specifically for debugging purposes: counters, timestamps, state indicators
 *   - data structure validation
 *   - visualize data structures by passing them to a jd_debug_visualize function
 *   - conditional compilation using #ifdef DEBUG blocks
 *   - verbose mode when running
 *   - runtime error detection: 
 *     - boundary checks for arrays and buffer accesses, 
 *     - parameter validations: validate function parameters, esp. for public API functions to catch erroneous usage
 *   - `--attribute--`
 *   - gdb-initiated breakpoints: __asm__("int $3")
 *   - #pragma directives to control compiler-specific behavior for debugging, like disabling optimizations for certain code blocks
 *   - custom verbose error message for incorrect use of api and suggestions for how to use it
 *   - set watchpoint/breakpoint?
 *   - instrumentation functions 
 *     - checking state of program (e.g., state of a data structure) (see SystemTrap/DTrace which are used alongside GDB)
 *   - error handling with stack traces (execinfo.h?)
 *   - variables for keeping track of the state of the program to make it easy to inspect those states at any breakpoint in gdb
 *   - signal handlers
 *   - logging during runtime; env variables or command-line arguments to control verbosity level of the logs
 *   - memory corruption detection: integrate custom memory allocation and deallocation functions that include padding bytes around allocated 
 *       memory to detect buffer overflows, to detect memory leaks, dangling pointers by providing a history of memory operations
 *   - performance monitoring for gdb examination:
 *     - include timestamps
 *     - execution counters
 *   - address sanitization (`-fsanitize=address`)?
 *   - undefined behavior sanitizer (`-fsanitize=undefined`)
 *   - reverse debugging helpers? (for `record`, `reverse-step`, `reverse-continue`)
 *   - watchdog timer for multithreaded or asynchronous code?
 *   - data structure integrity checks: For example, linked lists can include code to verify that they are not corrupted before operations
 *   
 *
 * Other Ideas
 *   - enforce structure; readable variable and function names
 *   - check initialization (lua script to do this? can macros do this?)
 *   - helpers for custom complex gdb commands? python
 *   - self-documenting code
 *
 *
 * Specific Functions:
 *   - jd_debug_log.add_timestamp()
 *   - jd_debug_log.log_state()             // ... of specific variables or data structures
 *   - jd_debug_log.set_verbosity(level)  
 *   - jd_debug_log.enable_file_logging(fil_path)
 *   - jd_debug_log.log_function_entry(function_name)
 *   - jd_debug_log.log_function_exit(function_name)
 *
 *   - jd_debug_visual.draw_memory_map()
 *
 *   - jd_debug_validation.check_data_integrity(data_structure)
 *   - jd_debug_validation.compare_memory_regions(region1, region2)
 *   - jd_debug_validation.api_call(api_function, expected_result)
 *
 *   - jd_debug_assert.validate_pointer(ptr)
 *   - jd_debug_assert.check_bounds(index.size)
 *
 *   - jd_debug_watch.add((var_name)
 *   - jd_debug_watch.remove(var_name)
 *   - jd_debug_watch.list_watches()
 *
 *   - jd_debug_memory.track_alloc(ptr,size)    // Tracks memory allocation for a pointer.
 *   - jd_debug_memory.track_free(ptr)
 *   - jd_debug_memory.report_leaks()
 *   - jd_debug_memory.check_corruption()       // Checks for memory corruption in tracked allocations.
 *   - jd_debug_memory.find_memory_leak()
 *   - jd_debug_memory.memory_usage_over_time()
 *
 *   - jd_debug_signal.handle_signal(signal, handler)   // Custom handling for Unix signals.
 *   - jd_debug_signal.debug_stack_on_signal(signal)    // Dumps stack trace when a specified signal is received.
 *
 *   - jd_debug_analysis.performance_start(marker)
 *   - jd_debug_analysis.performance_end(marker)
 *   - jd_debug_analysis.compare_snapshots(snapshot1, snapshot2)
 *
 *   - jd_debug_breakpoint.set_conditional_breakpoint(condition)
 *   - jd_debug_breakpoint.remove_breakpoitn(breakpoint_id)
 *   - jd_debug_breakpoitn.list()
 *   - ..log/simulate_environment(config)
    */

/**
 *   - track_file_operations()
 *   - log_filesystem_activity()




/* jd_gfx.h */


// ==========================================================
// JD_DEBUG
// ==========================================================

#ifndef JD_DEBUG_H
#define JD_DEBUG_H



#ifdef JD_GFX_IMPLEMENTATION 
   #define JD static 
#else
   #define JD extern

#endif



#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
    __FILE__, __LINE__, __func__, ##args)
#else
#define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
#endif

// ==========================================================
// Type Definitions and Enums
// ==========================================================

// ==========================================================
// Implementations
// ==========================================================


#ifdef JD_DEBUG_IMPLEMENTATION



// ==========================================================
// Global Variables Declarations
/// ==========================================================

#endif // JD_DEBUG_IMPLEMENTATION
#endif // JD_DEBUG_H

// ==========================================================
