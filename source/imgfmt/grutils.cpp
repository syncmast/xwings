//---------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include "grutils.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
void AddColorNoise(TBitmap &clip, int amount)
{
	byte_t *p0;
	uint_t x, y, r, g, b;

    clip.PixelFormat = TPixelFormat(pf24bit);
  	for(y = 0; y < clip.Height; y++)
  	{
    	p0 = clip.ScanLine[y];
    	for(x = 0; x < clip.Width; x++)
    	{
      		r = p0[x*3]+(rand()%amount-(amount >> 1));
      		g = p0[x*3+1]+(rand()%amount-(amount >> 1));
      		b = p0[x*3+2]+(rand()%amount-(amount >> 1));
      		p0[x*3] = TRIMVAL(r,0,255);
      		p0[x*3+1] = TRIMVAL(g,0,255);
      		p0[x*3+2] = TRIMVAL(b,0,255);
    	};
  	};
}
//---------------------------------------------------------------------------
void AddMonoNoise(TBitmap &clip, int amount)
{
	byte_t *p0;
	uint_t x, y, r, g, b;
	int a;

    clip.PixelFormat = TPixelFormat(pf24bit);
  	for(y = 0; y < clip.Height; y++)
  	{
    	p0 = clip.ScanLine[y];
    	for(x = 0; x < clip.Width; x++)
    	{
      		a = random()%amount-(amount >> 1);
      		r = p0[x*3]+a;
      		g = p0[x*3+1]+a;
      		b = p0[x*3+2]+a;
      		p0[x*3] = TRIMVAL(r,0,255);
      		p0[x*3+1] = TRIMVAL(g,0,255);
      		p0[x*3+2] = TRIMVAL(b,0,255);
    	};
  	};
}
//---------------------------------------------------------------------------
void ShadowUpLeft(TBitmap &src)
{
	uint_t x, y;
  	TBitmap bmp;
  	byte_t *P1, *P2;

    src.PixelFormat = TPixelFormat(pf24bit);
	bmp.Width = src.Width;
  	bmp.Height = src.Height;
  	bmp.PixelFormat = TPixelFormat(pf24bit);
  	bmp.Canvas->Draw(0, 0, src);
    for( y = 0; y <= bmp.Height - 5; y++)
    {
    	P1 = bmp.ScanLine[y];
      	P2 = bmp.ScanLine[y + 4];
      	for( x = 0; x < bmp.Width - 5; x++)
        	if(P1[x*3] > P2[(x+4)*3])
        	{
        		P1[x*3] = P2[(x+4)*3]+1;
        		P1[x*3+1] = P2[(x+4)*3+1]+1;
        		P1[x*3+2] = P2[(x+4)*3+2]+1;
        	};
    };
//  	src.Assign(bmp);
    src = bmp;
}
//---------------------------------------------------------------------------
void ShadowUpRight(TBitmap &src)
{
	uint_t x,y;
  	TBitmap bmp;
  	byte_t *P1, *P2;

    src.PixelFormat = TPixelFormat(pf24bit);
  	bmp.Width = src.Width;
  	bmp.Height = src.Height;
  	bmp.PixelFormat = TPixelFormat(pf24bit);
  	bmp.Canvas->Draw(0, 0, src);
    for( y = 0; y <= bmp.Height -5; y++)
    {
    	P1 = bmp.ScanLine[y];
      	P2 = bmp.ScanLine[y+4];
      	for( x = bmp.Width-1; x >=4; x--)
        	if(P1[x*3] > P2[(x-4)*3])
        	{
        		P1[x*3] = P2[(x-4)*3]+1;
        		P1[x*3+1] = P2[(x-4)*3+1]+1;
        		P1[x*3+2] = P2[(x-4)*3+2]+1;
        	};
    };
//  	src.Assign(bmp);
    src = bmp;
}
//---------------------------------------------------------------------------
void ShadowDownLeft(TBitmap &src)
{
	uint_t x,y;
  	TBitmap bmp;
  	byte_t *P1, *P2;

    src.PixelFormat = TPixelFormat(pf24bit);
  	bmp.Width = src.Width;
  	bmp.Height = src.Height;
  	bmp.PixelFormat = TPixelFormat(pf24bit);
  	bmp.Canvas->Draw(0, 0, src);
    for(y = bmp.Height - 1; y >=4; y--)
    {
    	P1 = bmp.ScanLine[y];
      	P2 = bmp.ScanLine[y-4];
      	for( x = 0; x <= bmp.Width-5; x++)
        	if(P1[x*3]>P2[(x+4)*3])
        	{
        		P1[x*3] = P2[(x+4)*3]+1;
        		P1[x*3+1] = P2[(x+4)*3+1]+1;
        		P1[x*3+2] = P2[(x+4)*3+2]+1;
        	};
    };
//  	src.Assign(bmp);
    src = bmp;
}
//---------------------------------------------------------------------------
void ShadowDownRight(TBitmap &src)
{
	uint_t x,y;
  	TBitmap bmp;
  	byte_t *P1, *P2;

    src.PixelFormat = TPixelFormat(pf24bit);
  	bmp.Width = src.Width;
  	bmp.Height = src.Height;
  	bmp.PixelFormat = TPixelFormat(pf24bit);
  	bmp.Canvas->Draw(0, 0, src);
    for(y = bmp.Height - 1; y >= 4; y--)
    {
    	P1 = bmp.ScanLine[y];
      	P2 = bmp.ScanLine[y-4];
      	for(x= bmp.Width - 1; x >= 4; x--)
        	if(P1[x*3]>P2[(x-4)*3])
        	{
         		P1[x*3] = P2[(x-4)*3]+1;
         		P1[x*3+1] = P2[(x-4)*3+1]+1;
         		P1[x*3+2] = P2[(x-4)*3+2]+1;
        	};
    };
//  	src.Assign(bmp);
    src = bmp;
}
//---------------------------------------------------------------------------
void Emboss(TBitmap &bmp, int amount)
{
	uint_t r, g, b, a;
	int m;
	byte_t *p1, *p2;

    bmp.PixelFormat = TPixelFormat(pf24bit);
	for(register uint_t y = 0; y < bmp.Height; y++)
  	{
    	p1 = bmp.ScanLine[y];
   		p2 = bmp.ScanLine[y+1];
    	for(register uint_t x = 0; x < bmp.Width; x++)
    	{
    		m = TRIMVAL(x + amount, 0, bmp.Width);
    		r = (p1[x * 3] + (p2[m * 3] ^ 0xFF)) >> 1;
    		g = (p1[x * 3 + 1] + (p2[m * 3 + 1] ^ 0xFF)) >> 1;
    		b = (p1[x * 3 + 2] + (p2[m * 3 + 2] ^ 0xFF)) >> 1;
    		a = TRIMVAL((30 * r + 59 * g + 11 * b) / 100, 0, 255);
      		p1[x*3] = a;
      		p1[x*3+1] = a;
      		p1[x*3+2] = a;
    	};
  	};
}
//---------------------------------------------------------------------------
void Saturation(TBitmap &clip, int amount)
{
	byte_t *p0;
	uint_t gray,r,g,b;

    clip.PixelFormat = TPixelFormat(pf24bit);
  	for(register uint_t y = 0; y < clip.Height; y++)
  	{
    	p0 = clip.ScanLine[y];
    	for(register uint_t x = 0; x < clip.Width; x++)
    	{
      		r = p0[x * 3];
      		g = p0[x * 3 + 1];
      		b = p0[x * 3 + 2];
//      		gray = (r + g + b) / 3;
      		gray = TRIMVAL((30 * r + 59 * g + 11 * b) / 100, 0, 255);
      		p0[x * 3] = TRIMVAL(gray + ((r - gray) * amount / 255), 0, 255);
      		p0[x * 3 + 1] = TRIMVAL(gray + ((g - gray) * amount / 255), 0, 255);
      		p0[x * 3 + 2] = TRIMVAL(gray + ((b - gray) * amount / 255), 0, 255);
    	};
  	};
}
//---------------------------------------------------------------------------
void __fastcall Blend(TBitmap &src1, TBitmap &src2, TBitmap &dst, int amount)
{
	uint_t w,h;
    byte_t *ps1, *ps2, *pd;

	w = src1.Width ;
	h = src1.Height;
	dst.Width = w;
	dst.Height = h;
	src1.PixelFormat = TPixelFormat(pf24bit);
	src2.PixelFormat = TPixelFormat(pf24bit);
	dst.PixelFormat = TPixelFormat(pf24bit);
	for(register uint_t y = 0; y < h; y++)
	{
 		ps1 = src1.ScanLine[y];
 		ps2 = src2.ScanLine[y];
 		pd = dst.ScanLine[y];
 		for(register uint_t x = 0; x < w; x++)
 		{
  			pd[x*3] = ((255-amount)*ps1[x*3]+amount*ps2[x*3]) / 255;
  			pd[x*3+1] = ((255-amount)*ps1[x*3+1]+amount*ps2[x*3+1]) / 255;
  			pd[x*3+2] = ((255-amount)*ps1[x*3+2]+amount*ps2[x*3+2]) / 255;
  		};
 	};
}
//---------------------------------------------------------------------------
void __fastcall Blend(TBitmap &src1, TBitmap &src2, TBitmap &alfa, TBitmap &dst)
{
	uint_t w,h;
    byte_t *ps1, *ps2, *pa, *pd;
    int amount;

	w = src1.Width ;
	h = src1.Height;
	dst.Width = w;
	dst.Height = h;
	src1.PixelFormat = TPixelFormat(pf24bit);
	src2.PixelFormat = TPixelFormat(pf24bit);
	dst.PixelFormat = TPixelFormat(pf24bit);
	for(register uint_t y = 0; y < h; y++)
	{
 		ps1 = src1.ScanLine[y];
 		ps2 = src2.ScanLine[y];
 		pa = alfa.ScanLine[y];
 		pd = dst.ScanLine[y];
 		for(register uint_t x = 0; x < w; x++)
 		{
 		    amount = pa[x];
  			pd[x*3] = ((255-amount)*ps1[x*3]+amount*ps2[x*3]) / 255;
  			pd[x*3+1] = ((255-amount)*ps1[x*3+1]+amount*ps2[x*3+1]) / 255;
  			pd[x*3+2] = ((255-amount)*ps1[x*3+2]+amount*ps2[x*3+2]) / 255;
  		};
 	};
}
//---------------------------------------------------------------------------
void Invert(TBitmap &src)
{
	uint_t w,h;
    byte_t *p;

	w = src.Width;
	h = src.Height;
	src.PixelFormat = TPixelFormat(pf24bit);
 	for(register uint_t y = 0; y < h; y++)
 	{
  		p = src.ScanLine[y];
  		for(register uint_t x = 0; x < w; x++)
  		{
   			p[x*3] = ~p[x*3];
   			p[x*3+1] = ~p[x*3+1];
   			p[x*3+2] = ~p[x*3+2];
   		};
  	};
}
//---------------------------------------------------------------------------
// resample image as triangles
void Triangles(TBitmap &src, int amount)
{
	struct TTriplet {
    byte_t r,g,b;
    } t;
	uint_t w,h;
	int tb,tm,te;
    byte_t *ps;

	w = src.Width;
 	h = src.Height;
 	src.PixelFormat = TPixelFormat(pf24bit);
 	if(amount < 5)
 		amount = 5;
 	amount = (amount / 2)*2+1;
 	tm = amount / 2;
	for(register uint_t y = 0; y < h; y++)
	{
  		ps = src.ScanLine[y];
  		t.r = ps[0];
  		t.g = ps[1];
  		t.b = ps[2];
  		tb = y % (amount-1);
  		if(tb > tm)
  			tb = 2 * tm - tb;
  		if(tb == 0)
  			tb = amount;
  		te = tm + abs(tm - int(y % amount));
  		for(register uint_t x = 0; x < w; x++)
  		{
    		if((x % tb)==0)
    		{
     			t.r = ps[x*3];
     			t.g = ps[x*3+1];
     			t.b = ps[x*3+2];
     		};
    		if(((x % te)==1) && (tb != 0))
    		{
     			t.r = ps[x*3];
     			t.g = ps[x*3+1];
     			t.b = ps[x*3+2];
     		};
    		ps[x*3] = t.r;
    		ps[x*3+1] = t.g;
    		ps[x*3+2] = t.b;
    	};
  	};
}
//---------------------------------------------------------------------------
void RippleTooth(TBitmap &src, int amount)
{
  	byte_t *P1, *P2;
  	byte_t b;

  	src.PixelFormat = TPixelFormat(pf24bit);
  	amount = min(int(src.Height / 2), amount);
    for(register uint_t y = src.Height - 1 - amount; y > 0; y--)
    {
    	P1 = src.ScanLine[y];
      	b = 0;
      	for(register uint_t x = 0; x < src.Width; x++)
      	{
        	P2 = src.ScanLine[y+b];
        	P2[x*3] = P1[x*3];
        	P2[x*3+1] = P1[x*3+1];
        	P2[x*3+2] = P1[x*3+2];
        	b++;
        	if(b > amount)
        		b = 0;
        };
    };
}
//---------------------------------------------------------------------------
void RippleTriangle(TBitmap &src, int amount)
{
  	byte_t *P1, *P2;
  	byte_t b;
  	bool doinc;

	src.PixelFormat = TPixelFormat(pf24bit);
  	amount = min(int(src.Height / 2), amount);
    for(register uint_t y = src.Height - 1 - amount; y > 0; y--)
    {
    	P1 = src.ScanLine[y];
      	b = 0;
      	doinc = true;
      	for(register uint_t x = 0; x < src.Width; x++)
      	{
        	P2 = src.ScanLine[y+b];
        	P2[x*3] = P1[x*3];
        	P2[x*3+1] = P1[x*3+1];
        	P2[x*3+2] = P1[x*3+2];
        	if(doinc)
        	{
          		b++;
          		if(b > amount)
          		{
             		doinc = false;
             		b = amount-1;
             	};
          	}
          	else
          	{
           		if(b==0)
           		{
             		doinc = true;
             		b = 2;
             	};
           		b--;
           	};
        };
    };
}
//---------------------------------------------------------------------------
void RippleRandom(TBitmap &src, int amount)
{
  	byte_t *P1, *P2;
  	byte_t b;

	amount = min(int(src.Height / 2), amount);
  	src.PixelFormat = TPixelFormat(pf24bit);
//  randomize();
    for(register uint_t y = src.Height -1-amount; y > 0; y--)
    {
    	P1 = src.ScanLine[y];
      	b = 0;
      	for(register uint_t x = 0; x < src.Width; x++)
      	{
        	P2 = src.ScanLine[y+b];
        	P2[x*3] = P1[x*3];
        	P2[x*3+1] = P1[x*3+1];
        	P2[x*3+2] = P1[x*3+2];
        	b = random()%amount;
        };
    };
}
//---------------------------------------------------------------------------
void Solorize(TBitmap &src, TBitmap &dst, int amount)
{
	uint_t w,h;
    byte_t *ps, *pd;
    int c;

	w = src.Width;
  	h = src.Height;
  	src.PixelFormat = TPixelFormat(pf24bit);
  	dst.PixelFormat = TPixelFormat(pf24bit);
  	for(register uint_t y = 0; y < h; y++)
  	{
   		ps = src.ScanLine[y];
   		pd = dst.ScanLine[y];
   		for(register uint_t x = 0; x < w; x++)
   		{
    		c = (ps[x*3]+ps[x*3+1]+ps[x*3+2]) / 3;
    		if(c > amount)
    		{
     			pd[x*3] = 255-ps[x*3];
     			pd[x*3+1] = 255-ps[x*3+1];
     			pd[x*3+2] = 255-ps[x*3+2];
     		}
     		else
     		{
     			pd[x*3] = ps[x*3];
     			pd[x*3+1] = ps[x*3+1];
     			pd[x*3+2] = ps[x*3+2];
     		};
    	};
   };
}
//---------------------------------------------------------------------------
void Posterize(TBitmap &src, TBitmap &dst, int amount)
{
	uint_t w,h;
    byte_t *ps, *pd;

	w = src.Width;
  	h = src.Height;
  	src.PixelFormat = TPixelFormat(pf24bit);
  	dst.PixelFormat = TPixelFormat(pf24bit);
  	for(register uint_t y = 0; y < h; y++)
  	{
   		ps = src.ScanLine[y];
   		pd = dst.ScanLine[y];
   		for(register uint_t x = 0; x < w; x++)
   		{
     		pd[x*3] = round(ps[x*3]/amount)*amount;
     		pd[x*3+1] = round(ps[x*3+1]/amount)*amount;
     		pd[x*3+2] = round(ps[x*3+2]/amount)*amount;
    	};
   	};
}
//---------------------------------------------------------------------------
void TurnRight(TBitmap &src, TBitmap &dst)
{
	uint_t w,h;
    byte_t *ps, *pd;

	h = src.Height;
 	w = src.Width;
 	src.PixelFormat = TPixelFormat(pf24bit);
 	dst.PixelFormat = TPixelFormat(pf24bit);
 	dst.Height = w;
 	dst.Width = h;
 	for(register uint_t y = 0; y <h; y++)
 	{
  		ps = src.ScanLine[y];
  		for(register uint_t x = 0; x < w; x++)
  		{
   			pd = dst.ScanLine[x];
   			pd[(h-1-y)*3] = ps[x*3];
   			pd[(h-1-y)*3+1] = ps[x*3+1];
   			pd[(h-1-y)*3+2] = ps[x*3+2];
   		};
  	};
}
//---------------------------------------------------------------------------
void Turn(TBitmap &src, TBitmap &dst)
{
	uint_t w,h;
    byte_t *ps, *pd;

	h = src.Height;
 	w = src.Width;
 	src.PixelFormat = TPixelFormat(pf24bit);
 	dst.PixelFormat = TPixelFormat(pf24bit);
 	dst.Height = w;
 	dst.Width = h;
 	for(register uint_t y = 0; y < h; y ++)
 	{
  		ps = src.ScanLine[y];
  		for(register uint_t x = 0; x < w; x++)
  		{
   			pd = dst.ScanLine[w-1-x];
   			pd[y*3]=ps[x*3];
   			pd[y*3+1]=ps[x*3+1];
   			pd[y*3+2]=ps[x*3+2];
   		};
  	};
}
//---------------------------------------------------------------------------
void HeightMap(TBitmap &src, int amount)
{
	TBitmap bm;
    uint_t w,h;
    byte_t *pb, *ps;
    int c;

	h = src.Height;
 	w = src.Width;
 	bm.Width = w;
 	bm.Height = h;
 	bm.PixelFormat = TPixelFormat(pf24bit);
 	src.PixelFormat = TPixelFormat(pf24bit);
 	bm.Canvas->Draw(0, 0, src);
 	for(register uint_t y = 0; y < h; y++)
 	{
   		pb = bm.ScanLine[y];
  		for(register uint_t x = 0; x < w; x++)
  		{
   			c= (pb[x*3]+pb[x*3+1]+pb[x*3+2])*amount/3/255;
   			c = int(y) - c;
   			if( c >= 0 )
   			{
     			ps = src.ScanLine[c];
     			ps[x*3] = pb[x*3];
     			ps[x*3+1] = pb[x*3+1];
     			ps[x*3+2] = pb[x*3+2];
     		};
   		};
  	};
}
//---------------------------------------------------------------------------
void Spray(TBitmap &clip, int amount)
{
	uint_t w,h;
	int x, y, val1;
	byte_t *pb, *ps;

	h = clip.Height;
  	w = clip.Width;
  	clip.PixelFormat = TPixelFormat(pf24bit);
  	for(register uint_t j = 0; j < h; j++)
  	{
  		ps = clip.ScanLine[j];
  		for(register uint_t i = 0; i < w; i++)
  		{
    		val1 = random()%amount - amount/2;
    		x = i + val1;
    		val1 = random()%amount - amount/2;
    		y = j + val1;
    		if(x>=0 && x<(int)w && y>=0 && y<(int)h)
    		{
    			pb = clip.ScanLine[y];
    			ps[i*3] = pb[x*3];
     			ps[i*3+1] = pb[x*3+1];
     			ps[i*3+2] = pb[x*3+2];
    		}
  		};
  	}
}
//---------------------------------------------------------------------------
void Mosaic(TBitmap &bm, uint_t size)
{
	uint_t x,y,i,j;
   	byte_t *p1, *p2;
   	byte_t r,g,b;

  	bm.PixelFormat = TPixelFormat(pf24bit);
	y = 0;
  	do{
    	p1 = bm.ScanLine[y];
    	x = 0;
    	do{
      		j = 1;
      		do{
      			p2 = bm.ScanLine[y];
      			x = 0;
      			do{
        			r = p1[x*3];
        			g = p1[x*3+1];
        			b = p1[x*3+2];
        			i = 1;
       				do{
       					p2[x*3] = r;
       					p2[x*3+1] = g;
       					p2[x*3+2] = b;
       					x++;
       					i++;
       				}while(x < bm.Width && i <= size);
      			}while(x < bm.Width);
      			j++;
      			y++;
      		}while(y < bm.Height && j <= size);
    	}while(y < bm.Height && x < bm.Width);
	}while(y < bm.Height);
}
//---------------------------------------------------------------------------
void Lightness(TBitmap &clip, int amount)
{
	byte_t *p0;
	uint_t r,g,b;

  	clip.PixelFormat = TPixelFormat(pf24bit);
	for(register uint_t y = 0; y < clip.Height; y++)
	{
    	p0 = clip.ScanLine[y];
    	for(register uint_t x = 0; x < clip.Width; x++)
    	{
      		r = p0[x*3];
      		g = p0[x*3+1];
      		b = p0[x*3+2];
      		p0[x*3] = TRIMVAL(r+((255-r)*amount) / 255, 0, 255);
      		p0[x*3+1] = TRIMVAL(g+((255-g)*amount) / 255, 0, 255);
      		p0[x*3+2] = TRIMVAL(b+((255-b)*amount) / 255, 0, 255);
    	};
  	};
}
//---------------------------------------------------------------------------
void Darkness(TBitmap &clip, int amount)
{
	byte_t *p0;
	uint_t r,g,b;

    clip.PixelFormat = TPixelFormat(pf24bit);
	for(register uint_t y = 0; y < clip.Height; y++)
	{
    	p0 = clip.ScanLine[y];
    	for(register uint_t x = 0; x < clip.Width; x++)
    	{
      		r = p0[x*3];
      		g = p0[x*3+1];
      		b = p0[x*3+2];
      		p0[x*3] = TRIMVAL(r-(r*amount) / 255, 0, 255);
      		p0[x*3+1] = TRIMVAL(g-(g*amount) / 255, 0, 255);
      		p0[x*3+2] = TRIMVAL(b-(b*amount) / 255, 0, 255);
    	};
  	};
}
//---------------------------------------------------------------------------
void SplitBlur(TBitmap &clip, uint_t amount)
{
	byte_t *p0, *p1, *p2;
	int cx;
	byte_t Buf[4][3];

    clip.PixelFormat = TPixelFormat(pf24bit);
  	for(register uint_t y = 0; y < clip.Height; y++)
  	{
    	p0 = clip.ScanLine[y];
    	if(y < amount)
    		p1 = clip.ScanLine[y];
    	else
    		p1 = clip.ScanLine[y - amount];
    	if(y < clip.Height - amount)
    		p2 = clip.ScanLine[y + amount];
    	else
    		p2 = clip.ScanLine[clip.Height - y];

    	for(register uint_t x = 0; x < clip.Width; x++)
    	{
      		if(x < amount)
      			cx = x;
      		else
      			cx = x - amount;
      		Buf[0][0] = p1[cx*3];
      		Buf[0][1] = p1[cx*3+1];
      		Buf[0][2] = p1[cx*3+2];
      		Buf[1][0] = p2[cx*3];
      		Buf[1][1] = p2[cx*3+1];
      		Buf[1][2] = p2[cx*3+2];
      		if(x < clip.Width - amount)
      			cx = x + amount;
      		else
      		    cx = clip.Width - x;
      		Buf[2][0] = p1[cx*3];
      		Buf[2][1] = p1[cx*3+1];
      		Buf[2][2] = p1[cx*3+2];
      		Buf[3][0] = p2[cx*3];
      		Buf[3][1] = p2[cx*3+1];
      		Buf[3][2] = p2[cx*3+2];
      		p0[x*3]=(Buf[0][0]+Buf[1][0]+Buf[2][0]+Buf[3][0]) >> 2;
      		p0[x*3+1]=(Buf[0][1]+Buf[1][1]+Buf[2][1]+Buf[3][1]) >> 2;
      		p0[x*3+2]=(Buf[0][2]+Buf[1][2]+Buf[2][2]+Buf[3][2]) >> 2;
    	};
	};
}
//---------------------------------------------------------------------------
void GaussianBlur(TBitmap &clip, uint_t amount)
{
	for(register uint_t i = amount; i > 0; i--)
  		SplitBlur(clip,3);
}
//---------------------------------------------------------------------------
void Contrast(TBitmap &clip, int amount)
{
	byte_t *p0;
	int rg,gg,bg,r,g,b;

    clip.PixelFormat = TPixelFormat(pf24bit);
	for(register uint_t y = 0; y < clip.Height; y++)
  	{
    	p0 = clip.ScanLine[y];
    	for(register uint_t x = 0; x < clip.Width; x++)
    	{
      		r = p0[x*3];
      		g = p0[x*3+1];
      		b = p0[x*3+2];
      		rg = (abs(127 - r) * amount) / 255;
      		gg = (abs(127 - g) * amount) / 255;
      		bg = (abs(127 - b) * amount) / 255;
      		r = (r > 127) ? (r + rg) : (r - rg);
      		g = (g > 127) ? (g + gg) : (g - gg);
      		b = (b > 127) ? (b + bg) : (b - bg);
      		p0[x*3] = TRIMVAL(r, 0, 255);
      		p0[x*3+1] = TRIMVAL(g, 0, 255);
      		p0[x*3+2] = TRIMVAL(b, 0, 255);
    	};
  	};
}
//---------------------------------------------------------------------------
void AntiAlias(TBitmap &clip, const TRect &rect)
{
	int XOrigin, YOrigin, XFinal, YFinal;
    byte_t *p0, *p1, *p2;

	XOrigin = rect.left;
	YOrigin = rect.top;
	XFinal = rect.right();
	YFinal = rect.bottom();
	XOrigin = max(1,XOrigin);
   	YOrigin = max(1,YOrigin);
   	XFinal = min((int)clip.Width - 2,XFinal);
   	YFinal = min((int)clip.Height - 2,YFinal);
   	clip.PixelFormat = TPixelFormat(pf24bit);
   	for(register int y = YOrigin; y <= YFinal; y++)
   	{
    	p0 = clip.ScanLine[y-1];
    	p1 = clip.ScanLine[y];
    	p2 = clip.ScanLine[y+1];
    	for(register int x = XOrigin; x <= XFinal; x++)
    	{
      		p1[x*3] = (p0[x*3]+p2[x*3]+p1[(x-1)*3]+p1[(x+1)*3]) / 4;
      		p1[x*3+1] = (p0[x*3+1]+p2[x*3+1]+p1[(x-1)*3+1]+p1[(x+1)*3+1]) / 4;
      		p1[x*3+2] = (p0[x*3+2]+p2[x*3+2]+p1[(x-1)*3+2]+p1[(x+1)*3+2]) / 4;
      	};
   	};
}
//---------------------------------------------------------------------------
void FishEye(TBitmap &bmp, TBitmap &dst, double amount)
{
	double xmid, ymid;
	double fx, fy;
	double r1, r2;
	int ifx, ify;
	double dx, dy;
	double rmax;
//	int ty, tx;
	double weight_x[2], weight_y[2];
	double weight;
	int new_red, new_green;
	int new_blue;
	double total_red, total_green;
	double total_blue;
//	int ix, iy;
	byte_t *sli, *slo;

  	xmid = bmp.Width/2;
  	ymid = bmp.Height/2;
  	rmax = dst.Width * amount;

  	for(register uint_t ty = 0; ty < dst.Height; ty++)
  	{
    	for(register uint_t tx = 0; tx < dst.Width; tx++)
    	{
      		dx = tx - xmid;
      		dy = ty - ymid;
      		r1 = sqrt(dx * dx + dy * dy);
      		if(r1 == 0)
      		{
        		fx = xmid;
        		fy = ymid;
      		}
      		else
      		{
        		r2 = rmax / 2 * (1 / (1 - r1/rmax) - 1);
        		fx = dx * r2 / r1 + xmid;
        		fy = dy * r2 / r1 + ymid;
      		};
      		ify = floor(fy);
      		ifx = floor(fx);
                // Calculate the weights.
      		if(fy >= 0)
      		{
        		weight_y[1] = fy - ify;
        		weight_y[0] = 1 - weight_y[1];
      		}
      		else
      		{
        		weight_y[0] = -(fy - ify);
        		weight_y[1] = 1 - weight_y[0];
      		};
      		if(fx >= 0)
      		{
        		weight_x[1] = fx - ifx;
        		weight_x[0] = 1 - weight_x[1];
      		}
      		else
      		{
        		weight_x[0] = -(fx - ifx);
        		weight_x[1] = 1 - weight_x[0];
      		};

      		if(ifx < 0)
        		ifx = bmp.Width - 1 - (-ifx % bmp.Width);
      		else if(ifx > (int)bmp.Width-1)
        		ifx = ifx % bmp.Width;
      		if(ify < 0)
        		ify = bmp.Height - 1 - (-ify % bmp.Height);
      		else if(ify > (int)bmp.Height-1)
        		ify = ify % bmp.Height;

      		total_red = 0.0;
      		total_green = 0.0;
      		total_blue  = 0.0;
      		for(register int ix = 0; ix < 2; ix++)
      		{
        		for(register int iy = 0; iy < 2; iy++)
        		{
          			if((ify + iy) < (int)bmp.Height)
            			sli = bmp.ScanLine[ify + iy];
          			else
            			sli = bmp.ScanLine[bmp.Height - ify - iy];
          			if((ifx + ix) < (int)bmp.Width)
          			{
            			new_red = sli[(ifx + ix)*3];
            			new_green = sli[(ifx + ix)*3+1];
            			new_blue = sli[(ifx + ix)*3+2];
          			}
          			else
          			{
            			new_red = sli[(bmp.Width - ifx - ix)*3];
            			new_green = sli[(bmp.Width - ifx - ix)*3+1];
            			new_blue = sli[(bmp.Width - ifx - ix)*3+2];
          			};
          			weight = weight_x[ix] * weight_y[iy];
          			total_red = total_red + new_red * weight;
          			total_green = total_green + new_green * weight;
          			total_blue = total_blue + new_blue * weight;
        		};
      		};
      		slo = dst.ScanLine[ty];
      		slo[tx*3] = round(total_red);
      		slo[tx*3+1] = round(total_green);
      		slo[tx*3+2] = round(total_blue);

    	};
  	};
}
//---------------------------------------------------------------------------
void Marble(TBitmap &src, TBitmap &dst, double scale, int turbulence, int var)
{
	int xm,ym;
    double xx, yy, ax, ay;
    byte_t *p1, *p2;
    int w, h;

	h = src.Height;
	w = src.Width;
	dst.SetSize(w, h);
//	dst.Assign(src);
	for(register int y = 0; y < h; y++)
	{
		switch(var % 3)
		{
			case 0:
    			yy = scale * cos((y % turbulence)/scale);
				break;
			case 1:
				yy = scale * sin((y % turbulence)/scale);
				break;
			case 2:
    	    	ax = (y % turbulence)/ scale;
    			yy = scale * sin(ax) * cos(1.5 * ax);
    	}
 		p1 = src.ScanLine[y];
      	for(register int x = 0; x < w; x++)
      	{
      		switch(var % 5)
			{
				case 0:
					xx = -scale * sin((x % turbulence)/ scale);
					break;
      			case 1:
      		 		xx = -scale * cos((x % turbulence)/ scale);
					break;
				case 2:
      		 		xx = -tan((x % turbulence) / scale) / scale;
      		 		break;
      		 	case 3:
      		 		xx = -tan((x % turbulence) / scale) / ( scale * scale);
      		 		break;
      		 	case 4:
      		       	ax = (x % turbulence) / scale;
      				xx = -scale * sin(2 * ax) * cos(ax);
      		};
      		switch(var % 12)
			{
				case 0:
					ax = x + xx + yy;
      				ay = y + yy + xx;
					break;
      			case 1:
					ax = x + xx + yy;
      				ay = y + yy - xx;
					break;
				case 2:
					ax = x + xx + yy;
      				ay = y - yy - xx;
      		 		break;
      		 	case 3:
					ax = x + xx - yy;
      				ay = y + yy + xx;
      		 		break;
      		 	case 4:
					ax = x + xx - yy;
      				ay = y + yy - xx;
      		 		break;
      		 	case 5:
					ax = x + xx - yy;
      				ay = y - yy - xx;
      		 		break;
      		 	case 6:
					ax = x - xx + yy;
      				ay = y + yy + xx;
      		 		break;
				case 7:
					ax = x - xx + yy;
      				ay = y + yy - xx;
      		 		break;
      		 	case 8:
					ax = x - xx + yy;
      				ay = y - yy - xx;
      		 		break;
				case 9:
					ax = x - xx - yy;
      				ay = y + yy + xx;
      		 		break;
				case 10:
					ax = x - xx - yy;
      				ay = y + yy - xx;
      		 		break;
				case 11:
					ax = x - xx - yy;
      				ay = y - yy - xx;
      		 		break;
      		};
      		xm = abs((long long)ax);
      		ym = abs((long long)ay);
      		if(ym < h)
      		{
       			p2 = dst.ScanLine[ym];
       			if(xm < w)
       			{
          			p2[xm*3] = p1[x*3];
          			p2[xm*3+1] = p1[x*3+1];
          			p2[xm*3+2] = p1[x*3+2];
          		};
       		};
      	};
    };
}
//---------------------------------------------------------------------------
void Convert2Res(const char *filename, const char *outfn, const char *resname)
{
   	FILE *infile, *outfile;
  	int tmp,count = 0;
  	char buf[7];

  	if ((infile = fopen(filename, "rb")) == NULL)
  	{
    	fprintf(stderr, "can't open %s\n", filename);
    	return;
  	}
	if ((outfile = fopen(outfn, "w")) == NULL)
  	{
    	fprintf(stderr, "can't open %s\n", filename);
    	fclose(infile);
    	return;
  	}
  	fputs("static const unsigned char ", outfile );
  	fputs( resname, outfile );
  	fputs( "[]={", outfile );
  	do{
  	    count++;
        tmp = fgetc( infile );
        if( tmp == EOF )
        {
            fseek( outfile, -1L, SEEK_CUR );
            fputs( "};", outfile );
            break;
        }
 //       sprintf( buf, "'\\x%x'", tmp );
        sprintf( buf, "%d", tmp );
        fputs( buf, outfile );
        fputc( ',', outfile );
        if( count == 20 )
        {
            fputc( '\n', outfile );
            count = 0;
        }
  	}while( 1 );

  	fclose( infile );
  	fclose( outfile );
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
