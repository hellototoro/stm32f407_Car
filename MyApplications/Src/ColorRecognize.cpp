/*
 * ColorRecognize.cpp
 *
 *  Created on: 2021年8月30日
 *      Author: huang
 */

#include <Inc/ColorRecognize.hpp>

namespace MyApplications {

ColorRecognize::ColorRecognize() {
    // TODO Auto-generated constructor stub

}

void ColorRecognize::imageBinarization(uint16_t wight, uint16_t height, uint16_t *src)
{
    for (uint16_t i = 0; i < wight; i++) {
        for (uint16_t j = 0; i < height; j++) {
            src[i*height+j] = (((src[i*height+j]&0xF800)>>8)*19595+((src[i*height+j]&0x07E0)>>3)*38469 +((src[i*height+j]&0x1f)<<3)*7472)>>16;
        }
    }
}

ColorRecognize::~ColorRecognize() {
    // TODO Auto-generated destructor stub
}

} /* namespace MyApplications */
