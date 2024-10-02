#include <X11/Xlib.h>
#include <unistd.h> 
#include <stdio.h>  

#define GRAPH_WIDTH 1000
#define GRAPH_HEIGHT 500
#define RESOLUTION 1

struct availableColors {

  XColor customBlue;

};

int* initTestArray() {

  int* testArr = new int[8];
  for(int i = 0; i < 8; i++) {

    testArr[i] = 2^i;

  }

  return testArr;
}

XPoint* arrToXPArr(int* intArr, int length) {

  XPoint* points = new XPoint[length]; 
  
  for(int i = 0; i < length; i++){

    points[i].y = intArr[i];
    points[i].x = RESOLUTION*i;
    
  }

  return points;
  
}

void initGraphPlane(Display* display, Window window, GC gc, availableColors avC) {


  XDrawLine(display, window, gc, 10, 10, GRAPH_WIDTH, 10);
  XDrawLine(display, window, gc, GRAPH_WIDTH, 10, GRAPH_WIDTH, GRAPH_HEIGHT);
  XDrawLine(display, window, gc, GRAPH_WIDTH, GRAPH_HEIGHT, 10, GRAPH_HEIGHT);
  XDrawLine(display, window, gc, 10, GRAPH_HEIGHT, 10,10);

  XSetForeground(display, gc, avC.customBlue.pixel);

  XDrawLine(display, window, gc, 10, 10, 50,50);  
  
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
  avC.customBlue.red = 65535;
  avC.customBlue.green = 0; 
  avC.customBlue.blue = 65535;
    

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
  
  for(;;) {
    XEvent e;
    XNextEvent(display, &e);
    if (e.type == MapNotify)
      break;
  }
  
  sleep(1);

  initGraphPlane(display,window,gc,avC);
  int* testArray = initTestArray();
  XPoint* points = arrToXPArr(testArray,8);
  XDrawLines(display, window, gc, points, 8, CoordModePrevious);
  
  
  XFlush(display);                                // send all data to X11 server

  sleep(2);

  XUnmapWindow(display, window);                  // close window, connection and free resources
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}
