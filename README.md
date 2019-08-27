# WP3-cost-model
A theoretical cost model for accessing and transporting data, based on hardware characteristics.

## Model description
At present only a very basic cost model is implemented, in the BasicCostModel class. This class can be extended, and key functions overridden to implement more detailed models. The basic assumptions of the model are:
- Every access is either `FREE`, `BASIC`, or `EXPENSIVE` (following the model [LLVM](https://llvm.org/) uses for modelling instruction costs).
- Data types can be modelled as sequences of these access types.
- Other access patterns can be modelled as sequences of these access types on some underlying data type. Total cost is evaluated by 'unrolling' the access pattern and data type in to an overall sequence, then summing up the associated cost.
- Different devices can be modelled as having a particular integer `BASIC` access cost, and a particular integer `EXPENSIVE` access cost.
- Links between devices can be modelled as having a particular latency in nanoseconds, and a particular inverse bandwidth (in nanoseconds per bit).

The overall structure of the model can be viewed in the UML diagram.

## Dependencies

### CMake
Currently CMake version 3.13 or higher is required. A GNU Makefile may be added later.

### C++17
The model uses features of the C++17 standard, so a compiler which supports them (i.e. GCC 7+) is required.

### Lemon Graph Library
The cost model uses [Lemon](https://lemon.cs.elte.hu/trac/lemon) to store information about the network topology. It also provides graph algorithms, such as Dijkstra for the shortest route. The latest version is 1.3.1. It can be downloaded [here](https://lemon.cs.elte.hu/trac/lemon), and is most easily built and installed using cmake.

### Catch2
Catch2 is a single-header test framework for C++. It can be found on [GitHub](https://github.com/catchorg/Catch2). Again, it is most easily built and installed using cmake. If necessary the cost model can be built without tests.

## Acknowledgement
This project has received funding from the European Union's Horizon 2020 research and innovation programme under grant agreement No 801039.

<img src="https://europa.eu/european-union/sites/europaeu/files/docs/body/flag_yellow_high.jpg" alt="EU Logo" width="150" align="middle"/>
