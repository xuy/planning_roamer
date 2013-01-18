#!/usr/bin/awk -f
#
# This script extracts data from the result dump of search and writes the data to files.
#
# The data dump looks like this:
# >>>>>>>  name
# [data segment 1]
# ------
# [data segment 2]
# <<<<<<<
#
# >>>>>>
# ....
# <<<<<<

# This script will store [data segment 1] to name.1, and [data segment 2] to name.2 .

/^>+/ { inside = 1; segment = 1; file_prefix = $2; file_name = file_prefix "." segment; }
/^<+/ { inside = 0; }
/^-+/ { segment = segment + 1; file_name = file_prefix "." segment; }
/^[^-><]+.*/  { if (inside==1) print $0 >> file_name; }
/^$/  { if (inside==1) print $0 >> file_name; }  # Keep the empty lines.
