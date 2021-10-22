/*
 * ColorRecognize.hpp
 *
 *  Created on: 2021年8月30日
 *      Author: huang
 */

#ifndef INC_COLORRECOGNIZE_HPP_
#define INC_COLORRECOGNIZE_HPP_

#include "main.h"

namespace MyApplications {

class ColorRecognize {
public:
    ColorRecognize();
    void imageBinarization(uint16_t wight, uint16_t height, uint16_t *src);
    virtual ~ColorRecognize();
};

} /* namespace MyApplications */

#endif /* INC_COLORRECOGNIZE_HPP_ */
