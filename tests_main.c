// This file is called "tests_main.c".
// When you turn it into an .exe and double click it,
// it acts like a small menu for running all your test programs.
// It does NOT know how to test things by itself.
// Instead, it calls other .exe files (tests_memory.exe, tests_parser.exe, etc.).
// So you can:
//   1) Build those test .exe files once.
//   2) Run this one "tests_main.exe".
//   3) Pick what you want to run from a simple text menu.

#include <stdio.h>    // for printf, fgets, puts
#include <stdlib.h>   // for system, exit, remove
#include <string.h>   // for strlen, strcmp

// On Windows we include <direct.h> so we can change the current folder.
// This lets you type a folder path and have the program "cd" into it.
#ifdef _WIN32
#include <direct.h>   // for _chdir
#define CHANGE_DIR _chdir
#else
#include <unistd.h>   // for chdir on non Windows (just in case)
#define CHANGE_DIR chdir
#endif

// This helper removes the newline character at the end of a string,
// if there is one. Example:
//   "hello\n" becomes "hello".
// We do this because fgets leaves the '\n' from pressing Enter.
static void trim_newline(char *s) {
    if (s == NULL) {
        return;
    }
    size_t len = strlen(s);
    if (len == 0) {
        return;
    }
    if (s[len - 1] == '\n' || s[len - 1] == '\r') {
        s[len - 1] = '\0';
    }
}

// This helper waits for the user to press Enter.
// It is just so the window does not close too fast after running a test.
static void wait_for_enter(void) {
    char buffer[8];
    printf("\nPress Enter to go back to the menu...");
    // We read a small line from the user and then ignore it.
    fgets(buffer, sizeof(buffer), stdin);
}

// This tiny structure remembers what happened for each group of tests.
// It is used at the end to print a short "report" for you.
typedef struct {
    const char *name;          // nice name like "Memory tests"
    int ran;                   // 1 if we tried to run it, 0 if never touched
    int build_ok;              // 1 if gcc built the temp exe, 0 if build failed
    int run_ok;                // 1 if the temp exe started, 0 if it could not start
    int exit_code;             // exit code from the temp exe (0 usually means all good)
    char fail_messages[20][256]; // up to 20 lines that start with "FAIL", each up to 255 characters
    int fail_count;            // how many of the above are actually filled
} SuiteResult;

// We have four groups: memory, parser, executor, integration.
// We keep one record for each of them.
static SuiteResult suite_results[4];

// Just a small helper so we can look up the right record by a number.
// We use:
//   0 -> memory
//   1 -> parser
//   2 -> executor
//   3 -> integration
static SuiteResult *get_suite_result(int index) {
    if (index < 0 || index > 3) {
        return NULL;
    }
    return &suite_results[index];
}

