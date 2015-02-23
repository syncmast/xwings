/*
** gcc -DXK_MISCELLANY -std=c99 -g -Wall -Werror -lX11 -lXrender -lXcomposite \
** x11-offscreen-redirection.c -o x11-offscreen-redirection
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xcomposite.h>

Picture
create_picture (Display*      pDisplay,
		Drawable      drawable,
		XRenderColor* pColor,
		int           iWidth,
		int           iHeight,
		Bool          bRepeat,
		int           iDepth,
		int           iFormat)
{
	XRenderPictureAttributes picAttribs;
	unsigned long		 ulMask = 0x00000000;
	Picture			 picture;
	Pixmap			 pixmap;

	pixmap = XCreatePixmap (pDisplay,
				drawable,
				iWidth  <= 0 ? 1 : iWidth,
				iHeight <= 0 ? 1 : iHeight,
				iDepth);

	if (bRepeat)
	{
		picAttribs.repeat = True;
		ulMask            = CPRepeat;
	}

	picture = XRenderCreatePicture (pDisplay,
					pixmap,
					XRenderFindStandardFormat (pDisplay,
								   iFormat),
					ulMask,
					&picAttribs);

	XRenderFillRectangle (pDisplay,
			      PictOpSrc,
			      picture,
			      pColor,
			      0,
			      0,
			      iWidth,
			      iHeight);

	XFreePixmap (pDisplay, pixmap);

	return picture;
}

Picture
create_a8_picture (Display*      pDisplay,
		   Drawable      drawable,
		   XRenderColor* pColor,
		   int           iWidth,
		   int           iHeight,
		   Bool          bRepeat)
{
	return create_picture (pDisplay,
			       drawable,
			       pColor,
			       iWidth,
			       iHeight,
			       bRepeat,
			       8,
			       PictStandardA8);
}

Picture
create_rgb24_picture (Display*      pDisplay,
		      Drawable      drawable,
		      XRenderColor* pColor,
		      int           iWidth,
		      int           iHeight,
		      Bool          bRepeat)
{
	return create_picture (pDisplay,
			       drawable,
			       pColor,
			       iWidth,
			       iHeight,
			       bRepeat,
			       24,
			       PictStandardRGB24);
}

Picture
create_argb32_picture (Display*      pDisplay,
		       Drawable      drawable,
		       XRenderColor* pColor,
		       int           iWidth,
		       int           iHeight,
		       Bool          bRepeat)
{
	return create_picture (pDisplay,
			       drawable,
			       pColor,
			       iWidth,
			       iHeight,
			       bRepeat,
			       32,
			       PictStandardARGB32);
}

void
paint (Display* pDisplay,
       Window   window,
       Visual*  pVisual,
       int      iWidth,
       int      iHeight)
{
	XRenderPictFormat*	 pPictFormatDefault = NULL;
	XRenderPictFormat*	 pPictFormatA8 = NULL;
	XRenderPictureAttributes picAttribs;
	XRenderColor		 colorTrans = {0x0000, 0x0000, 0x0000, 0x0000};
	XRenderColor		 colorWhite = {0xffff, 0xffff, 0xffff, 0xffff};
	Picture			 destPict;
	Picture			 maskPict;
	Picture			 solidPict;
	Picture			 gradientPict;
	XTrapezoid		 trapezoid;
	XLinearGradient		 linearGradient;
	XFixed			 aColorStops[2];
	XRenderColor		 aColorList[2];

	/* get some typical formats */
	pPictFormatDefault = XRenderFindVisualFormat (pDisplay, pVisual);
	pPictFormatA8 = XRenderFindStandardFormat (pDisplay, PictStandardA8);

	/* this is my target- or destination-drawable derived from the window */
	destPict = XRenderCreatePicture (pDisplay,
					 window,
					 pPictFormatDefault,
					 0,
					 &picAttribs);

	/* two intermediate pictures */
	solidPict = create_a8_picture (pDisplay,
				       window,
				       &colorWhite,
				       iWidth,
				       iHeight,
				       False);
	maskPict = create_a8_picture (pDisplay,
				      window,
				      &colorTrans,
				      iWidth,
				      iHeight,
				      False);

	/* define a as rectangle/square as trapezoid */
	trapezoid.top        = XDoubleToFixed (0.0f);
	trapezoid.bottom     = XDoubleToFixed ((double) iHeight);
	trapezoid.left.p1.x  = XDoubleToFixed (0.0f);
	trapezoid.left.p1.y  = XDoubleToFixed (0.0f);
	trapezoid.left.p2.x  = XDoubleToFixed (0.0f);
	trapezoid.left.p2.y  = XDoubleToFixed ((double) iHeight);
	trapezoid.right.p1.x = XDoubleToFixed ((double) iWidth);
	trapezoid.right.p1.y = XDoubleToFixed (0.0f);
	trapezoid.right.p2.x = XDoubleToFixed ((double) iWidth);
	trapezoid.right.p2.y = XDoubleToFixed ((double) iHeight);

	/* draw a trapezoid in the mask-picture */
	XRenderCompositeTrapezoids (pDisplay,
				    PictOpOver,
				    solidPict,
				    maskPict,
				    pPictFormatA8,
				    0, 0,
				    &trapezoid,
				    1);

	/* coordinates for the start- and end-point of the linear gradient are
	** in  window-space, they are not normalized like in cairo... here using
	** the height of the window */
	linearGradient.p1.x = XDoubleToFixed (0.0f);
	linearGradient.p1.y = XDoubleToFixed (0.0f);
	linearGradient.p2.x = XDoubleToFixed (0.0f);
	linearGradient.p2.y = XDoubleToFixed ((double) iHeight);

	/* offsets for color-stops have to be stated in normalized form,
	** which means within the range of [0.0, 1.0f] */
	aColorStops[0] = XDoubleToFixed (0.0f);
	aColorStops[1] = XDoubleToFixed (1.0f);

	/* there's nothing much to say about a XRenderColor,
	** each R/G/B/A-component is an unsigned int (16 bit) */
	aColorList[0].red   = 0x0000;
	aColorList[0].green = 0x0000;
	aColorList[0].blue  = 0x0000;
	aColorList[0].alpha = 0xffff;

	aColorList[1].red   = 0x7777;
	aColorList[1].green = 0x7777;
	aColorList[1].blue  = 0x7777;
	aColorList[1].alpha = 0xffff;

	/* create the actual gradient-filling picture */
	gradientPict = XRenderCreateLinearGradient (pDisplay,
						    &linearGradient,
						    aColorStops,
						    aColorList,
						    2);

	/* render a trapezoid */
	XRenderComposite (pDisplay,
			  PictOpSrc,
			  gradientPict,
			  maskPict,
			  destPict,
			  0, 0,
			  0, 0,
			  0, 0,
			  iWidth,
			  iHeight);

	XRenderFreePicture (pDisplay, destPict);
	XRenderFreePicture (pDisplay, solidPict);
	XRenderFreePicture (pDisplay, maskPict);
	XRenderFreePicture (pDisplay, gradientPict);
}

