// Compile : gcc -Wno-format -o WebCam WebCam.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
// Run : ./WebCam

//~ #include <gtk/gtk.h>
//~ #include <stdio.h>
//~ #include <string.h>
//~ #include <time.h>

#define _GNU_SOURCE

#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <time.h>
#include <sys/mman.h>

const int screenWidth = 1024, screenHight = 720;
int main_box_space, main_nbox_space, main_box_side_margin, main_box_main_width;
int logo_width, logo_hight;
int imBack_width, imBack_hight;
int main_box_label_margin;

GtkCssProvider *cssProvider;
GtkBuilder *builder;

//mainWindow
GtkWidget *window;
GtkWidget *box_cam;
GtkWidget *display_cam;
GtkWidget *lb_cam, *timer;

// Function Timer
gboolean timer_handler(GtkWidget *);
int cpuUtil, cpu[100];

void gtk_mainWindow_init();
void gtk_mainWindow_setAttrib();
void gtk_mainWindow_connect();
void gtk_builder_and_attrib_init();
 
int main(int argc, char **argv){
	for (int i = 0; i< 100; i++ ) cpu[i] = 0;
	/*deklarasi variabel
	 * ====================================*/
	/*=====================================*/
	gtk_init(&argc, &argv);
	/*main code
	 * ====================================*/
	//setup
	gtk_builder_and_attrib_init();

	gtk_mainWindow_init();
	gtk_mainWindow_connect();
	gtk_mainWindow_setAttrib();

	g_timeout_add_seconds(1, (GSourceFunc) timer_handler, timer);
	
	//Time units smaller than seconds are possible (higher_overhead);
	// guint ID = g_timeout_add(250, (GSourceFunc) timer_handler, data_pointer);
	
	//start
	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show(window);
	g_object_unref(builder);
	gtk_main();
	return 0;
}

gboolean timer_handler(GtkWidget *timer){
	char tmp[1024];
	time_t t = time(0);
	gtk_label_set_text(GTK_LABEL(timer), ctime(&t));	// update time of day
	
	static long time1 = 0, time2 = 0;
	static int flag = 0;
	char line[128], dummy[32];

	FILE *p1 = popen("head -1 /proc/stat","r");

	fgets(line,128,p1);
	sscanf(line,"%s %ld", dummy, &time2);
	fclose(p1);

	if (! flag) {
		flag = 1;
		time1 = time2;
		return TRUE;
		}

	cpuUtil = time2 - time1;

	for (int i = 99; i > 0; i--) cpu[i] = cpu[i-1];

	cpu[0] = cpuUtil/8;
	printf("percent of cpu utilization = %ld%%\n", cpuUtil );
	time1 = time2;
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

} 

void gtk_mainWindow_setAttrib(){
	//~ gtk_window_fullscreen(GTK_WINDOW(window));

	gtk_widget_set_name (window, "myWindow");
	gtk_widget_set_name (box_cam, "box_cam");
	gtk_widget_set_name (display_cam, "display_cam");
	gtk_widget_set_name(lb_cam, "lb_cam");
	gtk_widget_set_name(timer, "timer");

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
}

void gtk_mainWindow_connect(){
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}


gboolean on_draw1_draw (GtkWidget *widget, cairo_t *cr, gpointer data) {

	guint width, height;

	width = gtk_widget_get_allocated_width (widget);   // of draw window#FF0000#040404#FFFFFF
	height = gtk_widget_get_allocated_height (widget); // of draw window

	cairo_set_line_width(cr, 1.0);
			
	for (int i = 0; i < 99; i++) {
		cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // red
		cairo_move_to (cr, (double) ((i * 4) + 10), (double) (2 * (100 - cpu[i]) + 10) );
		cairo_line_to (cr, (double) (((i + 1) * 4) + 10), (double) (2 * (100 - cpu[i + 1]) +10) );
		cairo_stroke (cr);
		}

	return FALSE;
	}
