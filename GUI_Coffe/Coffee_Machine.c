// Compile : gcc `pkg-config --cflags gtk+-3.0` "Coffee_Machine.c"  -o "Coffee_Machine"   `pkg-config --libs gtk+-3.0`
// Run : ./Coffee_Machine

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

const gchar *harga = NULL;
const gchar *menu = NULL;

const int screenWidth = 3000, screenHight = 3000;
int main_box_space, main_nbox_space, main_box_side_margin, main_box_main_width;
int logo_width, logo_hight;
int imBack_width, imBack_hight;
int main_box_label_margin;

GtkCssProvider *cssProvider;
GtkBuilder *builder;

GtkWidget *timer, *timer1, *timer2, *timer3, *timer4;

//mainWindow
GtkWidget *window;
GtkWidget *main_box, *main_box_top, *MenuContainer;
GtkWidget *bt_menu;
GtkWidget *display_iklan;
GtkWidget *main_label, *clock_lb;


/* windowMenu */
GtkWidget *windowMenu;
GtkWidget *menu_box, *menu_box_display, *menu_box_bt, *menu_box_top, *menu_box_back, *menu_grid_bt, *bt_iced_box, *bt_hot_box;
GtkWidget *bt_iced, *bt_hot, *bt_menu_back;
GtkWidget *bt_iced_logo, *bt_hot_logo, *bt_menu_back_logo;
GtkWidget *menu_label, *hot_label, *iced_label, *clock_lb1;

/* windowMenuHot */
GtkWidget *windowMenuHot;
GtkWidget *menuHot_box, *menuHot_box_back, *menuHot_box_display, *menuHot_box_top;
GtkWidget *menuHot_grid_bt, *menuHot_grid_bt1, *menuHot_grid_bt2, *menuHot_grid_bt3, *menuHot_grid_bt4, *menuHot_grid_bt5, *menuHot_grid_bt6, *menuHot_grid_bt7, *menuHot_grid_bt8;
GtkWidget *clock_lb2, *menuHot_label, *menuHot_label_bt1, *menuHot_label_bt2, *menuHot_label_bt3, *menuHot_label_bt4, *menuHot_label_bt5, *menuHot_label_bt6, *menuHot_label_bt7, *menuHot_label_bt8;
GtkWidget *menuHot_bt1, *menuHot_bt2, *menuHot_bt3, *menuHot_bt4, *menuHot_bt5, *menuHot_bt6, *menuHot_bt7, *menuHot_bt8, *bt_menuHot_back;
GtkWidget *menuHot_harga1, *menuHot_harga2, *menuHot_harga3, *menuHot_harga4, *menuHot_harga5, *menuHot_harga6, *menuHot_harga7, *menuHot_harga8;
GtkWidget *menuHot_logo_bt1, *menuHot_logo_bt2, *menuHot_logo_bt3, *menuHot_logo_bt4, *menuHot_logo_bt5, *menuHot_logo_bt6, *menuHot_logo_bt7, *menuHot_logo_bt8, *bt_menuHot_back_logo;

/* windowMenuIced */
GtkWidget *windowMenuIced;
GtkWidget *menuIced_box, *menuIced_box_back, *menuIced_box_display, *menuIced_box_top;
GtkWidget *menuIced_grid_bt, *menuIced_grid_bt1, *menuIced_grid_bt2, *menuIced_grid_bt3, *menuIced_grid_bt4, *menuIced_grid_bt5, *menuIced_grid_bt6, *menuIced_grid_bt7, *menuIced_grid_bt8;
GtkWidget *clock_lb3, *menuIced_label, *menuIced_label_bt1, *menuIced_label_bt2, *menuIced_label_bt3, *menuIced_label_bt4, *menuIced_label_bt5, *menuIced_label_bt6, *menuIced_label_bt7, *menuIced_label_bt8;
GtkWidget *menuIced_bt1, *menuIced_bt2, *menuIced_bt3, *menuIced_bt4, *menuIced_bt5, *menuIced_bt6, *menuIced_bt7, *menuIced_bt8, *bt_menuIced_back;
GtkWidget *menuIced_harga1, *menuIced_harga2, *menuIced_harga3, *menuIced_harga4, *menuIced_harga5, *menuIced_harga6, *menuIced_harga7, *menuIced_harga8;
GtkWidget *menuIced_logo_bt1, *menuIced_logo_bt2, *menuIced_logo_bt3, *menuIced_logo_bt4, *menuIced_logo_bt5, *menuIced_logo_bt6, *menuIced_logo_bt7, *menuIced_logo_bt8, *bt_menuIced_back_logo;

/* windowPayment*/
GtkWidget *windowPayment;
GtkWidget *payment_box, *payment_box_back, *payment_box_top, *payment_box_display, *payment_box_display1, *payment_box_menu, *payment_box_bt;
GtkWidget *payment_label, *payment_label1, *clock_lb4, *bt_payment_label;
GtkWidget *payment_box_gopay, *payment_box_ovo, *payment_box_qris, *payment_box_dana, *payment_box_shopeePay;
GtkWidget *bt_payment_back, *bt_payment, *payment_stack;
GtkWidget *payment_harga_gopay, *payment_harga_ovo, *payment_harga_qris, *payment_harga_dana, *payment_harga_shopeePay;
GtkWidget *payment_menu_gopay, *payment_menu_ovo, *payment_menu_qris, *payment_menu_dana, *payment_menu_shopeePay;
GtkWidget *payment_gopay, *payment_ovo, *payment_qris, *payment_dana, *payment_shopeePay;
GtkWidget *payment_gopay_bg, *payment_ovo_bg, *payment_qris_bg, *payment_dana_bg, *payment_shopeePay_bg;
GtkWidget *payment_logo_gopay, *payment_logo_ovo, *payment_logo_qris, *payment_logo_dana, *payment_logo_shopeePay, *bt_payment_logo, *bt_payment_back_logo;

/* windowSucces*/
GtkWidget *windowSucces;
GtkWidget *bt_succes, *box_succes, *succes_logo, *succes_label;

// Function Timer
gboolean timer_handler(GtkWidget *);

void gtk_mainWindow_init();
void gtk_mainWindow_setAttrib();
void gtk_mainWindow_connect();
void gtk_builder_and_attrib_init();
void gtk_image_opt(GtkWidget *widget, char *fileName, int width, int hight, int margin_left);

static void start_windowMenu();
static void view_windowMenu();
void gtk_windowMenu_init();
void gtk_windowMenu_setAttrib();
void gtk_windowMenu_connect();
static void close_windowMenu();

