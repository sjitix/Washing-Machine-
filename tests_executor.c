#include <stdio.h>
#include "memory.h"
#include "parser.h"
#include "executor.h"

// These keep track of how many little checks we did
// and how many of them were ok / not ok.
static int tests_passed = 0;
static int tests_failed = 0;

// This is just a label so when something breaks we can say
// "this is the test that was running when it broke".
static const char *current_test_name = "no test yet";

// This checks that a thing is "true" (non‑zero).
// If it is not true, we print a message saying what went wrong.
void assert_true(int condition, const char *message) {
    if (!condition) {
        printf("FAIL in [%s]: %s\n", current_test_name, message);
        tests_failed++;
    } else {
        tests_passed++;
    }
}

// This checks that two whole numbers (ints) are the same.
// If they are different, we say what we expected and what we actually got.
void assert_eq_int(int expected, int actual, const char *message) {
    if (expected != actual) {
        printf("FAIL in [%s]: %s (wanted %d, got %d)\n",
               current_test_name, message, expected, actual);
        tests_failed++;
    } else {
        tests_passed++;
    }
}

// TEST 1: basic "make a variable and put a number in it".
//
// What this test is doing:
// - We use a tiny program file called "executor_basic.txt".
// - That file should contain these 2 lines:
//
//     Mal x 4   (make a variable named x with 4 slots)
//     Ass x 5   (put the number 5 into the first slot of x)
//
// - We ask the parser to read that file into commands.
// - Then we tell the executor to run the commands one by one.
// - At the end we check:
//      * x exists
//      * x has size 4
//      * x[0] (first cell) is 5
//      * the other cells are still 0
void test_executor_mal_and_ass_basic(void) {
    current_test_name = "make x and store 5 in x[0]";
    printf("\nRunning: %s\n", current_test_name);

    // Make sure memory starts in a clean state.
    memory_init();

    // Read the tiny program from the file.
    int count = parse("executor_basic.txt");
    assert_eq_int(2, count,
                  "executor_basic.txt should give us exactly 2 commands");
    if (count != 2) {
        // if parsing failed, don't try to execute junk
        return;
    }

    // Run the two commands from the file.
    execute(0); // runs: Mal x 4
    execute(1); // runs: Ass x 5

    // Now we look at what happened to x.
    Variable vx = var_get('x');
    assert_true(vx != NULL, "x should exist after we create it with Mal");

    // x should have 4 slots.
    assert_eq_int(4, var_size(vx), "x should have size 4");

    // First slot should be 5 because of Ass x 5.
    int v0 = var_read_at(vx, 0);
    assert_eq_int(5, v0, "x[0] should be 5 after Ass x 5");

    // The other slots should still be zero.
    for (int i = 1; i < var_size(vx); i++) {
        int vi = var_read_at(vx, i);
        assert_eq_int(0, vi, "x[i] for i>0 should still be 0");
    }
}

// TEST 2: "increase a cell and then decrease it again".
//
// What this test is doing:
// - We use a tiny program file called "executor_incdec.txt".
// - That file should contain these 4 lines:
// - After Inc then Dec on x[1], the value in x[1] should end up back at 0.
// - We also check that x[0] and x[2] did not change by accident.
void test_executor_inc_and_dec_same_cell(void) {
    current_test_name = "Inc then Dec the same cell in x";
    printf("\nRunning: %s\n", current_test_name);

    memory_init();

    int count = parse("executor_incdec.txt");
    assert_eq_int(4, count,
                  "executor_incdec.txt should have 4 commands");
    if (count != 4) {
        return;
    }

    // Run the first three commands to set things up.
    execute(0); // Mal x 3
    execute(1); // Ass x 7      -> x[0] = 7
    execute(2); // Inc x 1      -> x[1] goes from 0 to 1

    Variable vx = var_get('x');
    assert_true(vx != NULL,
                "x should exist after Mal and Ass and Inc");

    // Check what the array looks like after Inc but before Dec.
    assert_eq_int(7, var_read_at(vx, 0),
                  "before Dec: x[0] should be 7");
    assert_eq_int(1, var_read_at(vx, 1),
                  "before Dec: x[1] should be 1 after Inc");
    assert_eq_int(0, var_read_at(vx, 2),
                  "before Dec: x[2] should still be 0");

    // Now run the Dec command which should undo the Inc on x[1].
    execute(3); // Dec x 1

    assert_eq_int(7, var_read_at(vx, 0),
                  "after Dec: x[0] should still be 7");
    assert_eq_int(0, var_read_at(vx, 1),
                  "after Dec: x[1] should be back to 0");
    assert_eq_int(0, var_read_at(vx, 2),
                  "after Dec: x[2] should still be 0");
}

