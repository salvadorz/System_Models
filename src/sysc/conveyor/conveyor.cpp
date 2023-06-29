/*******************************************************************************
 * Copyright (C) 2023 by Salvador Z                                            *
 *                                                                             *
 * This file is part of SYSTEM_MODELS                                          *
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

#include "conveyor.hpp"
#include <systemc.h>

/**
 * @brief Scanner module
 * This module simulates a human scanning bags and placing them onto the
 * conveyor belt. It sends a scanner status packet to the control system using a fifo
 * @param Input control_packet from control system
 * @param Output scanner_sts_packet to control system
 * 
 */
class scanner : public sc_module {
private:
  int                 bag_id;
  int                 running;
  int                 var_delay;
  int                 slots_available;
  control_packet     *control_pkt_ptr;
  scanner_sts_packet *scanner_pkt_ptr;

public:
  sc_port<sc_fifo_out_if<scanner_sts_packet *> > out;
  sc_port<sc_fifo_in_if<control_packet *> >      in;

  SC_HAS_PROCESS(scanner);

  scanner(sc_module_name name) : sc_module(name) {
    // process declaration
    SC_THREAD(scanner_thread);

    bag_id = 0;
    running = 0; // off initially
  }

  /**
   * @brief scanner_thread
   *
   * @return void
   */
  void scanner_thread() {

    while (true) {
      /* This simulates the amount of time between bags scans
       * and placing onto the conveyor belt.
       */
      var_delay = (rand() % BARCODE_SCANNER_REPORT_RATE_VARIANCE_SECS) + 1;
      wait(var_delay, SC_SEC);

      /**
       * Check for received control packets
       */
      slots_available = in->num_available();

      if (slots_available != 0) {
        in->read(control_pkt_ptr);

        if (control_pkt_ptr->get_msg() == CONTROL_PKT_MSG_TURN_ON) {
          running = CONTROL_PKT_MSG_TURN_ON;

          printf("\nInfo: scanner_inst control_pkt received: \n");
          printf("Info: Turning On \n");
        } else if (control_pkt_ptr->get_msg() == CONTROL_PKT_MSG_TURN_OFF) {
          running = CONTROL_PKT_MSG_TURN_OFF;

          printf("\nInfo: scanner_inst control_pkt received: \n");
          printf("Info: Turning Off \n");
        }
        control_pkt_ptr->print();

        delete control_pkt_ptr; // free memo
      }

      if (CONTROL_PKT_MSG_TURN_ON == running) {
        scanner_pkt_ptr = new scanner_sts_packet();

        scanner_pkt_ptr->set_timestamp(sc_time_stamp());
        // generate a bag_ID, and set it in the packet
        scanner_pkt_ptr->set_bag_id(++bag_id);
        if (0 > bag_id) {
          bag_id = 0;
        }
      }
    } // end while
  }   // end scanner_thread
};



  int sc_main(int, char *[]) {

    sc_start();
    return 0;
  }