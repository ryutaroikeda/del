#!/bin/bash
# Exit after first failure
set -e
echo "Running tests"
test_count=0
function set_up {
  make
  make install
  rm -r /tmp/trash
}
function tear_down {
  echo "Ran $test_count tests"
}
function do_test {
  let "test_count += 1"
  echo "Running $1"
  $1
}
function test_1 {
  touch junk
  trash junk 
  ! test -f junk
}
function test_2 {
  mkdir junk
  trash junk
  ! test -d junk
}
function test_3 {
  mkdir junk
  touch junk/junk
  trash junk/junk
  ! test -f junk/junk
  trash junk
  ! test -d junk
}
function test_4 {
  touch junk1
  touch junk2
  trash junk1 junk2
  ! test -f junk1
  ! test -f junk2
}
set_up
do_test test_1
do_test test_2
do_test test_3
do_test test_4
tear_down
