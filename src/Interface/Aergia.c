#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

typedef struct Aergia
{
  GtkWidget *window;
  
  GtkWidget *hex_mode;
  GtkWidget *load_button;
  GtkWidget *step_button;
  GtkWidget *prev_button;
  GtkWidget *end_button;
  GtkWidget *quit_button;

  GtkWidget *box_buttons;
  GtkWidget *box_main;
  GtkWidget *box_visu;

  GtkWidget *image;
  int step;

  GtkWidget *status;

} Aergia;

void load(GtkButton* button, gpointer data)
{
    /*
      to load the image
    */
    button = button;
    GtkWidget *dialog;
    Aergia *aergia = data;

    GdkPixbuf *pixbuf;
    GtkWidget *empty;

    gint res;

    
    
    //File chooser dialog
    GtkFileFilter *filter = gtk_file_filter_new();

    gtk_file_filter_add_pixbuf_formats (filter);


    dialog = gtk_file_chooser_dialog_new ("Choose a sudoku image",
                                          NULL,
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          "Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          "Open",
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);


    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(dialog),filter);
    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);

	//hex mode 0=on , 2=off
	char *mode; 
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(aergia->hex_mode))== TRUE)
	  mode = "0";
	else
	  mode = "2";
      
        //treatment here
        char commande[200];
        sprintf(commande,"%s %s %s","./main", filename,mode);
        int res = system(commande);
	res = res;
        //

        empty = gtk_image_new_from_file("output/treatment/original.png");
        pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(empty));

        int width = gdk_pixbuf_get_width (pixbuf);
        int height = gdk_pixbuf_get_height (pixbuf);

        pixbuf = gdk_pixbuf_scale_simple(pixbuf, 650, 650*height/width, GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(aergia->image),pixbuf);



        aergia->step = 0;
        gtk_label_set_label(GTK_LABEL(aergia->status),"orignal");

        gtk_widget_set_sensitive(aergia->step_button,TRUE);
        gtk_widget_set_sensitive(aergia->prev_button,FALSE);
        gtk_widget_set_sensitive(aergia->end_button,TRUE);
        //free
        g_free (filename);

    }

    gtk_widget_destroy (dialog);


}

void next_step(GtkButton* button, gpointer data)
{
    Aergia *aergia = data;

    GdkPixbuf *pixbuf;
    GtkWidget *empty;
    int dir = 0;
    if (aergia->step_button == GTK_WIDGET(button))
        dir = 1;
    else
        dir = -1;


    char *filenames[] = {"original",
        "grayscale",
        "contrast",
        "brightness",
        "noise",
        "blur",
        "threshold",
        "sobel",
        "lines",
        "rotate",
        "scale",
        "perspective"};
    char filename[100];
    if ((aergia->step<11 && dir == 1) || (aergia->step>0 && dir == -1))
    {
        aergia->step += dir;
        gtk_label_set_label(GTK_LABEL(aergia->status),filenames[aergia->step]);

        sprintf(filename,"%s%s.png","output/treatment/", filenames[aergia->step]);

        empty = gtk_image_new_from_file(filename);
        pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(empty));

        int width = gdk_pixbuf_get_width (pixbuf);
        int height = gdk_pixbuf_get_height (pixbuf);

        pixbuf = gdk_pixbuf_scale_simple(pixbuf, 650, 650*height/width, GDK_INTERP_BILINEAR);
        gtk_image_set_from_pixbuf(GTK_IMAGE(aergia->image),pixbuf);
        if (aergia->step == 11)
	{
            gtk_widget_set_sensitive(GTK_WIDGET(aergia->step_button),FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(aergia->end_button),FALSE);
	}

    }
    if (aergia->step>= 11)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(aergia->step_button),FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(aergia->end_button),FALSE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(aergia->step_button),TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(aergia->end_button),TRUE);
    }
    if (aergia->step>0)
        gtk_widget_set_sensitive(GTK_WIDGET(aergia->prev_button),TRUE);
    else
        gtk_widget_set_sensitive(GTK_WIDGET(aergia->prev_button),FALSE);

}

void magic(GtkButton* button, gpointer data)
{
    button = button;
    Aergia *aergia = data;

    GdkPixbuf *pixbuf;
    GtkWidget *empty;

    aergia->step = 11;
    gtk_label_set_label(GTK_LABEL(aergia->status),"perspective");

    empty = gtk_image_new_from_file("output/treatment/perspective.png");
    pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(empty));

    int width = gdk_pixbuf_get_width (pixbuf);
    int height = gdk_pixbuf_get_height (pixbuf);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 650, 650*height/width, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(aergia->image),pixbuf);
    gtk_widget_set_sensitive(GTK_WIDGET(aergia->step_button),FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(aergia->end_button),FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(aergia->prev_button),TRUE);
}

void quit(GtkButton* button, gpointer data)
{
  Aergia *aergia = data;
  button = button;
  gtk_window_close(GTK_WINDOW(aergia->window));
}

