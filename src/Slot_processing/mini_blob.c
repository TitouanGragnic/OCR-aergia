#include "mini_blob.h"

struct coor* append_coor(struct coor* tab,int len,struct coor l)
{
     struct coor* res = realloc(tab,(len+1)*sizeof(struct coor));
     res[len] = l;
     return res;
}

struct blob* append_blob(struct blob* tab,int len,struct blob l)
{
     struct blob* res = realloc(tab,(len+1)*sizeof(struct blob));
     res[len] = l;
     return res;
}

struct coor pop_coor(struct coor *tab,int len)
{
     struct coor res = tab[len];
     return res;
}

struct coor *suppr(struct coor *tab, int len)
{
     struct coor *res = realloc(tab,(len)*sizeof(struct coor));
     return res;
}

struct blob blobFromImage(SDL_Surface *img, struct blob **blobs)
{
     Uint32 pixel;
     Uint8 r,g,b;
     int max = 0;
     struct blob maxi = {0,0,0,0,0,0,0,0};

     int w = img->w;
     int h = img->h;
     double id = 1;

     double *matr = calloc(w*h,sizeof(double));

     struct coor *liste = calloc(0,sizeof(struct coor));
     int lenl = 0;
     int lenb = 0;

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

	       struct coor start = {i,j};
	       liste = append_coor(liste,lenl,start);
	       lenl+=1;
	       //append dans liste
	       int sum_x=0,sum_y=0,n_pixels=0,max_x=0,max_y=0;
	       int min_x = w+1, min_y=h+1;
	       while(lenl != 0)//liste pas vide)
	       {

		    //dequeue liste
		    lenl -= 1;
		    struct coor top = pop_coor(liste,lenl);
		    liste = suppr(liste,lenl);

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
			      pixel = get_pixel(img, l, k);
			      SDL_GetRGB(pixel, img->format, &r, &g, &b);
			      if(r == 0 || matr[l*h+k] > -1)
				   continue;

			      struct coor next = {l,k};
			      matr[l*h+k] = id;
			      //append next in liste
			      liste = append_coor(liste,lenl,next);
			      lenl += 1;
			 }
		    }
	       }
	       struct blob nextcentre =
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
	       *blobs = append_blob(*blobs,lenb,nextcentre);
	       lenb += 1;
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
     return maxi;
}
