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
 * @file num_generator.hpp
 * @author Salvador Z
 * @version 1.0
 * @brief File for generator module to inject values into the pipe
 *
 */

#ifndef NUM_GENERATOR_HPP_
#define NUM_GENERATOR_HPP_

// Includes
#include <systemc.h>

struct num_generator : sc_module {
  sc_in<bool>    clk;
  sc_out<double> out1;
  sc_out<double> out2;

  SC_CTOR(num_generator) {
    SC_METHOD(generate);
    dont_initialize(); // prevent initialization for SC_METHODs and SC_THREADs
    sensitive << clk.pos();
  }

  void generate() {
    static double a = 200.5;
    static double b = 100.5;

    a -= (rand() % 10);
    b -= (rand() % 10);

    out1.write(a);
    out2.write(b);
  }
};

#endif /* NUM_GENERATOR_HPP_ */