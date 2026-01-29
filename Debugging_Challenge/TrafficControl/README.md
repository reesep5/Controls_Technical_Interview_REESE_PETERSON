# Traffic Light Problem (1 hour)

There's a busy traffic intersection outside your building. The roads at the intersection are horizontal street and vertical street. 
You are an engineer with the department of transportation and you're trying to come up with a new traffic light pattern that 
prevents cars from crashing in the intersection but also minimizes traffic jams. You ask a junior engineer to come up with a new 
traffic pattern and simulate the traffic flow at the intersection for two minutes or until 40 cars pass through. They work on it 
but then come to you saying they're stuck and they can't get the simulation to work. The traffic lights are exhibiting erratic 
behavior, crashes aren't reported properly, and their output results of cumulative time waiting look off. They ask you to review 
their code and help them make it work.

Your goal:
- Find all the errors in the code to get the simulation working properly
- Give feedback on how to make the code better next time

You will additionally be assessed on:
- The quality of your code

A few things to note about where the junior engineer is 98% certain there are NOT errors:
- There should be no issues with the 'advanceLane' function or the functions that start with 'draw'

A few other things to note about the simulation:
- Cars waiting to go into the intersection can only move into the intersection if the light is green.
- Cars already in the intersection can move out of the intersection even when the light is red or yellow.
- A crash should occur if a car on horizontal street and a car on vertical street are in the intersection at the same time.
- Cars IN the intersection are denoted with an 'X'. Cars on either side of the intersection are denoted with a '+'. 
  This will make more sense once you try running the program.
- Only one car from each lane can be IN the intersection at once. For example, if two cars are waiting in the eastbound 
  lane to cross the intersection, they cannot both move into the intersection at the same time. The first car in the 
  lane will move into the intersection. Then, the second car will move into the intersection as the first car leaves. 

To build the project, type the following in your bash terminal:
gcc traffic_light_problem.c -o traffic_light_problem

To run the project, type the following in your bash terminal:
./traffic_light_problem.exe

Candidate Action Items:
- Find the errors in the code to make the simulation work as the junior engineer intended
- Make any other improvements to the code as you see fit
- Answer the following questions:
  - What would you tell the junior engineer about ways that they could improve their code in the future? (1-3 sentences)
    - The first major way to improve their code is commenting out what they want the code to do, as there were spots, like the broken conditionals of the current color, where having a comment to read out what is supposed to happen would help to spot the errors. Another way to improve their code is increasing attention to detail, as there were multiple spots where either something was misspelled, had the wrong return type, or had a variable from the wrong struct. Finally, it is important to minimize code duplication, as that can significantly increase the chance of errors popping up when discrepancies are incidentally added.
  - How else would you change this code to make it better so you can build on it in the future?(1-10 sentences).
    - One item to change would be the logic in `advanceLane()` and `checkForCrashes()` to potentially expand to multiple cars being in the intersection. Another change would be to expand the `intersection_s` struct to include a turning lane. This would also necessitate editing `lane_of_cars_s` to include both a list of cars turning and cars going straight, as well as expanding `checkForCrashes()` to set a crash condition if a car turning hits a car going straight. Finally, I would also add different termination conditions to allow for not only a different number of total cars, but also for the number of cars that can pass through each lane. Changing the number of cars allowed to pass through each lane would potentially pair well with the popularity variable, as you would expect higher throughput for a more popular lane.

Don't forget; we are interested in both your solution and your thought process.

Good Luck!