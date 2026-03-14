# Algorithms Library

This library provides a collection of algorithms implemented in C++. It is designed to be modular and easy to integrate into your projects.
## Compiling and Running C++ Files

To compile and execute the C++ files in this library, you need a C++ compiler. On Windows, you can use Microsoft's `cl.exe` compiler, which is included with Visual Studio.

### Installing the Compiler

1. **Install Visual Studio**: Download and install [Visual Studio](https://visualstudio.microsoft.com/). During installation, select the "Desktop development with C++" workload.
2. **Set up the environment**: Open the "Developer Command Prompt for Visual Studio" from the Start menu. This sets up the environment variables for `cl.exe`.

### Compiling with VS Code

1. **Install VS Code**: Download and install [Visual Studio Code](https://code.visualstudio.com/).
2. **Install C++ Extension**: In VS Code, install the "C/C++" extension by Microsoft.
3. **Open the project folder**: Use `File > Open Folder` to open your project.
4. **Configure tasks**: Create a `tasks.json` file in the `.vscode` directory to define build tasks for compiling with `cl.exe` and outputting files to the `build` folder.

Example `tasks.json`:
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build cpp",
            "type": "shell",
            "command": "cl.exe",
            "args": [
                "/EHsc",
                "src\\main.cpp",
                "/I",
                "include",
                "/Fo:build\\",      // Object files go to build/
                "/Fe:build\\main.exe" // Executable goes to build/
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

5. **Build and run**: Press `Ctrl+Shift+B` to build. Run the executable from the terminal:
```bash
build\main.exe
```

### Compiling from Command Line

In the Developer Command Prompt, compile and run a file, placing output in the `build` folder:
```bash
cl.exe /EHsc src\main.cpp /I include /Fo:build\ /Fe:build\main.exe
build\main.exe
```

This will compile `main.cpp`, place object files and the executable in the `build` folder, and you can execute the program from there.

## Building the Library

The library uses CMake as its build system. To build the library, follow these steps:

1. **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd <repository-directory>
    ```

2. **Generate build files with CMake:**
    ```bash
    cmake ..
    ```

3. **Build the library:**
    ```bash
    cmake --build .
    ```

This gives us
```
build/
   algorithms.lib
   test_graph.exe
```


## Using the Library

After building, you can link the generated library to your project. The library header files are located in the `include/` directory.

### Example Usage
