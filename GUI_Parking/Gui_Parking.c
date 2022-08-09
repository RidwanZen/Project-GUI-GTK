// Compile : gcc -Wno-format -o Gui_Parking Gui_Parking.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` $(mysql_config --cflags) $(mysql_config --libs) -export-dynamic
// Run : ./Gui_Parking

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <mysql.h>

char *IntToStr(int x);

int i = 0, j = 0;
int num1 = 0, num2 = 0;
int count1 = 0, count2 = 0;
char *status_server = "SERVER OFFLINE";
int flag = 0;

/* Declare for MySql  */
#define MAX_STRING 128
char query[MAX_STRING] = {0};

MYSQL *conn;
MYSQL *conn1;
MYSQL *conn2;
MYSQL_RES *res;
MYSQL_RES *res1;
MYSQL_RES *res2;
MYSQL_ROW row;
MYSQL_ROW row1;
MYSQL_ROW row2;
	
char *server = "192.168.1.15";	// set IP Server
char *user = "ridwan";	// user name
char *password = ""; /* set me first */ //password user
char *database = "parking_area";	// database

const int screenWidth = 2080, screenHight = 2080;
int main_box_space, main_nbox_space, main_box_side_margin, main_box_main_width;
int logo_width, logo_hight;
int imBack_width, imBack_hight;
int main_box_label_margin;

GtkCssProvider *cssProvider;
GtkBuilder *builder;

GtkWidget *timer;

// Function Timer
gboolean timer_handler(GtkWidget *);

//mainWindow
GtkWidget *window;
GtkWidget *main_box, *main_grid_home, *main_box_log, *main_box_log1, *main_stack;
GtkWidget *lb_uid, *lb_saldo, *lb_status, *lb_server;
GtkWidget *value_uid, *value_saldo, *value_status;

//log_transaksi
GtkWidget *view_log;
GtkWidget *log_grid, *log_1, *log_2, *log_3, *log_4, *log_5, *log_6, *log_7, *log_8, *log_9, *log_10;
GtkWidget *scroll, *bt_clear_log;
GtkWidget *log_lb_no, *log_lb_waktu, *log_lb_uid, *log_lb_cabang, *log_lb_gerbang, *log_lb_gardu, *log_lb_saldoawal, *log_lb_tarif, *log_lb_saldoakhir, *log_lb_status;
GtkWidget *label1[100],*label2[100],*label3[100],*label4[100],*label5[100],*label6[100],*label7[100],*label8[100],*label9[100],*label10[100];

//log_peralatan
GtkWidget *view_log1;
GtkWidget *log_grid1, *log_11, *log_12, *log_13;
GtkWidget *scroll1, *bt_clear_log1;
GtkWidget *log_lb_no2, *log_lb_waktu2, *log_lb_status2;
GtkWidget *label11[100],*label12[100],*label13[100];

char 	tmp[1024], tmp1[1024];	// general use
char	data0[100],data1[100],data2[100],	// home
		data3[100],data4[100],data5[100],data6[100],data7[100],data8[100],data9[100],data10[100],data11[100],data12[100],	// log_transaksi
		data13[100],data14[100],data15[100];	// log_peralatan

void gtk_mainWindow_init();
void gtk_mainWindow_setAttrib();
void gtk_mainWindow_connect();
void gtk_builder_and_attrib_init();
void gtk_image_opt(GtkWidget *widget, char *fileName, int width, int hight, int margin_left);
void finish_with_error(MYSQL *con);
void clear_log_transaksi();
void clear_log_peralatan();

int main(int argc, char **argv){
	/*deklarasi variabel
	 * ====================================*/
	main_box_space = screenWidth/100;
	main_box_side_margin = screenWidth/50;
	main_box_main_width = screenWidth/60;
	main_nbox_space = screenWidth/100;
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
	
	/* Update per 1 seconds */
	guint ID = g_timeout_add(200, (GSourceFunc) timer_handler, timer);
	
	//start
	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show_all(window);
	g_object_unref(builder);
	gtk_main();

	return EXIT_SUCCESS;
	}
	

