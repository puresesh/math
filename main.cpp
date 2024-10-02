#include <X11/Xlib.h>
#include <unistd.h> // sleep()
#include <stdio.h>  // fprintf()

int main(void)
{
  Display* display = XOpenDisplay(NULL);          // open connection

  if (!display)
  {
    fprintf(stderr, "error:"
      "	Can't open connection to display server."
      " Probably X server is not started.\n");
    return 1;
  }

  XColor myblue;
  myblue.red = 0;
  myblue.green = 0;
  myblue.blue = 65535;


  int blackColor = BlackPixel(display, DefaultScreen(display));
  int whiteColor = WhitePixel(display, DefaultScreen(display));
  
  Window window = XCreateSimpleWindow(
				      display,
				      DefaultRootWindow(display),                   // parent window
				      0,//x
				      0,//y
				      720,//width
				      480,//height
				      2, //border width         
				      whiteColor,//border color
				      whiteColor); // background color

  XSelectInput(display, window, StructureNotifyMask);

  XMapWindow(display, window);                    // make new window visible
  
  GC gc = XCreateGC(display, window, 0, NULL);

  XSetForeground(display, gc, blackColor);
  
  for(;;) {
    XEvent e;
    XNextEvent(display, &e);
    if (e.type == MapNotify)
      break;
  }
  
  sleep(1);

  XDrawLine(display, window, gc, 10, 10, 100, 10);
  XDrawLine(display, window, gc, 10, 10, 10, 100);
  
  XFlush(display);                                // send all data to X11 server

  sleep(2);

  XUnmapWindow(display, window);                  // close window, connection and free resources
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}
