#!/bin/sh

mkdir -p "$1"
cd "$1"
git init

cat >"./.git/info/attributes" <<EOF
* filter=crypt
EOF

cat >>"./.git/config" <<EOF
[filter "crypt"]
	clean = "/usr/bin/env bash -c 'tee \
    >(gpg -ear 26934BE1 2>&1 | tr \\"\\\\n\\" \\"$\\" | sed \\"s/\\\\\$\$/\\\\n/\\") \
    >(gpg -ear 5496F89F 2>&1 | tr \\"\\\\n\\" \\"$\\" | sed \\"s/\\\\\$\$/\\\\n/\\") \
    1>/dev/null | gpg -qs --batch'"
	smudge = "/usr/bin/env bash -c 'gpg -qd --batch | \
    while read line; \
    do echo \\"\$line\\" | tr \\"$\\" \\"\\n\\" | \
        gpg -qd --batch && OK=1 && break; \
    done; \
    if [ -z OK ]; then exit 1; fi'"
	required
EOF
