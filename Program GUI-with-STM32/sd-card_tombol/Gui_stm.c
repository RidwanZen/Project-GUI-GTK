// Compile : gcc -Wno-format -o Gui_stm Gui_stm.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
// Run : ./Gui_stm

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

void send_data(char* a);
char input[1000];

const int screenWidth = 3000, screenHight = 3000;
int main_box_space, main_nbox_space, main_box_side_margin, main_box_main_width;
int logo_width, logo_hight;
int imBack_width, imBack_hight;
int main_box_label_margin;

GtkCssProvider *cssProvider;
GtkBuilder *builder;

//mainWindow
GtkWidget *window;
GtkWidget *main_box, *main_grid_led, *main_grid_soal, *main_box_entry;
GtkWidget *bt_led, *bt_soal_1, *bt_soal_2, *bt_soal_3, *bt_soal_4, *bt_soal_5, *bt_hello;
GtkWidget *lb_led, *lb_soal, *lb_input;
GtkWidget *entry_text;

void gtk_mainWindow_init();
void gtk_mainWindow_setAttrib();
void gtk_mainWindow_connect();
void gtk_builder_and_attrib_init();
void gtk_image_opt(GtkWidget *widget, char *fileName, int width, int hight, int margin_left);

int main(int argc, char **argv){
	/*deklarasi variabel
	 * ====================================*/
	main_box_space = screenWidth/100;
	main_box_side_margin = screenWidth/50;
	main_box_main_width = screenWidth/60;
	main_nbox_space = screenWidth/200;
	main_box_label_margin = screenHight/8;

	logo_width = screenWidth/8;
	logo_hight = screenHight/15;
	imBack_width = screenWidth/100;
	imBack_hight = screenHight/40;
	/*=====================================*/
	gtk_init(&argc, &argv);
	/*main code
	 * ====================================*/
	//setup
	gtk_builder_and_attrib_init();

	gtk_mainWindow_init();
	gtk_mainWindow_connect();
	gtk_mainWindow_setAttrib();
	
	//start
	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show(window);
	g_object_unref(builder);
	gtk_main();
	return 0;
}

void gtk_builder_and_attrib_init(){
	builder=gtk_builder_new();
	gtk_builder_add_from_file (builder, "GUI_Stm32.glade", NULL);

	cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "style.css", NULL);
}


void gtk_image_opt(GtkWidget *widget, char *fileName, int width, int hight, int margin_left){
	GdkPixbuf *my_img = gdk_pixbuf_new_from_file(fileName, NULL); //load picture from file 'filename'
	my_img = gdk_pixbuf_scale_simple(my_img, width, hight, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(widget), my_img);
	gtk_widget_set_margin_start(widget, margin_left);
}

void gtk_mainWindow_init(){
	window=GTK_WIDGET(gtk_builder_get_object(builder, "windowMain"));
	main_box=GTK_WIDGET(gtk_builder_get_object(builder, "main_box"));
	main_box_entry=GTK_WIDGET(gtk_builder_get_object(builder, "main_box_entry"));
	main_grid_led=GTK_WIDGET(gtk_builder_get_object(builder, "main_grid_led"));
	main_grid_soal=GTK_WIDGET(gtk_builder_get_object(builder, "main_grid_soal"));

	//button
	bt_led=GTK_WIDGET(gtk_builder_get_object(builder, "bt_led"));
	bt_soal_1=GTK_WIDGET(gtk_builder_get_object(builder, "bt_soal_1"));
	bt_soal_2=GTK_WIDGET(gtk_builder_get_object(builder, "bt_soal_2"));
	bt_soal_3=GTK_WIDGET(gtk_builder_get_object(builder, "bt_soal_3"));
	bt_soal_4=GTK_WIDGET(gtk_builder_get_object(builder, "bt_soal_4"));
	bt_soal_5=GTK_WIDGET(gtk_builder_get_object(builder, "bt_soal_5"));
	bt_hello=GTK_WIDGET(gtk_builder_get_object(builder, "bt_hello"));
	
	entry_text=GTK_WIDGET(gtk_builder_get_object(builder, "entry_text"));
	
	//label
	lb_led=GTK_WIDGET(gtk_builder_get_object(builder, "lb_led"));
	lb_soal=GTK_WIDGET(gtk_builder_get_object(builder, "lb_soal"));
	lb_input=GTK_WIDGET(gtk_builder_get_object(builder, "lb_input"));

} 

