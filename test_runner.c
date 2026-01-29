#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

// Simple text-based menu system for Windows
void print_banner(void);
void print_menu(void);
void run_all_tests(void);
void run_memory_tests(void);
void run_parser_tests(void);
void run_executor_tests(void);
void run_integration_tests(void);
void show_test_descriptions(void);
void print_help_advice(void);
void wait_for_enter(void);

int main() {
    int choice;
    char input[10];
    
    print_banner();
    
    while (1) {
        print_menu();
        printf("\nEnter your choice (1-8): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        choice = atoi(input);
        
        switch (choice) {
            case 1:
                run_all_tests();
                break;
            case 2:
                run_memory_tests();
                break;
            case 3:
                run_parser_tests();
                break;
            case 4:
                run_executor_tests();
                break;
            case 5:
                run_integration_tests();
                break;
            case 6:
                show_test_descriptions();
                break;
            case 7:
                print_help_advice();
                break;
            case 8:
                printf("\nThank you for testing! Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice! Please enter a number between 1 and 8.\n");
                break;
        }
        
        wait_for_enter();
    }
    
    return 0;
}

void wait_for_enter(void) {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); // Clear input buffer
}

void print_banner(void) {
    printf("\n");
    printf("========================================\n");
    printf("       INTERPRETER TEST SUITE RUNNER    \n");
    printf("========================================\n");
    printf("  User-Friendly Interface for Testing   \n");
    printf("========================================\n");
}

void print_menu(void) {
    printf("\n");
    printf("MAIN MENU\n");
    printf("=========\n");
    printf("1. Run ALL tests (Comprehensive Check)\n");
    printf("2. Run MEMORY tests (Variable Management)\n");
    printf("3. Run PARSER tests (File Reading)\n");
    printf("4. Run EXECUTOR tests (Command Execution)\n");
    printf("5. Run INTEGRATION tests (Full Programs)\n");
    printf("6. Show test descriptions\n");
    printf("7. Get help & advice\n");
    printf("8. Exit\n");
}

void run_all_tests(void) {
    printf("\n");
    printf("RUNNING ALL TEST SUITES\n");
    printf("=======================\n");
    
    printf("\nStep 1: Running MEMORY tests...\n");
    int mem_result = system("tests_memory.exe");
    printf("Memory tests finished with exit code: %d (0 = success)\n", mem_result);
    
    printf("\nStep 2: Running PARSER tests...\n");
    int parser_result = system("tests_parser.exe");
    printf("Parser tests finished with exit code: %d (0 = success)\n", parser_result);
    
    printf("\nStep 3: Running EXECUTOR tests...\n");
    int exec_result = system("tests_executor.exe");
    printf("Executor tests finished with exit code: %d (0 = success)\n", exec_result);
    
    printf("\nStep 4: Running INTEGRATION tests...\n");
    int int_result = system("tests_integration.exe");
    printf("Integration tests finished with exit code: %d (0 = success)\n", int_result);
    
    // Show summary
    printf("\nTEST SUITE SUMMARY:\n");
    printf("==================\n");
    
    int total_failed = (mem_result != 0) + (parser_result != 0) + 
                       (exec_result != 0) + (int_result != 0);
    int total_passed = 4 - total_failed;
    
    if (total_failed == 0) {
        printf("SUCCESS: ALL TEST SUITES PASSED! (4/4)\n");
        printf("Your interpreter is working correctly!\n");
    } else {
        printf("WARNING: SOME TESTS FAILED: %d passed, %d failed\n", total_passed, total_failed);
        printf("Check the output above for detailed error messages.\n");
    }
}

void run_memory_tests(void) {
    printf("\n");
    printf("RUNNING MEMORY TESTS\n");
    printf("====================\n");
    printf("Testing: Variable allocation, reading, writing, and\n");
    printf("         freeing in the 100-cell memory system\n");
    
    printf("\nRunning memory tests...\n");
    int result = system("tests_memory.exe");
    
    printf("\nMEMORY TEST ANALYSIS:\n");
    if (result == 0) {
        printf("SUCCESS: Memory management is working correctly!\n");
        printf("  - Variables can be created and destroyed properly\n");
        printf("  - Memory cells retain their values\n");
        printf("  - No memory corruption between variables\n");
    } else {
        printf("FAILURE: Memory tests failed! Common issues:\n");
        printf("  * Variables might be overlapping in memory\n");
        printf("  * Memory might not be properly initialized to 0\n");
        printf("  * Freeing might not work correctly\n");
        printf("  * Memory bounds might not be checked\n");
    }
}