static void start_windowMenuHot();
static void view_windowMenuHot();
void gtk_windowMenuHot_init();
void gtk_windowMenuHot_setAttrib();
void gtk_windowMenuHot_connect();
static void close_windowMenuHot();
static void cek_menuHot(GtkWidget *btWidget);

static void start_windowMenuIced();
static void view_windowMenuIced();
void gtk_windowMenuIced_init();
void gtk_windowMenuIced_setAttrib();
void gtk_windowMenuIced_connect();
static void close_windowMenuIced();
static void cek_menuIced(GtkWidget *btWidget);

static void start_windowPayment();
static void view_windowPayment();
void gtk_windowPayment_init();
void gtk_windowPayment_setAttrib();
void gtk_windowPayment_connect();
static void close_windowPayment();

static void start_windowSucces();
static void view_windowSucces();
void gtk_windowSucces_init();
void gtk_windowSucces_setAttrib();
void gtk_windowSucces_connect();
static void close_windowSucces();

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
	
	start_windowMenu();
	start_windowMenuHot();
	start_windowMenuIced();
	start_windowPayment();
	start_windowSucces();
	
	guint ID = g_timeout_add_seconds(1, (GSourceFunc) timer_handler, timer);
	
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
	gtk_label_set_text(GTK_LABEL(timer1), ctime(&t));	// update time of day
	gtk_label_set_text(GTK_LABEL(timer2), ctime(&t));	// update time of day
	gtk_label_set_text(GTK_LABEL(timer3), ctime(&t));	// update time of day
	gtk_label_set_text(GTK_LABEL(timer4), ctime(&t));	// update time of day
	return TRUE; // False kills the timer
}

void gtk_builder_and_attrib_init(){
	builder=gtk_builder_new();
	gtk_builder_add_from_file (builder, "kopi.glade", NULL);

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
	window=GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
	MenuContainer=GTK_WIDGET(gtk_builder_get_object(builder, "menu_grid_bt"));
	main_box=GTK_WIDGET(gtk_builder_get_object(builder, "main_box"));
	main_box_top=GTK_WIDGET(gtk_builder_get_object(builder, "main_box_top"));
	
	display_iklan=GTK_WIDGET(gtk_builder_get_object(builder, "display_iklan"));
	
	//button
	bt_menu=GTK_WIDGET(gtk_builder_get_object(builder, "bt_menu"));
	
	//label
	main_label=GTK_WIDGET(gtk_builder_get_object(builder, "main_label"));
	clock_lb=GTK_WIDGET(gtk_builder_get_object(builder, "clock_lb"));
	timer=GTK_WIDGET(gtk_builder_get_object(builder, "clock_lb"));

} 

void gtk_mainWindow_setAttrib(){
	gtk_window_fullscreen(GTK_WINDOW(window));

	gtk_widget_set_name (window, "myWindow");
	gtk_widget_set_name (MenuContainer, "menu_grid_bt");
	gtk_widget_set_name (bt_menu, "bt_menu");
	gtk_widget_set_name (main_box, "main_box");
	gtk_widget_set_name (main_box_top, "main_box_top");
	gtk_widget_set_name (display_iklan, "display_iklan");
	gtk_widget_set_name(main_label, "main_label");
	gtk_widget_set_name(clock_lb, "clock_lb");

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	int gspace_row = screenWidth/60; 
	int gspace_col = screenWidth/60;
	gtk_grid_set_row_spacing(GTK_GRID(MenuContainer), gspace_row);
	gtk_grid_set_column_spacing(GTK_GRID(MenuContainer), gspace_col);
	gtk_box_set_spacing(GTK_BOX(main_box), gspace_row);
	gtk_widget_set_margin_top(main_box, main_box_space);
	gtk_widget_set_margin_bottom(main_box, main_box_space);
	gtk_widget_set_margin_start(main_box, main_box_side_margin);
	gtk_widget_set_margin_end(main_box, main_box_side_margin);
}

void gtk_mainWindow_connect(){
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(bt_menu, "clicked", G_CALLBACK (view_windowMenu), NULL);
}

/* windowMenu */
static void start_windowMenu(){
	int argc; char **argv;
	gtk_init(&argc, &argv);
	gtk_builder_and_attrib_init();

	gtk_windowMenu_init();
	gtk_windowMenu_setAttrib();
	gtk_windowMenu_connect();

	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show(windowMenu);
	gtk_widget_hide(windowMenu);
}

static void view_windowMenu(){
	gtk_widget_show(windowMenu);
}

void gtk_windowMenu_init(){
	windowMenu=GTK_WIDGET(gtk_builder_get_object(builder, "windowMenu"));
	menu_box=GTK_WIDGET(gtk_builder_get_object(builder, "menu_box"));
	menu_box_display=GTK_WIDGET(gtk_builder_get_object(builder, "menu_box_display"));
	menu_box_bt=GTK_WIDGET(gtk_builder_get_object(builder, "menu_box_bt"));
	menu_box_back=GTK_WIDGET(gtk_builder_get_object(builder, "menu_box_back"));
	menu_box_top=GTK_WIDGET(gtk_builder_get_object(builder, "menu_box_top"));
	bt_iced_box=GTK_WIDGET(gtk_builder_get_object(builder, "bt_iced_box"));
	bt_hot_box=GTK_WIDGET(gtk_builder_get_object(builder, "bt_hot_box"));
	menu_grid_bt=GTK_WIDGET(gtk_builder_get_object(builder, "menu_grid_bt"));
	
	bt_iced=GTK_WIDGET(gtk_builder_get_object(builder, "bt_iced"));
	bt_hot=GTK_WIDGET(gtk_builder_get_object(builder, "bt_hot"));
	bt_menu_back=GTK_WIDGET(gtk_builder_get_object(builder, "bt_menu_back"));
	
	bt_hot_logo=GTK_WIDGET(gtk_builder_get_object(builder, "bt_hot_logo"));
	bt_iced_logo=GTK_WIDGET(gtk_builder_get_object(builder, "bt_iced_logo"));
	bt_menu_back_logo=GTK_WIDGET(gtk_builder_get_object(builder, "bt_menu_back_logo"));
	
	menu_label=GTK_WIDGET(gtk_builder_get_object(builder, "menu_label"));
	hot_label=GTK_WIDGET(gtk_builder_get_object(builder, "hot_label"));
	iced_label=GTK_WIDGET(gtk_builder_get_object(builder, "iced_label"));
	clock_lb1=GTK_WIDGET(gtk_builder_get_object(builder, "clock_lb1"));
	
	timer1=GTK_WIDGET(gtk_builder_get_object(builder, "clock_lb1"));
	
}

