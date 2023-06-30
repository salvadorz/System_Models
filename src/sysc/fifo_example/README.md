This is a basic example to show how to use SystemC.
can use it as a template to create more complex models.

# Fifo Example

This Example doesn't burn any time but it implements the notion of processes.

The FIFO which stores ten characters. The FIFO will have blocking read and write interfaces such that characters are always reliably delivered.

## Design  
```diagram 
| +-----------+     +-----------+   +-----------+  |
| |           |---->|           |   |           |  |
| | producer  |     | FIFO      |   | consumer  |  |
| |           |     |           |-->|           |  |
| +-----------+     +-----------+   +-----------+  |
``` 

First, we specify the separate read and write interfaces to the FIFO. Let’s say for illustration that the write interface allows the FIFO to be reset, while the read interface allows a non-blocking query of the number of characters currently in the FIFO.

```cpp
class write_if : public sc_interface {
public:
    virtual void write(char) = 0;
    virtual void reset() = 0;
};
class read_if : public sc_interface {
public:
    virtual void read(char &) = 0;
    virtual int num_available() = 0;
};
```
> An interface is an abstract base class in C++ that inherits from sc_interface. Interfaces specify a set of access methods for a channel but provide no implementation of those methods.

Next, specify a high-level model for the FIFO. The FIFO uses C++ multiple inheritance to inherit both the read and write interfaces, and also the properties of channels from `sc_channel`.

```cpp
class fifo : public sc_channel, public write_if, public read_if {
public:
  fifo() : num_elements(0), first(0) {}
  void write(char c) {
    if (num_elements == max_elements) wait(read_event);
    data[(first + num_elements) % max_elements] = c;
    ++num_elements;
    write_event.notify();
  }
  void read(char &c) {
    if (num_elements == 0) wait(write_event);
    c = data[first];
    --num_elements;
    first = (first + 1) % max_elements;
    read_event.notify();
  }
  void reset() {
    num_elements = first = 0;
  }
  int num_available() {
    return num_elements;
  }

private:
  enum e { max_elements = 10 }; // just a constant in class scope
  char     data[max_elements];
  int      num_elements, first;
  sc_event write_event, read_event;
};
```

The channel provides the implementation for the access methods specified in the read and write interfaces. First, it has a constructor that sets the number of available characters in the FIFO to zero. Next, it has an implementation of the write method which waits if necessary until there is space available in the FIFO, then adds the new character into the proper place within the data array. (The FIFO functionality is implemented using a circular buffer within the data array). Finally the write method notifies the `write_event`, which will then resume the execution of
any read requests which are waiting for new input.

The read method implementation looks similar. It waits if necessary until there is data within the FIFO, then reads the first item and removes it from the data array by incrementing the *first* index. Finally it notifies the `read_event`, which will then resume the execution of any write requests which are waiting for space to be made available.

Recall that processes in SystemC have their own independent threads of execution. When a producer process invokes the write method of the FIFO above, the producer process may need to have its execution suspended until there is space available in the FIFO. The `wait()` call within the write method achieves this suspension of the producer process. Importantly, this suspension is handled completely by the channel and is not visible to the producer process. If the producer process is suspended, it will be resumed the next time the consumer process reads a character from the FIFO, which will result in a notification of the read_event, signaling that space is now available within the FIFO.

**SystemC 1.0** also provided a `wait()` call, but the call could not take any arguments because a process was always sensitive to a fixed set of signals. In the example above we see a significant extension beyond SystemC 1.0: The `wait()` call can now have arguments specified (possibly many), and processes can be sensitive to both events and signals. The ability to provide arguments to the wait() call is termed dynamic sensitivity. This term is used because as processes execute they can dynamically select the set of events which will cause the process to be resumed when they `wait()`. Not only can processes wait on specific events in SystemC, but they can also wait for a specific amount of time. For example, a process can call `wait(200, SC_NS)`, which would cause the process to suspend execution for exactly `200` nanoseconds, or it could call `wait(200, SC_NS, e)`, which would cause it to wait until event `e` is notified or a 200 nanosecond timeout occurs.

Events are the fundamental synchronization primitives within SystemC, and they have several important differences from signals, which were the only synchronization mechanism available in SystemC 1.0. Unlike signals, an event does not have a type and does not transmit a value— events only transfer control from one thread of execution to another. Also unlike signals,an event notification always causes sensitive processes to be resumed, while an assignment to a signal only causes sensitive processes to be resumed if the new signal value is different from the previous value. Finally, event notifications can be specified to occur immediately, in one delta step in the future, or at a specific time in the future. Signal assignments always occur exactly one delta step in the future.