// function looping
gboolean timer_handler(GtkWidget *timer){
	char tmp[1024];
	time_t t = time(0);
	gtk_label_set_text(GTK_LABEL(timer), ctime(&t));	// update time of day
	gtk_label_set_text (GTK_LABEL(lb_server), status_server);	// update set label text status_server
	
	/* kondisi jika server OFFLINE */
	if(status_server == "SERVER OFFLINE"){
		//~ // Initialize MySql
		conn = mysql_init(NULL);
		conn1 = mysql_init(NULL);
		conn2 = mysql_init(NULL);
		
			//~ /* Connect to database */
			if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
				if (mysql_real_connect(conn1, server, user, password, database, 0, NULL, 0)) {
					if (mysql_real_connect(conn2, server, user, password, database, 0, NULL, 0)) {
						status_server = "SERVER ONLINE";	//set server ONLINE
						printf("connection succesfull!\n\n");
					}
				}
			}
			else{	// jika gagal connect to server
				//~ fprintf(stderr, "%s\n", mysql_error(conn));
				status_server = "SERVER OFFLINE";	// set server OFFLINE
			}
	}
	
	/* Jika kondisi server ONLINE */
	if(status_server == "SERVER ONLINE"){
		/* jika flag 0, akan select data table */
		if(flag == 0){
				/* Select SQL query data */
			if (!mysql_query(conn, "SELECT * FROM tb_monitor")) {
				if (!mysql_query(conn1, "SELECT * FROM tb_log_transaksi")) {
					if (!mysql_query(conn2, "SELECT * FROM tb_log_peralatan")) {
						flag = 1;
					}else{status_server = "SERVER OFFLINE";}
				}else{status_server = "SERVER OFFLINE";}
			}else{	// jika gagal select table, artinya server OFFLINE
				status_server = "SERVER OFFLINE";	
					mysql_close(conn);	// closs connection
					mysql_close(conn1);
					mysql_close(conn2);
					printf("connection loss!\n\n");
				}
		}
		
		/* Jika berhasil select data table */
		if(flag == 1){
	
			res = mysql_store_result(conn);	//mentransfer kumpulan hasil dari kueri terakhir
			res1 = mysql_store_result(conn1);
			res2 = mysql_store_result(conn2);

			/* Update data for menu HOME */	
			while(row=mysql_fetch_row(res))
				{
					/* Copy data menu Home */
					memcpy(data0,(uint8_t*)row[0],100);	// copy data uid
					sprintf(data1,"Rp.%s",row[1]);	// copy data saldo-akhir
					memcpy(data2,(uint8_t*)row[2],100);	// copy data status transaksi
					
					gtk_label_set_text (GTK_LABEL(value_uid), (const gchar* ) data0);	// set label uid
					gtk_label_set_text (GTK_LABEL(value_saldo), (const gchar* ) data1);	// set label saldo
					gtk_label_set_text (GTK_LABEL(value_status), (const gchar* ) data2); // set label status

				}
		
			/* ADD data for Log_Transaksi */	
			count1 = i;
			// num1 berisi jumlah baris; dijalankan saat ada data baru pada database
			for(num1 = mysql_num_rows(res1);num1 > count1;num1 = count1)
				{	
					row1=mysql_fetch_row(res1);	// mengambil 1 baris dari jumlah yang didapatkan
					for(int k = 0;k < i;k++) // pengulangan untuk memasukkan data yang baru diupdate
						{
							row1=mysql_fetch_row(res1);	
						}

					/* Copy data menu Log_Transaksi */
					memcpy(data3,(uint8_t*)row1[0],100);	// data nomer
					memcpy(data4,(uint8_t*)row1[1],100);	// data tanggal
					memcpy(data5,(uint8_t*)row1[2],100);	// data uid
					memcpy(data6,(uint8_t*)row1[3],100);	// data cabang
					memcpy(data7,(uint8_t*)row1[4],100);	// data gerbang
					memcpy(data8,(uint8_t*)row1[5],100);	// data gardu
					memcpy(data9,(uint8_t*)row1[6],100);	// data saldo_awal
					memcpy(data10,(uint8_t*)row1[7],100);	// data tarif
					memcpy(data11,(uint8_t*)row1[8],100);	// data saldo_akhir
					memcpy(data12,(uint8_t*)row1[9],100);	// data status_transaksi	
					
					/* buat label baru dengan data yang di input */
					label1[i] = gtk_label_new (data3);	// label nomer
					label2[i] = gtk_label_new (data4);	// label tanggal
					label3[i] = gtk_label_new (data5);	// label uid
					label4[i] = gtk_label_new (data6);	// label cabang
					label5[i] = gtk_label_new (data7);	// label gerbang
					label6[i] = gtk_label_new (data8);	// label gardu
					label7[i] = gtk_label_new (data9);	// label saldo_awal
					label8[i] = gtk_label_new (data10);	// label tarif
					label9[i] = gtk_label_new (data11);	// label saldo_akhir
					label10[i] = gtk_label_new (data12); // label status_transaksi
					// set alignment justify
					gtk_label_set_justify (GTK_LABEL(label1[i]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label2[i]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label3[i]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label4[i]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label5[i]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label6[i]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label7[i]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label8[i]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label9[i]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label10[i]), GTK_JUSTIFY_CENTER);	
					
					/* Menambahkan widget label ke dalam grid */	
					gtk_grid_attach (GTK_GRID(log_1), label1[i], 1, i, 1, 1);
					gtk_grid_attach (GTK_GRID(log_2), label2[i], 1, i, 1, 1);
					gtk_grid_attach (GTK_GRID(log_3), label3[i], 1, i, 1, 1);
					gtk_grid_attach (GTK_GRID(log_4), label4[i], 1, i, 1, 1);
					gtk_grid_attach (GTK_GRID(log_5), label5[i], 1, i, 1, 1);
					gtk_grid_attach (GTK_GRID(log_6), label6[i], 1, i, 1, 1);
					gtk_grid_attach (GTK_GRID(log_7), label7[i], 1, i, 1, 1);
					gtk_grid_attach (GTK_GRID(log_8), label8[i], 1, i, 1, 1);
					gtk_grid_attach (GTK_GRID(log_9), label9[i], 1, i, 1, 1);
					gtk_grid_attach (GTK_GRID(log_10), label10[i], 1, i, 1, 1);

					i++;
					count1 = i;
				}
				
			/* ADD data for Log_Peralatan */	
			count2 = j;
			// num2 berisi jumlah baris; dijalankan saat ada data baru pada database
			for(num2 = mysql_num_rows(res2);num2 > count2;num2 = count2)
				{	
					row2=mysql_fetch_row(res2);	// mengambil 1 baris dari jumlah yang didapatkan
					for(int k = 0;k < j;k++)	// pengulangan untuk memasukkan data yang baru diupdate
						{
							row2=mysql_fetch_row(res2);	
						}

					/* Copy data menu Log_Peralatan */
					memcpy(data13,(uint8_t*)row2[0],100);	// data nomer
					memcpy(data14,(uint8_t*)row2[1],100);	// data tanggal
					memcpy(data15,(uint8_t*)row2[2],100);	// data status device	
					/* buat label baru dengan data yang di input */
					label11[j] = gtk_label_new (data13);	// data nomer
					label12[j] = gtk_label_new (data14);	// data tanggal
					label13[j] = gtk_label_new (data15);	// data status device
					// set alignment justify
					gtk_label_set_justify (GTK_LABEL(label11[j]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label12[j]), GTK_JUSTIFY_CENTER);
					gtk_label_set_justify (GTK_LABEL(label13[j]), GTK_JUSTIFY_CENTER);
					/* Menambahkan widget label ke dalam grid */
					gtk_grid_attach (GTK_GRID(log_11), label11[j], 1, j, 1, 1);
					gtk_grid_attach (GTK_GRID(log_12), label12[j], 1, j, 1, 1);
					gtk_grid_attach (GTK_GRID(log_13), label13[j], 1, j, 1, 1);

					j++;
					count2 = j;
				}
			/* Free memory */
			mysql_free_result(res);
			mysql_free_result(res1);
			mysql_free_result(res2);
			
			gtk_widget_show_all(window);
			flag = 0;
		}
	}
	return TRUE; // False kills the timer
}

