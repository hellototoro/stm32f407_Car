/*
 * TrackingMode.hpp
 *
 *  Created on: Nov 4, 2021
 *      Author: huang
 */

#ifndef TRACKING_MODE_TRACKINGMODE_HPP_
#define TRACKING_MODE_TRACKINGMODE_HPP_

#include "main.h"

namespace MyDrivers {

enum class TrackingModeDir {
    left,
    right,
    none
};

class TrackingMode {
public:

public:
    TrackingMode();
    TrackingModeDir direction(void);
    void TrackingTask(void);
    bool isTracking(void);
    virtual ~TrackingMode();

private:

};

} /* namespace MyDrivers */

#endif /* TRACKING_MODE_TRACKINGMODE_HPP_ */
