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

//****************************************************************************
// Include(s)
//****************************************************************************
#include "traffic_light_problem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************************************************
// Variable(s)
//****************************************************************************
struct intersection_s myIntersection = {0};

//****************************************************************************
// Private Function Prototype(s):
//****************************************************************************
static void initIntersection(void);
static traffic_light_colors_t setHorizontalTrafficLight(struct intersection_s intersection);
static traffic_light_colors_t setVerticalTrafficLight(struct intersection_s intersection);
static void advanceLane(traffic_light_colors_t trafficColor, struct lane_of_cars_s * lane);
static void drawIntersection(struct intersection_s intersection);
static void drawUpperVerticalRoad(traffic_light_colors_t trafficColor, struct lane_of_cars_s northboundCars, struct lane_of_cars_s southboundCars);
static void drawLowerVerticalRoad(traffic_light_colors_t trafficColor, struct lane_of_cars_s northboundCars, struct lane_of_cars_s southboundCars);
static void drawWestboundLane(traffic_light_colors_t trafficColor, struct lane_of_cars_s westboundCars);
static void drawEastboundLane(traffic_light_colors_t trafficColor, struct lane_of_cars_s eastboundCars);
static void delay(int16_t ms);
static int8_t checkForCrashes(void);

//****************************************************************************
// Public function(s):
//****************************************************************************

//Main function to run the traffic simulation
int main(void)
{
	//Seed my random number generator
	srand(time(0));

	//Initialize the intersection
	initIntersection();

	//Run traffic through the intersection for a set period of time
	for(int8_t i = 0; i < 120; i++)
	{
		//Update the traffic lights
		myIntersection.horizontalTrafficColor = setHorizontalTrafficLight(myIntersection);
		myIntersection.verticalTrafficColor = setVerticalTrafficLight(myIntersection);

		//Advance the lanes if possible
		advanceLane(myIntersection.horizontalTrafficColor, &myIntersection.westboundCars);
		advanceLane(myIntersection.horizontalTrafficColor, &myIntersection.eastboundCars);
		advanceLane(myIntersection.verticalTrafficColor, &myIntersection.northboundCars);
		advanceLane(myIntersection.verticalTrafficColor, &myIntersection.southboundCars);

		//Draw the intersection
		system("clear");
		drawIntersection(myIntersection);
		fflush(stdout);
		delay(1000);

		//Check to make sure no cars have crashed
		if(checkForCrashes() == 1)
		{
			printf("FAIL: Car crash!\n");
			drawIntersection(myIntersection);
			fflush(stdout);
			return 0;
		}

		//Check if all the cars have left the lanes
		int8_t totalCarsThatHaveLeft = myIntersection.northboundCars.carsThatHaveLeft + myIntersection.southboundCars.carsThatHaveLeft + myIntersection.eastboundCars.carsThatHaveLeft + myIntersection.westboundCars.carsThatHaveLeft;
		if(totalCarsThatHaveLeft >= 40)
		{
			int totalWaitTime = myIntersection.northboundCars.timeWaiting + myIntersection.southboundCars.timeWaiting + myIntersection.westboundCars.timeWaiting + myIntersection.eastboundCars.timeWaiting;
			printf("SUCCESS: You got all the cars through in %i seconds! The total wait time was: %i seconds!\n", i, totalWaitTime);
			return 0;
		}
	}

	//If the animation time's out, let them know their score.
	int8_t totalCarsThatMadeIt = myIntersection.northboundCars.carsThatHaveLeft + myIntersection.southboundCars.carsThatHaveLeft + myIntersection.westboundCars.carsThatHaveLeft + myIntersection.eastboundCars.carsThatHaveLeft;
	int totalWaitTime = myIntersection.northboundCars.timeWaiting + myIntersection.southboundCars.timeWaiting + myIntersection.westboundCars.timeWaiting + myIntersection.eastboundCars.timeWaiting;
	printf("FAIL: Traffic Jam! You ran out of time. You got %i/40 cars through in 120 seconds. The total wait time was: %i seconds.\n", totalCarsThatMadeIt, totalWaitTime);
	return 0;
}

//****************************************************************************
// Private function(s):
//****************************************************************************

static void initIntersection(void)
{
	myIntersection.horizontalTrafficColor = RED;
	myIntersection.verticalTrafficColor = RED;

	myIntersection.eastboundCars.popularity = 3;
	myIntersection.westboundCars.popularity = 5;
	myIntersection.northboundCars.popularity = 2;
	myIntersection.southboundCars.popularity = 4;
}

/**
 * Set the traffic light for the specific through path
 * 
 * @param throughPathLightColor current color of the through path light
 * @param crossPathLightColor current color of the cross path light
 * @param throughPathCarsWaiting number of cars waiting on the through path
 * @param crossPathCarsWaiting number of cars waiting on the cross path
 * 
 * @return New color of the through path light
 */
