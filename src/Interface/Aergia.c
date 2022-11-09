#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

void load(GtkButton* button, gpointer data)
{
  /*
    to load the image
   */
  GtkWidget *dialog;
  GtkWidget *empty;
  empty = (GtkWidget*)data;

  GdkPixbuf *pixbuf;
  GtkWidget *image;
  
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File",
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
					"Cancel",
					GTK_RESPONSE_CANCEL,
					"Open",
					GTK_RESPONSE_ACCEPT,
					NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
      filename = gtk_file_chooser_get_filename (chooser);
      
      image = gtk_image_new_from_file(filename);
      pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
      pixbuf = gdk_pixbuf_scale_simple(pixbuf, 600, 600, GDK_INTERP_BILINEAR);
      gtk_image_set_from_pixbuf(GTK_IMAGE(empty),pixbuf);
      //free
      g_free (filename);
      
    }

  gtk_widget_destroy (dialog);
  

}

void side_buttons(GtkWidget *box,int w, int h,GtkWidget *empty)//contains logo too
{
  /*
  creates the buttons
   */
  GtkWidget *button1;
  GtkWidget *button2;
  
  GtkWidget *logo = gtk_image_new_from_file("logo.png");
  GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(logo));
  
  //buttons part
  
  button1 = gtk_button_new_with_label("Load image");
  button2 = gtk_button_new_with_label("Operate Magic");

  gtk_widget_set_size_request(GTK_WIDGET(button1),200,50);
  gtk_widget_set_size_request(GTK_WIDGET(button2),200,50);

  gtk_box_pack_end(GTK_BOX(box), button2, TRUE, FALSE, 50);
  gtk_box_pack_end(GTK_BOX(box), button1, TRUE, FALSE, 50);

  //loader
  g_signal_connect(button1,"clicked", G_CALLBACK(load), (gpointer)empty);

  //logo part

  pixbuf = gdk_pixbuf_scale_simple(pixbuf, w/75*24,w/75*7 , GDK_INTERP_BILINEAR);
  logo = gtk_image_new_from_pixbuf(pixbuf);
  gtk_box_pack_end(GTK_BOX(box), logo, TRUE, TRUE, 50);
}


void visualizer(GtkWidget *box)
{
  /*
    creates the slider
   */
  GtkWidget *angleslider;
  angleslider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,360,1); 
  gtk_widget_set_size_request(GTK_WIDGET(angleslider),600,50);
  gtk_box_pack_end(GTK_BOX(box),angleslider,TRUE,TRUE,50);
}

void resize_image()
{
  /*
    work in progress (dynamic resize)
   */
  //gdk_pixbuf_scale_simple(pixbuf, w, h, GDK_INTERP_BILINEAR);
}


int main(int argc, char **argv)
{
  const char *title;
  int width = 1000;
  int height = 800;
  
  gtk_init(&argc, &argv);

  GtkWidget *window; //declare window

  GtkWidget *box_buttons;
  GtkWidget *box_main;
  GtkWidget *box_visu;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  gtk_window_set_title(GTK_WINDOW(window),"Aergia 0.1");//change window title here
  gtk_window_set_default_size(GTK_WINDOW(window), width, height);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  //gtk_window_set_resizable(GTK_WINDOW(window),FALSE); //disable resize

  //title = gtk_window_get_title(GTK_WINDOW(window)); //get title
  //gtk_window_get_size(GTK_WINDOW(window), &width, &height);

  //SET BOXES
  box_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
  box_main = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
  box_visu = gtk_box_new(GTK_ORIENTATION_VERTICAL,100);

  //VISUALIZER

  GtkWidget *empty = gtk_image_new_from_file("empty.png");
  //GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(empty)); //if resize needed
  //logo = gtk_image_new_from_pixbuf(pixbuf);
  gtk_widget_set_size_request(GTK_WIDGET(empty),650,550);
  gtk_box_pack_start(GTK_BOX(box_visu), empty, TRUE, TRUE, 50);
  
  visualizer(box_visu);
  
  
  //BUTTONS
  side_buttons(box_buttons,width,height,empty);
 
  //main box
  gtk_container_add(GTK_CONTAINER(window),box_main);
  gtk_box_pack_start(GTK_BOX(box_main),box_buttons,TRUE,TRUE,50);
  gtk_box_pack_start(GTK_BOX(box_main),box_visu,TRUE,TRUE,50);

  //gtk_box_set_homogeneous(GTK_BOX(box_buttons), TRUE);

  g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit), NULL); //close window cleanly
  //g_signal_connect(container, "size-allocate", G_CALLBACK(resize_image), NULL);//resize     //##W.I.P
  
  gtk_widget_show_all(window); //show the window

  gtk_main();

  return 0;
}

