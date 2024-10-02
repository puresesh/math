#include <X11/Xlib.h>
#include <unistd.h> 
#include <stdio.h>  

#define GRAPH_WIDTH 1000
#define GRAPH_HEIGHT 500

struct availableColors {

  XColor customBlue;

};

XPoint* arrToXPArr(int intArr[]) {

  XPoint* points[(sizeof(intArr)/sizeof(intArr[0]))] = {}; 
  
  for(int element : intArr){

    

  }

  return *points;
  
}

void initGraphPlane(Display* display, Window window, GC gc, availableColors avC) {


  XDrawLine(display, window, gc, 10, 10, GRAPH_WIDTH, 10);
  XDrawLine(display, window, gc, GRAPH_WIDTH, 10, GRAPH_WIDTH, GRAPH_HEIGHT);
  XDrawLine(display, window, gc, GRAPH_WIDTH, GRAPH_HEIGHT, 10, GRAPH_HEIGHT);
  XDrawLine(display, window, gc, 10, GRAPH_HEIGHT, 10,10);

  XSetForeground(display, gc, avC.customBlue.pixel);

  XDrawLine(display, window, gc, 10, 10, 50,50);  
  
  XDrawLines(display, window, gc, points, 8, CoordModePrevious);
  
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
  
  XFlush(display);                                // send all data to X11 server

  sleep(2);

  XUnmapWindow(display, window);                  // close window, connection and free resources
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}
