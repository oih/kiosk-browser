#include <stdio.h>
#include <signal.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <webkit/webkit.h>

gboolean on_key_press(GtkWidget*, GdkEventKey*, gpointer);

void reload_browser(int);
void maximize();
void unmaximize();

static WebKitWebView* web_view;
static GtkWidget *window;
gchar* default_url = "http://oih.ac/";

int main(int argc, char** argv) {
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());

  signal(SIGHUP, reload_browser);

  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(web_view));

  if(argc > 1) {
    webkit_web_view_load_uri(web_view, argv[1]);
  }
  else {
    webkit_web_view_load_uri(web_view, default_url);
  }

  maximize();
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}

gboolean on_key_press(GtkWidget* window, GdkEventKey* key, gpointer userdata) {
  if(key->type == GDK_KEY_PRESS && key->keyval == GDK_F5) {
    reload_browser(0);
  }
  return FALSE;
}

void reload_browser(int signum) {
  webkit_web_view_reload_bypass_cache(web_view);
}

void maximize() {
  gtk_window_maximize(GTK_WINDOW(window));
  gtk_window_fullscreen(GTK_WINDOW(window));
  gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
}

void unmaximize() {
  gtk_window_unmaximize(GTK_WINDOW(window));
  gtk_window_unfullscreen(GTK_WINDOW(window));
  gtk_window_set_decorated(GTK_WINDOW(window), TRUE);
  gtk_window_resize(GTK_WINDOW(window), 1280, 768);
}
