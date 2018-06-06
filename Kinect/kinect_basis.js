import librarytests.*;
import org.openkinect.*;
import org.openkinect.processing.*;

Kinect kinect;

void setup() {
    size(640, 480);
    kinect = new Kinect(this);
    kinect.start();
    kinect.enableRGB(true);
    //kinect.enableIR(true);    //grayscale, IR camera data
    //kinect.enableDepth(true);  //for depth camera data

    //For RGB:
    PImage img = kinect.getVideoImage();
    w = img.width;
    h = img.height;
}

void draw() {
    background(0);
    kinect.processDepthImage(false)   //no image processing of depth camera for speed
    int[] depth = kinect.getRawDepth();

    int leastDepth = -1;

    for(int i = 0;i<depth.length;i++)
    {
        if(i==0 || depth[i] < depth[leastDepth])
            leastDepth = i;
    }

    image(kinect.getVideoImage(), 0, 0);//display RGB Image

    int leastY = leastDepth/w;
    int leastX = leastDepth%w;

    fill(255, 0, 0);
    ellipse(leastX, leastY, 20, 20);
}

void stop() {
    kinect.quit();
    super.stop();
}

//depth to meters
float[] depthLookUp = new float[2048];
for (int i = 0; i < depthLookUp.length; i++) {
  depthLookUp[i] = rawDepthToMeters(i);
}

float rawDepthToMeters(int depthValue) {
  if (depthValue < 2047) {
    return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
  }
  return 0.0f;
}
