#!/bin/sh
(cd Testing/UnitTests && make)
./Testing/UnitTests/test
(cd Testing/UnitTests && make clean)
