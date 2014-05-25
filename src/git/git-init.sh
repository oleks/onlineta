#!/usr/bin/env bash

# Create a Git repository that encrypts and signs all the data before it
# commits and decrypts and verifies at a checkout.

# Usage: ./git-init.sh <name> [<keys>]
# Example: ./git-init.sh crypto 43FFA757 B7FFF4BD

# If no keys are specified, the default (set for gpg2) is used.
# The default gpg2 key is always used for signing.

DIR=$1
KEYS="${@:2}"

if [ -n "$KEYS" ]; then
  RECIPIENTS=$(echo "$KEYS" | sed -E 's/(^| )/ -r /g')
fi

git init --quiet "$DIR" || exit 1

cat >"$DIR/.git/info/attributes" <<EOF
* filter=crypt
EOF

cat >>"$DIR/.git/config" <<EOF
[filter "crypt"]
	clean = "gpg2 --encrypt --sign --quiet$RECIPIENTS"
	smudge = "gpg2 --decrypt --quiet"
	required
EOF
