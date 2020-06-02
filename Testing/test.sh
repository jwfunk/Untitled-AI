#!/bin/sh
(cd Testing/UnitTests && make)
./Testing/UnitTests/neuronUnitTests
./Testing/UnitTests/networkUnitTests
./Testing/UnitTests/trainerUnitTests
(cd Testing/UnitTests && make clean)
