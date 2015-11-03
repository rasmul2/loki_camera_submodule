The ideal installation would calculate the motion curve that follows the sphere most closely
and would then use that to animate a character/object on a seperate screen in front of the 
person. 

Currently, the camera output shows the process of achieving the motion tracking of the sphere:

the top left image is the webcam converted to black and white
the top right is the difference in the webcame image frame by frame
the bottom left shows all the pixels that pass a certain range of the frames max brightness
the middle left then multiplies the frame difference by the brightness to get a track of motion closest to the camera
the middle right then changes everything over a certain value to black and the rest to white to show just the tracked motion cloud
finally, the sphere takes an average of all of those cloud points and changes its position based on that 