# Telecom Data Serialization & Analysis System

A modular C application engineered to parse raw telecom log data, utilizing dynamic memory allocation and pointers to optimize runtime efficiency. Developed natively in a Linux (WSL2/Ubuntu) environment and managed via custom Makefiles.

## Technical Highlights
* **Systems-Level Memory Management:** Constructed a custom linked list data structure from scratch to recursively traverse, filter, and aggregate network statistics.
* **Binary File I/O:** Implemented binary serialization using `fread` and `fwrite` to write `C` structs directly to raw `.bin` files and safely restore them to heap memory without segmentation faults.
* **Build Automation:** Utilized GNU Makefiles for automated cross-platform compilation and testing.

## Technologies Used
* **Language:** C
* **Environment:** Linux (Ubuntu/WSL2), GCC
* **Tools:** GDB (GNU Debugger), Makefiles, Git
