#include <stdio.h>
#include "memory.h"

// Just prints how many of our tests passed or not
static int tests_passed = 0;
static int tests_failed = 0;

// Basically tryna check if our test gave back the thing we wanted from it 
void assert_true(int condition, const char *message) {
    if (!condition) {
        printf("FAIL: %s\n", message);
        tests_failed++;
    } else {
        tests_passed++;
    }
}

void assert_eq_int(int expected, int actual, const char *message) {
    if (expected != actual) {
        printf("FAIL: %s (expected %d, got %d)\n", message, expected, actual);
        tests_failed++;
    } else {
        tests_passed++;
    }
}

// Our long list of Tests

// 1) After memory_init + var_allocate, variable exists, size is right, all zeros 
void test_memory_single_allocation(void) {
    memory_init();

    int ok = var_allocate('x', 10);
    assert_true(ok == 1, "var_allocate('x', 10) should succeed");

    Variable vx = var_get('x');
    assert_true(vx != NULL, "var_get('x') should not be NULL after allocation");

    int sz = var_size(vx);
    assert_eq_int(10, sz, "var_size(x) should be 10");

    for (int i = 0; i < sz; i++) {
        int val = var_read_at(vx, i);
        assert_eq_int(0, val, "allocated memory cells should be initialized to 0");
    }
}

// 2) Making sure two variables donjt fuck up each other up
void test_memory_multiple_allocations_do_not_overlap(void) {
    memory_init();

    int ok1 = var_allocate('a', 5);
    int ok2 = var_allocate('b', 5);
    assert_true(ok1 == 1 && ok2 == 1, "var_allocate('a',5) and var_allocate('b',5) should succeed");

    Variable va = var_get('a');
    Variable vb = var_get('b');
    assert_true(va != NULL && vb != NULL, "a and b should both exist");

    // Write different patterns into a and b 
    for (int i = 0; i < 5; i++) {
        var_write_at(va, i, 10 + i);  /* 10,11,12,13,14 */
        var_write_at(vb, i, 20 + i);  /* 20,21,22,23,24 */
    }

    // Check a still has its pattern 
    for (int i = 0; i < 5; i++) {
        int va_val = var_read_at(va, i);
        int vb_val = var_read_at(vb, i);
        assert_eq_int(10 + i, va_val, "a[i] should keep its own value");
        assert_eq_int(20 + i, vb_val, "b[i] should keep its own value");
    }
}

// 3) Freeing and reallocating reuses space
void test_memory_free_and_reallocate_same_size(void) {
    memory_init();

    int okx = var_allocate('x', 8);
    assert_true(okx == 1, "var_allocate('x', 8) should succeed");

    Variable vx = var_get('x');
    assert_true(vx != NULL, "x should exist after allocation");

    for (int i = 0; i < 8; i++) {
        var_write_at(vx, i, 100 + i);
    }

    int freed = var_free('x');
    assert_true(freed == 1, "var_free('x') should succeed");
    assert_true(var_get('x') == NULL, "x should not exist after free");

    int oky = var_allocate('y', 8);
    assert_true(oky == 1, "var_allocate('y', 8) should succeed after freeing x");

    Variable vy = var_get('y');
    assert_true(vy != NULL, "y should exist after allocation");

    // We mainly care that y works correctly: can write and read 
    for (int i = 0; i < 8; i++) {
        var_write_at(vy, i, 200 + i);
    }
    for (int i = 0; i < 8; i++) {
        int val = var_read_at(vy, i);
        assert_eq_int(200 + i, val, "y[i] should hold values we wrote");
    }
}

// 4) var_exists and var_get basic behavior 
void test_memory_var_exists_and_get(void) {
    memory_init();

    assert_true(var_exists('z') == 0, "var_exists('z') should be false before allocation");
    assert_true(var_get('z') == NULL, "var_get('z') should be NULL before allocation");

    int ok = var_allocate('z', 3);
    assert_true(ok == 1, "var_allocate('z', 3) should succeed");

    assert_true(var_exists('z') == 1, "var_exists('z') should be true after allocation");
    assert_true(var_get('z') != NULL, "var_get('z') should not be NULL after allocation");

    int freed = var_free('z');
    assert_true(freed == 1, "var_free('z') should succeed");

    assert_true(var_exists('z') == 0, "var_exists('z') should be false after free");
    assert_true(var_get('z') == NULL, "var_get('z') should be NULL after free");
}

// runs all the tests and stufff

int main(void) {
    test_memory_single_allocation();
    test_memory_multiple_allocations_do_not_overlap();
    test_memory_free_and_reallocate_same_size();
    test_memory_var_exists_and_get();

    printf("\nMemory tests passed: %d\n", tests_passed);
    printf("Memory tests failed: %d\n", tests_failed);

    return (tests_failed == 0) ? 0 : 1;
}