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
 * @file stage1.hpp
 * @author Salvador Z
 * @version 1.0
 * @brief File for stage1 process
 *
 */

#ifndef STAGE1_HPP_
#define STAGE1_HPP_

// Includes
#include <systemc.h>

// Inheritance from sc_module
struct stage1 : sc_module {
  sc_in<double>  in1;  // Input port
  sc_in<double>  in2;  // Input port
  sc_out<double> sum;  // Output port
  sc_out<double> diff; // Output port

  sc_in<bool> clk; // clock

  void addsub() { // Process
    double a;
    double b;

    a = in1.read(); // Read input port
    b = in2.read(); // Read input port

    sum.write(a + b);  // Write output port
    diff.write(a - b); // Write output port
  }

  // Constructor
  SC_CTOR(stage1) {
    SC_METHOD(addsub);      // Declare addsub as SC_METHOD
    dont_initialize();      // prevent initialization for SC_METHODs and SC_THREADs
    sensitive << clk.pos(); // make it sensitive to positive clock edge
  }
};

#endif /* STAGE1_HPP_ */