/*******************************************************************************
 * Copyright (C) 2023 by Salvador Z                                            *
 *                                                                             *
 * This file is part of SYS_MODELS                                                *
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
 * @file test_probe_display.hpp
 * @author Salvador Z
 * @version 1.0
 * @brief File for test_probe_display module
 *
 */

#ifndef TEST_PROBE_DISPLAY_HPP_
#define TEST_PROBE_DISPLAY_HPP_

// Includes
#include <systemc.h>

struct test_probe_display : sc_module {
  sc_in<double> in;
  sc_in<bool>   clk;

  void print_test() {
    cout << "test_probe_display: " << in.read() << endl;
  }

  SC_CTOR(test_probe_display) {
    SC_METHOD(print_test);
    dont_initialize(); // prevent initialization for SC_METHODs and SC_THREADs
    sensitive << clk.pos();
  }
};

#endif /* TEST_PROBE_DISPLAY_HPP_ */