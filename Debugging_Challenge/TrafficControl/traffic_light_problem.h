/****************************************************************************
	[Project] Controls Technical Interview
*****************************************************************************
	[Lead developer] Rachel Harris, rharris at dephy dot com.
	[Contributors] 
*****************************************************************************
	[This file] traffic_light_problem: Changes the traffic light colors to 
	move cars through an intersection while minimizing wait time at red lights. 
****************************************************************************
	[Created] 2022-11-04 | rharris | Initial creation
****************************************************************************/

#ifndef INC_TRAFFIC_LIGHT_PROBLEM_H
#define INC_TRAFFIC_LIGHT_PROBLEM_H

//****************************************************************************
// Include(s)
//****************************************************************************
#include <stdint.h>

//****************************************************************************
// Definition(s)
//****************************************************************************
typedef enum
{
	RED = 'R',
	GREEN = 'G',
	YELLOW = 'Y'
} traffic_light_colors_t;

//****************************************************************************
// Structures(s)
//****************************************************************************
struct lane_of_cars_s
{
	int8_t carsWaitingAtIntersection;
	int8_t carsInIntersection;
	int8_t carsLeavingIntersection;
	int8_t carsThatHaveLeft;
	
	int8_t popularity;
	int8_t timeWaiting;
};

struct intersection_s
{
	traffic_light_colors_t horizontalTrafficColor;
	traffic_light_colors_t verticalTrafficColor;

	struct lane_of_cars_s eastboundCars;
	struct lane_of_cars_s westboundCars;
	struct lane_of_cars_s northboundCars;
	struct lane_of_cars_s southboundCars;
};


#endif //INC_TRAFFIC_LIGHT_PROBLEM_H
