#include <stdio.h>
#include "parser.h"
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

void assert_eq_char(char expected, char actual, const char *message) {
    if (expected != actual) {
        printf("FAIL: %s (expected %c, got %c)\n", message, expected, actual);
        tests_failed++;
    } else {
        tests_passed++;
    }
}

 // Our long list of Tests

void test_parse_simple_program(void) {
    //parse our little test file thingy
    int count = parse("parser_test1.txt");
    assert_eq_int(4, count, "parse should return 4 commands");

    //Mal x 6
    Command c0 = get_command(0);
    assert_eq_int(MAL, cmd_get_op(c0), "command 0 op should be MAL");
    assert_eq_char('x', cmd_get_var1(c0), "command 0 var1 should be x");
    assert_eq_int(6, cmd_get_number(c0), "command 0 number should be 6");

    //Ass x 4
    Command c1 = get_command(1);
    assert_eq_int(ASS, cmd_get_op(c1), "command 1 op should be ASS");
    assert_eq_char('x', cmd_get_var1(c1), "command 1 var1 should be x");
    assert_eq_int(4, cmd_get_number(c1), "command 1 number should be 4");

    //Add x y
    Command c2 = get_command(2);
    assert_eq_int(ADD, cmd_get_op(c2), "command 2 op should be ADD");
    assert_eq_char('x', cmd_get_var1(c2), "command 2 var1 should be x");
    assert_eq_char('y', cmd_get_var2(c2), "command 2 var2 should be y");

    //Fre x
    Command c3 = get_command(3);
    assert_eq_int(FRE, cmd_get_op(c3), "command 3 op should be FRE");
    assert_eq_char('x', cmd_get_var1(c3), "command 3 var1 should be x");
}

// Runs the Tests

int main(void) {
    test_parse_simple_program();

    printf("\nParser tests passed: %d\n", tests_passed);
    printf("Parser tests failed: %d\n", tests_failed);

    return (tests_failed == 0) ? 0 : 1;
}