void gtk_windowMenu_setAttrib(){
	gtk_window_fullscreen(GTK_WINDOW(windowMenu));

	gtk_widget_set_name(windowMenu, "windowMenu");
	gtk_widget_set_name(menu_box, "menu_box");
	gtk_widget_set_name(menu_box_back, "menu_box_back");
	gtk_widget_set_name(menu_box_bt, "menu_box_bt");
	gtk_widget_set_name(menu_box_display, "menu_box_display");
	gtk_widget_set_name(menu_grid_bt, "menu_grid_bt");
	gtk_widget_set_name(bt_iced_box, "bt_iced_box");
	gtk_widget_set_name(bt_hot_box, "bt_hot_box");
	
	gtk_widget_set_name(menu_label, "menu_label");
	gtk_widget_set_name(iced_label, "iced_label");
	gtk_widget_set_name(hot_label, "hot_label");
	gtk_widget_set_name(clock_lb1, "clock_lb1");
	
	gtk_widget_set_name(bt_iced, "bt_iced");
	gtk_widget_set_name(bt_hot, "bt_hot");
	gtk_widget_set_name(bt_menu_back, "bt_menu_back");
	
	gtk_widget_set_name(bt_iced_logo, "bt_iced_logo");
	gtk_widget_set_name(bt_hot_logo, "bt_hot_logo");
	gtk_widget_set_name(bt_menu_back_logo, "bt_menu_back_logo");
	
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	//set dimension
	int imWidth, imHight;
	int im_margin = screenHight/40;
	imWidth=imHight=screenWidth/15;
	gtk_image_opt(bt_hot_logo, "img/output-onlinepngtools.png", imWidth, imHight, 0);
	//gtk_image_opt(bt_iced_logo, "img/iced.jpg", imWidth, imHight, 0);
	
	
	int gspace_row = screenWidth/100; 
	int gspace_col = screenWidth/100;
	gtk_grid_set_row_spacing(GTK_GRID(menu_grid_bt), gspace_row);
	gtk_grid_set_column_spacing(GTK_GRID(menu_grid_bt), gspace_col);
	gtk_box_set_spacing(GTK_BOX(menu_box), gspace_row);
	gtk_widget_set_margin_top(menu_box, main_box_space);
	gtk_widget_set_margin_bottom(menu_box, main_box_space);
	gtk_widget_set_margin_start(menu_box, main_box_side_margin);
	gtk_widget_set_margin_end(menu_box, main_box_side_margin);
}

void gtk_windowMenu_connect(){
	g_signal_connect(windowMenu, "destroy", G_CALLBACK(close_windowMenu), NULL);
	g_signal_connect(bt_iced, "clicked", G_CALLBACK (view_windowMenuIced), NULL);
	g_signal_connect(bt_hot, "clicked", G_CALLBACK (view_windowMenuHot), NULL);
	g_signal_connect(bt_menu_back, "clicked", G_CALLBACK (close_windowMenu), NULL);
}

static void close_windowMenu(){
	gtk_widget_hide(windowMenu);
}

/* windowMenuHot */
static void start_windowMenuHot(){
	int argc; char **argv;
	gtk_init(&argc, &argv);
	gtk_builder_and_attrib_init();

	gtk_windowMenuHot_init();
	gtk_windowMenuHot_setAttrib();
	gtk_windowMenuHot_connect();

	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show(windowMenuHot);
	gtk_widget_hide(windowMenuHot);
}

static void view_windowMenuHot(){
	gtk_widget_show(windowMenuHot);
}

void gtk_windowMenuHot_init(){
	windowMenuHot=GTK_WIDGET(gtk_builder_get_object(builder, "windowMenuHot"));
	/* MenuHot box*/
	menuHot_box=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_box"));
	menuHot_box_display=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_box_display"));
	menuHot_box_back=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_box_back"));
	menuHot_box_top=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_box_top"));
	/* MenuHot grid*/
	menuHot_grid_bt=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_grid_bt"));
	menuHot_grid_bt1=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_grid_bt1"));
	menuHot_grid_bt2=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_grid_bt2"));
	menuHot_grid_bt3=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_grid_bt3"));
	menuHot_grid_bt4=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_grid_bt4"));
	menuHot_grid_bt5=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_grid_bt5"));
	menuHot_grid_bt6=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_grid_bt6"));
	menuHot_grid_bt7=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_grid_bt7"));
	menuHot_grid_bt8=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_grid_bt8"));
	/* MenuHot button*/
	menuHot_bt1=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_bt1"));
	menuHot_bt2=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_bt2"));
	menuHot_bt3=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_bt3"));
	menuHot_bt4=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_bt4"));
	menuHot_bt5=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_bt5"));
	menuHot_bt6=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_bt6"));
	menuHot_bt7=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_bt7"));
	menuHot_bt8=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_bt8"));
	bt_menuHot_back=GTK_WIDGET(gtk_builder_get_object(builder, "bt_menuHot_back"));
	/* MenuHot label*/
	clock_lb2=GTK_WIDGET(gtk_builder_get_object(builder, "clock_lb2"));
	menuHot_label=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_label"));
	menuHot_label_bt1=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_label_bt1"));
	menuHot_label_bt2=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_label_bt2"));
	menuHot_label_bt3=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_label_bt3"));
	menuHot_label_bt4=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_label_bt4"));
	menuHot_label_bt5=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_label_bt5"));
	menuHot_label_bt6=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_label_bt6"));
	menuHot_label_bt7=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_label_bt7"));
	menuHot_label_bt8=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_label_bt8"));
	timer2=GTK_WIDGET(gtk_builder_get_object(builder, "clock_lb2"));
	/* MenuHot logo*/
	menuHot_logo_bt1=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_logo_bt1"));
	menuHot_logo_bt2=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_logo_bt2"));
	menuHot_logo_bt3=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_logo_bt3"));
	menuHot_logo_bt4=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_logo_bt4"));
	menuHot_logo_bt5=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_logo_bt5"));
	menuHot_logo_bt6=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_logo_bt6"));
	menuHot_logo_bt7=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_logo_bt7"));
	menuHot_logo_bt8=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_logo_bt8"));
	bt_menuHot_back_logo=GTK_WIDGET(gtk_builder_get_object(builder, "bt_menuHot_back_logo"));
	/* MenuHot harga*/
	menuHot_harga1=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_harga1"));
	menuHot_harga2=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_harga2"));
	menuHot_harga3=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_harga3"));
	menuHot_harga4=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_harga4"));
	menuHot_harga5=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_harga5"));
	menuHot_harga6=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_harga6"));
	menuHot_harga7=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_harga7"));
	menuHot_harga8=GTK_WIDGET(gtk_builder_get_object(builder, "menuHot_harga8"));
	
}