// TEST 3: "do Add, Sub, and Mul on two single‑cell variables".
//
// What this test is doing:
// - We use a file called "executor_arith.txt" with these 7 lines:
//
//     Mal x 1   (make x with 1 slot)
//     Mal y 1   (make y with 1 slot)
//     Ass x 2   (x[0] = 2)
//     Ass y 3   (y[0] = 3)
//     Add x y   (x[0] = x[0] + y[0])  -> 2 + 3 = 5
//     Sub x y   (x[0] = x[0] - y[0])  -> 5 - 3 = 2
//     Mul x y   (x[0] = x[0] * y[0])  -> 2 * 3 = 6
//
// - We run the commands in order and check that x[0] changes
//   to the numbers we expect after each operation.
void test_executor_add_sub_mul_chain(void) {
    current_test_name = "Add, Sub, Mul on x[0] and y[0]";
    printf("\nRunning: %s\n", current_test_name);

    memory_init();

    int count = parse("executor_arith.txt");
    assert_eq_int(7, count,
                  "executor_arith.txt should have 7 commands");
    if (count != 7) {
        return;
    }

    // Set up x and y and load their starting values.
    execute(0); // Mal x 1
    execute(1); // Mal y 1
    execute(2); // Ass x 2
    execute(3); // Ass y 3

    Variable vx = var_get('x');
    Variable vy = var_get('y');
    assert_true(vx != NULL && vy != NULL,
                "x and y should both exist");

    assert_eq_int(2, var_read_at(vx, 0),
                  "x[0] should start as 2");
    assert_eq_int(3, var_read_at(vy, 0),
                  "y[0] should start as 3");

    // After Add: x[0] = 2 + 3 = 5
    execute(4); // Add x y
    assert_eq_int(5, var_read_at(vx, 0),
                  "after Add: x[0] should be 5");

    // After Sub: x[0] = 5 - 3 = 2
    execute(5); // Sub x y
    assert_eq_int(2, var_read_at(vx, 0),
                  "after Sub: x[0] should go back to 2");

    // After Mul: x[0] = 2 * 3 = 6
    execute(6); // Mul x y
    assert_eq_int(6, var_read_at(vx, 0),
                  "after Mul: x[0] should be 6");
}

// TEST 4: "do And and Xor on each cell of two arrays".
//
// Here we want to check the more "vector‑like" commands AND and XOR.
//
// We use a file called "executor_andxor.txt" with these 4 lines:
//
//     Mal x 3   (x has 3 slots: x[0], x[1], x[2])
//     Mal y 3   (y has 3 slots: y[0], y[1], y[2])
//     And x y   (for each i, x[i] = (x[i] * y[i]) % 2)
//     Xor x y   (for each i, x[i] = (x[i] + y[i]) % 2)
//
// Important: before we call And and Xor, we manually write values
// into x and y so we know exactly what answer we should get.
void test_executor_and_and_xor_on_arrays(void) {
    current_test_name = "And and Xor for each cell of x with y";
    printf("\nRunning: %s\n", current_test_name);

    memory_init();

    int count = parse("executor_andxor.txt");
    assert_eq_int(4, count,
                  "executor_andxor.txt should have 4 commands");
    if (count != 4) {
        return;
    }

    // First two commands only make x and y.
    execute(0); // Mal x 3
    execute(1); // Mal y 3

    Variable vx = var_get('x');
    Variable vy = var_get('y');
    assert_true(vx != NULL && vy != NULL,
                "x and y should exist after Mal");

    // ---- Test AND ----
    // Set x and y to small, easy‑to‑reason‑about patterns.
    // x: [1, 0, 1]
    // y: [0, 1, 1]
    var_write_at(vx, 0, 1);
    var_write_at(vx, 1, 0);
    var_write_at(vx, 2, 1);

    var_write_at(vy, 0, 0);
    var_write_at(vy, 1, 1);
    var_write_at(vy, 2, 1);

    // Now run "And x y" from the file.
    // For each i: x[i] = (x[i] * y[i]) % 2
    // i=0: (1*0)%2 = 0
    // i=1: (0*1)%2 = 0
    // i=2: (1*1)%2 = 1
    execute(2); // And x y

    assert_eq_int(0, var_read_at(vx, 0),
                  "after And: x[0] should be 0");
    assert_eq_int(0, var_read_at(vx, 1),
                  "after And: x[1] should be 0");
    assert_eq_int(1, var_read_at(vx, 2),
                  "after And: x[2] should be 1");

    // ---- Test XOR ----
    // Put new values into x and y again so we know what XOR should do.
    // x: [1, 0, 1]
    // y: [1, 1, 0]
    var_write_at(vx, 0, 1);
    var_write_at(vx, 1, 0);
    var_write_at(vx, 2, 1);

    var_write_at(vy, 0, 1);
    var_write_at(vy, 1, 1);
    var_write_at(vy, 2, 0);

    // Now run "Xor x y" from the file.
    // For each i: x[i] = (x[i] + y[i]) % 2
    // i=0: (1+1)%2 = 0
    // i=1: (0+1)%2 = 1
    // i=2: (1+0)%2 = 1
    execute(3); // Xor x y

    assert_eq_int(0, var_read_at(vx, 0),
                  "after Xor: x[0] should be 0");
    assert_eq_int(1, var_read_at(vx, 1),
                  "after Xor: x[1] should be 1");
    assert_eq_int(1, var_read_at(vx, 2),
                  "after Xor: x[2] should be 1");
}

// This is the main function that actually runs all our tests.
// The program will exit with 0 if everything passed,
// or 1 if something went wrong.
int main(void) {
    test_executor_mal_and_ass_basic();
    test_executor_inc_and_dec_same_cell();
    test_executor_add_sub_mul_chain();
    test_executor_and_and_xor_on_arrays();

    printf("\nExecutor tiny checks passed: %d\n", tests_passed);
    printf("Executor tiny checks failed: %d\n", tests_failed);

    return (tests_failed == 0) ? 0 : 1;
}