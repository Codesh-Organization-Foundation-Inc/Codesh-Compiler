#!/bin/bash

# This script was made specifically because of ICU.
# It's hell to bundle
# Hell to install
# So this is what you get

set -e

if [ $# -lt 2 ]; then
    echo "Usage: $0 <build_dir> <out_dir>"
    exit 1
fi

BUILD_DIR="$1"
OUT_DIR="$2"

if [ ! -f "$BUILD_DIR/codeshc" ]; then
    echo "Binary not found at $BUILD_DIR/codeshc"
    echo "Run build.sh first"
    exit 1
fi
rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"

cp "$BUILD_DIR/codeshc" "$OUT_DIR/codesh-raw"
cp "$BUILD_DIR/תלמוד־קודש.jar" "$OUT_DIR/תלמוד־קודש.jar"

# Bundle ICU
for lib in libicui18n.so.76 libicuuc.so.76 libicudata.so.76; do
    path=$(ldconfig -p | grep "^\s*$lib " | awk '{print $NF}' | head -1)
    if [ -z "$path" ]; then
        echo "Warning: $lib not found on this system, skipping"
    else
        # Copy the real file (resolve symlink)
        cp -L "$path" "$OUT_DIR/$lib"
    fi
done

cat > "$OUT_DIR/codeshc" << 'EOF'
#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd)"
LD_LIBRARY_PATH="$DIR:$LD_LIBRARY_PATH" exec "$DIR/codesh-raw" "$@"
EOF
chmod +x "$OUT_DIR/codeshc"

# Bundle install/uninstall scripts
SCRIPTS_DIR="$(dirname "$0")/.."
for script in install-global.sh uninstall-global.sh; do
    cp "$SCRIPTS_DIR/$script" "$OUT_DIR/$script"
done
chmod +x "$OUT_DIR/install-global.sh" "$OUT_DIR/uninstall-global.sh"

tar -czf codeshc-portable.tar.gz -C "$OUT_DIR" .

echo "Created codeshc-portable.tar.gz successfully"