void gtk_mainWindow_setAttrib(){
	//~ gtk_window_fullscreen(GTK_WINDOW(window));

	gtk_widget_set_name (window, "myWindow");
	gtk_widget_set_name (main_box, "main_box");
	gtk_widget_set_name (bt_led, "bt_led");
	gtk_widget_set_name (bt_soal_1, "bt_soal_1");
	gtk_widget_set_name (bt_soal_2, "bt_soal_2");
	gtk_widget_set_name (bt_soal_3, "bt_soal_3");
	gtk_widget_set_name (bt_soal_4, "bt_soal_4");
	gtk_widget_set_name (bt_soal_5, "bt_soal_5");
	gtk_widget_set_name (bt_hello, "bt_hello");
	gtk_widget_set_name (entry_text, "entry_text");
	gtk_widget_set_name (main_box, "main_box");
	gtk_widget_set_name (main_box_entry, "main_box_entry");
	gtk_widget_set_name (main_grid_led, "main_grid_led");
	gtk_widget_set_name (main_grid_soal, "main_grid_soal");
	gtk_widget_set_name(lb_led, "lb_led");
	gtk_widget_set_name(lb_soal, "lb_soal");
	gtk_widget_set_name(lb_input, "lb_input");

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	int gspace_row = screenWidth/60; 
	int gspace_col = screenWidth/60;
	gtk_grid_set_row_spacing(GTK_GRID(main_grid_soal), gspace_row);
	gtk_grid_set_column_spacing(GTK_GRID(main_grid_soal), gspace_col);
	gtk_box_set_spacing(GTK_BOX(main_box), gspace_row);
	gtk_widget_set_margin_top(main_box, main_box_space);
	gtk_widget_set_margin_bottom(main_box, main_box_space);
	gtk_widget_set_margin_start(main_box, main_box_side_margin);
	gtk_widget_set_margin_end(main_box, main_box_side_margin);
}

void gtk_mainWindow_connect(){
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	//~ g_signal_connect(bt_led, "clicked", G_CALLBACK (gtk_main_quit), NULL);
}

void on_LED_toggled (GtkCheckButton *b)
{
	gboolean T = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
	if (T) {
		send_data("a");
		gtk_button_set_label (GTK_BUTTON (bt_led), "ON");
		}
	else {
		send_data("b");
		gtk_button_set_label (GTK_BUTTON (bt_led), "OFF");
	}
}

void on_entry_changed (GtkEntry *e) {
	sprintf(input, "%s", gtk_entry_get_text(e));
	send_data(input);
}

void on_button_ok_clicked (GtkButton *b) {
	send_data("H");
	}

void on_button_soal1_clicked (GtkButton *b) {
	send_data("1");
	
	}
	
void on_button_soal2_clicked (GtkButton *b) {
	send_data("2");
	
	}
	
void on_button_soal3_clicked (GtkButton *b) {
	send_data("3");
	
	}
	
void on_button_soal4_clicked (GtkButton *b) {
	send_data("4");
	
	}
	
void on_button_soal5_clicked (GtkButton *b) {
	send_data("5");
	
	}

void send_data (char *a){
	int fd, len;
	char text[255];
	struct termios options; /* Serial ports setting */

	fd = open("/dev/ttyACM0", O_RDWR | O_NDELAY | O_NOCTTY);
	if (fd < 0) {
		perror("Error opening serial port");
		
	}

	/* Read current serial port settings */
	// tcgetattr(fd, &options);
	
	/* Set up serial port */
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	/* Apply the settings */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);

	/* Write to serial port */
	strcpy(text, a);
	len = strlen(text);
	len = write(fd, text, len);
	

	close(fd);
	}
