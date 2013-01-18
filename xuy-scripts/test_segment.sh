#!/bin/bash
cat test_segment.txt| awk -f segment.awk
cat hello.*
rm hello.*