void gtk_builder_and_attrib_init(){
	builder=gtk_builder_new();
	gtk_builder_add_from_file (builder, "parking.glade", NULL);

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
	window=GTK_WIDGET(gtk_builder_get_object(builder, "Main_window"));
	main_box=GTK_WIDGET(gtk_builder_get_object(builder, "main_box"));
	main_grid_home=GTK_WIDGET(gtk_builder_get_object(builder, "main_grid_home"));
	main_box_log=GTK_WIDGET(gtk_builder_get_object(builder, "main_box_log"));
	main_stack=GTK_WIDGET(gtk_builder_get_object(builder, "main_stack"));

	// value
	value_uid=GTK_WIDGET(gtk_builder_get_object(builder, "value_uid"));
	value_saldo=GTK_WIDGET(gtk_builder_get_object(builder, "value_saldo"));
	value_status=GTK_WIDGET(gtk_builder_get_object(builder, "value_status"));
	
	//label
	lb_uid=GTK_WIDGET(gtk_builder_get_object(builder, "lb_uid"));
	lb_saldo=GTK_WIDGET(gtk_builder_get_object(builder, "lb_saldo"));
	lb_status=GTK_WIDGET(gtk_builder_get_object(builder, "lb_status"));
	
	// timer
	timer=GTK_WIDGET(gtk_builder_get_object(builder, "timer"));
	lb_server=GTK_WIDGET(gtk_builder_get_object(builder, "lb_server"));
	
	// Button Clear Log
	bt_clear_log=GTK_WIDGET(gtk_builder_get_object(builder, "bt_clear_log"));
	bt_clear_log1=GTK_WIDGET(gtk_builder_get_object(builder, "bt_clear_log1"));
	
	/* Log_Transaksi */
	view_log=GTK_WIDGET(gtk_builder_get_object(builder, "view_log"));
	scroll=GTK_WIDGET(gtk_builder_get_object(builder, "scroll"));
	log_grid=GTK_WIDGET(gtk_builder_get_object(builder, "log_grid"));
	log_1=GTK_WIDGET(gtk_builder_get_object(builder, "log1"));
	log_2=GTK_WIDGET(gtk_builder_get_object(builder, "log2"));
	log_3=GTK_WIDGET(gtk_builder_get_object(builder, "log3"));
	log_4=GTK_WIDGET(gtk_builder_get_object(builder, "log4"));
	log_5=GTK_WIDGET(gtk_builder_get_object(builder, "log5"));
	log_6=GTK_WIDGET(gtk_builder_get_object(builder, "log6"));
	log_7=GTK_WIDGET(gtk_builder_get_object(builder, "log7"));
	log_8=GTK_WIDGET(gtk_builder_get_object(builder, "log8"));
	log_9=GTK_WIDGET(gtk_builder_get_object(builder, "log9"));
	log_10=GTK_WIDGET(gtk_builder_get_object(builder, "log10"));
	
	log_lb_no=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_no1"));
	log_lb_waktu=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_waktu1"));
	log_lb_uid=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_uid1"));
	log_lb_cabang=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_cabang1"));
	log_lb_gerbang=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_gerbang1"));
	log_lb_gardu=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_gardu1"));
	log_lb_saldoawal=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_saldoawal1"));
	log_lb_tarif=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_tarif1"));
	log_lb_saldoakhir=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_saldoakhir1"));
	log_lb_status=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_status1"));
	
	/* Log_Peralatan */
	view_log1=GTK_WIDGET(gtk_builder_get_object(builder, "view_log1"));
	scroll1=GTK_WIDGET(gtk_builder_get_object(builder, "scroll1"));
	log_grid1=GTK_WIDGET(gtk_builder_get_object(builder, "log_grid1"));
	log_11=GTK_WIDGET(gtk_builder_get_object(builder, "log11"));
	log_12=GTK_WIDGET(gtk_builder_get_object(builder, "log12"));
	log_13=GTK_WIDGET(gtk_builder_get_object(builder, "log13"));
	
	log_lb_no2=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_no2"));
	log_lb_waktu2=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_waktu2"));
	log_lb_status2=GTK_WIDGET(gtk_builder_get_object(builder, "log_lb_status2"));
} 

