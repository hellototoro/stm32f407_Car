/*
 * Key.hpp
 *
 *  Created on: Nov 4, 2021
 *      Author: huang
 */

#ifndef KEY_KEY_HPP_
#define KEY_KEY_HPP_

#include "main.h"

namespace MyDrivers {
enum class key_id {
    key0 = 0,
    key1,
    keyNull
};

class Key {
public:
    Key(int key_number);
    void keyTask(void);
    bool keyIsPress(void);
    virtual ~Key();

private:
    GPIO_TypeDef* port;
    uint16_t pin;
    int key_number;
    bool status;
    key_id key;
};

} /* namespace MyDrivers */

#endif /* KEY_KEY_HPP_ */
