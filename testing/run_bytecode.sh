#!/bin/bash

for test_file in testing/byte_code/b_"$1"/*.abc; do
    echo "$test_file"
    ./alpha_vm.out -i "$test_file"
done