void gtk_windowMenuHot_setAttrib(){
	gtk_window_fullscreen(GTK_WINDOW(windowMenuHot));

	gtk_widget_set_name(windowMenuHot, "windowMenuHot");
	/* MenuHot box*/
	gtk_widget_set_name(menuHot_box, "menuHot_box");
	gtk_widget_set_name(menuHot_box_display, "menuHot_box_display");
	gtk_widget_set_name(menuHot_box_back, "menuHot_box_back");
	gtk_widget_set_name(menuHot_box_top, "menuHot_box_top");
	/* MenuHot button*/
	gtk_widget_set_name(menuHot_bt1, "menuHot_bt1");
	gtk_widget_set_name(menuHot_bt2, "menuHot_bt2");
	gtk_widget_set_name(menuHot_bt3, "menuHot_bt3");
	gtk_widget_set_name(menuHot_bt4, "menuHot_bt4");
	gtk_widget_set_name(menuHot_bt5, "menuHot_bt5");
	gtk_widget_set_name(menuHot_bt6, "menuHot_bt6");
	gtk_widget_set_name(menuHot_bt7, "menuHot_bt7");
	gtk_widget_set_name(menuHot_bt8, "menuHot_bt8");
	gtk_widget_set_name(bt_menuHot_back, "bt_menuHot_back");
	/* MenuHot label*/
	gtk_widget_set_name(clock_lb2, "clock_lb2");
	gtk_widget_set_name(menuHot_label, "menuHot_label");
	gtk_widget_set_name(menuHot_label_bt1, "menuHot_label_bt1");
	gtk_widget_set_name(menuHot_label_bt2, "menuHot_label_bt2");
	gtk_widget_set_name(menuHot_label_bt3, "menuHot_label_bt3");
	gtk_widget_set_name(menuHot_label_bt4, "menuHot_label_bt4");
	gtk_widget_set_name(menuHot_label_bt5, "menuHot_label_bt5");
	gtk_widget_set_name(menuHot_label_bt6, "menuHot_label_bt6");
	gtk_widget_set_name(menuHot_label_bt7, "menuHot_label_bt7");
	gtk_widget_set_name(menuHot_label_bt8, "menuHot_label_bt8");
	/* MenuHot logo*/
	gtk_widget_set_name(menuHot_logo_bt1, "menuHot_logo_bt1");
	gtk_widget_set_name(menuHot_logo_bt2, "menuHot_logo_bt2");
	gtk_widget_set_name(menuHot_logo_bt3, "menuHot_logo_bt3");
	gtk_widget_set_name(menuHot_logo_bt4, "menuHot_logo_bt4");
	gtk_widget_set_name(menuHot_logo_bt5, "menuHot_logo_bt5");
	gtk_widget_set_name(menuHot_logo_bt6, "menuHot_logo_bt6");
	gtk_widget_set_name(menuHot_logo_bt7, "menuHot_logo_bt7");
	gtk_widget_set_name(menuHot_logo_bt8, "menuHot_logo_bt8");
	gtk_widget_set_name(bt_menuHot_back_logo, "bt_menuHot_back_logo");
	/* MenuHot harga*/
	gtk_widget_set_name(menuHot_harga1, "menuHot_harga1");
	gtk_widget_set_name(menuHot_harga2, "menuHot_harga2");
	gtk_widget_set_name(menuHot_harga3, "menuHot_harga3");
	gtk_widget_set_name(menuHot_harga4, "menuHot_harga4");
	gtk_widget_set_name(menuHot_harga5, "menuHot_harga5");
	gtk_widget_set_name(menuHot_harga6, "menuHot_harga6");
	gtk_widget_set_name(menuHot_harga7, "menuHot_harga7");
	gtk_widget_set_name(menuHot_harga8, "menuHot_harga8");
	
	gtk_widget_set_name(menuHot_grid_bt, "menuHot_grid_bt");
	
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	int gspace_row = screenWidth/100; 
	int gspace_col = screenWidth/100;
	gtk_grid_set_row_spacing(GTK_GRID(menuHot_grid_bt), gspace_row);
	gtk_grid_set_column_spacing(GTK_GRID(menuHot_grid_bt), gspace_col);
	gtk_box_set_spacing(GTK_BOX(menuHot_box), gspace_row);
	gtk_widget_set_margin_top(menuHot_box, main_box_space);
	gtk_widget_set_margin_bottom(menuHot_box, main_box_space);
	gtk_widget_set_margin_start(menuHot_box, main_box_side_margin);
	gtk_widget_set_margin_end(menuHot_box, main_box_side_margin);
	
	
}

void gtk_windowMenuHot_connect(){
	g_signal_connect(windowMenuHot, "destroy", G_CALLBACK(close_windowMenuHot), NULL);
	g_signal_connect(bt_menuHot_back, "clicked", G_CALLBACK (close_windowMenuHot), NULL);
	g_signal_connect(menuHot_bt1, "clicked", G_CALLBACK (cek_menuHot), NULL);
	g_signal_connect(menuHot_bt2, "clicked", G_CALLBACK (cek_menuHot), NULL);
	g_signal_connect(menuHot_bt3, "clicked", G_CALLBACK (cek_menuHot), NULL);
	g_signal_connect(menuHot_bt4, "clicked", G_CALLBACK (cek_menuHot), NULL);
	g_signal_connect(menuHot_bt5, "clicked", G_CALLBACK (cek_menuHot), NULL);
	g_signal_connect(menuHot_bt6, "clicked", G_CALLBACK (cek_menuHot), NULL);
	g_signal_connect(menuHot_bt7, "clicked", G_CALLBACK (cek_menuHot), NULL);
	g_signal_connect(menuHot_bt8, "clicked", G_CALLBACK (cek_menuHot), NULL);
}

static void close_windowMenuHot(){
	gtk_widget_hide(windowMenuHot);
}

