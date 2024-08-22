#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[32m"
#define RED_COLOR "\033[31m"
#define CYAN_COLOR "\033[36m"

#define LOG_FILE "compile_run.log"

typedef void (*compile_func)(const char *source_file, const char *output_file);

double get_elapsed_time(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

void log_message(const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file) {
        fprintf(log_file, "%s\n", message);
        fclose(log_file);
    }
}

void execute_and_log(const char *command) {
    FILE *fp;
    char path[1035];
    char log_message_buffer[2048];

    fp = popen(command, "r");
    if (fp == NULL) {
        snprintf(log_message_buffer, sizeof(log_message_buffer), "Error: Failed to run command: %s", command);
        log_message(log_message_buffer);
        return;
    }

    while (fgets(path, sizeof(path) - 1, fp) != NULL) {
        snprintf(log_message_buffer, sizeof(log_message_buffer), "%s", path);
        log_message(log_message_buffer);
    }

    int status = pclose(fp);
    if (status == -1) {
        snprintf(log_message_buffer, sizeof(log_message_buffer), "Error: Failed to close command stream: %s", command);
        log_message(log_message_buffer);
    } else if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        snprintf(log_message_buffer, sizeof(log_message_buffer), "Error: Command failed with exit code %d: %s", WEXITSTATUS(status), command);
        log_message(log_message_buffer);
    }
}

void compile_c(const char *source_file, const char *output_file) {
    char compile_command[150];
    sprintf(compile_command, "gcc %s -o %s", source_file, output_file);
    log_message(compile_command);
    execute_and_log(compile_command);
}

void compile_cpp(const char *source_file, const char *output_file) {
    char compile_command[150];
    sprintf(compile_command, "g++ %s -o %s", source_file, output_file);
    log_message(compile_command);
    execute_and_log(compile_command);
}

void compile_java(const char *source_file, const char *output_file) {
    char compile_command[150];
    sprintf(compile_command, "javac %s", source_file);
    log_message(compile_command);
    execute_and_log(compile_command);
}

void run_program(const char *output_file) {
    if (access(output_file, X_OK) != 0) {
        printf(RED_COLOR "Error: Output file '%s' does not exist or is not executable.\n" RESET_COLOR, output_file);
        return;
    }

    char run_command[150];
    sprintf(run_command, "./%s", output_file);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    int run_status = system(run_command);
    gettimeofday(&end_time, NULL);
    double run_time = get_elapsed_time(start_time, end_time);

    if (run_status == 0) {
        printf(CYAN_COLOR "\nProgram executed successfully.\n");
        printf("Execution Time  : %.2f seconds\n" RESET_COLOR, run_time);
    } else {
        printf(RED_COLOR "Error running the program.\n" RESET_COLOR);
    }
}

compile_func get_compile_function(const char *extension) {
    if (strcmp(extension, ".c") == 0) {
        return compile_c;
    } else if (strcmp(extension, ".cpp") == 0) {
        return compile_cpp;
    } else if (strcmp(extension, ".java") == 0) {
        return compile_java;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        printf(RED_COLOR "Usage: %s <source_file> [output_file]\n" RESET_COLOR, argv[0]);
        return 1;
    }

    char *source_file = argv[1];
    char output_file[100];
    char *extension = strrchr(source_file, '.');

    if (argc == 3) {
        strcpy(output_file, argv[2]);
    } else {
        if (extension) {
            if (strcmp(extension, ".c") == 0) {
                snprintf(output_file, sizeof(output_file), "%s.o", source_file);
            } else if (strcmp(extension, ".cpp") == 0) {
                snprintf(output_file, sizeof(output_file), "%s.o", source_file);
            } else if (strcmp(extension, ".java") == 0) {
                snprintf(output_file, sizeof(output_file), "%s.class", source_file);
            } else {
                printf(RED_COLOR "Error: Unsupported source file extension.\n" RESET_COLOR);
                return 1;
            }
        } else {
            printf(RED_COLOR "Error: Source file should have a valid extension.\n" RESET_COLOR);
            return 1;
        }
    }

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    compile_func compile = get_compile_function(extension);
    if (compile) {
        compile(source_file, output_file);
    } else {
        printf(RED_COLOR "Error: Unsupported source file extension.\n" RESET_COLOR);
        return 1;
    }
    gettimeofday(&end_time, NULL);
    double compile_time = get_elapsed_time(start_time, end_time);

    printf(CYAN_COLOR "Compilation successful.\n");
    printf(CYAN_COLOR "Output file          : %s\n", output_file);
    printf(CYAN_COLOR "Compilation Time     : %.2f seconds\n", compile_time);
    printf(RESET_COLOR "\n");

    if (strcmp(extension, ".java") != 0) {
        run_program(output_file);
    }

    return 0;
}