int 
main (int    argc,
      char** argv)
{
	Display*             pDisplay           = NULL;
	int                  iEventBase         = 0;
	int                  iErrorBase         = 0;
	int                  iMajor             = 0;
	int                  iMinor             = 0;
	XVisualInfo*         pVisInfo           = NULL;
	XVisualInfo          visInfoTemplate;
	int                  iNumOfEntries      = 0;
	int                  i                  = 0;
	XRenderPictFormat*   pPictFormat        = NULL;
	XSetWindowAttributes windowAttribs;
	int                  iWindowAttribsMask = 0;
	Window               window;
	Atom                 property;
	Atom                 type;
	unsigned char        aucWindowTitle[]   = "offscreen redirection";
	Bool                 bKeepGoing         = True;
	XEvent               event;

	/* connect to display */
	pDisplay = XOpenDisplay (NULL);
	if (!pDisplay)
	{
		printf ("Could not open display\n");
		return 1;
	}

	/* check for Xcomposite-extension */
	if (!XCompositeQueryExtension (pDisplay, &iEventBase, &iErrorBase))
	{
		XCloseDisplay (pDisplay);
		printf ("Xcomposite-extension not found");
		return 2;
	}

	/* see which version of Xcomposite is available */
	XCompositeQueryVersion (pDisplay, &iMajor, &iMinor);
	if (iMajor == 0 && iMinor < 4)
	{
		XCloseDisplay (pDisplay);
		printf ("At least Xcomposite 0.4 needed\n");
		return 3;
	}
	printf ("Found Xcomposite %d.%d\n", iMajor, iMinor);

	/* check for Xrender-extension */
	if (!XRenderQueryExtension (pDisplay, &iEventBase, &iErrorBase))
	{
		XCloseDisplay (pDisplay);
		printf ("Xrender-extension not found\n");
		return 4;
	}

	/* see which version is available */
	XRenderQueryVersion (pDisplay, &iMajor, &iMinor);
	if (iMajor == 0 && iMinor < 10)
	{
		XCloseDisplay (pDisplay);
		printf ("At least Xrender 0.10 needed\n");
		return 5;
	}
	printf ("Found Xrender %d.%d\n", iMajor, iMinor);

	/* find a rgba-visual */
	visInfoTemplate.depth      = 32;
	visInfoTemplate.red_mask   = 0xff0000;
	visInfoTemplate.green_mask = 0x00ff00;
	visInfoTemplate.blue_mask  = 0x0000ff;
	pVisInfo = XGetVisualInfo (pDisplay,
				   VisualDepthMask  |
				   VisualRedMaskMask|
				   VisualRedMaskMask|
				   VisualRedMaskMask,
				   &visInfoTemplate,
				   &iNumOfEntries);
	if (!pVisInfo)
	{
		XCloseDisplay (pDisplay);
		printf ("Failed to obtain VisInfo!\n");
		return 6;
	}
	else
	{
		printf ("Found %d matching Visuals.\n", iNumOfEntries);

		printf ("\nFirst matching visual is...\n");
		printf ("visual-id: 0x%02X\n",
			(int) pVisInfo[0].visualid);
		printf ("  screen:        %d\n",
			pVisInfo[0].screen);
		printf ("  depth:         %d\n",
			pVisInfo[0].depth);
		printf ("  red-mask:      0x%08lx\n",
			pVisInfo[0].red_mask);
		printf ("  green-mask:    0x%08lx\n",
			pVisInfo[0].green_mask);
		printf ("  blue-mask:     0x%08lx\n",
			pVisInfo[0].blue_mask);
		printf ("  colormap-size: %d\n",
			pVisInfo[0].colormap_size);
		printf ("  bits-per-rgb:  %d\n\n",
			pVisInfo[0].bits_per_rgb);
	}

	/* find the first usable visual-format for Xrender */
	for (i = 0; i < iNumOfEntries; i++)
	{
		pPictFormat = XRenderFindVisualFormat (pDisplay,
						       pVisInfo[i].visual);
		if (!pPictFormat)
			continue;

		if (pPictFormat->direct.alphaMask > 0)
			break;
	}

	if (i == iNumOfEntries)
	{
		XCloseDisplay (pDisplay);
		printf ("No suitable visual-format for Xrender found.\n");
		return 7;
	}

	/* make operations asynchronous */
	XSynchronize (pDisplay, False);

	/* set attribs for the window to be created... and  */
	windowAttribs.event_mask       = KeyPressMask |
					 ExposureMask;
	windowAttribs.colormap         = XCreateColormap (
						pDisplay,
						RootWindow (pDisplay,
							    pVisInfo[i].screen),
						pVisInfo[i].visual,
						AllocNone);
	iWindowAttribsMask             = CWEventMask;
	/*iWindowAttribsMask             = CWEventMask | CWColormap;*/

	/* create and open window */
	window = XCreateWindow (pDisplay,
				RootWindow (pDisplay, pVisInfo[i].screen),
				50,
				50,
				300,
				300,
				0,
				CopyFromParent,
				/*pVisInfo[i].depth,*/
				InputOutput,
				CopyFromParent,
				/*pVisInfo[i].visual,*/
				iWindowAttribsMask,
				&windowAttribs);
	XMapWindow (pDisplay, window);

	/* the list of XVisualInfos is no longer needed */
	XFree ((void*) pVisInfo);

	/* redirect window to offscreen-storage */
	XCompositeRedirectWindow (pDisplay,
				  window,
				  CompositeRedirectAutomatic);

	/* this is indeed how you change the window-title under pure X11 */
	property = XInternAtom (pDisplay, "_NET_WM_NAME", True);
	type     = XInternAtom (pDisplay, "UTF8_STRING", True);
	XChangeProperty (pDisplay,
			 window,
			 property,
			 type,
			 8,
			 PropModeReplace,
			 aucWindowTitle,
			 21);

	/* what kind of events are we interested in */
	XSelectInput (pDisplay, window, KeyPressMask | ExposureMask);

	/* event-loop */
	while (bKeepGoing)
	{
		XNextEvent (pDisplay, &event);
		switch (event.type)
		{
			case KeyPress :
				if (XLookupKeysym (&event.xkey, 0) == XK_Escape)
					bKeepGoing = False;
				if (XLookupKeysym (&event.xkey, 0) == XK_q)
					bKeepGoing = False;
			break;

			case Expose :
				if (event.xexpose.count < 1)
					paint (pDisplay,
					       window,
					       DefaultVisual (pDisplay, 0),
					       event.xexpose.width,
					       event.xexpose.height);
			break;
		}
	}

	/* shutdown, cleanup */
	XCompositeUnredirectWindow (pDisplay,
				    window,
				    CompositeRedirectAutomatic);
	XFreeColormap(pDisplay, windowAttribs.colormap);	    
	XUnmapWindow (pDisplay, window);
	XDestroyWindow (pDisplay, window);
	XCloseDisplay (pDisplay);

	return 0;
}