static void cek_menuHot(GtkWidget *btWidget){
	
	if (btWidget == menuHot_bt1){
		harga = gtk_label_get_text (GTK_LABEL (menuHot_harga1));
		menu = gtk_label_get_text (GTK_LABEL (menuHot_label_bt1));
	}
	if (btWidget == menuHot_bt2){
		harga = gtk_label_get_text (GTK_LABEL (menuHot_harga2));
		menu = gtk_label_get_text (GTK_LABEL (menuHot_label_bt2));
	}
	if (btWidget == menuHot_bt3){
		harga = gtk_label_get_text (GTK_LABEL (menuHot_harga3));
		menu = gtk_label_get_text (GTK_LABEL (menuHot_label_bt3));
	}
	if (btWidget == menuHot_bt4){
		harga = gtk_label_get_text (GTK_LABEL (menuHot_harga4));
		menu = gtk_label_get_text (GTK_LABEL (menuHot_label_bt4));
	}
	if (btWidget == menuHot_bt5){
		harga = gtk_label_get_text (GTK_LABEL (menuHot_harga5));
		menu = gtk_label_get_text (GTK_LABEL (menuHot_label_bt5));
	}
	if (btWidget == menuHot_bt6){
		harga = gtk_label_get_text (GTK_LABEL (menuHot_harga6));
		menu = gtk_label_get_text (GTK_LABEL (menuHot_label_bt6));
	}
	if (btWidget == menuHot_bt7){
		harga = gtk_label_get_text (GTK_LABEL (menuHot_harga7));
		menu = gtk_label_get_text (GTK_LABEL (menuHot_label_bt7));
	}
	if (btWidget == menuHot_bt8){
		harga = gtk_label_get_text (GTK_LABEL (menuHot_harga8));
		menu = gtk_label_get_text (GTK_LABEL (menuHot_label_bt8));
	}
		
	gtk_label_set_text (GTK_LABEL(payment_harga_gopay), (const gchar* ) harga);
	gtk_label_set_text (GTK_LABEL(payment_menu_gopay), (const gchar* ) menu);
	
	gtk_label_set_text (GTK_LABEL(payment_harga_ovo), (const gchar* ) harga);
	gtk_label_set_text (GTK_LABEL(payment_menu_ovo), (const gchar* ) menu);
	
	gtk_label_set_text (GTK_LABEL(payment_harga_qris), (const gchar* ) harga);
	gtk_label_set_text (GTK_LABEL(payment_menu_qris), (const gchar* ) menu);
	
	gtk_label_set_text (GTK_LABEL(payment_harga_dana), (const gchar* ) harga);
	gtk_label_set_text (GTK_LABEL(payment_menu_dana), (const gchar* ) menu);
	
	gtk_label_set_text (GTK_LABEL(payment_harga_shopeePay), (const gchar* ) harga);
	gtk_label_set_text (GTK_LABEL(payment_menu_shopeePay), (const gchar* ) menu);
	
	view_windowPayment();
}

/* windowMenuIced */
static void start_windowMenuIced(){
	int argc; char **argv;
	gtk_init(&argc, &argv);
	gtk_builder_and_attrib_init();

	gtk_windowMenuIced_init();
	gtk_windowMenuIced_setAttrib();
	gtk_windowMenuIced_connect();

	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show(windowMenuIced);
	gtk_widget_hide(windowMenuIced);
}

static void view_windowMenuIced(){
	gtk_widget_show(windowMenuIced);
}

void gtk_windowMenuIced_init(){
	windowMenuIced=GTK_WIDGET(gtk_builder_get_object(builder, "windowMenuIced"));
	/* MenuIced box*/
	menuIced_box=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_box"));
	menuIced_box_display=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_box_display"));
	menuIced_box_back=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_box_back"));
	menuIced_box_top=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_box_top"));
	/* MenuIced grid*/
	menuIced_grid_bt=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_grid_bt"));
	menuIced_grid_bt1=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_grid_bt1"));
	menuIced_grid_bt2=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_grid_bt2"));
	menuIced_grid_bt3=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_grid_bt3"));
	menuIced_grid_bt4=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_grid_bt4"));
	menuIced_grid_bt5=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_grid_bt5"));
	menuIced_grid_bt6=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_grid_bt6"));
	menuIced_grid_bt7=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_grid_bt7"));
	menuIced_grid_bt8=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_grid_bt8"));
	/* MenuIced button*/
	menuIced_bt1=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_bt1"));
	menuIced_bt2=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_bt2"));
	menuIced_bt3=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_bt3"));
	menuIced_bt4=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_bt4"));
	menuIced_bt5=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_bt5"));
	menuIced_bt6=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_bt6"));
	menuIced_bt7=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_bt7"));
	menuIced_bt8=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_bt8"));
	bt_menuIced_back=GTK_WIDGET(gtk_builder_get_object(builder, "bt_menuIced_back"));
	/* MenuIced label*/
	clock_lb3=GTK_WIDGET(gtk_builder_get_object(builder, "clock_lb3"));
	menuIced_label=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_label"));
	menuIced_label_bt1=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_label_bt1"));
	menuIced_label_bt2=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_label_bt2"));
	menuIced_label_bt3=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_label_bt3"));
	menuIced_label_bt4=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_label_bt4"));
	menuIced_label_bt5=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_label_bt5"));
	menuIced_label_bt6=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_label_bt6"));
	menuIced_label_bt7=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_label_bt7"));
	menuIced_label_bt8=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_label_bt8"));
	timer3=GTK_WIDGET(gtk_builder_get_object(builder, "clock_lb3"));
	/* MenuIced logo*/
	menuIced_logo_bt1=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_logo_bt1"));
	menuIced_logo_bt2=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_logo_bt2"));
	menuIced_logo_bt3=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_logo_bt3"));
	menuIced_logo_bt4=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_logo_bt4"));
	menuIced_logo_bt5=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_logo_bt5"));
	menuIced_logo_bt6=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_logo_bt6"));
	menuIced_logo_bt7=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_logo_bt7"));
	menuIced_logo_bt8=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_logo_bt8"));
	bt_menuIced_back_logo=GTK_WIDGET(gtk_builder_get_object(builder, "bt_menuIced_back_logo"));
	/* MenuIced harga*/
	menuIced_harga1=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_harga1"));
	menuIced_harga2=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_harga2"));
	menuIced_harga3=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_harga3"));
	menuIced_harga4=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_harga4"));
	menuIced_harga5=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_harga5"));
	menuIced_harga6=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_harga6"));
	menuIced_harga7=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_harga7"));
	menuIced_harga8=GTK_WIDGET(gtk_builder_get_object(builder, "menuIced_harga8"));
	
}