// This helper runs tests directly from the C files.
// Big idea:
//   - We pick the right gcc command based on the exe_name.
//   - We ALWAYS compile the C files into a small temporary program.
//   - We run that temporary program.
//   - We delete the temporary program so only the .c files are left.
//   - We remember what happened in suite_results so we can print a report.
// So everything really lives in the C files, and the .exe is
// just a short‑lived helper.
static void run_one_test_exe(const char *exe_name, const char *friendly_name, int suite_index) {
    const char *build_command = NULL;
    const char *temp_exe = "temp_test_runner.exe";
    SuiteResult *result = get_suite_result(suite_index);

    if (result != NULL) {
        result->name = friendly_name;
        result->ran = 1;
        result->build_ok = 0;
        result->run_ok = 0;
        result->exit_code = -1;
        result->fail_count = 0;
    }

    printf("\n========================================\n");
    printf("Running tests from C files: %s\n", friendly_name);
    printf("========================================\n");

    // Choose the right gcc command based on which test the user picked.
    if (strcmp(exe_name, "tests_memory.exe") == 0) {
        build_command = "gcc -Wall -pedantic -g tests_memory.c memory.c errors.c -o temp_test_runner.exe";
    } else if (strcmp(exe_name, "tests_parser.exe") == 0) {
        build_command = "gcc -Wall -pedantic -g tests_parser.c parser.c -o temp_test_runner.exe";
    } else if (strcmp(exe_name, "tests_executor.exe") == 0) {
        build_command = "gcc -Wall -pedantic -g tests_executor.c parser.c memory.c executor.c errors.c -o temp_test_runner.exe";
    } else if (strcmp(exe_name, "tests_integration.exe") == 0) {
        build_command = "gcc -Wall -pedantic -g tests_integration.c parser.c memory.c executor.c errors.c -o temp_test_runner.exe";
    } else {
        printf("\nI do not know how to run tests for '%s'.\n", exe_name);
        wait_for_enter();
        return;
    }

    printf("Compiling tests from C files...\n");

    // Step 1: compile the C files into the temporary exe.
    int build_result = system(build_command);
    if (build_result != 0) {
        printf("\nBuild failed (code %d).\n", build_result);
        printf("Please check that gcc is installed and all .c files are here.\n");
        if (result != NULL) {
            result->build_ok = 0;
            result->run_ok = 0;
            result->exit_code = build_result;
        }
        wait_for_enter();
        return;
    }

    if (result != NULL) {
        result->build_ok = 1;
    }

    // Step 2: run the temporary exe that holds the tests.
    // We capture its text output into a file so we can:
    //   - show you everything it printed
    //   - pull out the "FAIL ..." lines for the final report
    printf("\nNow running the compiled tests from: %s\n", temp_exe);
    const char *output_file = "temp_test_output.txt";
    char run_command[512];
    snprintf(run_command, sizeof(run_command), "%s > %s", temp_exe, output_file);
    int run_result = system(run_command);

    if (run_result == -1) {
        printf("\nCould not start '%s'.\n", temp_exe);
        if (result != NULL) {
            result->run_ok = 0;
            result->exit_code = -1;
        }
    } else {
        if (result != NULL) {
            result->run_ok = 1;
            result->exit_code = run_result;
        }
        // Read back everything the test program printed and show it here.
        FILE *out = fopen(output_file, "r");
        if (out != NULL) {
            char line[512];
            printf("\n----- Detailed output from %s -----\n", friendly_name);
            while (fgets(line, sizeof(line), out)) {
                // print the line exactly as the test wrote it
                fputs(line, stdout);
                // if the line starts with "FAIL", remember it for the report
                if (strncmp(line, "FAIL", 4) == 0 && result != NULL) {
                    if (result->fail_count < 20) {
                        // copy at most 255 chars so we do not overflow
                        strncpy(result->fail_messages[result->fail_count], line, 255);
                        // make sure it is always zero terminated
                        result->fail_messages[result->fail_count][255] = '\0';
                        result->fail_count++;
                    }
                }
            }
            fclose(out);
            printf("----- End of detailed output for %s -----\n", friendly_name);
        } else {
            printf("\n(Note: could not open '%s' to show detailed output.)\n", output_file);
        }

        if (run_result == 0) {
            printf("\nTests finished with exit code 0 (usually means all passed).\n");
        } else {
            printf("\nTests finished with non zero exit code: %d\n", run_result);
            printf("This usually means at least one test failed.\n");
        }
    }

    // Step 3: delete the temporary exe and the temporary output file
    // so only the .c files remain.
    if (remove(temp_exe) == 0) {
        printf("\nClean up: deleted temporary test program '%s'.\n", temp_exe);
    } else {
        printf("\nNote: could not delete '%s'. It might still be in use or locked.\n", temp_exe);
    }
    if (remove(output_file) != 0) {
        // not a big problem if this fails, so we just mention it once
        printf("\nNote: could not delete '%s'. You can delete it by hand if you want.\n", output_file);
    }

    wait_for_enter();
}

