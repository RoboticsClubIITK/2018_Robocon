# Robocon 2018 Project

This repository contains code written for the Robocon 2018 Project

## Build

For building the ROS packages (namely process_image and arduino):

1. Create a catkin worspace following the guidelines given [here](http://wiki.ros.org/catkin/Tutorials/create_a_workspace)
```bash
mkdir -p ~/catkin_ws/src
catkin_init_workspace
cd ~/catkin_ws/
catkin_make
```

2. Clone this repository to your catkin workspace
```bash
cd ~/catkin_ws/src
git clone https://github.com/AUV-IITK/auv2016.git
```

3. Run the [build.sh](utils/build.sh) script
```bash
~/catkin_ws/src/auv2016/build.sh
```