static traffic_light_colors_t setTrafficLight(traffic_light_colors_t throughPathLightColor, traffic_light_colors_t crossPathLightColor, int8_t throughPathCarsWaiting, int8_t crossPathCarsWaiting)
{
	static int8_t t = 0;
	traffic_light_colors_t newColor = throughPathLightColor;

	switch(throughPathLightColor)
	{
		// If both lights are red and there are more cars waiting on the through path, turn the through path light green
		case RED:
			if((throughPathCarsWaiting >= crossPathCarsWaiting) && crossPathLightColor == RED)
			{
				newColor = GREEN;
				t = 0;
			}
			break;
		
		// If the light is green and either the number of cars waiting on the cross path exceeds the number of cars or the light has been green for more than 10 s,
		// switch the light to yellow
		case GREEN:
			t++;
			if((throughPathCarsWaiting < crossPathCarsWaiting) || t > 10)
			{
				newColor = YELLOW;
				t = 0;
			}
			break;
		// If the light is yellow for more than a second to let any last car through, set the light to red
		case YELLOW:
			t++;
			if(t > 1)
			{
				newColor = RED;
				t = 0;
			}
			break;

		default:
			newColor = RED;
			t = 0;	
	}

	return newColor;
}

/**
 * Set the horizontal traffic light color
 * 
 * @param intersection the intersection struct
 * @return The new color of the horizontal traffic light
 */
static traffic_light_colors_t setHorizontalTrafficLight(struct intersection_s intersection)
{
	int8_t horizontalCarsWaiting = intersection.eastboundCars.carsWaitingAtIntersection + intersection.westboundCars.carsWaitingAtIntersection;
	int8_t verticalCarsWaiting = intersection.northboundCars.carsWaitingAtIntersection + intersection.southboundCars.carsWaitingAtIntersection;
	return setTrafficLight(intersection.horizontalTrafficColor, intersection.verticalTrafficColor, horizontalCarsWaiting, verticalCarsWaiting);
}

/**
 * Set the vertical traffic light color
 * 
 * @param intersection the intersection struct
 * @return The new color of the vertical traffic light
 */
static traffic_light_colors_t setVerticalTrafficLight(struct intersection_s intersection)
{
	int8_t horizontalCarsWaiting = intersection.eastboundCars.carsWaitingAtIntersection + intersection.westboundCars.carsWaitingAtIntersection;
	int8_t verticalCarsWaiting = intersection.northboundCars.carsWaitingAtIntersection + intersection.southboundCars.carsWaitingAtIntersection;
	return setTrafficLight(intersection.verticalTrafficColor, intersection.horizontalTrafficColor, verticalCarsWaiting, horizontalCarsWaiting);
}

/**
 * Move the cars in the lane based on the traffic light
 * 
 * @param trafficColor color of the traffic light for the lane
 * @param lane specific lane to move the cars
 */
static void advanceLane(traffic_light_colors_t trafficColor, struct lane_of_cars_s * lane)
{
	//Move any cars on the leaving side of the intersection into oblivion
	//but mark them in the total lane count
	if(lane->carsLeavingIntersection > 0)
	{
		lane->carsLeavingIntersection--;
		lane->carsThatHaveLeft++;
	}

	//Move any cars in the intersection to the leaving side of the intersection
	if(lane->carsInIntersection > 0)
	{
		lane->carsInIntersection--;
		lane->carsLeavingIntersection++;
	}

	//Move a waiting car into the intersection if possible
	if((trafficColor == GREEN) && (lane->carsWaitingAtIntersection > 0))
	{
		lane->carsWaitingAtIntersection--;
		lane->carsInIntersection++;
	}

	//Add cars to the lane randomly (but not more than 10 cars can ever be added to the lane ever)
	if(lane->carsWaitingAtIntersection + lane->carsInIntersection + lane->carsLeavingIntersection + lane->carsThatHaveLeft < 10)
	{
		int addCarToLane = rand();
		if((addCarToLane % lane->popularity) == 0)
		{
			lane->carsWaitingAtIntersection++;	
		}
	}

	//Keep track of how long cars have cumulatively waited at this part of the intersection
	lane->timeWaiting += lane->carsWaitingAtIntersection;
}

static void drawIntersection(struct intersection_s intersection)
{
	drawUpperVerticalRoad(intersection.verticalTrafficColor,intersection.northboundCars,intersection.southboundCars);
	drawWestboundLane(intersection.horizontalTrafficColor,intersection.westboundCars);
	printf("----------          ---------- \n");
	drawEastboundLane(intersection.horizontalTrafficColor,intersection.eastboundCars);
	drawLowerVerticalRoad(intersection.verticalTrafficColor,intersection.northboundCars,intersection.southboundCars);
}

