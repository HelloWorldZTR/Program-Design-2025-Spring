import subprocess

# Compile the C++ program
compile_process = subprocess.run(['g++', 'E.cpp', '-o', 'E'], capture_output=True, text=True)
if compile_process.returncode != 0:
    print("Compilation failed:")
    print(compile_process.stderr)
    exit(1)

# Run the compiled program with input from data.in
with open('data2.in', 'r') as input_file, open('output.txt', 'w') as output_file:
    run_process = subprocess.run(['./E'], stdin=input_file, stdout=output_file, stderr=subprocess.PIPE, text=True)
    if run_process.returncode != 0:
        print("Program failed:")
        print(run_process.returncode)
        print(run_process.stderr)
        exit(1)
    #print first 10 lines of stderr
    for i in run_process.stderr.splitlines()[:10]:
        print(i)

# Compare the output with data.out
with open('output.txt', 'r') as output_file, open('data.out', 'r') as expected_output_file:
    output = output_file.read()
    expected_output = expected_output_file.read()

    # Compare which line is different
    output_lines = output.splitlines()
    expected_output_lines = expected_output.splitlines()
    for i, (output_line, expected_output_line) in enumerate(zip(output_lines, expected_output_lines)):
        if output_line != expected_output_line:
            print(f"Line {i + 1} is different:")
            print(f"Output: {output_line}")
            print(f"Expected: {expected_output_line}")
            break
    else:
        print("All lines are correct!")