// This helper prints the main menu that you see on the screen.
// It does not read anything, it only shows the options.
static void print_menu(void) {
    printf("\n");
    printf("========================================\n");
    printf("             TEST MENU                  \n");
    printf("========================================\n");
    printf("1) Run memory tests\n");
    printf("2) Run parser tests\n");
    printf("3) Run executor tests\n");
    printf("4) Run integration tests\n");
    printf("5) Run ALL tests (1 + 2 + 3 + 4)\n");
    printf("0) Quit\n");
    printf("Choose an option and press Enter: ");
}

// This helper prints a short "at a glance" report after running tests.
// It reads the suite_results array and explains, for each group, what happened.
static void print_summary_report(void) {
    printf("\n========================================\n");
    printf("           TEST SUMMARY REPORT          \n");
    printf("========================================\n");

    for (int i = 0; i < 4; i++) {
        SuiteResult *r = get_suite_result(i);
        if (r == NULL || r->name == NULL) {
            continue;
        }

        printf("- %s: ", r->name);

        if (!r->ran) {
            printf("NOT RUN in this session.\n");
            continue;
        }

        if (!r->build_ok) {
            printf("BUILD FAILED.\n");
            printf("  -> gcc could not compile the test program.\n");
            continue;
        }

        if (!r->run_ok) {
            printf("COULD NOT START TEST PROGRAM.\n");
            printf("  -> temp_test_runner.exe did not run.\n");
            continue;
        }

        if (r->exit_code == 0) {
            printf("PASSED (all checks reported success).\n");
        } else {
            printf("FAILED (exit code %d).\n", r->exit_code);
            if (r->fail_count > 0) {
                printf("  What went wrong (one line per failing check):\n");
                for (int j = 0; j < r->fail_count; j++) {
                    printf("    %s", r->fail_messages[j]);
                    // the lines we stored already contain their own '\n'
                }
            } else {
                // If we did not manage to capture any "FAIL" lines, we still
                // want to tell you in plain words what this group was trying to test.
                printf("  (We did not see any individual FAIL lines, but something went wrong.)\n");
                printf("  This group of tests was checking:\n");
                if (i == 0) {
                    printf("    - How variables are created, freed, and stored in memory.\n");
                    printf("    - That new cells start at 0 and do not overlap with other variables.\n");
                    printf("    - That var_exists, var_get, var_read_at, and var_write_at behave sensibly.\n");
                } else if (i == 1) {
                    printf("    - That the parser reads tiny program files correctly.\n");
                    printf("    - That every command has the right op code, variable names, and numbers.\n");
                } else if (i == 2) {
                    printf("    - That running commands really changes memory the way the comments say.\n");
                    printf("    - For example: Mal makes arrays, Ass writes numbers, Inc/Dec change cells,\n");
                    printf("      Add/Sub/Mul/And/Xor update values correctly, and Fre removes variables.\n");
                } else if (i == 3) {
                    printf("    - That whole mini programs (from .txt files) behave end to end.\n");
                    printf("    - For example: creating x and y, doing math, printing values, and freeing them\n");
                    printf("      without crashing and leaving memory in the expected final state.\n");
                } else {
                    printf("    - A set of checks in one of the tests_*.c files.\n");
                }
                printf("  To see the exact details, try running that test group on its own from the menu.\n");
            }
        }
    }

    printf("========================================\n");
}

