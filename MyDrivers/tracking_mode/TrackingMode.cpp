/*
 * TrackingMode.cpp
 *
 *  Created on: Nov 4, 2021
 *      Author: huang
 */

#include <tracking_mode/TrackingMode.hpp>

namespace MyDrivers {

TrackingMode::TrackingMode() {
    // TODO Auto-generated constructor stub

}

void TrackingMode::TrackingTask(void)
{
    if ( (GPIO_PIN_RESET == HAL_GPIO_ReadPin(TRACKING_CENTRE_GPIO_Port, TRACKING_CENTRE_Pin)) || 
         (GPIO_PIN_RESET == HAL_GPIO_ReadPin(TRACKING_LEFT_GPIO_Port, TRACKING_LEFT_Pin)) || 
         (GPIO_PIN_RESET == HAL_GPIO_ReadPin(TRACKING_RIGHT_GPIO_Port, TRACKING_RIGHT_Pin))
         ) {

         }
}

TrackingModeDir TrackingMode::direction(void)
{
    if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(TRACKING_CENTRE_GPIO_Port, TRACKING_CENTRE_Pin)) {
        if (GPIO_PIN_SET == HAL_GPIO_ReadPin(TRACKING_LEFT_GPIO_Port, TRACKING_LEFT_Pin)) {
            return TrackingModeDir::left;
        } else if (GPIO_PIN_SET == HAL_GPIO_ReadPin(TRACKING_RIGHT_GPIO_Port, TRACKING_RIGHT_Pin)) {
            return TrackingModeDir::right;
        }
    }
    return TrackingModeDir::none;
}

bool TrackingMode::isTracking(void)
{

}

TrackingMode::~TrackingMode() {
    // TODO Auto-generated destructor stub
}

} /* namespace MyDrivers */
