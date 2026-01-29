/*

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "parser.h"
#include "executor.h"

// Test tracking variables
static int tests_passed = 0;
static int tests_failed = 0;
static const char *current_test_name = "no test yet";

// Assertion helpers
void assert_true(int condition, const char *message) {
    if (!condition) {
        printf("FAIL in [%s]: %s\n", current_test_name, message);
        tests_failed++;
    } else {
        tests_passed++;
    }
}

void assert_eq_int(int expected, int actual, const char *message) {
    if (expected != actual) {
        printf("FAIL in [%s]: %s (expected %d, got %d)\n",
               current_test_name, message, expected, actual);
        tests_failed++;
    } else {
        tests_passed++;
    }
}

// TEST 1: Basic integration test with existing file
void test_integration_basic_lifecycle(void) {
    current_test_name = "Basic variable lifecycle program";
    printf("\nRunning: %s\n", current_test_name);
    
    // Run the program from integration_basic.txt
    memory_init();
    int count = parse("integration_basic.txt");
    
    assert_true(count > 0, "Should parse commands from integration_basic.txt");
    if (count <= 0) {
        printf("ERROR: Could not parse integration_basic.txt\n");
        return;
    }
    
    // Execute all commands
    for (int i = 0; i < count; i++) {
        execute(i);
    }
    
    // After the program, x and y should be freed
    assert_true(var_get('x') == NULL, "x should be freed at the end");
    assert_true(var_get('y') == NULL, "y should be freed at the end");
    
    free_list();
}

// TEST 2: Complex arithmetic program
void test_integration_complex_arithmetic(void) {
    current_test_name = "Complex arithmetic program";
    printf("\nRunning: %s\n", current_test_name);
    
    // Create a test program
    FILE *f = fopen("test_complex_arith.txt", "w");
    if (!f) {
        printf("ERROR: Could not create test file\n");
        return;
    }
    
    fprintf(f, "Mal a 3\n");
    fprintf(f, "Mal b 3\n");
    fprintf(f, "Ass a 10\n");
    fprintf(f, "Ass b 5\n");
    fprintf(f, "Add a b\n");    // a[0] = 15
    fprintf(f, "Sub a b\n");    // a[0] = 10
    fprintf(f, "Mul a b\n");    // a[0] = 50
    fprintf(f, "Inc a 1\n");    // a[1] = 1
    fprintf(f, "Dec b 0\n");    // b[0] = 4
    fclose(f);
    
    // Run the program
    memory_init();
    int count = parse("test_complex_arith.txt");
    
    assert_true(count > 0, "Should parse complex arithmetic program");
    if (count <= 0) {
        remove("test_complex_arith.txt");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        execute(i);
    }
    
    // Verify results
    Variable va = var_get('a');
    Variable vb = var_get('b');
    
    assert_true(va != NULL, "a should exist");
    assert_true(vb != NULL, "b should exist");
    
    // a[0] should be 50: 10+5=15, 15-5=10, 10*5=50
    assert_eq_int(50, var_read_at(va, 0), "a[0] should be 50");
    
    // a[1] should be 1 (from Inc)
    assert_eq_int(1, var_read_at(va, 1), "a[1] should be 1");
    
    // b[0] should be 4: 5-1=4
    assert_eq_int(4, var_read_at(vb, 0), "b[0] should be 4");
    
    remove("test_complex_arith.txt");
    free_list();
}

// TEST 3: Memory allocation and freeing cycle
void test_integration_memory_cycle(void) {
    current_test_name = "Memory allocation/freeing cycle";
    printf("\nRunning: %s\n", current_test_name);
    
    memory_init();
    
    // Allocate variable a
    int alloc_result = var_allocate('a', 10);
    assert_true(alloc_result == 1, "Should allocate variable a");
    
    Variable va = var_get('a');
    assert_true(va != NULL, "Variable a should exist");
    
    // Write some values
    for (int i = 0; i < 10; i++) {
        var_write_at(va, i, i * 10);
    }
    
    // Free variable a
    int free_result = var_free('a');
    assert_true(free_result == 1, "Should free variable a");
    assert_true(var_get('a') == NULL, "Variable a should not exist after free");
    
    // Allocate variable b (should reuse a's memory)
    alloc_result = var_allocate('b', 10);
    assert_true(alloc_result == 1, "Should allocate variable b in freed space");
    
    Variable vb = var_get('b');
    assert_true(vb != NULL, "Variable b should exist");
    
    // Write to b
    var_write_at(vb, 0, 999);
    assert_eq_int(999, var_read_at(vb, 0), "Should be able to write/read from reused memory");
    
    free_list();
}

// TEST 4: Error handling integration
void test_integration_error_handling(void) {
    current_test_name = "Error handling in integration";
    printf("\nRunning: %s\n", current_test_name);
    
    memory_init();
    
    // Test 1: Accessing non-existent variable
    Variable v = var_get('z');
    assert_true(v == NULL, "Non-existent variable should return NULL");
    
    // Test 2: Allocate, write, read - valid operations
    int alloc_result = var_allocate('t', 5);
    assert_true(alloc_result == 1, "Should allocate variable t");
    
    Variable vt = var_get('t');
    var_write_at(vt, 0, 42);
    assert_eq_int(42, var_read_at(vt, 0), "Should read back written value");
    
    // Test 3: Variable size
    assert_eq_int(5, var_size(vt), "Variable should have correct size");
    
    // Test 4: Variable existence
    assert_true(var_exists('t') == 1, "var_exists should return 1 for existing variable");
    assert_true(var_exists('z') == 0, "var_exists should return 0 for non-existent variable");
    
    free_list();
}

// TEST 5: Test with existing executor files
void test_integration_with_existing_files(void) {
    current_test_name = "Integration with existing test files";
    printf("\nRunning: %s\n", current_test_name);
    
    // Test with executor_basic.txt
    printf("  Testing executor_basic.txt...\n");
    memory_init();
    int count = parse("executor_basic.txt");
    assert_true(count == 2, "Should parse 2 commands from executor_basic.txt");
    
    for (int i = 0; i < count; i++) {
        execute(i);
    }
    
    Variable vx = var_get('x');
    assert_true(vx != NULL, "x should exist");
    assert_eq_int(5, var_read_at(vx, 0), "x[0] should be 5");
    
    free_list();
}

// TEST 6: Complete program with multiple operations
void test_integration_complete_program(void) {
    current_test_name = "Complete program with multiple operations";
    printf("\nRunning: %s\n", current_test_name);
    
    // Create test program
    FILE *f = fopen("test_complete.txt", "w");
    if (!f) return;
    
    fprintf(f, "Mal x 2\n");
    fprintf(f, "Mal y 2\n");
    fprintf(f, "Ass x 8\n");
    fprintf(f, "Ass y 2\n");
    fprintf(f, "Add x y\n");   // x = 10
    fprintf(f, "Mul x y\n");   // x = 20
    fprintf(f, "Inc x 1\n");   // x[1] = 1
    fprintf(f, "Pra x\n");     // Print x array
    fprintf(f, "Fre x\n");
    fprintf(f, "Fre y\n");
    fclose(f);
    
    // Run program
    memory_init();
    int count = parse("test_complete.txt");
    
    if (count > 0) {
        printf("  Running %d commands...\n", count);
        for (int i = 0; i < count; i++) {
            execute(i);
        }
        
        // After Fre commands, variables should be freed
        assert_true(var_get('x') == NULL, "x should be freed");
        assert_true(var_get('y') == NULL, "y should be freed");
    }
    
    remove("test_complete.txt");
    free_list();
}

int main(void) {
    printf("\n========================================\n");
    printf("        INTEGRATION TEST SUITE\n");
    printf("========================================\n");
    
    // Run all integration tests
    test_integration_basic_lifecycle();
    test_integration_complex_arithmetic();
    test_integration_memory_cycle();
    test_integration_error_handling();
    test_integration_with_existing_files();
    test_integration_complete_program();
    
    printf("\n========================================\n");
    printf("INTEGRATION TEST RESULTS:\n");
    printf("  Tests passed: %d\n", tests_passed);
    printf("  Tests failed: %d\n", tests_failed);
    printf("  Total checks: %d\n", tests_passed + tests_failed);
    
    if (tests_failed == 0) {
        printf("\nSUCCESS: ALL INTEGRATION TESTS PASSED!\n");
        printf("  The interpreter components work well together.\n");
        printf("  Complete programs execute correctly.\n");
    } else {
        printf("\nWARNING: SOME INTEGRATION TESTS FAILED\n");
        printf("  Check individual test messages above.\n");
    }
    printf("========================================\n");
    
    return (tests_failed == 0) ? 0 : 1;
}