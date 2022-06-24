#include <gtk/gtk.h>

static int *tmpresultX,*tmpresultY,msize;
static double **resultR,**resultG,**resultB; 

typedef struct{
 unsigned int *X;
 unsigned int *Y;
 unsigned int MSize;
} coordinate_matrix;  

struct matrix {
   int rows;
   int cols;
   double** matriks;
};

typedef struct{
 struct matrix matrix_R;
 struct matrix matrix_G;
 struct matrix matrix_B;
 coordinate_matrix Coordinate;
} color_coordinate_matrix;

static GtkWidget *del_jendela,*del_vbox,*del_hbox, *del_label[10],*del_gambar1;

GtkCssProvider *cssProvider;
GtkBuilder *builder;

//mainWindow
GtkWidget *window;
GtkWidget *box_cam;
GtkWidget *display_cam;
GtkWidget *lb_cam, *timer;
GtkWidget *bt_take_pic;

// Function Timer
gboolean timer_handler(GtkWidget *);

void gtk_mainWindow_init();
void gtk_mainWindow_setAttrib();
void gtk_mainWindow_connect();
void gtk_builder_and_attrib_init();

void init_gtk_component(int *argc, char ***argv){
	gtk_init(argc, argv);
	
	//setup
	gtk_builder_and_attrib_init();

	gtk_mainWindow_init();
	gtk_mainWindow_connect();
	gtk_mainWindow_setAttrib();
	
    //~ del_jendela = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    //~ //gtk grid========================================
    //~ del_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    //~ del_hbox = gtk_grid_new();
    //~ gtk_grid_set_column_homogeneous(GTK_GRID(del_hbox), TRUE);
    //~ const char *format = "<span style=\"normal\" font_desc=\"Sans 17\" foreground=\"#000000\" >\%s</span>";
    //~ char *markup;
	//~ //------------------
    //~ del_label[1] = gtk_label_new("");
    //~ markup = g_markup_printf_escaped(format, "what the Program see");
	//~ gtk_label_set_markup(GTK_LABEL(del_label[1]), markup);
	//~ g_free(markup);
    //================================================
}

void show_gtk_component(GdkPixbuf *pix1){
	display_cam = gtk_image_new_from_pixbuf(pix1);
	//~ gtk_grid_attach(GTK_GRID(del_hbox), del_gambar1,  0, 0, 1, 1);
    //~ gtk_grid_attach(GTK_GRID(del_hbox), del_label[1],  0, 1, 1, 1);
    //~ gtk_box_pack_start(GTK_BOX(del_vbox), del_hbox, TRUE, TRUE, 0);

    //~ gtk_container_add(GTK_CONTAINER(del_jendela), del_vbox);
    
    //~ gtk_window_set_position(GTK_WINDOW(del_jendela), GTK_WIN_POS_CENTER);
    //~ gtk_widget_show_all (del_jendela);
    //~ g_signal_connect (del_jendela, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    //~ gtk_main ();
}

GdkPixbuf* put_pixel_to_pixbuf(GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha)
{
  int width, height, rowstride, n_channels;
  guchar *pixels, *p;

  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);
  n_channels = gdk_pixbuf_get_n_channels (pixbuf);

  p = pixels + y * rowstride + x * n_channels;
    p[0] = red;
    p[1] = green;
    p[2] = blue;
    p[3] = alpha;
    return pixbuf;
}

static GdkPixbuf* load_rgb_matrix_data_from_stream(color_coordinate_matrix ccordinate){
	GdkColorspace colorspace = GDK_COLORSPACE_RGB;
	GdkPixbuf* pixbuf;
	pixbuf = gdk_pixbuf_new(colorspace, TRUE, 8, ccordinate.matrix_R.cols,ccordinate.matrix_R.rows);
	double matrixR[ccordinate.matrix_R.rows*ccordinate.matrix_R.cols];
	double matrixG[ccordinate.matrix_R.rows*ccordinate.matrix_R.cols];
	double matrixB[ccordinate.matrix_R.rows*ccordinate.matrix_R.cols];
	int i,j,k;
	int pixel_height,pixel_width,startx,endx;
	int x,y,r,g,b;
	
	pixbuf = gdk_pixbuf_new(colorspace, TRUE, 8, ccordinate.matrix_R.cols,ccordinate.matrix_R.rows);
	
	k = 0;
	for (i=1;i<ccordinate.matrix_R.rows;i++){
		for (j=0;j<ccordinate.matrix_R.cols;j++){
			k++;
			matrixR[k] = ccordinate.matrix_R.matriks[i][j];
			matrixG[k] = ccordinate.matrix_G.matriks[i][j];
			matrixB[k] = ccordinate.matrix_B.matriks[i][j];
		}
	}
	
	
	pixel_width = ccordinate.matrix_R.cols;
	pixel_height = ccordinate.matrix_R.rows;
	endx = pixel_width;
	
	for (int y = 0;y < (pixel_height);y++){
		endx = y*pixel_width;
		startx = endx-pixel_width+1;	
		for (int i = startx;i < (endx+1);i++){
			if ((ccordinate.Coordinate.X[i] >= 0)&(ccordinate.Coordinate.Y[i] >= 0)){
				x = pixel_width+i-endx;
				r = matrixR[i];
				g = matrixG[i];
				b = matrixB[i];
				pixbuf = put_pixel_to_pixbuf(pixbuf, x, y, r, g, b, 255);
			}
		}	
	}
	return pixbuf;
}

/*	*/

gboolean timer_handler(GtkWidget *timer){
	char tmp[1024];
	time_t t = time(0);
	gtk_label_set_text(GTK_LABEL(timer), ctime(&t));	// update time of day

	gtk_widget_queue_draw (display_cam);

	return TRUE; // False kills the timer
}

void gtk_builder_and_attrib_init(){
	builder=gtk_builder_new();
	gtk_builder_add_from_file (builder, "WebCam.glade", NULL);

	cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "style.css", NULL);
}

void gtk_mainWindow_init(){
	window=GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
	box_cam=GTK_WIDGET(gtk_builder_get_object(builder, "box_cam"));
	
	display_cam=GTK_WIDGET(gtk_builder_get_object(builder, "display_cam"));
	
	//label
	lb_cam=GTK_WIDGET(gtk_builder_get_object(builder, "lb_cam"));
	timer=GTK_WIDGET(gtk_builder_get_object(builder, "timer"));
	//button
	bt_take_pic=GTK_WIDGET(gtk_builder_get_object(builder, "bt_take_pic"));

} 

void gtk_mainWindow_setAttrib(){
	//~ gtk_window_fullscreen(GTK_WINDOW(window));

	gtk_widget_set_name (window, "myWindow");
	gtk_widget_set_name (box_cam, "box_cam");
	gtk_widget_set_name (display_cam, "display_cam");
	gtk_widget_set_name(lb_cam, "lb_cam");
	gtk_widget_set_name(timer, "timer");
	gtk_widget_set_name(bt_take_pic, "bt_take_pic");

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
}

void gtk_mainWindow_connect(){
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

char* gabung_kata(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *IntToStr(int x){
	char *str=(char *)malloc(10 * sizeof (char));
	sprintf(str, "%d", x);
	return str;
}