void run_parser_tests(void) {
    printf("\n");
    printf("RUNNING PARSER TESTS\n");
    printf("====================\n");
    printf("Testing: Reading program files, recognizing commands,\n");
    printf("         and converting them to internal structures\n");
    
    printf("\nSample test file (parser_test1.txt):\n");
    printf("  Mal x 6    # Create variable x with 6 cells\n");
    printf("  Ass x 4    # Assign 4 to x[0]\n");
    printf("  Add x y    # Add y to x\n");
    printf("  Fre x      # Free variable x\n");
    
    printf("\nRunning parser tests...\n");
    int result = system("tests_parser.exe");
    
    printf("\nPARSER TEST ANALYSIS:\n");
    if (result == 0) {
        printf("SUCCESS: Parser is working correctly!\n");
        printf("  - Commands are recognized properly\n");
        printf("  - Parameters are extracted correctly\n");
        printf("  - File reading works as expected\n");
    } else {
        printf("FAILURE: Parser tests failed! Common issues:\n");
        printf("  * Commands might not be recognized correctly\n");
        printf("  * Variable names or numbers might be parsed wrong\n");
        printf("  * File reading might have issues\n");
    }
}

void run_executor_tests(void) {
    printf("\n");
    printf("RUNNING EXECUTOR TESTS\n");
    printf("======================\n");
    printf("Testing: Execution of individual commands (Mal, Ass,\n");
    printf("         Inc, Dec, Add, Sub, Mul, And, Xor, Fre)\n");
    
    printf("\nTest files used:\n");
    printf("* executor_basic.txt: Mal x 4, Ass x 5\n");
    printf("* executor_incdec.txt: Mal x 3, Ass x 7, Inc x 1, Dec x 1\n");
    printf("* executor_arith.txt: Arithmetic operations with x and y\n");
    printf("* executor_andxor.txt: Bitwise operations on arrays\n");
    
    printf("\nRunning executor tests...\n");
    int result = system("tests_executor.exe");
    
    printf("\nEXECUTOR TEST ANALYSIS:\n");
    if (result == 0) {
        printf("SUCCESS: Executor is working correctly!\n");
        printf("  - All commands execute properly\n");
        printf("  - Arithmetic operations give correct results\n");
        printf("  - Bitwise operations work on arrays\n");
        printf("  - Memory is managed correctly during execution\n");
    } else {
        printf("FAILURE: Executor tests failed! Common issues:\n");
        printf("  * Arithmetic might give wrong results\n");
        printf("  * Inc/Dec might not work on the right cell\n");
        printf("  * And/Xor might not handle arrays correctly\n");
        printf("  * Memory might not be updated properly\n");
    }
}

void run_integration_tests(void) {
    printf("\n");
    printf("RUNNING INTEGRATION TESTS\n");
    printf("=========================\n");
    printf("Testing: Complete program execution from parsing to\n");
    printf("         final result, testing multiple features\n");
    
    printf("\nWhat these tests check:\n");
    printf("* Complete workflow: parse -> execute -> verify\n");
    printf("* Complex programs with multiple variables and operations\n");
    printf("* Edge cases and error conditions\n");
    printf("* Memory cleanup after program execution\n");
    
    printf("\nRunning integration tests...\n");
    int result = system("tests_integration.exe");
    
    printf("\nINTEGRATION TEST ANALYSIS:\n");
    if (result == 0) {
        printf("SUCCESS: Integration tests passed!\n");
        printf("  - Complete programs work end-to-end\n");
        printf("  - Parser and executor work together correctly\n");
        printf("  - Memory is properly managed throughout\n");
        printf("  - Complex scenarios are handled correctly\n");
    } else {
        printf("FAILURE: Integration tests failed! Issues:\n");
        printf("  * Parser and executor might not work together\n");
        printf("  * Memory might leak during program execution\n");
        printf("  * Complex programs might have logic errors\n");
        printf("  * Edge cases might not be handled properly\n");
    }
}

