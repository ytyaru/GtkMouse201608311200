#include <gtk-2.0/gtk/gtk.h>
#include <cairo.h>
#include <glib.h>
#include <glib/gprintf.h> // g_sprintf

int g_mouseX = 0;
int g_mouseY = 0;
GtkWidget *drawing_area = NULL;
gboolean Draw(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	cairo_t *cr;
	cr = gdk_cairo_create(widget->window);
	
	cairo_select_font_face (cr, "Serif", CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 24);

	gchar posStr[256] = "";
	g_sprintf(posStr, "(%d,%d)", (int)g_mouseX, (int)g_mouseY);
	cairo_set_source_rgba(cr, 1, 0, 0, 1);
	cairo_move_to(cr, (int)g_mouseX, (int)g_mouseY);
	cairo_show_text(cr, posStr);

	cairo_destroy(cr);
	return FALSE;
}
gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	g_print ("delete_event\n");
	return FALSE; // windowが"delete_event"により破棄される
	//return TRUE; // windowが"delete_event"により破棄されない
}
void destroy(GtkWidget *widget, gpointer data)
{
	g_print ("destroy\n");
	gtk_main_quit ();
}
gint motion_notify_event (GtkWidget *widget, GdkEventMotion *event)
{
	g_mouseX = (int)event->x;
	g_mouseY = (int)event->y;
	g_print("motion_notify_event (%f,%f)\n", event->x, event->y);
	gtk_widget_queue_draw_area(
		drawing_area,
		0,
		0,
		400,
		300);
	return TRUE;
}
int main(int argc, char* argv[])
{
	gtk_set_locale();
	gtk_init(&argc, &argv);

	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Mouse Point Draw");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_signal_connect (GTK_OBJECT (window), "delete_event",
						GTK_SIGNAL_FUNC (delete_event), NULL);
	gtk_signal_connect (GTK_OBJECT (window), "destroy",
						GTK_SIGNAL_FUNC (destroy), NULL);
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (drawing_area, 400, 300);
	gtk_container_add(GTK_CONTAINER(window), drawing_area);
	g_signal_connect(G_OBJECT(drawing_area), "expose_event", G_CALLBACK(Draw), NULL);
	
	gtk_signal_connect (GTK_OBJECT (drawing_area), "motion_notify_event",
		(GtkSignalFunc) motion_notify_event, NULL);
	gtk_widget_set_events(drawing_area,
		GDK_POINTER_MOTION_MASK
	);
	
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}