void gtk_mainWindow_setAttrib(){
	//~ gtk_window_fullscreen(GTK_WINDOW(window));

	gtk_widget_set_name (window, "myWindow");
	gtk_widget_set_name (main_box, "main_box");
	gtk_widget_set_name (value_saldo, "value_saldo");
	gtk_widget_set_name (value_status, "value_status");
	gtk_widget_set_name (value_uid, "value_uid");
	gtk_widget_set_name (main_grid_home, "main_grid_home");
	gtk_widget_set_name (main_box_log, "main_box_log");
	gtk_widget_set_name (main_stack, "main_stack");
	gtk_widget_set_name (lb_uid, "lb_uid");
	gtk_widget_set_name (lb_saldo, "lb_saldo");
	gtk_widget_set_name (lb_status, "lb_status");
	gtk_widget_set_name (timer, "timer");
	gtk_widget_set_name (lb_server, "lb_server");
	
	gtk_widget_set_name(view_log, "view_log");
	gtk_widget_set_name(log_grid, "log_grid");
	
	gtk_widget_set_name(bt_clear_log, "bt_clear_log");
	gtk_widget_set_name(bt_clear_log1, "bt_clear_log1");
	
	/* Log_Transaksi */
	gtk_widget_set_name(log_lb_no, "log_lb_no");
	gtk_widget_set_name(log_lb_waktu, "log_lb_waktu");
	gtk_widget_set_name(log_lb_uid, "log_lb_uid");
	gtk_widget_set_name(log_lb_cabang, "log_lb_cabang");
	gtk_widget_set_name(log_lb_gerbang, "log_lb_gerbang");
	gtk_widget_set_name(log_lb_gardu, "log_lb_gardu");
	gtk_widget_set_name(log_lb_saldoawal, "log_lb_saldoawal");
	gtk_widget_set_name(log_lb_tarif, "log_lb_tarif");
	gtk_widget_set_name(log_lb_saldoakhir, "log_lb_saldoakhir");
	gtk_widget_set_name(log_lb_status, "log_lb_status");
	
	gtk_widget_set_name(log_1, "log_1");
	gtk_widget_set_name(log_2, "log_2");
	gtk_widget_set_name(log_3, "log_3");
	gtk_widget_set_name(log_4, "log_4");
	gtk_widget_set_name(log_5, "log_5");
	gtk_widget_set_name(log_6, "log_6");
	gtk_widget_set_name(log_7, "log_7");
	gtk_widget_set_name(log_8, "log_8");
	gtk_widget_set_name(log_9, "log_9");
	gtk_widget_set_name(log_10, "log_10");
	
	/* Log_Peralatan */
	gtk_widget_set_name(log_lb_no2, "log_lb_no2");
	gtk_widget_set_name(log_lb_waktu2, "log_lb_waktu2");
	gtk_widget_set_name(log_lb_status2, "log_lb_status2");
	
	gtk_widget_set_name(log_11, "log_11");
	gtk_widget_set_name(log_12, "log_12");
	gtk_widget_set_name(log_13, "log_13");

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
	int gspace_row = screenWidth/60; 
	int gspace_col = screenWidth/60;
	gtk_grid_set_row_spacing(GTK_GRID(main_grid_home), gspace_row);
	gtk_grid_set_column_spacing(GTK_GRID(main_grid_home), gspace_col);
	gtk_box_set_spacing(GTK_BOX(main_box), gspace_row);
	gtk_widget_set_margin_top(main_box, main_box_space);
	gtk_widget_set_margin_bottom(main_box, main_box_space);
	gtk_widget_set_margin_start(main_box, main_box_side_margin);
	gtk_widget_set_margin_end(main_box, main_box_side_margin);
}

