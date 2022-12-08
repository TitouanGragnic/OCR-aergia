#include "../../include/detection/hough_transform.h"

void hough_transform(SDL_Surface *img, int *matrice)
{
    /*
      create space of hough in matrice
    */
    int w = img->w;
    int h = img -> h;

    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            if (pixel_value(img, i, j) == 255)
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

list_l* find_line(int* matr,int len,int w,int h,int max)
{
    /*
      return list of lines
    */
    list_l* res = NULL;
    for(int p =0;p<len;p++)
        for(int t = 0; t<360;t++)
            if(matr[p*360+t]>max*0.6)
            {
                line tmp;
                double theta = (double)t*(M_PI/180);
                double k=-cos(theta)/sin(theta);
                tmp.y1 = p/sin(theta);
                tmp.y2 = k*w + p/sin(theta);
                if(fabs(tmp.y1)<w*h && fabs(tmp.y2)<w*h)
                {
                    tmp.x1 = 0;
                    tmp.x2 = w;
                }
                else
                {
                    k = -sin(theta)/cos(theta);
                    tmp.x1 =  p/cos(theta);
                    tmp.x2 = k*h+ p/cos(theta);
                    tmp.y1 = 0;
                    tmp.y2 = h;
                }
                res = append_linked_list_l(res,tmp);
            }
    return res;
}

int same_line(line l1, line l2)
{
    /*
      return l1~l2
    */
    return fabs(l1.x1-l2.x1)<50 && fabs(l1.x2-l2.x2)<50 &&
        fabs(l1.y1-l2.y1)<50 && fabs(l1.y2-l2.y2)<50;
}

list_l* simplify_line(list_l* tab)
{
    /*
      merge line ~equale in tab and return the new tab
    */
    list_l* tmp = tab;
    while(tmp != NULL)
    {
        int nb = 1;
        line sum;
        sum.x1=tmp->value.x1;
        sum.x2=tmp->value.x2;
        sum.y1=tmp->value.y1;
        sum.y2=tmp->value.y2;
        list_l* pred = tmp;
        list_l* tmp2 = tmp->next;
        while(tmp2 != NULL)
        {
            if(same_line(tmp->value,tmp2->value))
            {
                nb++;
                sum.x1+=tmp2->value.x1;
                sum.x2+=tmp2->value.x2;
                sum.y1+=tmp2->value.y1;
                sum.y2+=tmp2->value.y2;
                pred->next = tmp2->next;
                free(tmp2);
                tmp2 = pred->next;
            }
            else
            {
                pred = tmp2;
                tmp2 = tmp2->next;
            }
        }
        sum.x1/=nb;
        sum.x2/=nb;
        sum.y1/=nb;
        sum.y2/=nb;
        tmp->value = sum;
        tmp = tmp->next;
    }
    return tab;
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

void draw_simple_line(SDL_Surface* img, list_l* tab)
{
    /*
      draw each line of tab
    */
    list_l* tmp = tab;
    while (tmp != NULL)
    {
        line l = tmp->value;
        drawLine(img,l.x1,l.y1,l.x2,l.y2);
        tmp = tmp->next;
    }
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

int get_angle(list_l* tab)
{
    /*
      get angle with most occurence in tab of line
    */
    int* histo = calloc(360,sizeof(int));
    while(tab != NULL)
    {
        histo[((int)(__get_angle(tab->value.x1,tab->value.y1,
                                 tab->value.x2,tab->value.y2)*180/M_PI))%90]+=1;
        tab = tab->next;
    }
    int max=0;
    for(int i = 0; i<360; i++)
        max = max<histo[i]? i :max;
    free(histo);
    return max;
}

SDL_Surface* hough_transform_rotate(SDL_Surface* edge_surface, SDL_Surface** bin_surface, img_thread* data)
{
    /*
      rotate edge_surface and bin_surface regarding anlge of most paralele lines find
      with the space of hough transforme.
    */
    int w = edge_surface->w;
    int h = edge_surface->h;
    int lenght = sqrt(w*w+h*h);

    int *matr = malloc(lenght * 360 * sizeof(int));
    SDL_Surface * step = copy_surface(edge_surface);


    for (int i = 0 ; i < lenght * 360; i++)
        matr[i] = 0;

    hough_transform(edge_surface, matr);
    int max = find_max(matr, lenght);

    //int nb = draw_lines_polaire(edge_surface, matr, lenght,0,max);
    list_l* lines = find_line(matr, lenght, w, h, max);
    lines = simplify_line(lines);

    draw_simple_line(step, lines);
    make_thread(data, step, "output/treatment/lines.png");

    int alpha = get_angle(lines);
    int angle = alpha%90;
    if ((angle >= 70 && angle <= 110) || (angle >= -20 && angle <= 20))
        alpha = 0;

    edge_surface = rotate(edge_surface, alpha, 0);
    *bin_surface = rotate(*bin_surface, alpha, 1);
    free(matr);
    free_linked_list_l(lines);
    SDL_FreeSurface(step);
    return edge_surface;
}
