# c89-tool

A Go/Odin-like build tool for strict C89 development.

## Features

- **Simple commands**: `c build .` and `c run .`
- **Strict C89 enforcement**: Catches C99+ features at compile time
- **Convention-based**: Zero configuration needed
- **Fast**: Direct compilation with clang
- **Multi-file support**: Automatically compiles all `.c` files in a directory

## Quick Start

```bash
# Install
curl -o ~/.local/bin/c https://raw.githubusercontent.com/YOUR_USERNAME/c89-tool/main/c
chmod +x ~/.local/bin/c

# Or use the install script
./install.sh

# Create a project
mkdir myproject && cd myproject
cat > main.c << 'EOF'
#include <stdio.h>
int main(void) {
    printf("Hello, C89!\n");
    return 0;
}
EOF

# Run it
c run .
```

## Usage

```bash
c build <target>    # Compile C89 source files
c run <target>      # Compile and run C89 program
```

**Target can be:**
- `.` - Current directory
- `<dir>` - Specific directory path
- `<file.c>` - Single C file

**Examples:**
```bash
c build .              # Build current directory
c run .                # Build and run
c build main.c         # Build single file
c run . arg1 arg2      # Run with arguments
```

## C89 Enforcement

The tool enforces strict C89 compliance:

- ✗ No `//` comments (only `/* */` allowed)
- ✗ No variable declarations in for-loops
- ✗ No mixed declarations and code
- ✗ Functions must use `void` for empty parameters
- ✗ All warnings are treated as errors

**Compiler flags:**
```bash
-std=c89                # ANSI C standard
-pedantic               # Strict compliance
-Werror                 # Warnings as errors
-Wall -Wextra           # All warnings
-Wstrict-prototypes     # Require prototypes
-Wmissing-prototypes    # Warn missing prototypes
-Wold-style-definition  # No old-style functions
-O2                     # Optimize
```

## Installation

### Quick Install

```bash
curl -o ~/.local/bin/c https://raw.githubusercontent.com/YOUR_USERNAME/c89-tool/main/c
chmod +x ~/.local/bin/c
```

### Manual Install

```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/c89-tool.git
cd c89-tool

# Run the install script
./install.sh
```

### Requirements

- **macOS/Linux**: bash, clang (or gcc)
- **PATH**: `~/.local/bin` should be in your PATH

To add `~/.local/bin` to your PATH:
```bash
# Add to ~/.bashrc or ~/.zshrc
export PATH="$HOME/.local/bin:$PATH"
```

## Project Structure

When you run `c build .` in a directory:

```
myproject/
├── main.c          # Your C source files
├── helper.c
├── helper.h
└── build/          # Generated (add to .gitignore)
    └── myproject   # Compiled binary
```

## Examples

### Single File

```bash
cat > hello.c << 'EOF'
#include <stdio.h>
int main(void) {
    printf("Hello, World!\n");
    return 0;
}
EOF

c run hello.c
```

### Multi-File Project

```bash
mkdir calculator && cd calculator

# main.c
cat > main.c << 'EOF'
#include <stdio.h>
#include "math.h"
int main(void) {
    printf("5 + 3 = %d\n", add(5, 3));
    return 0;
}
EOF

# math.h
cat > math.h << 'EOF'
#ifndef MATH_H
#define MATH_H
int add(int a, int b);
#endif
EOF

# math.c
cat > math.c << 'EOF'
#include "math.h"
int add(int a, int b) {
    return a + b;
}
EOF

c run .
```

## How It Works

1. **Discovery**: Finds all `.c` files in the target directory
2. **Validation**: Ensures at least one file contains `main()`
3. **Compilation**: Compiles all sources together with strict C89 flags
4. **Output**: Places binary in `./build/` directory
5. **Execution**: (for `run` command) Executes the binary

## Comparison to Other Tools

| Feature | c89-tool | go | odin |
|---------|----------|----|----- |
| Convention-based | ✓ | ✓ | ✓ |
| Silent success | ✓ | ✓ | ✓ |
| Single command | ✓ | ✓ | ✓ |
| Language | C89 | Go | Odin |

## .gitignore

Add this to your project's `.gitignore`:
```
build/
*.o
*.a
```

## Troubleshooting

**Command not found:**
```bash
# Check if ~/.local/bin is in PATH
echo $PATH

# If not, add it to ~/.bashrc or ~/.zshrc
export PATH="$HOME/.local/bin:$PATH"
```

**Compilation errors:**
- The tool is strict! Make sure your code is C89 compliant
- Common issues: `//` comments, for-loop declarations, missing `void` in `main()`

## License

MIT License - See LICENSE file

## Contributing

Pull requests welcome! Please ensure:
- Code follows existing style
- Tool remains zero-config
- C89 enforcement stays strict