// This helper lets you change which folder the program is "in".
// Why this matters:
//   When we call system(\"tests_memory.exe\") the system looks
//   in the current folder for that file.
//   If your tests live in some other folder, you can tell this
//   program to move there first.
//
// Steps:
//   - Ask user for a folder path.
//   - If they just press Enter we keep the current folder.
//   - If they type something we try to switch to that folder.
//   - If it fails we print an error but still keep going.
static void maybe_change_folder_at_start(void) {
    char path[512];

    printf("Optional: type a folder path where your test .exe files live.\n");
    printf("For example: C:\\\\Users\\\\YourName\\\\Downloads\\\\Washing-Machine--adt-version-2 (2)\\\\Washing-Machine--adt-version-2\n");
    printf("If you just press Enter, we will stay in the current folder.\n");
    printf("Folder path: ");

    if (fgets(path, sizeof(path), stdin) == NULL) {
        // If reading failed, just stay where we are.
        printf("Could not read input, staying in the current folder.\n");
        return;
    }

    trim_newline(path);

    // If the user typed nothing (empty string), we stay in the current folder.
    if (path[0] == '\0') {
        printf("Staying in the current folder.\n");
        return;
    }

    // Try to change to the folder the user typed.
    if (CHANGE_DIR(path) == 0) {
        printf("Now using this folder for tests: %s\n", path);
    } else {
        printf("Could not change to folder:\n");
        printf("  %s\n", path);
        printf("Please check that it exists and try again later.\n");
        printf("For now we will stay in the original folder.\n");
    }
}

// This is the main function of tests_main.c.
// It is what actually runs when you double click tests_main.exe.
//
// Big picture:
//   1) Optionally let you change to a different folder.
//   2) Loop forever showing a menu until you choose 0 (Quit).
//   3) Based on your choice, call one or more of the test .exe files.
//
// Important:
//   For this to work you must first build these files into .exe:
//     tests_memory.exe
//     tests_parser.exe
//     tests_executor.exe
//     tests_integration.exe
//   And they must live in the folder you are using.
int main(void) {
    char choice_line[16];

    printf("========================================\n");
    printf("          SIMPLE TEST LAUNCHER          \n");
    printf("========================================\n\n");

    // First, see if the user wants to switch to another folder.
    maybe_change_folder_at_start();

    // Make sure the summary records start out clean.
    for (int i = 0; i < 4; i++) {
        suite_results[i].name = NULL;
        suite_results[i].ran = 0;
        suite_results[i].build_ok = 0;
        suite_results[i].run_ok = 0;
        suite_results[i].exit_code = 0;
    }

    // Now we keep showing the menu until the user decides to quit.
    while (1) {
        print_menu();

        // Read their choice as a line of text.
        if (fgets(choice_line, sizeof(choice_line), stdin) == NULL) {
            printf("\nCould not read input. Exiting.\n");
            break;
        }

        // Clean up the newline at the end.
        trim_newline(choice_line);

        // Option 0: quit.
        if (strcmp(choice_line, "0") == 0) {
            printf("\nGoodbye. Closing tests_main.exe now.\n");
            break;
        }

        // Option 1: run only memory tests.
        else if (strcmp(choice_line, "1") == 0) {
            run_one_test_exe("tests_memory.exe", "Memory tests", 0);
        }

        // Option 2: run only parser tests.
        else if (strcmp(choice_line, "2") == 0) {
            run_one_test_exe("tests_parser.exe", "Parser tests", 1);
        }

        // Option 3: run only executor tests.
        else if (strcmp(choice_line, "3") == 0) {
            run_one_test_exe("tests_executor.exe", "Executor tests", 2);
        }

        // Option 4: run only integration tests.
        else if (strcmp(choice_line, "4") == 0) {
            run_one_test_exe("tests_integration.exe", "Integration tests", 3);
        }

        // Option 5: run all tests one after another.
        else if (strcmp(choice_line, "5") == 0) {
            printf("\nRunning ALL tests (memory, parser, executor, integration)...\n");

            run_one_test_exe("tests_memory.exe", "Memory tests", 0);
            run_one_test_exe("tests_parser.exe", "Parser tests", 1);
            run_one_test_exe("tests_executor.exe", "Executor tests", 2);
            run_one_test_exe("tests_integration.exe", "Integration tests", 3);

            // After all four have been attempted, show a neat summary.
            print_summary_report();
            wait_for_enter();
        }

        // Any other input: tell the user it was not understood.
        else {
            printf("\nSorry, I did not understand that choice: \"%s\"\n", choice_line);
            printf("Please type 0, 1, 2, 3, 4, or 5 and then press Enter.\n");
            wait_for_enter();
        }
    }

    return 0;
}