void show_test_descriptions(void) {
    printf("\n");
    printf("TEST SUITE DESCRIPTIONS\n");
    printf("=======================\n");
    
    printf("\n1. MEMORY TESTS (tests_memory.c)\n");
    printf("   Purpose: Test the 100-cell memory management system\n");
    printf("   Tests:\n");
    printf("   * Basic variable allocation with correct size\n");
    printf("   * Reading and writing to individual cells\n");
    printf("   * Multiple variables don't interfere\n");
    printf("   * Freeing and reusing memory\n");
    printf("   * Variable existence checking\n");
    
    printf("\n2. PARSER TESTS (tests_parser.c)\n");
    printf("   Purpose: Test reading and interpreting program files\n");
    printf("   Tests:\n");
    printf("   * Parsing all command types correctly\n");
    printf("   * Extracting variable names and numbers\n");
    printf("   * Handling different file formats\n");
    printf("   * Using parser_test1.txt as test input\n");
    
    printf("\n3. EXECUTOR TESTS (tests_executor.c)\n");
    printf("   Purpose: Test execution of individual commands\n");
    printf("   Tests:\n");
    printf("   * Mal: Variable creation\n");
    printf("   * Ass: Value assignment\n");
    printf("   * Inc/Dec: Increment/decrement\n");
    printf("   * Add/Sub/Mul: Arithmetic\n");
    printf("   * And/Xor: Bitwise operations\n");
    printf("   * Uses executor_*.txt files as test programs\n");
    
    printf("\n4. INTEGRATION TESTS (tests_integration.c)\n");
    printf("   Purpose: Test complete program execution\n");
    printf("   Tests:\n");
    printf("   * Complete workflow from parse to execute\n");
    printf("   * Complex multi-variable programs\n");
    printf("   * Edge cases and error handling\n");
    printf("   * Memory cleanup verification\n");
}

void print_help_advice(void) {
    printf("\n");
    printf("HELP & TESTING ADVICE\n");
    printf("=====================\n");
    
    printf("\nCOMPILATION INSTRUCTIONS (for Windows):\n");
    printf("Before running tests, compile them with these commands:\n");
    printf("  gcc -o tests_memory.exe tests_memory.c memory.c errors.c\n");
    printf("  gcc -o tests_parser.exe tests_parser.c parser.c\n");
    printf("  gcc -o tests_executor.exe tests_executor.c memory.c parser.c executor.c errors.c\n");
    printf("  gcc -o tests_integration.exe tests_integration.c memory.c parser.c executor.c errors.c\n");
    printf("  gcc -o test_runner.exe test_runner.c\n");
    
    printf("\nIMPORTANT FOR WINDOWS USERS:\n");
    printf("* All test files must have .exe extension\n");
    printf("* Use 'program.exe' NOT './program'\n");
    printf("* Make sure all .txt test files are in the same folder\n");
    printf("* Run test_runner.exe from Command Prompt or PowerShell\n");
    
    printf("\nWHEN TESTS FAIL:\n");
    printf("1. Check the test output above for error messages\n");
    
    printf("\n2. Common issues and fixes:\n");
    printf("   * Memory tests fail: Check variable overlap in Main_Array\n");
    printf("   * Parser tests fail: Verify command syntax in test files\n");
    printf("   * Executor tests fail: Check arithmetic/logic in executor.c\n");
    printf("   * Integration tests fail: Look for workflow issues\n");
    
    printf("\n3. Testing workflow recommendations:\n");
    printf("   * Always run tests before and after making changes\n");
    printf("   * Start with memory tests (foundation)\n");
    printf("   * Then parser, then executor, then integration\n");
    printf("   * Add new tests when adding new features\n");
    
    printf("\n4. Exit codes:\n");
    printf("   * 0 = All tests passed\n");
    printf("   * 1 = Some tests failed\n");
    printf("   * Other codes = Compilation or runtime errors\n");
}