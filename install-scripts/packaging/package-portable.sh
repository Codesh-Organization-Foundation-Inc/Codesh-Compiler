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
mkdir -p "$OUT_DIR"

STAGING_DIR="$(mktemp -d /tmp/codeshc-portable-staging.XXXXXX)"
trap 'rm -rf "$STAGING_DIR"' EXIT

cp "$BUILD_DIR/codeshc" "$STAGING_DIR/codeshc-raw"
cp "$BUILD_DIR/תלמוד־קודש.jar" "$STAGING_DIR/תלמוד־קודש.jar"

# Bundle ICU
for lib in libicui18n.so.76 libicuuc.so.76 libicudata.so.76; do
    path=$(ldconfig -p | grep "^\s*$lib " | awk '{print $NF}' | head -1)
    if [ -z "$path" ]; then
        echo "Warning: $lib not found on this system, skipping"
    else
        # Copy the real file (resolve symlink)
        cp -L "$path" "$STAGING_DIR/$lib"
    fi
done

cat > "$STAGING_DIR/codeshc" << 'EOF'
#!/bin/bash
SELF="$(readlink -f "$0" 2>/dev/null || realpath "$0" 2>/dev/null || echo "$0")"
DIR="$(cd "$(dirname "$SELF")" && pwd)"
LD_LIBRARY_PATH="$DIR:$LD_LIBRARY_PATH" exec "$DIR/codeshc-raw" "$@"
EOF
chmod +x "$STAGING_DIR/codeshc"

# Bundle install/uninstall scripts
SCRIPTS_DIR="$(dirname "$0")/.."
for script in install-global.sh uninstall-global.sh; do
    cp "$SCRIPTS_DIR/$script" "$STAGING_DIR/$script"
done
chmod +x "$STAGING_DIR/install-global.sh" "$STAGING_DIR/uninstall-global.sh"

tar -czf "$OUT_DIR/codeshc-portable.tar.gz" -C "$STAGING_DIR" .

echo "Created $OUT_DIR/codeshc-portable.tar.gz successfully"