### Communication Modeling Example
To complete the communication modeling example, let’s now show how the FIFO can be used as a communication channel between a producer and a consumer module:

```cpp
class producer : public sc_module {
public:
  sc_port<write_if> out; // the producer's output port
  SC_CTOR(producer)      // the module constructor
  {
    SC_THREAD(main); // start the producer process
  }
  void main() // the producer process
  {
    char c;
    while (true) {
      ... out->write(c);     // write c into the fifo
      if (...) out->reset(); // reset the fifo
    }
  }
};
class consumer : public sc_module {
public:
  sc_port<read_if> in; // the consumer's input port
  SC_CTOR(consumer)    // the module constructor
  {
    SC_THREAD(main); // start the consumer process
  }
  void main() // the consumer process
  {
    char c;
    while (true) {
      in->read(c);                      // read c from the fifo
      if (in->num_available() > 5) ...; // perhaps speed up processing ...
    }
  }
};

class top : sc_module {
public:
  fifo      fifo_inst;     // a fifo instance
  producer *producer_inst; // a producer instance
  consumer *consumer_inst; // a consumer instance
  SC_CTOR(top)             // the module constructor
  {
    producer_inst = new producer("Producer1");
    // bind the fifo to the producer's output port
    producer_inst->out(fifo_inst);
    consumer_inst = new consumer("Consumer1");
    // bind the fifo to the consumer's input port
    consumer_inst->in(fifo_inst);
  }
};
```

It is important to note that the output port of the producer has a template type argument which is specified in this case to be the write interface (`write_if`) of the FIFO, while similarly the consumer input port has template type argument which is the read interface of the FIFO.
Because of this, only those interface methods specified within the write interface are available for use within the producer, while only those interface methods specified in the read interface are available in the consumer. Thus, the consumer could not call the `fifo.reset()` method as the producer does, for example.
In this example the FIFO instance is bound to the producer’s output port and the consumer’s input port. However, the producer’s output port is not aware that it is bound to a FIFO. It actually only knows that it is bound to an object which implements the write interface `write_if`. Similarly the consumer’s input is not aware that it is bound to a FIFO, it only knows that it is bound to an object which implements the read interface `read_if`. This hiding of the channel implementation is intentional and is a key ingredient in providing flexible communication modeling and communication refinement within SystemC.

Here are several examples of how the hiding of the channel implementation mentioned above aids communication modeling and communication refinement:

1. Imagine that the designer wished to experiment with a slightly different functional specification of this design which used a FIFO that discarded characters when it was full, rather than suspending execution of the producer process. All the designer would need to do is to write the channel implementation for this new FIFO which reuses the existing read and write interfaces. Then the only change needed to the original design would be the substitution of the new FIFO for the original one within the top module.
   
2. Imagine that the designer wished to refine this design to an implementation running on an RTOS. The producer and consumer processes would be implemented using independent threads running on the RTOS, while the original FIFO instance would be replaced with a new one in which the read and write methods directly access the RTOS’s built-in queuing services. Again, the only change needed in the original design is the substitution of the original FIFO with the new one in the top module.

3. Imagine that the designer wished to refine this design to a custom hardware implementation which uses a hardware FIFO. Again, a new FIFO channel is written to replace the existing one in the top module. Channels in SystemC can contain other channels and modules, just as modules can contain multiple child modules. In this case we write a new FIFO channel which instantiates the hardware FIFO within it, and which contains the hardware signals necessary to interface with the hardware FIFO. We then implement the read and write interface methods within this channel to drive the hardware signals with the protocol required to cause data to be properly loaded into and unloaded from the hardware FIFO. This then allows us to simulate the design while using a model of the actual hardware FIFO. As a final step in the communication refinement process, the code which implements the write and read protocols to access the hardware FIFO within the channel can be inlined into the producer and consumer, respectively, enabling this code to be synthesized and optimized with the producer and consumer code. (The SystemC language does not automate this inlining step, but it does provide the constructs needed so that tools can perform this task.)

One final note on this example. For simplicity the FIFO channel which is presented above is only able to store characters. In practice channels such as this would be written using C++ templates to allow the data type to be specified at the time that the channel is instantiated. Using this technique, a single FIFO channel could store any C++ data type, including user-defined datatypes. SystemC fully supports this template-based design technique.