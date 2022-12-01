#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

typedef struct Aergia
{
  GtkWidget *window; 
  
  GtkWidget *load_button;
  GtkWidget *step_button;
  GtkWidget *end_button;
  
  GtkWidget *box_buttons;
  GtkWidget *box_main;
  GtkWidget *box_visu;

  GtkWidget *image;
  
} Aergia;

void load(GtkButton* button, gpointer data)
{
  /*
    to load the image
   */
  GtkWidget *dialog;
  Aergia *aergia = data;

  GdkPixbuf *pixbuf;
  GtkWidget *empty;
  
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
      
      empty = gtk_image_new_from_file(filename);
      pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(empty));
      pixbuf = gdk_pixbuf_scale_simple(pixbuf, 650, 650, GDK_INTERP_BILINEAR);
      gtk_image_set_from_pixbuf(GTK_IMAGE(aergia->image),pixbuf);
      //treatment here


      gtk_widget_set_sensitive(aergia->step_button,TRUE);
      
      //free
      g_free (filename);
      
    }

  gtk_widget_destroy (dialog);
  

}

void side_buttons(Aergia aergia,int w, int h)//contains logo too
{
  /*
  creates the buttons
   */
  
  GtkWidget *logo = gtk_image_new_from_file("logo.png");
  GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(logo));
  
  //old buttons part pos
  
 
  //loader
  

  //logo part

  pixbuf = gdk_pixbuf_scale_simple(pixbuf, w/75*24,w/75*7 , GDK_INTERP_BILINEAR);
  logo = gtk_image_new_from_pixbuf(pixbuf);
  gtk_box_pack_end(GTK_BOX(aergia.box_buttons), logo, TRUE, TRUE, 50);

 
  
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
  int width = 1000;
  int height = 800;
  
  gtk_init(&argc, &argv);
  
  GtkWidget *window;
  GtkWidget *load_button;
  GtkWidget *step_button;
  GtkWidget *end_button;
  GtkWidget *box_button;
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
  box_button = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
  box_main = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
  box_visu = gtk_box_new(GTK_ORIENTATION_VERTICAL,100);

  //VISUALIZER

  GtkWidget *empty = gtk_image_new_from_file("empty.png");

  //gtk_box_set_homogeneous(GTK_BOX(box_buttons), TRUE);

  //buttons
  load_button = gtk_button_new_with_label("Load image");
  step_button = gtk_button_new_with_label("Next Step");
  end_button = gtk_button_new_with_label("Operate Magic");

  gtk_widget_set_size_request(GTK_WIDGET(load_button),200,50);
  gtk_widget_set_size_request(GTK_WIDGET(step_button),200,50);
  gtk_widget_set_size_request(GTK_WIDGET(end_button),200,50);

  gtk_widget_set_sensitive(GTK_WIDGET(step_button),FALSE);

  gtk_box_pack_end(GTK_BOX(box_button), end_button, TRUE, FALSE, 50);
  gtk_box_pack_end(GTK_BOX(box_button), step_button, TRUE, FALSE, 50);
  gtk_box_pack_end(GTK_BOX(box_button), load_button, TRUE, FALSE, 50);


  gtk_widget_set_size_request(GTK_WIDGET(empty),650,550);
  gtk_box_pack_start(GTK_BOX(box_visu), empty, TRUE, TRUE, 50);

  gtk_container_add(GTK_CONTAINER(window),box_main);
  gtk_box_pack_start(GTK_BOX(box_main),box_button,TRUE,TRUE,50);
  gtk_box_pack_start(GTK_BOX(box_main),box_visu,TRUE,TRUE,50);
  
  //Aergia

  Aergia aergia = {window,
		   load_button,
		   step_button,
		   end_button,
		   box_button,
		   box_main,
		   box_visu,
		   empty};

  //GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(empty)); //if resize needed
  //logo = gtk_image_new_from_pixbuf(pixbuf);
  
  //BUTTONS
  side_buttons(aergia,width,height);

  g_signal_connect(aergia.load_button,"clicked", G_CALLBACK(load), &aergia);
  
  g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit), NULL); //close window cleanly
  //g_signal_connect(container, "size-allocate", G_CALLBACK(resize_image), NULL);//resize     //##W.I.P
  
  
  gtk_widget_show_all(aergia.window); //show the window

  gtk_main();

  return 0;
}

