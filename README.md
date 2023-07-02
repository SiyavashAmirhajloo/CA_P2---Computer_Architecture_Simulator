# Computer Architecture Simulator

## Description
This project is a simulator implemented in C that emulates the execution of machine code instructions. It provides a basic framework for fetching, decoding, executing, and storing instructions, simulating the behavior of a processor.

## Getting Started
To get started with the simulator, follow these steps:

1. Clone the repository: `git clone https://github.com/your-username/project-name.git`
2. Navigate to the project directory: `cd project-name`
3. Compile the code: `gcc -o simulator Simulator.c`
4. Run the simulator: `./simulator machprog.mc finalprog.txt`

Make sure to replace `machprog.mc` with your machine code file and `finalprog.txt` with the desired output file.

## Usage
The simulator takes two command-line arguments: the machine code file and the output file. It reads the machine code instructions from the input file, simulates their execution, and writes the final result to the output file.

## Features
- Fetching: The simulator fetches machine code instructions from the input file.
- Decoding: It decodes the instructions and determines their type (R, I, or J).
- Execution: The simulator executes the instructions based on their opcode and operands.
- Memory: It includes a main memory and register file to store and manipulate data.
- Output: The simulator displays the contents of the register file and writes the final result to the output file.

## Contributing
Contributions to the project are welcome. If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

