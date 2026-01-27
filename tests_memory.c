#include <stdio.h>
#include "memory.h"


void free_list(void);

// how many little checks passed / failed
static int tests_passed = 0;
static int tests_failed = 0;

// so we can say which big test is currently running
static const char *current_test_name = "unknown test";

// Basically tryna check if our test gave back the thing we wanted from it 
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
        printf("FAIL in [%s]: %s (wanted %d, got %d)\n",
               current_test_name, message, expected, actual);
        tests_failed++;
    } else {
        tests_passed++;
    }
}

// Our long list of Tests

// 1) After memory_init + var_allocate, variable exists, size is right, all zeros 
void test_memory_single_allocation(void) {
    // This test is checking that making one variable "x" with size 10:
    // - really starts from a clean slate using memory_init,
    // - really gives "x" exactly 10 cells,
    // - and really sets every new cell to 0 instead of some random junk.
    current_test_name = "single alloc makes x with right size + zeros";
    printf("\nRunning: %s\n", current_test_name);

    memory_init();

    int ok = var_allocate('x', 10);
    assert_true(ok == 1, "alloc for x (size 10) should work, not explode");

    Variable vx = var_get('x');
    assert_true(vx != NULL, "x should actually exist after we alloc it");

    int sz = var_size(vx);
    assert_eq_int(10, sz, "x should have size 10, not something random");

    for (int i = 0; i < sz; i++) {
        int val = var_read_at(vx, i);
        assert_eq_int(0, val, "new cells should all be zero, not junk");
    }
}

// 2) Making sure two variables don’t mess each other up
void test_memory_multiple_allocations_do_not_overlap(void) {
    // This test is checking that when we have two variables "a" and "b":
    // - both can be created at the same time,
    // - each one can hold its own simple pattern of numbers,
    // - writing into "a" does not secretly change "b", and the other way round.
    current_test_name = "two vars keep their own values";
    printf("\nRunning: %s\n", current_test_name);

    memory_init();

    int ok1 = var_allocate('a', 5);
    int ok2 = var_allocate('b', 5);
    assert_true(ok1 == 1 && ok2 == 1,
                "alloc for a and b should both work");

    Variable va = var_get('a');
    Variable vb = var_get('b');
    assert_true(va != NULL && vb != NULL,
                "a and b should both actually exist");

    // Write different patterns into a and b 
    for (int i = 0; i < 5; i++) {
        var_write_at(va, i, 10 + i);  // 10,11,12,13,14 
        var_write_at(vb, i, 20 + i);  // 20,21,22,23,24 
    }

    // Check a and b still have their own patterns 
    for (int i = 0; i < 5; i++) {
        int va_val = var_read_at(va, i);
        int vb_val = var_read_at(vb, i);
        assert_eq_int(10 + i, va_val,
                      "a[i] should keep its own pattern, not b’s");
        assert_eq_int(20 + i, vb_val,
                      "b[i] should keep its own pattern, not a’s");
    }
}

// 3) Freeing and reallocating reuses space
void test_memory_free_and_reallocate_same_size(void) {
    // This test is checking that when we:
    // - create "x" of size 8 and then free it,
    // - we can later create "y" of the same size,
    // - and "y" behaves like a normal fresh variable where we can write and read values.
    current_test_name = "free x then reuse same size for y";
    printf("\nRunning: %s\n", current_test_name);

    memory_init();

    int okx = var_allocate('x', 8);
    assert_true(okx == 1, "alloc for x (size 8) should work");

    Variable vx = var_get('x');
    assert_true(vx != NULL, "x should exist after we alloc it");

    for (int i = 0; i < 8; i++) {
        var_write_at(vx, i, 100 + i);
    }

    int freed = var_free('x');
    assert_true(freed == 1, "freeing x should say it worked");
    assert_true(var_get('x') == NULL, "x should vanish after we free it");

    int oky = var_allocate('y', 8);
    assert_true(oky == 1, "alloc for y (same size) should still work");

    Variable vy = var_get('y');
    assert_true(vy != NULL, "y should exist after we alloc it");

    // We mainly care that y works correctly: can write and read 
    for (int i = 0; i < 8; i++) {
        var_write_at(vy, i, 200 + i);
    }
    for (int i = 0; i < 8; i++) {
        int val = var_read_at(vy, i);
        assert_eq_int(200 + i, val,
                      "y[i] should keep the values we shoved in there");
    }
}

// 4) var_exists and var_get basic behavior 
void test_memory_var_exists_and_get(void) {
    // This test is checking that the helper functions around names work:
    // - before we create "z", var_exists('z') is false and var_get('z') gives NULL,
    // - after we allocate "z", both say "z is here" and give back a real pointer,
    // - after we free "z", both go back to "z is not here" again.
    current_test_name = "var_exists + var_get behave before/after alloc + free";
    printf("\nRunning: %s\n", current_test_name);

    memory_init();

    assert_true(var_exists('z') == 0,
                "z should NOT exist before we alloc it");
    assert_true(var_get('z') == NULL,
                "var_get('z') should give NULL before alloc");

    int ok = var_allocate('z', 3);
    assert_true(ok == 1, "alloc for z (size 3) should work");

    assert_true(var_exists('z') == 1,
                "z should exist after alloc");
    assert_true(var_get('z') != NULL,
                "var_get('z') should not be NULL after alloc");

    int freed = var_free('z');
    assert_true(freed == 1, "freeing z should say it worked");

    assert_true(var_exists('z') == 0,
                "z should NOT exist after free");
    assert_true(var_get('z') == NULL,
                "var_get('z') should go back to NULL after free");
}

// 5) make sure free_list doesn't totally break things
void test_memory_free_list_allows_reinit(void) {
    // This test is checking the more hidden clean up code:
    // - we create and free "a" and "b" so there are some free blocks,
    // - we call free_list() which should throw away that internal free list,
    // - we then call memory_init() and allocate "c",
    // - if that works, it means free_list did not poison things and we can safely start over.
    current_test_name = "free_list nukes freelist but we can start over";
    printf("\nRunning: %s\n", current_test_name);

    // start fresh
    memory_init();

    // grab a couple of chunks so the free list actually has stuff
    int oka = var_allocate('a', 10);
    int okb = var_allocate('b', 20);
    assert_true(oka == 1 && okb == 1,
                "alloc a and b should both work");

    // put them back so we definitely have free blocks hanging around
    int freeda = var_free('a');
    int freedb = var_free('b');
    assert_true(freeda == 1 && freedb == 1,
                "free a and b should both say they worked");

    // this is the thing we actually care about
    free_list();

    // pretend we just started the program again
    memory_init();

    // if this works, free_list is good to go
    int okc = var_allocate('c', 30);
    assert_true(okc == 1,
                "alloc after free_list + memory_init should still work fine");
}

// runs all the tests and stuff
int main(void) {
    test_memory_single_allocation();
    test_memory_multiple_allocations_do_not_overlap();
    test_memory_free_and_reallocate_same_size();
    test_memory_var_exists_and_get();
    test_memory_free_list_allows_reinit();

    printf("\nMemory tiny checks passed: %d\n", tests_passed);
    printf("Memory tiny checks failed: %d\n", tests_failed);

    return (tests_failed == 0) ? 0 : 1;
}