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

/**
 * @file conveyor.hpp
 * @author Salvador Z
 * @version 1.0
 * @brief File for conveyor module
 *
 */

#ifndef CONVEYOR_HPP_
#define CONVEYOR_HPP_

// Includes
#include <systemc.h>

#define INSTANCE_NAME_STRING_LEN 256

// -----------------------------
// baggage scanner constants
// -----------------------------
#define BARCODE_SCANNER_REPORT_RATE_VARIANCE_SECS 2 // 0 to n-1 sec

#define MAX_NUMBER_BAGS_IN_SYSTEM 16
#define BAG_COUNT_HYSTERESIS      4

// -----------------------------
// conveyor segment constants
// -----------------------------
#define NUM_CONVEYOR_SEGMENTS 1

#define CONVEYOR_REPORT_RATE_MS 10      // 10 ms
#define DESIRED_CONVEYOR_SPEED  0.5     // m/s (meters per sec)
#define DIST_PER_ENCODER_COUNT  0.00001 // m (0.01 mm per count)
#define ENCODER_COUNT_INCREMENT \
  (DESIRED_CONVEYOR_SPEED * CONVEYOR_REPORT_RATE_MS * 0.001) / DIST_PER_ENCODER_COUNT
// (0.5 * 10 * 0.001) / 0.00001 = 500 (5mm per 10ms)

#define TEMPERATURE_MEAN     45 // degrees C
#define TEMPERATURE_VARIANCE 4

#define VIBRATION_MEAN     12 // in mils
#define VIBRATION_VARIANCE 10

// -----------------------------
// control system constants
// -----------------------------
#define CONTROL_SYSTEM_RATE_US 1 // 1 us

// control packet defines
#define CONTROL_PKT_MSG_TURN_OFF 0
#define CONTROL_PKT_MSG_TURN_ON  1

/**
 * @brief Class scanner_sts_packet
 *
 */
class scanner_sts_packet {
private:
  sc_time timestamp;
  int     bag_id;
  int     pox_bag;

public:
  scanner_sts_packet() : pox_bag(0) {
    /*
        timestamp = sc_time_stamp();
        bag_id    = 0;
*/
  }

  void set_timestamp(sc_time ts) {
    timestamp = ts;
  }

  sc_time get_timestamp() {
    return timestamp;
  }

  void set_bag_id(int bag_id) {
    this->bag_id = bag_id;
  }

  int get_bag_id() {
    return bag_id;
  }

  int get_pox_bag() {
    return pox_bag;
  }

  void print() {
    // print packet contents
    cout << "scanner_sts_packet::print(): Timestamp  ="
         << " " << timestamp << endl;
    printf("                        Baggage ID = %d\n", bag_id);
  }
};

/**
 * @brief Class conveyor_sts_packet
 *
 */
class conveyor_sts_packet {
private:
  sc_time      timestamp;
  int          my_id;       // segment ID
  unsigned int current_cnt; // current encoder count
  int          temperature; // degrees C
  int          vibration;   // in mils, 1 inch = 1000 mils
public:
  conveyor_sts_packet() : my_id(0), current_cnt(0), temperature(0), vibration(0) {
    /*
        timestamp   = sc_time_stamp();
        my_id       = 0;
        current_cnt = 0;
        temperature = 0;
        vibration   = 0;
*/
  }

  void set_timestamp(sc_time ts) {
    timestamp = ts;
  }

  sc_time get_timestamp() {
    return timestamp;
  }

  void set_id(int id) {
    my_id = id;
  }

  int get_id() {
    return my_id;
  }

  void set_current_cnt(unsigned int cnt) {
    current_cnt = cnt;
  }

  unsigned int get_current_cnt() {
    return current_cnt;
  }

  void set_temperature(int temp) {
    temperature = temp;
  }

  int get_temperature() {
    return temperature;
  }

  void set_vibration(int vib) {
    vibration = vib;
  }

  int get_vibration() {
    return vibration;
  }

  void print() {
    // print packet contents
    cout << "conveyor_sts_packet::print(): Timestamp  ="
         << " " << timestamp << endl;
    printf("                          Segment ID = %d\n", my_id);
    printf("                       Current Count = %d\n", current_cnt);
    printf("                       Temperature  = %d\n", temperature);
    printf("                       Vibration    = %d\n", vibration);
  }
};

/**
 * @brief Class control_packet
 *
 */
class control_packet {
private:
  sc_time timestamp;
  int     msg;
  int     data;

public:
  control_packet() : msg(0), data(0) {
    /*
        timestamp = sc_time_stamp();
        msg       = 0;
        data      = 0;
*/
  }

  void set_timestamp(sc_time ts) {
    timestamp = ts;
  }

  sc_time get_timestamp() {
    return timestamp;
  }

  void set_msg(int msg) {
    this->msg = msg;
  }

  int get_msg() {
    return msg;
  }

  void set_data(int data) {
    this->data = data;
  }

  int get_data() {
    return data;
  }

  void print() {
    // print packet contents
    cout << "control_packet::print(): Timestamp  ="
         << " " << timestamp << endl;
    printf("                      Message     = %d\n", msg);
    printf("                      Data        = %d\n", data);
  }
};
#endif /* CONVEYOR_HPP_ */