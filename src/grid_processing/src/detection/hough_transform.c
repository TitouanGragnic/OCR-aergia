#include "../../include/detection/hough_transform.h"

void hough_transform(SDL_Surface *img, int *matrice)
{
     /*
       create space of hough in matrice
     */
     int w = img->w;
     int h = img -> h;
     Uint8 r,g,b;
     Uint32 pixel;

     for(int i =0 ;i<w;i++)
	  for(int j = 0;j<h;j++)
	  {
	       pixel = get_pixel(img, i, j);
	       SDL_GetRGB(pixel, img->format, &r, &g, &b);
	       if(r==255)
	       {
		    for(int theta = 0;theta<360;theta++)
		    {
			 double t = theta*(M_PI/180);
			 int p = i*cos(t)+j*sin(t);
			 if(p>0 && p<sqrt(w*w+h*h))
			      matrice[p*360+theta]+=1;
		    }
	       }
	  }
}

int find_max(int* matr,int len)
{
     /*
       return max of matr
      */
     int max = 0;
     for(int k = 0; k<360*len; k++)
	  max = max<matr[k]? matr[k] : max;
     return max;
}

struct line* find_line(int* matr,int len,int nb,int w,int h,int max)
{
     /*
       return list of lines
     */
     struct line* res = malloc(nb*sizeof(struct line));
     int i = 0;
     for(int p =0;p<len;p++)
	  for(int t = 0; t<360;t++)
	       if(matr[p*360+t]>max*0.6)
	       {
		    double theta = (double)t*(M_PI/180);
		    double k=-cos(theta)/sin(theta);
		    res[i].y1 = p/sin(theta);
		    res[i].y2 = k*w + p/sin(theta);
		    if(fabs(res[i].y1)<w*h && fabs(res[i].y2)<w*h)
		    {
			 res[i].x1 = 0;
			 res[i].x2 = w;
		    }
		    else
		    {
			 k = -sin(theta)/cos(theta);
			 res[i].x1 =  p/cos(theta);
			 res[i].x2 = k*h+ p/cos(theta);
			 res[i].y1 = 0;
			 res[i].y2 = h;
		    }
		    i+=1;
	       }
     return res;
}

struct line* append_line(struct line* tab,int len,struct line l)
{
     struct line* res = realloc(tab,(len+1)*sizeof(struct line));
     res[len] = l;
     return res;
}

int same_line(struct line l1,struct line l2)
{
     /*
       return l1~l2
      */
     return fabs(l1.x1-l2.x1)<50 && fabs(l1.x2-l2.x2)<50 &&
	  fabs(l1.y1-l2.y1)<50 && fabs(l1.y2-l2.y2)<50;
}

struct line* simplify_line(struct line* tab,int *len)
{
     /*
       merge line ~equale in tab and return the new tab
     */
     int* states = calloc(*len,sizeof(int));
     int size = 0;
     struct line* res= malloc(0);
     int i = 0;
     while(i<*len)
     {
	  if(!states[i])
	  {
	       int nb = 1;
	       struct line sum;
	       sum.x1=tab[i].x1;
	       sum.x2=tab[i].x2;
	       sum.y1=tab[i].y1;
	       sum.y2=tab[i].y2;
	       int j = i+1;
	       states[i] = 1;

	       while(j<*len)
	       {
		    if(!states[j] && same_line(tab[i],tab[j]))
		    {
			 nb++;
			 sum.x1+=tab[j].x1;
			 sum.x2+=tab[j].x2;
			 sum.y1+=tab[j].y1;
			 sum.y2+=tab[j].y2;
			 states[j] = 1;
		    }
		    j++;
	       }
	       sum.x1/=nb;
	       sum.x2/=nb;
	       sum.y1/=nb;
	       sum.y2/=nb;
	       res = append_line(res,size, sum);
	       size+=1;
	  }
	  i++;
     }
     free(tab);
     free(states);
     *len = size;
     return res;
}

void draw_line_polaire(SDL_Surface* img,int p, int t)
{
     /*
       draw line in img from polar coordonate
     */
     int w= img->w ;
     int h = img->h;
     int j;
     for(int i =0 ;i<w;i++){
	  double theta = (double)t*(M_PI/180);
	  double k = -cos(theta)/sin(theta);
	  double temp_p = p/sin(theta);
	  j =(int)(k*(double)i+temp_p) ;
	  if( j<h && j>=0 ){
	       dotBlue(img,w,h,i,j);
	  }
     }
}




int draw_lines_polaire(SDL_Surface* img, int* matr,int len,int var,int max)
{
     /*
       return number of line with value>threshold
       if var : draw these lines
     */
     int nb = 0;
     for(int p =0;p<len;p++)
	  for(int t = 0; t<360;t++)
	       if(matr[p*360+t]>max*0.6)
	       {
		    if(var)
			 draw_line_polaire(img,p, t);
		    nb++;
	       }
     return nb;
}

void draw_simple_line(SDL_Surface* img, struct line* tab,int len)
{
     /*
       draw each line of tab
     */
     for(int i =0;i<len;i++)
	  drawLine(img,tab[i].x1,tab[i].y1,tab[i].x2,tab[i].y2);

}
double __get_angle(int x1, int y1, int x2 , int y2)
{
     /*
       get angle of line regard of x axis
     */
     double a = abs(x1-x2);
     double b = abs(y1-y2);
     return atan(b/a);
}

int get_angle(struct line* tab,int len)
{
     /*
       get angle with most occurence in tab of line
     */
     int* histo = calloc(360,sizeof(int));
     for(int i = 0; i<len; i++)
	  histo[((int)(__get_angle(tab[i].x1,tab[i].y1,tab[i].x2,tab[i].y2)*180/M_PI))%90]+=1;
     int max=0;
     for(int i = 0; i<360; i++)
	  max = max<histo[i]? i :max;
     free(histo);
     return max;
}

SDL_Surface* hough_transform_rotate(SDL_Surface* edge_surface, SDL_Surface** bin_surface)
{
     /*
       rotate edge_surface and bin_surface regarding anlge of most paralele lines find
       with the space of hough transforme.
     */
     int w =edge_surface->w;
     int h =edge_surface->h;
     int lenght = sqrt(w*w+h*h);

     int *matr = malloc(lenght*360*sizeof(int));

     for(int i = 0 ; i < lenght*360; i++)
	  matr[i] = 0;

     hough_transform(edge_surface,matr);
     int max = find_max(matr, lenght);

     int nb = draw_lines_polaire(edge_surface, matr, lenght,0,max);
     struct line* lines = find_line( matr, lenght, nb,w,h,max);
     lines = simplify_line(lines,&nb);
     int alpha = get_angle(lines,nb);
     int angle = alpha%90;
     printf("%d\n",angle);
     if ((angle >= 70 && angle <= 110) || (angle >= -20 && angle <= 20))
	  alpha = 0;

     edge_surface = rotate(edge_surface,alpha,lines,nb,0);
     *bin_surface = rotate(*bin_surface,alpha,lines,nb,1);
     free(matr);
     free(lines);
     return edge_surface;
}