int main(int argc, char **argv)
{
    int width = 1000;
    int height = 800;

    //GdkRGBA bgcolor = {0.40,0.60,0.63,1};
    const char* __asan_default_option() { return "detect_leaks = 0";}
    __asan_default_option();

    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *hex_mode;
    GtkWidget *load_button;
    GtkWidget *step_button;
    GtkWidget *prev_button;
    GtkWidget *end_button;
    GtkWidget *quit_button;
    
    GtkWidget *box_button;
    GtkWidget *box_main;
    GtkWidget *box_visu;
    int step = 1;
    GtkWidget *status = gtk_label_new_with_mnemonic("empty");


    //check box hexa
    hex_mode = gtk_check_button_new_with_label("Hexadecimal grid");

    
    //Window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Aergia 0.4");
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    //gtk_window_set_resizable(GTK_WINDOW(window),FALSE); //disable resize

    //SET BOXES
    box_button = gtk_box_new(GTK_ORIENTATION_VERTICAL, 50);
    box_main = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
    box_visu = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);

    //VISUALIZER

    GtkWidget *empty = gtk_image_new_from_file("empty2.png");

    //Icon
    GtkWidget *foldericon;
    foldericon = gtk_image_new_from_icon_name ("folder",GTK_ICON_SIZE_MENU);
    GtkWidget *gonexticon;
    gonexticon = gtk_image_new_from_icon_name ("go-next",GTK_ICON_SIZE_MENU);
    GtkWidget *goprevicon;
    goprevicon = gtk_image_new_from_icon_name ("go-previous",GTK_ICON_SIZE_MENU);
    GtkWidget *goendicon;
    goendicon = gtk_image_new_from_icon_name ("go-last",GTK_ICON_SIZE_MENU);
    GtkWidget *quiticon;
    quiticon = gtk_image_new_from_icon_name ("window-close",GTK_ICON_SIZE_MENU);
    //buttons
    
    load_button = gtk_button_new_with_label(" Load image");
    gtk_button_set_always_show_image (GTK_BUTTON(load_button),TRUE);
    gtk_button_set_image (GTK_BUTTON(load_button),foldericon);
    
    step_button = gtk_button_new_with_label(" Next Step");
    gtk_button_set_always_show_image (GTK_BUTTON(step_button),TRUE);
    gtk_button_set_image (GTK_BUTTON(step_button),gonexticon);
    
    prev_button = gtk_button_new_with_label(" Previous Step");
    gtk_button_set_always_show_image (GTK_BUTTON(prev_button),TRUE);
    gtk_button_set_image (GTK_BUTTON(prev_button),goprevicon);
    
    end_button = gtk_button_new_with_label(" Operate Magic");
    gtk_button_set_always_show_image (GTK_BUTTON(end_button),TRUE);
    gtk_button_set_image (GTK_BUTTON(end_button),goendicon);
    
    quit_button = gtk_button_new_with_label(" Quit");
    gtk_button_set_always_show_image (GTK_BUTTON(quit_button),TRUE);
    gtk_button_set_image (GTK_BUTTON(quit_button),quiticon);

    //names
    gtk_widget_set_name(load_button,"load_button");
    gtk_widget_set_name(step_button,"step_button");
    gtk_widget_set_name(prev_button,"prev_button");
    gtk_widget_set_name(end_button,"end_button");
    
    //size
    gtk_widget_set_size_request(GTK_WIDGET(load_button),100,50);
    gtk_widget_set_size_request(GTK_WIDGET(step_button),100,50);
    gtk_widget_set_size_request(GTK_WIDGET(prev_button),100,50);
    gtk_widget_set_size_request(GTK_WIDGET(end_button),100,50);
    gtk_widget_set_size_request(GTK_WIDGET(quit_button),100,50);

    
    //enabling/disabling 
    gtk_widget_set_sensitive(GTK_WIDGET(step_button),FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(prev_button),FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(end_button),FALSE);

    //boxing
    gtk_box_pack_end(GTK_BOX(box_button), quit_button, TRUE, FALSE, 70);
    gtk_box_pack_end(GTK_BOX(box_button), end_button, TRUE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(box_button), prev_button, TRUE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(box_button), step_button, TRUE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(box_button), load_button, TRUE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(box_button), hex_mode, TRUE, FALSE, 20);


    gtk_widget_set_size_request(GTK_WIDGET(empty),850,0);
    gtk_box_pack_start(GTK_BOX(box_visu), empty, TRUE, TRUE, 20);

    gtk_box_pack_end(GTK_BOX(box_visu),GTK_WIDGET(status), TRUE, TRUE, 20);

    gtk_container_add(GTK_CONTAINER(window),box_main);
    gtk_box_pack_start(GTK_BOX(box_main),box_button,TRUE,FALSE,50);
    gtk_box_pack_start(GTK_BOX(box_main),box_visu,TRUE,FALSE,50);

    //colors
    //gtk_widget_override_background_color (window,GTK_STATE_FLAG_NORMAL,&bgcolor);

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);


    //Aergia

    Aergia aergia = {window,
		     hex_mode,
		     load_button,
		     step_button,
		     prev_button,
		     end_button,
		     quit_button,
		     box_button,
		     box_main,
		     box_visu,
		     empty,
		     step,
		     status};

    //Top logo
    
    GtkWidget *logo = gtk_image_new_from_file("logo2.png");
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(logo));
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, width/75*36,width/75*12 , GDK_INTERP_BILINEAR);
    logo = gtk_image_new_from_pixbuf(pixbuf);
    gtk_box_pack_end(GTK_BOX(aergia.box_buttons), logo, TRUE, TRUE, 15);

    //Signals

    g_signal_connect(aergia.load_button,"clicked", G_CALLBACK(load), &aergia);
    g_signal_connect(aergia.end_button,"clicked", G_CALLBACK(magic), &aergia);
    g_signal_connect(aergia.prev_button,"clicked", G_CALLBACK(next_step), &aergia);
    g_signal_connect(aergia.step_button,"clicked", G_CALLBACK(next_step), &aergia);
    g_signal_connect(aergia.quit_button,"clicked", G_CALLBACK(quit), &aergia);

    g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit), NULL); //close window cleanly

    gtk_widget_show_all(aergia.window); //show the window

    gtk_main();

    return 0;
}