void gtk_windowMenuIced_setAttrib(){
	gtk_window_fullscreen(GTK_WINDOW(windowMenuIced));

	gtk_widget_set_name(windowMenuIced, "windowMenuIced");
	/* MenuIced box*/
	gtk_widget_set_name(menuIced_box, "menuIced_box");
	gtk_widget_set_name(menuIced_box_display, "menuIced_box_display");
	gtk_widget_set_name(menuIced_box_back, "menuIced_box_back");
	gtk_widget_set_name(menuIced_box_top, "menuIced_box_top");
	/* MenuIced button*/
	gtk_widget_set_name(menuIced_bt1, "menuIced_bt1");
	gtk_widget_set_name(menuIced_bt2, "menuIced_bt2");
	gtk_widget_set_name(menuIced_bt3, "menuIced_bt3");
	gtk_widget_set_name(menuIced_bt4, "menuIced_bt4");
	gtk_widget_set_name(menuIced_bt5, "menuIced_bt5");
	gtk_widget_set_name(menuIced_bt6, "menuIced_bt6");
	gtk_widget_set_name(menuIced_bt7, "menuIced_bt7");
	gtk_widget_set_name(menuIced_bt8, "menuIced_bt8");
	gtk_widget_set_name(bt_menuIced_back, "bt_menuIced_back");
	/* MenuIced label*/
	gtk_widget_set_name(clock_lb3, "clock_lb3");
	gtk_widget_set_name(menuIced_label, "menuIced_label");
	gtk_widget_set_name(menuIced_label_bt1, "menuIced_label_bt1");
	gtk_widget_set_name(menuIced_label_bt2, "menuIced_label_bt2");
	gtk_widget_set_name(menuIced_label_bt3, "menuIced_label_bt3");
	gtk_widget_set_name(menuIced_label_bt4, "menuIced_label_bt4");
	gtk_widget_set_name(menuIced_label_bt5, "menuIced_label_bt5");
	gtk_widget_set_name(menuIced_label_bt6, "menuIced_label_bt6");
	gtk_widget_set_name(menuIced_label_bt7, "menuIced_label_bt7");
	gtk_widget_set_name(menuIced_label_bt8, "menuIced_label_bt8");
	/* MenuIced logo*/
	gtk_widget_set_name(menuIced_logo_bt1, "menuIced_logo_bt1");
	gtk_widget_set_name(menuIced_logo_bt2, "menuIced_logo_bt2");
	gtk_widget_set_name(menuIced_logo_bt3, "menuIced_logo_bt3");
	gtk_widget_set_name(menuIced_logo_bt4, "menuIced_logo_bt4");
	gtk_widget_set_name(menuIced_logo_bt5, "menuIced_logo_bt5");
	gtk_widget_set_name(menuIced_logo_bt6, "menuIced_logo_bt6");
	gtk_widget_set_name(menuIced_logo_bt7, "menuIced_logo_bt7");
	gtk_widget_set_name(menuIced_logo_bt8, "menuIced_logo_bt8");
	gtk_widget_set_name(bt_menuIced_back_logo, "bt_menuIced_back_logo");
	/* MenuIced harga*/
	gtk_widget_set_name(menuIced_harga1, "menuIced_harga1");
	gtk_widget_set_name(menuIced_harga2, "menuIced_harga2");
	gtk_widget_set_name(menuIced_harga3, "menuIced_harga3");
	gtk_widget_set_name(menuIced_harga4, "menuIced_harga4");
	gtk_widget_set_name(menuIced_harga5, "menuIced_harga5");
	gtk_widget_set_name(menuIced_harga6, "menuIced_harga6");
	gtk_widget_set_name(menuIced_harga7, "menuIced_harga7");
	gtk_widget_set_name(menuIced_harga8, "menuIced_harga8");
	
	gtk_widget_set_name(menuIced_grid_bt, "menuIced_grid_bt");
	
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	int gspace_row = screenWidth/100; 
	int gspace_col = screenWidth/100;
	gtk_grid_set_row_spacing(GTK_GRID(menuIced_grid_bt), gspace_row);
	gtk_grid_set_column_spacing(GTK_GRID(menuIced_grid_bt), gspace_col);
	gtk_box_set_spacing(GTK_BOX(menuIced_box), gspace_row);
	gtk_widget_set_margin_top(menuIced_box, main_box_space);
	gtk_widget_set_margin_bottom(menuIced_box, main_box_space);
	gtk_widget_set_margin_start(menuIced_box, main_box_side_margin);
	gtk_widget_set_margin_end(menuIced_box, main_box_side_margin);
}

void gtk_windowMenuIced_connect(){
	g_signal_connect(windowMenuIced, "destroy", G_CALLBACK(close_windowMenuIced), NULL);
	g_signal_connect(bt_menuIced_back, "clicked", G_CALLBACK (close_windowMenuIced), NULL);
	g_signal_connect(menuIced_bt1, "clicked", G_CALLBACK (cek_menuIced), NULL);
	g_signal_connect(menuIced_bt2, "clicked", G_CALLBACK (cek_menuIced), NULL);
	g_signal_connect(menuIced_bt3, "clicked", G_CALLBACK (cek_menuIced), NULL);
	g_signal_connect(menuIced_bt4, "clicked", G_CALLBACK (cek_menuIced), NULL);
	g_signal_connect(menuIced_bt5, "clicked", G_CALLBACK (cek_menuIced), NULL);
	g_signal_connect(menuIced_bt6, "clicked", G_CALLBACK (cek_menuIced), NULL);
	g_signal_connect(menuIced_bt7, "clicked", G_CALLBACK (cek_menuIced), NULL);
	g_signal_connect(menuIced_bt8, "clicked", G_CALLBACK (cek_menuIced), NULL);
}

static void close_windowMenuIced(){
	gtk_widget_hide(windowMenuIced);
}

