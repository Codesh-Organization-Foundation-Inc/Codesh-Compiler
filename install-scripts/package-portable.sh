#!/bin/bash

# This script was made specifically because of ICU.
# It's hell to bundle
# Hell to install
# So this is what you get

set -e

if [ $# -lt 2 ]; then
    echo "Usage: $0 <binary> <out_dir>"
    exit 1
fi

BINARY="$1"
OUT_DIR="$2"

if [ ! -f "$BINARY" ]; then
    echo "Binary not found at $BINARY"
    echo "Run build.sh first"
    exit 1
fi
rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"

cp "$BINARY" "$OUT_DIR/codesh-raw"

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

# Bundle install script
cp "$(dirname "$0")/install-global.sh" "$OUT_DIR/install-global.sh"
chmod +x "$OUT_DIR/install-global.sh"

tar -czf codeshc-portable.tar.gz -C "$OUT_DIR" .

echo "Created codeshc-portable.tar.gz successfully"