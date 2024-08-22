#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef void (*compile_func)(const char *source_file, const char *output_file);

double get_elapsed_time(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

void compile_c(const char *source_file, const char *output_file) {
    char compile_command[150];
    sprintf(compile_command, "gcc %s -o %s", source_file, output_file);
    system(compile_command);
}

void compile_cpp(const char *source_file, const char *output_file) {
    char compile_command[150];
    sprintf(compile_command, "g++ %s -o %s", source_file, output_file);
    system(compile_command);
}

// Add new compile functions here for additional languages

void run_program(const char *output_file) {
    char run_command[150];
    sprintf(run_command, "./%s", output_file);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    int run_status = system(run_command);
    gettimeofday(&end_time, NULL);
    double run_time = get_elapsed_time(start_time, end_time);

    if (run_status == 0) {
        printf("Program executed successfully.\n");
        printf("Time taken for execution: %.2f seconds\n", run_time);
    } else {
        printf("Error running the program.\n");
    }
}

compile_func get_compile_function(const char *extension) {
    if (strcmp(extension, ".c") == 0) {
        return compile_c;
    } else if (strcmp(extension, ".cpp") == 0) {
        return compile_cpp;
    }
    // Add more conditions here for additional languages
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    char *source_file = argv[1];
    char output_file[100];

    // Copy the source file name and replace .c/.cpp with .o for the output file
    strcpy(output_file, source_file);
    char *dot = strrchr(output_file, '.');
    if (dot) {
        compile_func compile = get_compile_function(dot);
        if (compile) {
            // Determine the output file extension based on the input file extension
            if (strcmp(dot, ".c") == 0 || strcmp(dot, ".cpp") == 0) {
                strcpy(dot, ".o");
            } else {
                printf("Error: Unsupported source file extension.\n");
                return 1;
            }

            // Measure time taken for compilation
            struct timeval start_time, end_time;
            gettimeofday(&start_time, NULL);
            compile(source_file, output_file);
            gettimeofday(&end_time, NULL);
            double compile_time = get_elapsed_time(start_time, end_time);

            printf("Compilation successful. Output file: %s\n", output_file);
            printf("Time taken for compilation: %.2f seconds\n", compile_time);

            // Run the compiled program
            run_program(output_file);
        } else {
            printf("Error: Unsupported source file extension.\n");
            return 1;
        }
    } else {
        printf("Error: Source file should have a valid extension.\n");
        return 1;
    }

    return 0;
}