static void drawUpperVerticalRoad(traffic_light_colors_t trafficColor, struct lane_of_cars_s northboundCars, struct lane_of_cars_s southboundCars)
{
	char * southboundCarPlaceholder;

	//Show southbound cars waiting at the intersection and northbound cars leaving the intersection
	for(int8_t i = 0; i < 10; i++)
	{
		if(i >= 10 - southboundCars.carsWaitingAtIntersection)
		{
			southboundCarPlaceholder  = "+";
		}
		else{southboundCarPlaceholder  = " ";}

		if(i == 9)
		{
			printf("          |  %s ! %c |          \n",southboundCarPlaceholder,trafficColor);
		}
		else if (i == 5)
		{
			if(northboundCars.carsLeavingIntersection > 0)
			{
				printf("          |  %s ! + |          \n",southboundCarPlaceholder);
			}
			else{printf("          |  %s !   |          \n",southboundCarPlaceholder);}
		}
		else{printf("          |  %s !   |          \n",southboundCarPlaceholder);}
	}

	//Show cars in the middle of the intersection
	if(southboundCars.carsInIntersection > 0)
	{
		printf("__________   X      __________ \n");
	}
	else
	{
		printf("__________          __________ \n");
	}
}

static void drawLowerVerticalRoad(traffic_light_colors_t trafficColor, struct lane_of_cars_s northboundCars, struct lane_of_cars_s southboundCars)
{
	char * northboundCarPlaceholder;

	//Show cars in the middle of the intersection
	if(northboundCars.carsInIntersection > 0)
	{
		printf("__________       X  __________ \n");
	}
	else
	{
		printf("__________          __________ \n");
	}

	//Show northbound cars waiting at the intersection and southbound cars leaving the intersection
	for(int8_t i = 0; i < 10; i++)
	{
		if(i < northboundCars.carsWaitingAtIntersection)
		{
			northboundCarPlaceholder  = "+";
		}
		else{northboundCarPlaceholder  = " ";}

		if(i == 0)
		{
			printf("          |  %c ! %s |          \n",trafficColor,northboundCarPlaceholder);
		}
		else if (i == 5)
		{
			if(southboundCars.carsLeavingIntersection > 0)
			{
				printf("          |  + ! %s |          \n",northboundCarPlaceholder);
			}
			else{printf("          |    ! %s |          \n",northboundCarPlaceholder);}
		}
		else{printf("          |    ! %s |          \n",northboundCarPlaceholder);}
	}
}

static void drawWestboundLane(traffic_light_colors_t trafficColor, struct lane_of_cars_s westboundCars)
{
	//Show cars moving away from the intersection
	//Note: Because we don't really care about these cars, I will only display one of them
	//even if there is more than one car moving away from the intersection
	if(westboundCars.carsLeavingIntersection > 0)
	{
		printf("    +    %c", trafficColor);
	}
	else
	{
		printf("         %c", trafficColor);
	}

	//Show cars in the middle of the intersection
	if(westboundCars.carsInIntersection > 0)
	{
		printf("       X  ");
	}
	else
	{
		printf("          ");
	}

	//Show cars waiting at the intersection
	for(int8_t i = 0; i < 10; i++)
	{
		if(i < westboundCars.carsWaitingAtIntersection)
		{
			printf("+");
		}
		else{printf(" ");}
	}
	printf("\n");
}

static void drawEastboundLane(traffic_light_colors_t trafficColor, struct lane_of_cars_s eastboundCars)
{

	//Show cars waiting at the intersection
	for(int8_t i = 0; i < 10; i++)
	{
		if(i >= 10 - eastboundCars.carsWaitingAtIntersection)
		{
			printf("+");
		}
		else{printf(" ");}
	}

	//Show cars in the middle of the intersection
	if(eastboundCars.carsInIntersection > 0)
	{
		printf("  X       ");
	}
	else
	{
		printf("          ");
	}

	//Show cars moving away from the intersection
	//Note: Because we don't really care about these cars, I will only display one of them
	//even if there is more than one car moving away from the intersection
	if(eastboundCars.carsLeavingIntersection > 0)
	{
		printf("%c   +     \n", trafficColor);
	}
	else
	{
		printf("%c         \n", trafficColor);
	}
}

static void delay(int16_t ms)
{
    clock_t startTime = clock();
 
    while(clock() < startTime + ms);
}

/**
 * Check if there is a crash
 */
static int8_t checkForCrashes(void)
{
	// Check if there are any cars in the intersection travelling vertically and horizontally in the intersection
	int8_t isHorizontalCarInIntersection = (myIntersection.westboundCars.carsInIntersection | myIntersection.eastboundCars.carsInIntersection);
	int8_t isVerticalCarInIntersection = (myIntersection.northboundCars.carsInIntersection | myIntersection.southboundCars.carsInIntersection);

	// If both directions have cars travelling through the intersection, there has been a crash. Otherwise, no crash
	return isHorizontalCarInIntersection && isVerticalCarInIntersection;
}
