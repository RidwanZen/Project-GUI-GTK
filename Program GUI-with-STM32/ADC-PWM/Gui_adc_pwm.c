// Compile : gcc -Wno-format -o Gui_adc_pwm Gui_adc_pwm.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
// Run : ./Gui_adc_pwm

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

void send_data(char* a);
int get_data();
char *IntToStr(int x);
char input[1000];
char *adc, *duty_pwm;
int pwm = 0;

char text[255];

const int screenWidth = 3000, screenHight = 3000;
int main_box_space, main_nbox_space, main_box_side_margin, main_box_main_width;
int logo_width, logo_hight;
int imBack_width, imBack_hight;
int main_box_label_margin;
gboolean timer_handler(GtkWidget *);
GtkCssProvider *cssProvider;
GtkBuilder *builder;

//mainWindow
GtkWidget *window;
GtkWidget *main_box, *main_grid_adc, *main_grid_pwm, *main_box_close;
GtkWidget *bt_min, *bt_plus, *bt_close;
GtkWidget *lb_adc, *lb_pwm, *lb_icon;
GtkWidget *adc_value, *pwm_value;

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
	guint ID = g_timeout_add(500,(GSourceFunc) timer_handler, adc_value);
	
	//start
	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show(window);
	g_object_unref(builder);
	gtk_main();
	return 0;
}
// function looping
gboolean timer_handler(GtkWidget *adc_value){

	get_data();
	
	gtk_label_set_text(GTK_LABEL(adc_value), text);	// update time of day
	return TRUE; // False kills the timer
}

void gtk_builder_and_attrib_init(){
	builder=gtk_builder_new();
	gtk_builder_add_from_file (builder, "GUI_adc_pwm.glade", NULL);

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
	main_box_close=GTK_WIDGET(gtk_builder_get_object(builder, "main_box_close"));
	main_grid_pwm=GTK_WIDGET(gtk_builder_get_object(builder, "main_grid_pwm"));
	main_grid_adc=GTK_WIDGET(gtk_builder_get_object(builder, "main_grid_adc"));

	//button
	bt_min=GTK_WIDGET(gtk_builder_get_object(builder, "bt_min"));
	bt_plus=GTK_WIDGET(gtk_builder_get_object(builder, "bt_plus"));
	bt_close=GTK_WIDGET(gtk_builder_get_object(builder, "bt_close"));
	
	adc_value=GTK_WIDGET(gtk_builder_get_object(builder, "adc_value"));
	pwm_value=GTK_WIDGET(gtk_builder_get_object(builder, "pwm_value"));
	
	//label
	lb_adc=GTK_WIDGET(gtk_builder_get_object(builder, "lb_adc"));
	lb_pwm=GTK_WIDGET(gtk_builder_get_object(builder, "lb_pwm"));
	lb_icon=GTK_WIDGET(gtk_builder_get_object(builder, "lb_icon"));

} 

void gtk_mainWindow_setAttrib(){
	//~ gtk_window_fullscreen(GTK_WINDOW(window));

	gtk_widget_set_name (window, "myWindow");
	gtk_widget_set_name (main_box, "main_box");
	gtk_widget_set_name (bt_min, "bt_min");
	gtk_widget_set_name (bt_plus, "bt_plus");
	gtk_widget_set_name (bt_close, "bt_close");
	gtk_widget_set_name (adc_value, "adc_value");
	gtk_widget_set_name (pwm_value, "pwm_value");
	gtk_widget_set_name (main_box, "main_box");
	gtk_widget_set_name (main_box_close, "main_box_close");
	gtk_widget_set_name (main_grid_pwm, "main_grid_pwm");
	gtk_widget_set_name (main_grid_adc, "main_grid_adc");
	gtk_widget_set_name(lb_adc, "lb_adc");
	gtk_widget_set_name(lb_pwm, "lb_pwm");
	gtk_widget_set_name(lb_icon, "lb_icon");

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	int gspace_row = screenWidth/60; 
	int gspace_col = screenWidth/60;
	gtk_grid_set_row_spacing(GTK_GRID(main_grid_pwm), gspace_row);
	gtk_grid_set_column_spacing(GTK_GRID(main_grid_pwm), gspace_col);
	gtk_box_set_spacing(GTK_BOX(main_box), gspace_row);
	gtk_widget_set_margin_top(main_box, main_box_space);
	gtk_widget_set_margin_bottom(main_box, main_box_space);
	gtk_widget_set_margin_start(main_box, main_box_side_margin);
	gtk_widget_set_margin_end(main_box, main_box_side_margin);
}

void gtk_mainWindow_connect(){
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(bt_close, "clicked", G_CALLBACK(gtk_main_quit), NULL);
}

// Mengkonvert dari Integer ke String
char *IntToStr(int x){
	char *str=(char *)malloc(10 * sizeof (char));	// Mengalokasikan blok memory sebesar 10byte dan di pointing ke pointer str
	sprintf(str, "%d", x);	// Memasukkan data integer x ke str
	return str;
}

// function tombol min(-)
void on_button_min_clicked (GtkButton *b) {
	
	if(pwm > 0 && pwm <= 100 ){
		pwm = pwm - 10;
	}
	duty_pwm = IntToStr(pwm); // konvert integer ke string
	gtk_label_set_text (GTK_LABEL(pwm_value), (const gchar*)duty_pwm); // set text label
	send_data(duty_pwm); // kirim data dengan UART
}

// function tombol plus(+)
void on_button_plus_clicked (GtkButton *b) {
	
	if(pwm >= 0 && pwm !=100 ){
		pwm = pwm + 10;
	}
	duty_pwm = IntToStr(pwm); // konvert integer ke string
	gtk_label_set_text (GTK_LABEL(pwm_value), (const gchar*)duty_pwm);  // set text label
	send_data(duty_pwm); // kirim data dengan UART
}
	
// function kirim data dengan UART
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
	
// ambil data dari UART	
int get_data(){
	int fd = 0, len;
	struct termios options; /* Serial ports setting */
	
	//int serial_port = open("/dev/ttyACM1", O_RDWR);
	fd = open("/dev/ttyACM0", O_RDWR);
	
	//~ // Read in existing settings, and handle any error
	if(tcgetattr(fd, &options) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
	}

	//~ /* Read current serial port settings */
	//~ // tcgetattr(fd, &options);
	options.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	options.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	options.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
	options.c_cflag |= CS8; // 8 bits per byte (most common)
	options.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	options.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	options.c_lflag &= ~ICANON;
	options.c_lflag &= ~ECHO; // Disable echo
	options.c_lflag &= ~ECHOE; // Disable erasure
	options.c_lflag &= ~ECHONL; // Disable new-line echo
	options.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	options.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

	options.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	options.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

	options.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	options.c_cc[VMIN] = 0;

	// Set in/out baud rate to be 115200
	cfsetispeed(&options, B115200);	// in
	cfsetospeed(&options, B115200);	//out
	
		// Save tty settings, also checking for error
	if (tcsetattr(fd, TCSANOW, &options) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
	}
	
	//~ /* Read from serial port */
	//memset(&read_buf, '\0', sizeof(read_buf));
	memset(&text, '\0', sizeof(text));
	
	//read(serial_port, &read_buf, 100);	
	int num_bytes = read(fd, &text, 255);

	
	// n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
	if (num_bytes < 0) {
      printf("Error reading: %s", strerror(errno));
      return 1;
	}
	
	//~ printf("Received string: %s\n", text);
	
	close(fd);
	
	return 0;
}