static void cek_menuIced(GtkWidget *btWidget){
	
	if (btWidget == menuIced_bt1){
		harga = gtk_label_get_text (GTK_LABEL (menuIced_harga1));
		menu = gtk_label_get_text (GTK_LABEL (menuIced_label_bt1));
	}
	if (btWidget == menuIced_bt2){
		harga = gtk_label_get_text (GTK_LABEL (menuIced_harga2));
		menu = gtk_label_get_text (GTK_LABEL (menuIced_label_bt2));
	}
	if (btWidget == menuIced_bt3){
		harga = gtk_label_get_text (GTK_LABEL (menuIced_harga3));
		menu = gtk_label_get_text (GTK_LABEL (menuIced_label_bt3));
	}
	if (btWidget == menuIced_bt4){
		harga = gtk_label_get_text (GTK_LABEL (menuIced_harga4));
		menu = gtk_label_get_text (GTK_LABEL (menuIced_label_bt4));
	}
	if (btWidget == menuIced_bt5){
		harga = gtk_label_get_text (GTK_LABEL (menuIced_harga5));
		menu = gtk_label_get_text (GTK_LABEL (menuIced_label_bt5));
	}
	if (btWidget == menuIced_bt6){
		harga = gtk_label_get_text (GTK_LABEL (menuIced_harga6));
		menu = gtk_label_get_text (GTK_LABEL (menuIced_label_bt6));
	}
	if (btWidget == menuIced_bt7){
		harga = gtk_label_get_text (GTK_LABEL (menuIced_harga7));
		menu = gtk_label_get_text (GTK_LABEL (menuIced_label_bt7));
	}
	if (btWidget == menuIced_bt8){
		harga = gtk_label_get_text (GTK_LABEL (menuIced_harga8));
		menu = gtk_label_get_text (GTK_LABEL (menuIced_label_bt8));
	}
		
	gtk_label_set_text (GTK_LABEL(payment_harga_gopay), (const gchar* ) harga);
	gtk_label_set_text (GTK_LABEL(payment_menu_gopay), (const gchar* ) menu);
	
	gtk_label_set_text (GTK_LABEL(payment_harga_ovo), (const gchar* ) harga);
	gtk_label_set_text (GTK_LABEL(payment_menu_ovo), (const gchar* ) menu);
	
	gtk_label_set_text (GTK_LABEL(payment_harga_qris), (const gchar* ) harga);
	gtk_label_set_text (GTK_LABEL(payment_menu_qris), (const gchar* ) menu);
	
	gtk_label_set_text (GTK_LABEL(payment_harga_dana), (const gchar* ) harga);
	gtk_label_set_text (GTK_LABEL(payment_menu_dana), (const gchar* ) menu);
	
	gtk_label_set_text (GTK_LABEL(payment_harga_shopeePay), (const gchar* ) harga);
	gtk_label_set_text (GTK_LABEL(payment_menu_shopeePay), (const gchar* ) menu);
	
	view_windowPayment();
}

/* windowPayment */
static void start_windowPayment(){
	int argc; char **argv;
	gtk_init(&argc, &argv);
	gtk_builder_and_attrib_init();

	gtk_windowPayment_init();
	gtk_windowPayment_setAttrib();
	gtk_windowPayment_connect();

	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show(windowPayment);
	gtk_widget_hide(windowPayment);
}

static void view_windowPayment(){
	gtk_widget_show(windowPayment);
}

void gtk_windowPayment_init(){
	windowPayment=GTK_WIDGET(gtk_builder_get_object(builder, "windowPayment"));
	/* Payment box*/
	payment_box=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box"));
	payment_box_display=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_display"));
	payment_box_display1=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_display1"));
	payment_box_back=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_back"));
	payment_box_top=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_top"));
	payment_box_gopay=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_gopay"));
	payment_box_ovo=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_ovo"));
	payment_box_qris=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_qris"));
	payment_box_dana=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_dana"));
	payment_box_shopeePay=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_shopeePay"));
	payment_box_menu=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_menu"));
	payment_box_bt=GTK_WIDGET(gtk_builder_get_object(builder, "payment_box_bt"));
	/* Payment stack*/
	payment_stack=GTK_WIDGET(gtk_builder_get_object(builder, "payment_stack"));
	/* Payment button*/
	bt_payment=GTK_WIDGET(gtk_builder_get_object(builder, "bt_payment"));
	bt_payment_back=GTK_WIDGET(gtk_builder_get_object(builder, "bt_payment_back"));
	/* Payment label*/
	clock_lb4=GTK_WIDGET(gtk_builder_get_object(builder, "clock_lb4"));
	payment_label=GTK_WIDGET(gtk_builder_get_object(builder, "payment_label"));
	payment_label1=GTK_WIDGET(gtk_builder_get_object(builder, "payment_label1"));
	bt_payment_label=GTK_WIDGET(gtk_builder_get_object(builder, "bt_payment_label"));
	timer4=GTK_WIDGET(gtk_builder_get_object(builder, "clock_lb4"));
	/* Payment logo*/
	payment_logo_gopay=GTK_WIDGET(gtk_builder_get_object(builder, "payment_logo_gopay"));
	payment_logo_ovo=GTK_WIDGET(gtk_builder_get_object(builder, "payment_logo_ovo"));
	payment_logo_qris=GTK_WIDGET(gtk_builder_get_object(builder, "payment_logo_qris"));
	payment_logo_dana=GTK_WIDGET(gtk_builder_get_object(builder, "payment_logo_dana"));
	payment_logo_shopeePay=GTK_WIDGET(gtk_builder_get_object(builder, "payment_logo_shopeePay"));
	bt_payment_back_logo=GTK_WIDGET(gtk_builder_get_object(builder, "bt_payment_back_logo"));
	bt_payment_logo=GTK_WIDGET(gtk_builder_get_object(builder, "bt_payment_logo"));
	/* Payment harga*/
	payment_harga_gopay=GTK_WIDGET(gtk_builder_get_object(builder, "payment_harga_gopay"));
	payment_harga_ovo=GTK_WIDGET(gtk_builder_get_object(builder, "payment_harga_ovo"));
	payment_harga_qris=GTK_WIDGET(gtk_builder_get_object(builder, "payment_harga_qris"));
	payment_harga_dana=GTK_WIDGET(gtk_builder_get_object(builder, "payment_harga_dana"));
	payment_harga_shopeePay=GTK_WIDGET(gtk_builder_get_object(builder, "payment_harga_shopeePay"));
	/* Payment menu*/
	payment_menu_gopay=GTK_WIDGET(gtk_builder_get_object(builder, "payment_menu_gopay"));
	payment_menu_ovo=GTK_WIDGET(gtk_builder_get_object(builder, "payment_menu_ovo"));
	payment_menu_qris=GTK_WIDGET(gtk_builder_get_object(builder, "payment_menu_qris"));
	payment_menu_dana=GTK_WIDGET(gtk_builder_get_object(builder, "payment_menu_dana"));
	payment_menu_shopeePay=GTK_WIDGET(gtk_builder_get_object(builder, "payment_menu_shopeePay"));
	
	payment_gopay_bg=GTK_WIDGET(gtk_builder_get_object(builder, "payment_gopay_bg"));
	
}