void gtk_mainWindow_connect(){
	/* close connection */
	mysql_close(conn);
	mysql_close(conn1);
	mysql_close(conn2);
	
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(bt_clear_log, "clicked", G_CALLBACK (clear_log_transaksi), NULL);
	g_signal_connect(bt_clear_log1, "clicked", G_CALLBACK (clear_log_peralatan), NULL);
}

// Mengkonvert dari Integer ke String
char *IntToStr(int x){
	char *str=(char *)malloc(10 * sizeof (char));	// Mengalokasikan blok memory sebesar 10byte dan di pointing ke pointer str
	sprintf(str, "%d", x);	// Memasukkan data integer x ke str
	return str;
}

void finish_with_error(MYSQL *conn)
{
  fprintf(stderr, "%s\n", mysql_error(conn));
  mysql_close(conn);
  exit(1);
}

/* function clear log transaksi */
void clear_log_transaksi(){
	    /* Menghapus semua data Table dan reset ke awal */   
    if (mysql_query(conn1, "TRUNCATE TABLE tb_log_transaksi")) {
        printf("Clear Failed\n");
        mysql_close(conn1);
    }
    /* pengulangan untuk menghapus label */
    for(int z = 0;z < count1;z++){
		gtk_widget_destroy(label1[z]);
		gtk_widget_destroy(label2[z]);
		gtk_widget_destroy(label3[z]);
		gtk_widget_destroy(label4[z]);
		gtk_widget_destroy(label5[z]);
		gtk_widget_destroy(label6[z]);
		gtk_widget_destroy(label7[z]);
		gtk_widget_destroy(label8[z]);
		gtk_widget_destroy(label9[z]);
		gtk_widget_destroy(label10[z]);
	}
    
    i = 0;
    count1 = 0;
    num1 = 0;

}

/* function clear log peralatan */
void clear_log_peralatan(){
	    /* Menghapus semua data Table dan reset ke awal */   
    if (mysql_query(conn2, "TRUNCATE TABLE tb_log_peralatan")) {
        printf("Clear Failed\n");
        mysql_close(conn2);
    }
    /* pengulangan untuk menghapus label */
    for(int z = 0;z<count2;z++){
		gtk_widget_destroy(label11[z]);
		gtk_widget_destroy(label12[z]);
		gtk_widget_destroy(label13[z]);
	}
    
    j = 0;
    count2 = 0;
    num2 = 0;
}
