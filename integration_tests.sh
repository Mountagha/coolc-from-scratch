#!/bin/bash

# Define the path to the compiler
COMPILER="path/to/my_compiler"

# Define directories
INPUT_DIR="tests/inputs"
EXPECTED_DIR="tests/expected"
OUTPUT_DIR="tests/outputs"

# Create the output directory if it doesn't exist
mkdir -p $OUTPUT_DIR

# Define test cases
TEST_CASES=("test1" "test2") # Add more test cases as needed

# Function to run a single test case
run_test() {
    local test=$1

    # Compile the source code
    $COMPILER "$INPUT_DIR/$test.src" -o "$OUTPUT_DIR/$test.out"
    if [ $? -ne 0 ]; then
        echo "Failed to compile $test.src"
        return 1
    fi

    # Run the compiled binary
    "$OUTPUT_DIR/$test.out" > "$OUTPUT_DIR/$test.output"
    if [ $? -ne 0 ]; then
        echo "Execution failed for $test.out"
        return 1
    fi

    # Compare the actual output with the expected output
    if diff -q "$OUTPUT_DIR/$test.output" "$EXPECTED_DIR/$test.expected" > /dev/null; then
        echo "Test $test passed."
    else
        echo "Test $test failed."
        echo "Expected:"
        cat "$EXPECTED_DIR/$test.expected"
        echo "Got:"
        cat "$OUTPUT_DIR/$test.output"
    fi
}

# Run all test cases
for test in "${TEST_CASES[@]}"; do
    run_test $test
done
