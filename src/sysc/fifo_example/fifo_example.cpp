/*******************************************************************************
 * Copyright (C) 2023 by Salvador Z                                            *
 *                                                                             *
 * This file is part of SYS_MODELS                                             *
 *                                                                             *
 *   Permission is hereby granted, free of charge, to any person obtaining a   *
 *   copy of this software and associated documentation files (the Software)   *
 *   to deal in the Software without restriction including without limitation  *
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,  *
 *   and/or sell copies ot the Software, and to permit persons to whom the     *
 *   Software is furnished to do so, subject to the following conditions:      *
 *                                                                             *
 *   The above copyright notice and this permission notice shall be included   *
 *   in all copies or substantial portions of the Software.                    *
 *                                                                             *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   *
 *   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARANTIES OF MERCHANTABILITY *
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL   *
 *   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR      *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,     *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE        *
 *   OR OTHER DEALINGS IN THE SOFTWARE.                                        *
 ******************************************************************************/

/**
 * @file fifo_example.cpp
 * @author Salvador Z
 * @date 27 Jun 2023
 * @brief File for fifo_example
 *
 */

#include <systemc.h> /*System C*/

class write_if : virtual public sc_interface {
public:
  virtual void write(char) = 0;
  virtual void reset()     = 0;
};

class read_if : virtual public sc_interface {
public:
  virtual void read(char &) = 0;
  virtual int  size()       = 0;
};

class fifo : public sc_channel, public write_if, public read_if {
private:
  enum el { max = 10 };
  char     data[max];
  int      elements_num, head;
  sc_event wr_event, rd_event;

public:
  fifo(sc_module_name name) : sc_channel(name), elements_num(0), head(0) {}

  void write(char c) {
    if (elements_num == max) {
      wait(rd_event);
    }
    data[(head + elements_num) % max] = c;
    ++elements_num;
    wr_event.notify();
  }

  void read(char &c) {
    if (elements_num == 0) {
      wait(wr_event);
    }
    c = data[head];
    --elements_num;
    head = (head + 1) % max;
    rd_event.notify();
  }

  void reset() {
    elements_num = head = 0;
  }

  int size() {
    return elements_num;
  }
};

class producer : public sc_module {
public:
  sc_port<write_if> out;

  SC_HAS_PROCESS(producer);

  producer(sc_module_name name) : sc_module(name) {
    SC_THREAD(producer_main);
  }

  void producer_main() {
    out->reset();
    const char *str = "Que estás como quieres de pies a cabeza y no se midió La naturaleza con tanta belleza "
                      "que en ti reunió !\n ";

    while (*str)
      out->write(*str++);
  }
};

class consumer : public sc_module {
public:
  sc_port<read_if> in;

  SC_HAS_PROCESS(consumer);

  consumer(sc_module_name name) : sc_module(name) {
    SC_THREAD(consumer_main);
  }

  void consumer_main() {
    char c;
    cout << endl << endl;

    while (true) {
      in->read(c);
      cout << c << flush;

      if (in->size() == 1) cout << "<1>" << flush;
      if (in->size() == 9) cout << "<9>" << flush;
    }
  }
};

class top : public sc_module {
public:
  fifo     *fifo_inst;
  producer *producer_inst;
  consumer *consumer_inst;

  top(sc_module_name name) : sc_module(name) {
    fifo_inst = new fifo("FIFO");

    producer_inst = new producer("Producer");
    producer_inst->out(*fifo_inst);

    consumer_inst = new consumer("Consumer");
    consumer_inst->in(*fifo_inst);
  }
};

int sc_main(int, char *[]) {
  top top_inst("Top_instance");
  sc_start();
  return 0;
}