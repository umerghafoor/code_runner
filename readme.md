Here’s a `README.md` file tailored for your program:

# Compile and Run Program

## Overview

This utility compiles and runs C, C++, and Java source files. It logs the compilation and execution processes into `compile_run.log` and provides detailed feedback on success or failure.

## Features

- **Compile**: Supports C, C++, and Java source files.
- **Run**: Executes the compiled program for C and C++ files.
- **Logging**: Logs all commands and outputs to `compile_run.log`.
- **Color-Coded Output**: Uses color codes to highlight error messages and status.

## Requirements

- **GCC**: Required for C and C++ compilation.
- **JDK**: Required for Java compilation.

## Installation

1. **Compile the Program**

   To compile the `compile_run` program, run:

   ```bash
   gcc -o compile_run compile_run.c
   ```

   Ensure you have `gcc` installed on your system. You may need to adjust the filename if it's different.

2. **Place the Program in Your Environment**

   To make the `compile_run` executable accessible from anywhere, place it in a directory that is part of your system's `PATH`. For example:

   ```bash
   sudo cp compile_run /usr/local/bin/
   ```

   Replace `compile_run` with the name of your compiled program if different.

## Usage

To use the program, run:

```bash
compile_run <source_file> [output_file]
```

- `<source_file>`: Path to the source file to compile.
- `[output_file]` (optional): Name of the output file. Defaults are used based on the source file extension if not provided.

### Examples

- **Compile and run a C program:**

  ```bash
  compile_run hello.c
  ```

- **Compile and run a C++ program with a custom output file:**

  ```bash
  compile_run hello.cpp my_program
  ```

- **Compile a Java program (does not run):**

  ```bash
  compile_run HelloWorld.java
  ```

## Notes

- **Error Handling**: All errors and outputs are logged to `compile_run.log`.
- **Permissions**: Ensure the output file is executable to run it successfully.
