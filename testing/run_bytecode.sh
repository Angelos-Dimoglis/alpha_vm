#!/bin/bash

if [ -z $1 ]; then
    echo "give phase number"
    exit
fi

for test_file in testing/byte_code/b_"$1"/*.abc; do
    echo "-------------------------------------------------------------------"
    echo "TESTFILE: $(basename "$test_file")"
    echo
    ./alpha_vm.out -i "$test_file" # 1>/dev/null
    echo
done
