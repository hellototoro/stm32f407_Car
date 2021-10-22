/*
 * ultrasonic.hpp
 *
 *  Created on: Sep 20, 2021
 *      Author: huang
 */

#ifndef COMPONENTS_ULTRASONIC_ULTRASONIC_HPP_
#define COMPONENTS_ULTRASONIC_ULTRASONIC_HPP_

class ultrasonic {
public:
	ultrasonic();
	void init();
	int getDistance(void);
	void ultrasonic_time_isr(void);
	virtual ~ultrasonic();

private:
	int distance;
private:
	void trig(void);
};

#endif /* COMPONENTS_ULTRASONIC_ULTRASONIC_HPP_ */
