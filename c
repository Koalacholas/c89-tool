#!/bin/bash
set -euo pipefail

# c - C89 Development Tool
# A Go/Odin-like build tool for strict C89 development

# Configuration
CC="clang"
CFLAGS="-std=c89 -pedantic -Werror -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -O2"
BUILD_DIR="build"

# Colors for output (optional, only for errors)
RED='\033[0;31m'
NC='\033[0m' # No Color

show_usage() {
    cat << 'EOF'
c: C89 development tool

Usage:
    c build <target>    Compile C89 source files
    c run <target>      Compile and run C89 program

Target can be:
    .           Current directory
    <dir>       Specific directory
    <file.c>    Single C file

Examples:
    c build .
    c run .
    c build main.c
    c run ../other
EOF
}

error_exit() {
    echo -e "${RED}c: $1${NC}" >&2
    exit 1
}

resolve_target() {
    local target="$1"

    if [[ "$target" == "." ]]; then
        echo "$(pwd)"
    elif [[ -f "$target" ]]; then
        if [[ "$target" == *.c ]]; then
            echo "$(cd "$(dirname "$target")" && pwd)/$(basename "$target")"
        else
            error_exit "not a C source file: $target"
        fi
    elif [[ -d "$target" ]]; then
        echo "$(cd "$target" && pwd)"
    else
        error_exit "target not found: $target"
    fi
}

find_sources() {
    local target="$1"

    if [[ -f "$target" ]]; then
        # Single file mode
        echo "$target"
    else
        # Directory mode - find all .c files (non-recursive)
        local sources
        sources=$(find "$target" -maxdepth 1 -name "*.c" -type f 2>/dev/null | sort)

        if [[ -z "$sources" ]]; then
            error_exit "no .c files found in directory: $target"
        fi

        echo "$sources"
    fi
}

get_output_name() {
    local target="$1"

    if [[ -f "$target" ]]; then
        # Single file - use filename without extension
        basename "$target" .c
    else
        # Directory - use directory name
        basename "$target"
    fi
}

validate_sources() {
    local sources="$1"

    # Check if at least one file contains main function
    local has_main=0
    while IFS= read -r file; do
        if grep -q "int[[:space:]]*main[[:space:]]*(" "$file" 2>/dev/null; then
            has_main=1
            break
        fi
    done <<< "$sources"

    if [[ $has_main -eq 0 ]]; then
        error_exit "no main() function found in source files"
    fi
}

compile() {
    local sources="$1"
    local output="$2"
    local target_dir="$3"

    # Create build directory
    mkdir -p "$target_dir/$BUILD_DIR"

    # Compile all sources together
    # shellcheck disable=SC2086
    if ! $CC $CFLAGS $sources -o "$target_dir/$BUILD_DIR/$output" 2>&1; then
        return 1
    fi

    return 0
}

run_binary() {
    local binary="$1"
    local target_dir="$2"
    shift 2  # Remove binary and target_dir from args

    # Execute the binary with any additional arguments
    exec "$target_dir/$BUILD_DIR/$binary" "$@"
}

build_command() {
    local target="$1"

    # Resolve target path
    local resolved_target
    resolved_target=$(resolve_target "$target")

    # Find source files
    local sources
    sources=$(find_sources "$resolved_target")

    # Validate sources
    validate_sources "$sources"

    # Get output name
    local output_name
    output_name=$(get_output_name "$resolved_target")

    # Determine target directory (where build/ will be created)
    local target_dir
    if [[ -f "$resolved_target" ]]; then
        target_dir=$(dirname "$resolved_target")
    else
        target_dir="$resolved_target"
    fi

    # Compile
    if ! compile "$sources" "$output_name" "$target_dir"; then
        exit 1
    fi

    # Silent success (like go build)
}

run_command() {
    local target="$1"
    shift  # Remove target from args to get program arguments

    # Resolve target path
    local resolved_target
    resolved_target=$(resolve_target "$target")

    # Find source files
    local sources
    sources=$(find_sources "$resolved_target")

    # Validate sources
    validate_sources "$sources"

    # Get output name
    local output_name
    output_name=$(get_output_name "$resolved_target")

    # Determine target directory
    local target_dir
    if [[ -f "$resolved_target" ]]; then
        target_dir=$(dirname "$resolved_target")
    else
        target_dir="$resolved_target"
    fi

    # Compile
    if ! compile "$sources" "$output_name" "$target_dir"; then
        exit 1
    fi

    # Run with remaining arguments
    run_binary "$output_name" "$target_dir" "$@"
}

# Main logic
main() {
    if [[ $# -eq 0 ]]; then
        show_usage
        exit 0
    fi

    local command="$1"

    case "$command" in
        build)
            if [[ $# -lt 2 ]]; then
                show_usage
                exit 1
            fi
            build_command "$2"
            ;;
        run)
            if [[ $# -lt 2 ]]; then
                show_usage
                exit 1
            fi
            shift  # Remove 'run'
            run_command "$@"
            ;;
        -h|--help|help)
            show_usage
            exit 0
            ;;
        *)
            echo "c: unknown command: $command" >&2
            echo "" >&2
            show_usage
            exit 1
            ;;
    esac
}

main "$@"
