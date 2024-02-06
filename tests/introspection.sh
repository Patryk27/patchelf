#! /bin/sh -e
SCRATCH=scratch/$(basename "$0" .sh)

rm -rf "${SCRATCH}"
mkdir -p "${SCRATCH}"

SCRATCHFILE=${SCRATCH}/introspection
cp introspection "$SCRATCHFILE"

doit() {
    set +x
    ../src/patchelf "$@" "$SCRATCHFILE"
    set -x
}

expect() {
    actual=$("$SCRATCHFILE" || true)
    expected="$1"

    if [ "$actual" != "$expected" ]; then
        echo "Expected '$expected' but got '$actual'"
        exit 1
    fi
}

expect "none"
doit --set-rpath short
expect "RUNPATH=short"
doit --set-rpath very-long-very-long-very-long-very-long
expect "RUNPATH=very-long-very-long-very-long-very-long"
doit --set-rpath short-again
expect "RUNPATH=short-again"
doit --force-rpath --set-rpath testo
expect "RPATH=testo"
doit --remove-rpath
expect "none"
