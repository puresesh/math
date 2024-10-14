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

#define L_DEF 0
#define R_DEF 500

#define Y_ZERO 50
#define X_ZERO 300


#define RENDER_FUNCTION (1000/(x+1))

struct availableColors {

  XColor customBlue;
  XColor customRed;

};

struct eXPoints {

  XPoint* points;
  int nOfPoints;

};

struct calculatedFunctionValues {

  float* xValues;
  float* yValues;
  int width;
  
};

XPoint* mapValuesToScalar(XPoint* points, int leftBorder, int rightBorder) {


  return points;

}

calculatedFunctionValues calculateFunction(int LDef, int RDef) {

  calculatedFunctionValues cFV;
  cFV.xValues = new float[GRAPH_WIDTH];
  cFV.yValues = new float[GRAPH_WIDTH];
  cFV.width = GRAPH_WIDTH;
  
  for(int x = 0; x < GRAPH_WIDTH; x++) {

    cFV.yValues[x] = RENDER_FUNCTION;
    cFV.xValues[x] = x;
    
  }
  
  
  return cFV;
}

XPoint* arrToXPArr(calculatedFunctionValues cFV, int length) {

  XPoint* points = new XPoint[length]; 
  
  for(int i = 0; i < length; i++){
    
    points[i].y = cFV.yValues[i]*-1 + Y_OFFSET + X_ZERO;
    points[i].x = cFV.xValues[i] + X_OFFSET + Y_ZERO;
    
    if(points[i].y > GRAPH_HEIGHT){
      points[i].y = GRAPH_HEIGHT;
    }
    
  }

  return points;
  
}

void initGraphPlane(Display* display, Window window, GC gc, availableColors avC) {

  XDrawLine(display, window, gc, 10, 10, GRAPH_WIDTH, 10);
  XDrawLine(display, window, gc, GRAPH_WIDTH, 10, GRAPH_WIDTH, GRAPH_HEIGHT);
  XDrawLine(display, window, gc, GRAPH_WIDTH, GRAPH_HEIGHT, 10, GRAPH_HEIGHT);
  XDrawLine(display, window, gc, 10, GRAPH_HEIGHT, 10,10);

  //x zero line  in blue
  XSetForeground(display, gc, avC.customBlue.pixel);
  XDrawLine(display, window, gc, X_OFFSET, X_ZERO+X_OFFSET, GRAPH_WIDTH, X_ZERO+X_OFFSET);

  for(int i = 0; i<GRAPH_WIDTH/10; i++) {

    XDrawLine(display, window, gc, i*10+X_OFFSET, X_ZERO-1+X_OFFSET, i*10+10, X_ZERO+1+X_OFFSET);  

  }

  //x zero line  in blue
  XSetForeground(display, gc, avC.customBlue.pixel);
  XDrawLine(display, window, gc, Y_OFFSET + Y_ZERO, Y_OFFSET, Y_ZERO + Y_OFFSET , GRAPH_HEIGHT);

  for(int i = 0; i<GRAPH_WIDTH/10; i++) {

    XDrawLine(display, window, gc, Y_ZERO-1+Y_OFFSET, i*10+Y_OFFSET, Y_ZERO+1+Y_OFFSET, i*10+10);  

  }
  
  
  XSetForeground(display, gc, avC.customRed.pixel);// red function


  
  //XDrawLine(display, window, gc, 10, 10, 50,50);  
  
}


void renderOnCanvas(Display* display, Window window, GC gc, availableColors avC) {

  initGraphPlane(display,window,gc,avC);
  calculatedFunctionValues testArray = calculateFunction(0,GRAPH_WIDTH);
  XPoint* points = arrToXPArr(testArray,GRAPH_WIDTH);
  XDrawLines(display, window, gc, points, GRAPH_WIDTH, CoordModeOrigin);


  
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

  renderOnCanvas(display,window,gc,avC);//start render Process
  
  XFlush(display); // send all data to X11 server

  sleep(50);

  XUnmapWindow(display, window);// close window, connection and free resources shoutout to ferox
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}
