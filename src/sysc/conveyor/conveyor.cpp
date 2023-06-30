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
#include "HashMap.hpp"
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
  int                 samples_available;
  control_packet     *control_pkt_ptr;
  scanner_sts_packet *scanner_pkt_ptr;

public:
  sc_port<sc_fifo_out_if<scanner_sts_packet *> > out;
  sc_port<sc_fifo_in_if<control_packet *> >      in;

  SC_HAS_PROCESS(scanner);

  scanner(sc_module_name name) : sc_module(name) {
    // process declaration
    SC_THREAD(scanner_thread);

    bag_id  = 0;
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
      samples_available = in->num_available();

      if (samples_available != 0) {
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
        out->write(scanner_pkt_ptr);
      }
    } // end while
  }   // end scanner_thread
};

/**
 * @brief Control_system module
 * This module controls the conveyor belt. It communicates with the scanner and the conveyor through sc_fifos
 * @param Input scanner_sts_packet from scanner
 * @param Input conveyor_sts_packet from conveyor system
 * @param Output control_packet to scanner system
 * @param Output control_packet to conveyor system
 */
class Control_System : public sc_module {
private:
  int index;
  int bag_id;
  int bag_count;
  int scanner_running;
  int samples_available;
  int control_system_loop_count;

  // Control packets
  control_packet *control_pkt_ptr;

  // Received status packets
  scanner_sts_packet  *scanner_pkt_ptr;
  conveyor_sts_packet *conveyor_pkt_ptr;

  // arrays that hold the pointers to the sc_ports
  sc_port<sc_fifo_in_if<conveyor_sts_packet *> > *seg_in_port[NUM_CONVEYOR_SEGMENTS];
  sc_port<sc_fifo_out_if<control_packet *> >     *seg_out_port[NUM_CONVEYOR_SEGMENTS];

  ds::HashMap bag_hash; // hash table to keep track of BagID's

public:
  // port list
  sc_port<sc_fifo_in_if<scanner_sts_packet *> > scanner_in;
  sc_port<sc_fifo_out_if<control_packet *> >    scanner_out;

  sc_port<sc_fifo_in_if<conveyor_sts_packet *> > seg0_in;
  sc_port<sc_fifo_out_if<control_packet *> >     seg0_out;

  SC_HAS_PROCESS(Control_System);

  Control_System(sc_module_name name, int csl_count) : sc_module(name), control_system_loop_count(csl_count), bag_hash(true, 128) {
    // process declaration
    SC_THREAD(control_system_thread);

    // initialize variables
    index             = 0;
    bag_id            = 0;
    bag_count         = 0;
    scanner_running   = 0;
    samples_available = 0;

    // initialize ports
    seg_in_port[0]  = &seg0_in;
    seg_out_port[0] = &seg0_out;
    //bag_hash        = new ds::HashMap(true, 128);
  }

  void control_system_thread() {

    control_pkt_ptr = new control_packet();

    control_pkt_ptr->set_timestamp(sc_time_stamp());
    control_pkt_ptr->set_msg(CONTROL_PKT_MSG_TURN_ON);
    control_pkt_ptr->set_data(0);

    scanner_out->write(control_pkt_ptr);
    scanner_running = CONTROL_PKT_MSG_TURN_ON;

    // Cycle through the conveyor segments
    for (index = 0; index < NUM_CONVEYOR_SEGMENTS; index++) {
      control_pkt_ptr = new control_packet();

      control_pkt_ptr->set_timestamp(sc_time_stamp());
      control_pkt_ptr->set_msg(CONTROL_PKT_MSG_TURN_ON);
      control_pkt_ptr->set_data(0);

      (*seg_out_port[index])->write(control_pkt_ptr); // send it to conveyor segment
    }

    while (true) {
      wait(CONTROL_SYSTEM_RATE_US, SC_US);

      // Check for received scanner status packets
      samples_available = scanner_in->num_available();
      if (samples_available != 0) {
        scanner_in->read(scanner_pkt_ptr);

        // save the scanner_pkt_ptr in the bag_hash
        bag_hash.Insert(scanner_pkt_ptr->get_bag_id(), scanner_pkt_ptr);

        ++bag_count;
        if ((scanner_running == CONTROL_PKT_MSG_TURN_ON) && (bag_count >= MAX_NUMBER_BAGS_IN_SYSTEM)) {

          // send a turn off command to the scanner
          control_pkt_ptr = new control_packet();

          control_pkt_ptr->set_timestamp(sc_time_stamp());
          control_pkt_ptr->set_msg(CONTROL_PKT_MSG_TURN_OFF);
          control_pkt_ptr->set_data(0);

          scanner_out->write(control_pkt_ptr);

          scanner_running = CONTROL_PKT_MSG_TURN_OFF;
        }
      } // end if samples_available

      // Check for received conveyor status packets
      samples_available = seg0_in->num_available();
      if (samples_available != 0) {
        seg0_in->read(conveyor_pkt_ptr);

        conveyor_pkt_ptr->print();

        // Check for alarm condition

        // Update bag postiiion and delete bags delivered
        bag_id = bag_hash.GetFirstKey();
        if (-1 != bag_id) {
          scanner_pkt_ptr = (scanner_sts_packet *)bag_hash.GetValue(bag_id);
        }

        if ((CONTROL_PKT_MSG_TURN_OFF == scanner_running) &&
            (bag_count < (MAX_NUMBER_BAGS_IN_SYSTEM - BAG_COUNT_HYSTERESIS))) {

          // send a turn on command to the scanner
          control_pkt_ptr = new control_packet();

          control_pkt_ptr->set_timestamp(sc_time_stamp());
          control_pkt_ptr->set_msg(CONTROL_PKT_MSG_TURN_ON);
          control_pkt_ptr->set_data(0);

          scanner_out->write(control_pkt_ptr);

          scanner_running = CONTROL_PKT_MSG_TURN_ON;
        }
        delete conveyor_pkt_ptr;
      }

      --control_system_loop_count;
      if (0 == control_system_loop_count) {
        break;
      }
    } // end while
      // stop
    cout.flush();
    sc_stop();
  } // end control_system_thread
  ~Control_System() {
  }
};

int sc_main(int, char *[]) {

  sc_start();
  return 0;
}