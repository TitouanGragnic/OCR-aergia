#include "../../include/detection/grid_detection.h"

blob blobFromImage(SDL_Surface *img, int size)
{
     Uint32 pixel;
     Uint8 r,g,b;
     int max = 0;
     blob maxi = {0,0,0,0,0,0,0,0};
     int w = img->w;
     int h = img->h;
     double id = 1;

     double *matr = calloc(w*h,sizeof(double));

     stack* S = get_empty_stack();

     for(int i = 0;i<w;i++)
     {
	  for(int j = 0;j<h;j++)
	  {
	       matr[i*h+j] = -1;
	  }
     }

     for(int i = 1;i<w-1;i++)
     {
	  for(int j = 1;j<h-1;j++)
	  {
	       id++;
	       pixel = get_pixel(img, i, j);
	       SDL_GetRGB(pixel, img->format, &r, &g, &b);
	       if(r == 0 || matr[i*h+j] > -1)
		    continue;

	       coor start = {i,j};
               push_stack(S,start);

	       int sum_x=0,sum_y=0,n_pixels=0,max_x=0,max_y=0;
	       int min_x = w+1, min_y=h+1;
	       while(!is_empty_stack(S))//stack pas vide
	       {
		    coor top = pop_stack(S);

		    matr[top.x*h+top.y] = id;
		    n_pixels += 1;

		    min_x = (top.x<min_x)?top.x:min_x;
		    min_y = (top.y<min_y)?top.y:min_y;
		    max_x = (top.x>max_x)?top.x:max_x;
		    max_y = (top.y>max_y)?top.y:max_y;
		    sum_y+=top.y;
		    sum_x+=top.x;

		    for(int k = top.y-1;k<=top.y+1;k++)
		    {
			 for(int l = top.x-1;l<=top.x+1;l++)
			 {
                             if(l<0 || k<0 || l>= w || k>= h)
                                 continue;
			      pixel = get_pixel(img, l, k);
			      SDL_GetRGB(pixel, img->format, &r, &g, &b);
			      if(r == 0 || matr[l*h+k] > -1)
				   continue;

			      coor next = {l,k};
			      matr[l*h+k] = id;
			      //push next in sack
			      push_stack(S,next);
			 }
		    }
	       }
	       if(n_pixels < size)
		    continue;
	       blob nextcentre =
		    {min_x,max_x,
		    min_y,max_y,
		    sum_x/n_pixels,
		    sum_y/n_pixels,
		    n_pixels,id};

	       if(nextcentre.n_pixels>max)
	       {
		    max = nextcentre.n_pixels;
		    maxi = nextcentre;
	       }
	  }
     }
     int var = 1;
     if(maxi.n_pixels == 0)
     {
	  maxi.max_y = img->h;
	  maxi.max_x = img->w;
	  var = 0;
     }
     for(int i = 0;i<w && var;i++)
     {
	  for(int j = 0;j<h;j++)
	  {
	       if(matr[i*h+j] != maxi.ID)
	       {
		    Uint32 newpixel = SDL_MapRGB(img->format, 0, 0, 0);
		    put_pixel(img,i,j,newpixel);
	       }
	  }
     }
     free(matr);
     free_stack(S);
     return maxi;
}
