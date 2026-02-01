#!/bin/bash
set -euo pipefail

# c89-tool installer

INSTALL_DIR="$HOME/.local/bin"
TOOL_NAME="c"

echo "Installing c89-tool..."

# Create install directory if it doesn't exist
mkdir -p "$INSTALL_DIR"

# Copy the tool
cp c "$INSTALL_DIR/$TOOL_NAME"
chmod +x "$INSTALL_DIR/$TOOL_NAME"

echo "✓ Installed to $INSTALL_DIR/$TOOL_NAME"

# Check if directory is in PATH
if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
    echo ""
    echo "⚠️  $INSTALL_DIR is not in your PATH"
    echo ""
    echo "Add this line to your ~/.bashrc or ~/.zshrc:"
    echo "  export PATH=\"\$HOME/.local/bin:\$PATH\""
    echo ""
    echo "Then run: source ~/.bashrc  (or ~/.zshrc)"
else
    echo "✓ $INSTALL_DIR is in PATH"
fi

echo ""
echo "Installation complete!"
echo ""
echo "Try it out:"
echo "  c --help"
