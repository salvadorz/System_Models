/*******************************************************************************
 * Copyright (C) 2023 by Salvador Z                                            *
 *                                                                             *
 * This file is part of SYS_MODEL                                              *
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
 * @file pipe.cpp
 * @author Salvador Z
 * @date 27 Jun 2023
 * @brief File for show a pipe processes in SystemC (RTL like)
 *
 */
#include "num_generator.hpp"
#include "stage1.hpp"
#include "stage2.hpp"
#include "stage3.hpp"
#include "test_probe_display.hpp"
#include <systemc.h>

int sc_main(int, char *[]) {

  // Signals
  sc_signal<double> s_in1;
  sc_signal<double> s_in2;
  sc_signal<double> s_sum;
  sc_signal<double> s_diff;
  sc_signal<double> s_prod;
  sc_signal<double> s_quot;
  sc_signal<double> s_powr;
  // Clock
  sc_signal<bool> s_clk;

  num_generator tst_generator("tst_generator"); // Instance of generator
  tst_generator(s_clk, s_in1, s_in2);           // Binding ports

  stage1 stg1("stage1");
  stg1.in1(s_in1);
  stg1.in2(s_in2);
  stg1.sum(s_sum);
  stg1.diff(s_diff);
  stg1.clk(s_clk);

  stage2 stg2("stage2");
  stg2(s_sum, s_diff, s_prod, s_quot, s_clk);

  stage3 stg3("stage3");               // instance of `stage3' module
  stg3(s_prod, s_quot, s_powr, s_clk); // Positional port binding

  test_probe_display disp("display"); // instance of `display' module
  disp(s_powr, s_clk);                // Positional port binding

  sc_start(0, SC_NS); // Initialize simulation
  for (int i = 0; i < 50; i++) {
    s_clk.write(1);
    sc_start(10, SC_NS);
    s_clk.write(0);
    sc_start(10, SC_NS);
  }
  return 0;
}
