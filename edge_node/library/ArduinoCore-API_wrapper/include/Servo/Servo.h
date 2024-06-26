/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SERVO_H_
#define SERVO_H_

namespace arduino {

class Servo
{
public:
    Servo();
    virtual ~Servo()
    {
    }

    uint8_t attach(int pin);                    // attach the given pin to the next free channel, sets pinMode, returns channel number or INVALID_SERVO if failure
    uint8_t attach(int pin, int min, int max);  // as above but also sets min and max values for writes. 
    void detach();

    void write(int value);                      // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
    void writeMicroseconds(int value);          // Write pulse width in microseconds 
    
    int read() const;                           // returns current pulse width as an angle between 0 and 180 degrees
    int readMicroseconds() const;               // returns current pulse width in microseconds for this servo (was read_us() in first release)
    bool attached() const;                      // return true if this servo is attached, otherwise false 
private:
    int pin;                                    // the number of the pin that the servo is attached to
    int min;                                    // the pulse width, in microseconds, corresponding to the minimum (0 degree) angle on the servo (defaults to 544)
    int max;                                    // the pulse width, in microseconds, corresponding to the maximum (180 degree) angle on the servo (defaults to 2400)
};

}

#endif /* SERVO_H_ */

