import subprocess
import os

# File paths
parent_dir = 'D:\\Develop\\algorithm\\Program-Design-2025-Spring\\hw15\\'
os.chdir(parent_dir)

source_file = "a.cpp"
input_file = "extra.in"
output_file = "extra.out"
compiled_file = "a.exe"

# Compile the C++ source file
compile_process = subprocess.run(["g++", source_file, "-o", compiled_file], capture_output=True, text=True)

if compile_process.returncode != 0:
    print("Compilation failed:")
    print(compile_process.stderr)
else:
    print("Compilation successful.")

    # Run the compiled program with input redirection
    with open(input_file, "r") as infile, open("program_output.txt", "w") as outfile:
        run_process = subprocess.run([compiled_file], stdin=infile, stdout=outfile, stderr=subprocess.PIPE, text=True)

    if run_process.returncode != 0:
        print("Runtime error:")
        print(run_process.stderr)
    else:
        print("Program executed successfully.")

        # Compare the output with the expected output
        with open("program_output.txt", "r") as program_output, open(output_file, "r") as expected_output:
            line_number = 1
            for program_line, expected_line in zip(program_output, expected_output):
                # if line_number == 11308:
                #     line_number += 1
                #     continue
                if program_line.strip() != expected_line.strip():
                    print("\n======================\nMismatch found:")
                    print(f"On line {line_number}:")
                    print(f"Program output: {program_line.strip()}")
                    print(f"Expected output: {expected_line.strip()}")
                    break
                line_number += 1
            else:
                print("Outputs match perfectly.")