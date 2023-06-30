# System Modeling

## System C
SystemC provides modeling constructs similar to those used in hardware description languages (HDLs) like Verilog or VHDL. These constructs allow users to build structural designs using modules, ports, and signals.

  * Modules: Modules are used to define the components of the design. They can be instantiated within other modules, allowing for hierarchical design.

  * Ports and Signals: Ports and signals facilitate communication of data between modules. Users declare ports and signals with specific data types, such as single bits, bit vectors, characters, integers, floating-point numbers, and vectors of integers. SystemC also supports four-state logic signals (0, 1, X, and Z).

  * Fixed-Point Numeric Type: SystemC introduces a fixed-point numeric type for modeling fixed-point numbers in digital signal processing applications. This type is not available in HDLs.

In HDLs like VHDL and Verilog, concurrent behaviors are modeled using processes or always blocks. Similarly, SystemC models concurrent behaviors using processes. A process in SystemC is an independent thread of control that resumes execution when specific events or signal changes occur and then suspends after performing an action.

However, there are some differences in the ability to specify conditions for resuming execution. In SystemC, a process can only be sensitive to changes in specific signals, and these signals must be pre-specified before simulation starts.

Since processes execute concurrently and may suspend and resume execution at user-specified points, SystemC process instances generally require their own independent execution stack. (An equivalent situation in the software world arises in multi-threaded applications— each thread requires its own execution stack.) Certain processes in SystemC which suspend at restricted points in their execution do not actually require an independent execution stack— these
processes types are termed `“SC_METHODs”`. Optimizing SystemC designs to take advantage of `SC_METHODs` provides dramatic simulation performance improvements when the number of process instances in a design is large.

Modeling hardware signals in software can be challenging due to their unique properties. Let's break down the challenges and how SystemC addresses them:

1. **Initialization with "X"**: In SystemC 1.0, the `sc_logic` and `sc_lv` data types allow hardware signals and registers to be initialized as "X" when the simulation starts. This is helpful for detecting reset problems using X propagation techniques during simulation.

2. **Multiple Drivers**: Hardware signals can have multiple drivers, and resolving the value based on each driving value is necessary. SystemC provides resolved logic signals to handle this modeling. The resolved value is automatically computed whenever any of the driving values change. For example, if a signal is driven with 1 and Z, the resolved value should be 1. If driven with 1 and 0, the resolved value should be X.

3. **Delayed Value Change**: Hardware signals do not immediately change their output value when assigned a new value, both in simulation and in the real world. There is always a small delay until the new value assigned to a signal becomes available to other processes in the design. This delay is crucial for modeling hardware behavior accurately, allowing operations like register swapping on a clock edge. In contrast, software variables cannot swap values without using a temporary variable.

SystemC 1.0, similar to VHDL and Verilog, supports delayed signal assignments and delta cycles to address the challenges of modeling hardware signals effectively. Delta cycles represent tiny time steps within the simulation that do not increase the user-visible time. Multiple delta steps can occur at a given time point. When a signal assignment occurs, other processes do not see the newly assigned value until the next delta step. Processes sensitive to the signal then resume execution if the signal value has changed compared to its previous value.

By incorporating these features, SystemC allows for accurate and realistic modeling of hardware signals and enables designers to simulate and analyze complex digital designs effectively.

## Objectives
One of the primary goals of the SystemC 1.1 release is to enable system level modeling, modeling of systems above the RTL level of abstraction, including systems which might be implemented in software or hardware or some combination of the two. One of the challenges in providing a system level design language is that there is a wide range of design models of computation, design abstraction levels, and design methodologies used in system design. To address this challenge in SystemC 1.1, a small but very general purpose modeling foundation has been added to the language. On top of this language foundation we can then add the more specific models of computation, design libraries, modeling guidelines, and design methodologies which are required for system design.

The small, general purpose modeling foundation in SystemC is termed the core language and is the central component of the SystemC 1.1 standard. 

Other components of the SystemC 1.1 standard include elementary library models which
build on the core language (e.g. timers, FIFOs, signals, etc.) and which are widely applicable.

It is recognized that many different models of computation and design methodologies may be used in conjunction with SystemC. For this reason the design libraries and models needed to support these specific design methodologies are considered to be separate from the SystemC core language standard

## Communication and Synchronization in System C
SystemC provides useful language features for system-level modeling. The structural description features like modules and ports, along with the extensive set of data types and concurrency expression using processes, are beneficial for system design. However, the hardware signal, which is the communication and synchronization mechanism in SystemC 1.0, may not be sufficient for general system-level modeling.

In SystemC 1.1, a new set of features is introduced to address this limitation and provide generalized modeling of communication and synchronization. These features are channels, interfaces, and events.

* **Channels**: Channels serve as containers for communication and synchronization. They can implement one or more interfaces, which define a set of access methods. However, the interface itself does not provide the implementation.

* **Interfaces**: Interfaces specify the access methods that should be implemented within a channel. They enable communication between different modules or processes.

* **Events**: Events are flexible and low-level synchronization primitives. They can be used to construct other forms of synchronization mechanisms.

By introducing channels, interfaces, and events, SystemC 1.1 allows designers to model a wide range of communication and synchronization scenarios encountered in system designs. Examples include hardware signals, various types of queues (FIFO, LIFO, message queues), semaphores, memories, and buses (both RTL and transaction-based models).

These enhanced features enable system-level designers to effectively represent and simulate complex communication and synchronization patterns, facilitating the modeling of diverse system designs and architectures.

-------------