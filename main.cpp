#include <X11/Xlib.h>
#include <unistd.h> 
#include <stdio.h>  
#include <iostream>
#include <cmath>

#define GRAPH_WIDTH 1000
#define GRAPH_HEIGHT 500
#define RESOLUTION 1

#define X_OFFSET 10
#define Y_OFFSET 10

struct availableColors {

  XColor customBlue;
  XColor customRed;

};

float* initTestArray() {

  float* testArr = new float[100];
  for(int x = 0; x < 100; x++) {

    testArr[x] = pow((x-50),2);

  }

  return testArr;
}

XPoint* arrToXPArr(float* intArr, int length) {

  XPoint* points = new XPoint[length]; 
  
  for(int i = 0; i < length; i++){

    points[i].y = intArr[i] + GRAPH_HEIGHT/2;
    points[i].x = (RESOLUTION*i) + X_OFFSET;
    
  }

  return points;
  
}

void initGraphPlane(Display* display, Window window, GC gc, availableColors avC) {


  XDrawLine(display, window, gc, 10, 10, GRAPH_WIDTH, 10);
  XDrawLine(display, window, gc, GRAPH_WIDTH, 10, GRAPH_WIDTH, GRAPH_HEIGHT);
  XDrawLine(display, window, gc, GRAPH_WIDTH, GRAPH_HEIGHT, 10, GRAPH_HEIGHT);
  XDrawLine(display, window, gc, 10, GRAPH_HEIGHT, 10,10);
  
  XSetForeground(display, gc, avC.customBlue.pixel);// blue zero

  XDrawLine(display, window, gc, 10, GRAPH_HEIGHT/2, GRAPH_WIDTH, GRAPH_HEIGHT/2);
  
  XSetForeground(display, gc, avC.customRed.pixel);// red function

  //XDrawLine(display, window, gc, 10, 10, 50,50);  
  
}

int main(void) {

  Display* display = XOpenDisplay(NULL);

  if (!display)
  {
    fprintf(stderr, "error:"
      "	Can't open connection to display server."
      " Probably X server is not started.\n");
    return 1;
  }

  availableColors avC;
  avC.customBlue.red = 0;
  avC.customBlue.green = 0; 
  avC.customBlue.blue = 65535;

  
  avC.customRed.red = 65535;
  avC.customRed.green = 0; 
  avC.customRed.blue = 0;
    

  int blackColor = BlackPixel(display, DefaultScreen(display));
  int whiteColor = WhitePixel(display, DefaultScreen(display));
  
  Window window = XCreateSimpleWindow(
				      display,
				      DefaultRootWindow(display),// parent window
				      0,//x
				      0,//y
				      720,//width
				      480,//height
				      2, //border width         
				      whiteColor,//border color
				      whiteColor); // background color

  XSelectInput(display, window, StructureNotifyMask);

  XMapWindow(display, window);                    
  
  GC gc = XCreateGC(display, window, 0, NULL); // graphic context for render?

  XSetForeground(display, gc, blackColor);

  XStoreName(display, window, "math renderer");
  
  for(;;) {
    XEvent e;
    XNextEvent(display, &e);
    if (e.type == MapNotify)
      break;
  }
  


  initGraphPlane(display,window,gc,avC);
  float* testArray = initTestArray();
  XPoint* points = arrToXPArr(testArray,100);
  XDrawLines(display, window, gc, points, 100, CoordModeOrigin);

  for(int i = 0; i < 50; i++) {
    
    std::cout << "y:" << points[i].y << "\n";
    std::cout << "x:" << points[i].x << "\n";
    std::cout << "int:" << testArray[i] << "\n";
    std::cout << "---\n";
    
  }
  
  XFlush(display);                                // send all data to X11 server

  sleep(50);

  XUnmapWindow(display, window);                  // close window, connection and free resources shoutout to ferox
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}
