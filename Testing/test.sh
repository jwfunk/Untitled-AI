#!/bin/sh
(cd Testing/UnitTests && make)
./Testing/UnitTests/neuronUnitTests
./Testing/UnitTests/networkUnitTests
(cd Testing/UnitTests && make clean)