void gtk_windowPayment_setAttrib(){
	gtk_window_fullscreen(GTK_WINDOW(windowPayment));

	gtk_widget_set_name(windowPayment, "windowPayment");
	/* Payment box*/
	gtk_widget_set_name(payment_box, "payment_box");
	gtk_widget_set_name(payment_box_display, "payment_box_display");
	gtk_widget_set_name(payment_box_display1, "payment_box_display1");
	gtk_widget_set_name(payment_box_back, "payment_box_back");
	gtk_widget_set_name(payment_box_top, "payment_box_top");
	gtk_widget_set_name(payment_box_gopay, "payment_box_gopay");
	gtk_widget_set_name(payment_box_ovo, "payment_box_ovo");
	gtk_widget_set_name(payment_box_qris, "payment_box_qris");
	gtk_widget_set_name(payment_box_dana, "payment_box_dana");
	gtk_widget_set_name(payment_box_shopeePay, "payment_box_shopeePay");
	gtk_widget_set_name(payment_box_menu, "payment_box_menu");
	gtk_widget_set_name(payment_box_bt, "payment_box_bt");
	/* Payment button*/
	gtk_widget_set_name(bt_payment, "bt_payment");
	gtk_widget_set_name(bt_payment_back, "bt_payment_back");
	/* Payment label*/
	gtk_widget_set_name(clock_lb4, "clock_lb4");
	gtk_widget_set_name(payment_label, "payment_label");
	gtk_widget_set_name(payment_label1, "payment_label1");
	gtk_widget_set_name(bt_payment_label, "bt_payment_label");
	/* Payment logo*/
	gtk_widget_set_name(payment_logo_gopay, "payment_logo_gopay");
	gtk_widget_set_name(payment_logo_ovo, "payment_logo_ovo");
	gtk_widget_set_name(payment_logo_qris, "payment_logo_qris");
	gtk_widget_set_name(payment_logo_dana, "payment_logo_dana");
	gtk_widget_set_name(payment_logo_shopeePay, "payment_logo_shopeePay");
	gtk_widget_set_name(bt_payment_back_logo, "bt_payment_back_logo");
	gtk_widget_set_name(bt_payment_logo, "bt_payment_logo");
	/* Payment harga*/
	gtk_widget_set_name(payment_harga_gopay, "payment_harga_gopay");
	gtk_widget_set_name(payment_harga_ovo, "payment_harga_ovo");
	gtk_widget_set_name(payment_harga_qris, "payment_harga_qris");
	gtk_widget_set_name(payment_harga_dana, "payment_harga_dana");
	gtk_widget_set_name(payment_harga_shopeePay, "payment_harga_shopeePay");
	/* Payment menu*/
	gtk_widget_set_name(payment_menu_gopay, "payment_menu_gopay");
	gtk_widget_set_name(payment_menu_ovo, "payment_menu_ovo");
	gtk_widget_set_name(payment_menu_qris, "payment_menu_qris");
	gtk_widget_set_name(payment_menu_dana, "payment_menu_dana");
	gtk_widget_set_name(payment_menu_shopeePay, "payment_menu_shopeePay");
	
	gtk_widget_set_name(payment_gopay_bg, "payment_gopay_bg");
	
	
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	int gspace_row = screenWidth/100; 
	int gspace_col = screenWidth/100;
	//~ gtk_grid_set_row_spacing(GTK_GRID(menuIced_grid_bt), gspace_row);
	//~ gtk_grid_set_column_spacing(GTK_GRID(menuIced_grid_bt), gspace_col);
	gtk_box_set_spacing(GTK_BOX(payment_box), gspace_row);
	gtk_widget_set_margin_top(payment_box, main_box_space);
	gtk_widget_set_margin_bottom(payment_box, main_box_space);
	gtk_widget_set_margin_start(payment_box, main_box_side_margin);
	gtk_widget_set_margin_end(payment_box, main_box_side_margin);
}

void gtk_windowPayment_connect(){
	g_signal_connect(windowPayment, "destroy", G_CALLBACK(close_windowPayment), NULL);
	g_signal_connect(bt_payment_back, "clicked", G_CALLBACK (close_windowPayment), NULL);
	g_signal_connect(bt_payment, "clicked", G_CALLBACK (view_windowSucces), NULL);
}

static void close_windowPayment(){
	gtk_widget_hide(windowPayment);
}

/* windowSucces */
static void start_windowSucces(){
	int argc; char **argv;
	gtk_init(&argc, &argv);
	gtk_builder_and_attrib_init();

	gtk_windowSucces_init();
	gtk_windowSucces_setAttrib();
	gtk_windowSucces_connect();

	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show(windowSucces);
	gtk_widget_hide(windowSucces);
}

static void view_windowSucces(){
	gtk_widget_show(windowSucces);
}

void gtk_windowSucces_init(){
	windowSucces=GTK_WIDGET(gtk_builder_get_object(builder, "windowSucces"));
	/* Succes box*/
	box_succes=GTK_WIDGET(gtk_builder_get_object(builder, "box_succes"));
	/* Succes button*/
	bt_succes=GTK_WIDGET(gtk_builder_get_object(builder, "bt_succes"));
	/* Succes label*/
	succes_label=GTK_WIDGET(gtk_builder_get_object(builder, "succes_label"));
	/* Succes logo*/
	succes_logo=GTK_WIDGET(gtk_builder_get_object(builder, "succes_logo"));
}

void gtk_windowSucces_setAttrib(){
	gtk_window_fullscreen(GTK_WINDOW(windowSucces));

	gtk_widget_set_name(windowSucces, "windowSucces");
	/* Succes box*/
	gtk_widget_set_name(box_succes, "box_succes");
	/* Succes button*/
	gtk_widget_set_name(bt_succes, "bt_succes");
	/* Succes label*/
	gtk_widget_set_name(succes_label, "succes_label");
	/* Succes logo*/
	gtk_widget_set_name(succes_logo, "succes_logo");
	
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void gtk_windowSucces_connect(){
	g_signal_connect(bt_succes, "clicked", G_CALLBACK (close_windowSucces), NULL);
}

static void close_windowSucces(){
	gtk_widget_hide(windowMenu);
	gtk_widget_hide(windowMenuHot);
	gtk_widget_hide(windowMenuIced);
	gtk_widget_hide(windowPayment);
	gtk_widget_hide(windowSucces);
}
