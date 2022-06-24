//compile gcc `pkg-config --cflags gtk+-3.0`  `pkg-config --libs gtk+-3.0` -ljpeg -DIO_READ -DIO_MMAP -DIO_USERPTR cobakamera.c -o cobakamera
// gcc -Wno-format -o cobakamera cobakamera.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic -ljpeg -DIO_READ -DIO_MMAP -DIO_USERPTR
#include "v4l2grab.c"

GdkPixbuf *cobapixbuf;
color_coordinate_matrix dataRGB;

char *namafile;
int count = 0;

gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data)
{
	
    //~ cobapixbuf = gdk_pixbuf_new_from_file("coba1.jpg",NULL);
	dataRGB = process_frame();
	cobapixbuf = load_rgb_matrix_data_from_stream(dataRGB);
       
	gdk_cairo_set_source_pixbuf(cr, cobapixbuf, 0, 0);
	cairo_paint(cr);
    cairo_stroke(cr);
    
    gtk_widget_queue_draw(display_cam);
    g_object_unref(cobapixbuf);
    
    return FALSE;
}

int main (int argc, char *argv[])
{
	init_gtk_component(&argc, &argv);
	
	//~ GtkWidget *window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	//~ g_signal_connect( window, "destroy", G_CALLBACK (gtk_main_quit), NULL );
	
	//~ GtkWidget *cobadraw = gtk_drawing_area_new();
	
	tmpresultX = calloc((height*width)+1,sizeof(int));
	tmpresultY = calloc((height*width)+1,sizeof(int));
	resultR = calloc(height+1,sizeof(double));
	resultG = calloc(height+1,sizeof(double));
	resultB = calloc(height+1,sizeof(double));
	for(int i = 0;i < height;i++){
		resultR[i] = calloc(width+1,sizeof(double));
		resultG[i] = calloc(width+1,sizeof(double));
		resultB[i] = calloc(width+1,sizeof(double));
	}
	
	deviceName = "/dev/video0";
	// open and initialize device
	deviceOpen();
	deviceInit();
	// start capturing
	captureStart();
	
	g_timeout_add_seconds(1, (GSourceFunc) timer_handler, timer);
	//~ gtk_widget_add_events( cobadraw, GDK_BUTTON_PRESS_MASK );
	gtk_widget_set_size_request( display_cam, 640, 480 );
	g_signal_connect(display_cam, "draw", G_CALLBACK(draw_cb), NULL );
	
	//~ gtk_container_add( GTK_CONTAINER (window), cobadraw);
	//~ gtk_widget_show(cobadraw);
	//~ gtk_widget_show(window);
	//~ gtk_main();
	
	//start
	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show(window);
	g_object_unref(builder);
	gtk_main();
	
	/*free memory*/
	free(tmpresultX),tmpresultX = NULL;
	free(tmpresultY),tmpresultY = NULL;
	for(int i = 0;i < height;i++){
		free(resultR[i]),resultR[i] = NULL;
		free(resultG[i]),resultG[i] = NULL;
		free(resultB[i]),resultB[i] = NULL;
	}
	// stop capturing
	captureStop();
	
	// close device
	deviceUninit();
	deviceClose();
	
	return 0;
}

void clicked_take_picture (GtkButton *b) {
	count++;
	dataRGB = process_frame();
		
		cobapixbuf = load_rgb_matrix_data_from_stream(dataRGB);
	namafile = gabung_kata(gabung_kata("Pic",IntToStr(count)),".jpg");
	gdk_pixbuf_save (cobapixbuf, namafile, "jpeg", NULL, "quality", "100", NULL);
	}
	
