# Train_Station_Scene
## Language 
This project was developed by C++ and OpenGL.

## Description
The simple graphic scene displays a model train(locomotive with three wagons) running around an oval track. The train runs continuously around the track. It will stop 5 seconds when it arrives at the train station. Then it will run through a tunnel. The whole scene is surrounded by mountains and grassland.  

![image](https://user-images.githubusercontent.com/79575944/162555012-393d6c2b-cdb4-4bfb-ac79-53c1b344efe0.png)

![image](https://user-images.githubusercontent.com/79575944/162555023-e0c36db9-821e-46f0-bfec-3b44c1a6ab4d.png)

When the train is far away from the road, the traffic light turns green, and the barriers’ arms are automatically lifted. It’s possible for vehicles to drive across the railway. 
When the train is coming towards the road, the traffic light turns red, and the barriers’ arms are shifted down. Vehicles cannot pass the railway at the moment.


## Control functions 

1. You can use Up/Down/Left/Right/Page Up/Page Down keys to control camera movements.  
2. Mouse click left side, height goes up. Mouse click right side, height goes down. 
3. Key ‘C’ for shifting three view modes.  


## Build and Run Command

g++ -Wall -o "assignment1" "assignment1.cpp" -lm -lGL -lGLU -lglut 
./assignment1 

## More details
please read my [report](https://github.com/ruizhang32/Train_Station_Scene/blob/main/COSC363%20Assignment1%20Report.pdf)
