#include <stdlib.h>
#include <stdio.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "left_ptr"

#define WIDTH 200
#define HEIGHT 100


void HandleHiddenWindows(Display *display,Window *children,unsigned int nChildren,
	unsigned int x,unsigned int y,unsigned int width,unsigned int height,
	Window rootWin,Window outWin,GC gc,Window realWin)
{
	unsigned int i;
	Window dummy;
	int x_return, y_return;
	unsigned int width_return, height_return;
	unsigned int border_width_return;
	unsigned int depth_return;
	unsigned int rx,ry,rwidth,rheight;
	
	for(i=0;i<nChildren;i++)
	{
	if (children[i] == realWin)
		continue;
	XGetGeometry(display,children[i],&dummy,&x_return, &y_return, &width_return,
		&height_return, &border_width_return, &depth_return);
	fprintf(stderr,"child %ud: %d %d %ud %ud\n",i,x_return,y_return,width_return,
		height_return);	
/*	if ((x>=x_return) || (x+width<=x_return+width_return)
		&&
	(y>=y_return) || (y+height<=y_return+height_return))
		{
			fprintf(stderr,"above self\n");
			XCopyArea(display, children[i], outWin, gc,
			x_return,y_return, WIDTH*2, HEIGHT*2, 0, 0);
		}
		*/
	}
}


int main(void)
{
	Display *display;
	int screen;
	Window rootWin,childWin,outWin,realWin;
	XEvent event;
	XImage *data,*data2;
	GC gc;
	XGCValues gcv;
	int rootx, rooty, dx, dy, x,y;
	unsigned long pixel;
	int width,height;
	unsigned int mask;
	Window winret, parent, *children;
	Window winret2, parent2, *children2;
	Window winret3, parent3, *children3;
	Window winret4, parent4, *children4;
	unsigned int nchildren,i;
	unsigned int nchildren2,i2;
	unsigned int nchildren3,i3;
	unsigned int nchildren4,i4;
	Pixmap pointer;

	Window dummy;
	int x_return, y_return;
	unsigned int width_return, height_return;
	unsigned int border_width_return;
	unsigned int depth_return;

	
	if (NULL == (display = XOpenDisplay(NULL)))
	{
		fprintf(stderr, "cannot connect to X server %s\n", getenv("DISPLAY"));
		return(1);
	}

	screen = DefaultScreen(display);
	rootWin = RootWindow(display, screen);

	outWin = XCreateSimpleWindow(display, rootWin, 0, 0, WIDTH*2,
		HEIGHT*2, 0, BlackPixel(display,screen), WhitePixel(display,screen));

	XSelectInput(display, rootWin, PointerMotionMask|PointerMotionHintMask);

	XQueryTree(display, rootWin, &winret, &parent, &children, &nchildren);
	for(i=0; i < nchildren; i++)
	{
		XQueryTree(display, children[i], &winret2, &parent2, &children2, &nchildren2);
		if (nchildren2 == 0)
			XSelectInput(display, children[i], PointerMotionMask|PointerMotionHintMask);
		for(i2=0; i2 < nchildren2; i2++)
		{
			XQueryTree(display, children2[i2], &winret3, &parent3, &children3, &nchildren3);
			if (nchildren3 == 0)
				XSelectInput(display, children2[i2], PointerMotionMask|PointerMotionHintMask);
			for(i3=0; i3 < nchildren3; i3++)
			{
				XSelectInput(display, children3[i3], PointerMotionMask|PointerMotionHintMask);

				XQueryTree(display, children3[i3], &winret4, &parent4, &children4, &nchildren4);
				if (nchildren4 == 0)
					XSelectInput(display, children3[i3], PointerMotionMask|PointerMotionHintMask);
				for(i4=0; i4 < nchildren4; i4++)
					XSelectInput(display, children4[i4], PointerMotionMask|PointerMotionHintMask);
			}
			if (children3)
				XFree(children3);
		}
		if (children2)
			XFree(children2);
	}

	XMapWindow(display,outWin);
	XFlush(display);

	pointer = XCreatePixmapFromBitmapData(display,rootWin,left_ptr_bits,
	left_ptr_width, left_ptr_height, BlackPixel(display,screen),
	WhitePixel(display,screen), DefaultDepth(display,screen));
	getchar();

	data2 = XGetImage (display, outWin, 0, 0, WIDTH*2, HEIGHT*2, 
		AllPlanes, ZPixmap);

	gc = XCreateGC(display, outWin, 0, NULL);
	gcv.function = GXcopy;
	XChangeGC(display, gc, GCFunction, &gcv);


	parent = outWin;
	do
	{
		realWin = parent;
		XQueryTree(display, realWin, &dummy, &parent, &children, &nchildren);
	}while(parent != rootWin);

	while (1)
	{
	XMotionEvent *mevent = (XMotionEvent *) &event;
	XNextEvent(display,&event);
	if (event.type == MotionNotify)
	{
		XQueryPointer (display, rootWin, &rootWin, &childWin,
			&rootx, &rooty, &dx, &dy, &mask); 
		
		width=WIDTH;
		height=HEIGHT;
		rootx=mevent->x_root-WIDTH/2;
		rooty=mevent->y_root-HEIGHT/2;
		fprintf(stderr," X Y %d %d\n",mevent->x_root,mevent->y_root);
		if (rootx < 0)
		{
			width+=rootx;
			rootx=0;
		}
		if (rooty < 0)
		{
			height+=rooty;
			rooty=0;
		}

		XGetGeometry(display,realWin,&dummy,&x_return, &y_return, &width_return,
			&height_return, &border_width_return, &depth_return);
		
		fprintf(stderr,":x y x y %d %d %d %d\n",x_return,y_return,width_return,height_return);

		if ((rootx+WIDTH/2 >= x_return) &&
		(rooty+HEIGHT/2 >= y_return) &&
		(rootx+WIDTH/2 <= x_return+width_return) &&
		(rooty+HEIGHT/2 <= y_return+height_return))
		{
			fprintf(stderr,"above self\n");
			HandleHiddenWindows(display,children,nchildren,
				x_return,y_return,width_return,height_return,rootWin,outWin,gc,realWin);

			//XCopyArea(display, rootWin, outWin, gc,
			//rootx,rooty, WIDTH*2, HEIGHT*2, 0, 0);
		}
		else
		{
			data = XGetImage (display, rootWin, rootx, rooty, 
				width, height, AllPlanes, ZPixmap);

			for (x = 0; x < width; x++)
				for (y = 0; y < height; y++)
				{
					pixel = XGetPixel(data, x, y);
					XPutPixel(data2, x*2, y*2, pixel);
					XPutPixel(data2, x*2+1, y*2, pixel);
					XPutPixel(data2, x*2, y*2+1, pixel);
					XPutPixel(data2, x*2+1, y*2+1, pixel);
				}

			XPutImage(display, outWin, gc, data2, 0, 0, 0, 0, WIDTH*2, HEIGHT*2);
		}
		XCopyArea(display,pointer,outWin,gc,0,0,left_ptr_width,left_ptr_height,
		WIDTH,HEIGHT);
		}
	}
	XUnmapWindow(display,outWin);
	XFreePixmap(display,pointer);
	if (children)
		XFree(children);
	XCloseDisplay(display);
	return 0;
}
