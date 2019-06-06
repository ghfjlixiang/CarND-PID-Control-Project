# CarND-PID-Control-Project

## Goals
### In this project I'll revisit the lake race track from the Behavioral Cloning Project. This time, however, I'll implement a PID controller in C++ to maneuver the vehicle around the track!.The simulator will provide you the cross track error (CTE) and the velocity (mph) in order to compute the appropriate steering angle.


## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/1972/view) individually and describe how I addressed each point in my implementation.  

---

### Implementation

#### 1. Complete the PID class

The function `Init` is design for initializing the pid controler's coefficients.I assigned proportional ,diffrential and integral coefficient respectively with three input parameters.

From the perspective of present, past and future, `p_error`, `i_error` and `d_error` are used to represent present error, past accumulated error and future error change trend respectively. In function `UpdateError`, I update current crosstrack err to `p_error`, it will be treated as a previous `cte`(short for crosstrack err) at the next moment. So I update the difference of the current crosscheck error and the previous one `p_error` to the `d_error`, and I update the sum of the crosstrack errors over time to `i_error`.

Simply put, PID is a linear combination of P, I, and D. Where P stands for the proportional term, the D for the differential term, and the I for integral term. Specifically for this project, steering is the combination of the proportion of the crosstrack error, and the proportion of the differential of the crosstrack error, and also the proportion of what's called the integral or the sum of all the crosstrack errors you ever observed.This is what I have done in function `TotalError`.

Notice the three coefficients that I used to calculate the total error,By doing that, I can implement a controller that varies the steering direction proportionally according to `Kp`, and differentially proportionally to `Kd`, and also integrally proportionally to `Ki`.So what I need to do next is to control the steering behavior of the car by adjusting these three parameters.

By the way , I add a function named `Twiddle` which was intended to be used to tune pid hyperparameters in project's simulation situation.Unfortunately, it did not achieve this goal in reality, and I was got stuck.I look forward to and thank the reviewer for giving me some practical suggestions and solutions in details.

#### 2. Initialize the pid variable
I changed the design of the main function so that I could easily manually adjust the PID parameters by entering different number and size of parameters on the command line.By calling the Init function of an instance of the pid class, I initialize the three pid coefficients with the parameters from the main function.Note that as you can see at the beginning of the main function, I have initialized differently for various types of input on the command line.

#### 3. Calculate steering value
After entering `onMessage` loop, firstlly I call the `UpdateError` function to update PID errors based on cte, then I call `TotalError` function to calculate the total error. Note that the steering value is between -1 and 1, so I need to convert the total error that may be out of range into the appropriate steering direction,rather than passing it directly to the simulator.

### Reflection

#### 1. Describe the effect each of the P, I, D components had in your implementation.
The P component is the propotional term.The steering in proportional to the crosstrack error.That is, steering angle is proportional by some factor of `Kp` to the crosstrack error, which means the larger the error, the more turnning towards the target direction,and vice versa.If I just use the p components, then I will get a p controller. When this is applied to a car, I observed that the car overshoots around the lane center and eventually drove off the road. The expectations presented here is consistent with those shown in the lecture.

The D component is the differential term. The D component is not normally used alone, but is used together with the P component to form the PD controller. In PD-control the steering is no just related to the crosstrack error by virtue of the gain parameter `Kp`, but also to the temporal derivative of the crosstrack error by virtue of the differential gain parameter `Kd`. The D component helps dampen the oscillations, what this means is that when the car has turned enough to reduce the crosstrack error, it won't just go shooting for the reference, but it will notice that it's already reducing the error.The error is becoming smaller over time. It will turns the front wheels in the opposite direction, and this will allow it to gracefully approach our target position.If PD controller is used without considering the system bias, we can expect it to wave along the center of the road.

The I component is the integral term. It makes steering in proportion to the sum of crosstrack errors over time. A problem that often occurs in robotics is called a "systematic bias". It goes as follows.When you ordered your car, you believed the front wheels were 100% aligned, but your mechanic may have made a mistake, and he aligned the wheels a little bit at an angle. So that's the reason why we need this term to compensate any bias in the steering or correct for drift. Based on PD controller, the integral component is introduced to obtain a PID controller where steering is proportional to the crosstrack errors before, and it's equally proportional to the differential of the crosstrack error, but now it's also proportional to what's called the integral or the sum of all the crosstrack errors you ever observed. 


#### 2. Describe how the final hyperparameters were chosen.
I choose the P, I and D coefficients through manual tuning. First I used the P controller and tried to set the P coefficient to different value and observed the effect of execution. If I modify `Kp` from 0.1 to 0.3, as we'll see, the faster the car changes direction, the greater the oscillation. No matter what value I choose, the upshot is that the car is going to oscillate badly in the center of the road and go out. So I ajust the D coefficient from 2 to 3.1. I foud that higher values of D
coefficient dampened the oscillations more. I also found that the car was able to drive around the track under a lot of pairs of P and D coefficients. Considering the tradeoff ,I choose 0.2 as the the P coefficient and 3 as the D coefficient finally. You can see the car successfully driving a lap around the track. Because the I coefficient doesn't do anything right now, so I have reason to believe that it didn't seem to have a material impact on driving around the simulator. To further explore the role of the I component, I chose a small coefficient 0.004 for it. As expected I don't see any significant effect of I component from the driving behavior of the car in the simulator. So I think there is any material bias in the steering, at least the I component has little impact on steering. But from debug print information, I can see that the sum of the cte for a cycle goes from a few hundred to a few dozen over time, by an order of magnitude.

---

### Discussion
#### 1.Maybe use another PID controller to control the speed
So far, all the hyperparameters are adjusting under a maximum speed of 30 MPH.when I increased the throttle, for example, let's increase the maximum speed to 50 or even to 100 MPH. I find that no matter how I adjust the parameters, it is very difficult to get the car to run a full lap along the center of the road. In particular, cars can easily run off the runway or hit the curb when passing a curve. So using another PID controller to control the speed is a good idea, from the perspective of the actual driving behavior of drivers, we can start to slow down when we pass a curve or find that we are too far from the center!
As a matter of fact, I don't know how to use more advanced and efficient methods. Maybe you can give me some detailed suggestions from the code review.I will look forward to it and thank you.