#include <config.h>
#include <string.h>
#include <gtk/gtk.h>
#include <goocanvas.h>


static GooCanvasItem *ellipse1, *ellipse2, *rect1, *rect2, *rect3, *rect4;


static void
start_animation_clicked (GtkWidget *button, gpointer data)
{
  /* Absolute. */
  goo_canvas_item_set_simple_transform (ellipse1, 100, 100, 1, 0);
  goo_canvas_item_animate (ellipse1, 500, 100, 2, 720, TRUE, 2000, 40,
			   GOO_CANVAS_ANIMATE_BOUNCE);

  goo_canvas_item_set_simple_transform (rect1, 100, 200, 1, 0);
  goo_canvas_item_animate (rect1, 100, 200, 1, 350, TRUE, 40 * 36, 40,
			   GOO_CANVAS_ANIMATE_RESTART);

  goo_canvas_item_set_simple_transform (rect3, 200, 200, 1, 0);
  goo_canvas_item_animate (rect3, 200, 200, 3, 0, TRUE, 400, 40,
			   GOO_CANVAS_ANIMATE_BOUNCE);

  /* Relative. */
  goo_canvas_item_set_simple_transform (ellipse2, 100, 400, 1, 0);
  goo_canvas_item_animate (ellipse2, 400, 0, 2, 720, FALSE, 2000, 40,
			   GOO_CANVAS_ANIMATE_BOUNCE);

  goo_canvas_item_set_simple_transform (rect2, 100, 500, 1, 0);
  goo_canvas_item_animate (rect2, 0, 0, 1, 350, FALSE, 40 * 36, 40,
			   GOO_CANVAS_ANIMATE_RESTART);

  goo_canvas_item_set_simple_transform (rect4, 200, 500, 1, 0);
  goo_canvas_item_animate (rect4, 0, 0, 3, 0, FALSE, 400, 40,
			   GOO_CANVAS_ANIMATE_BOUNCE);
}


static void
stop_animation_clicked (GtkWidget *button, gpointer data)
{
  goo_canvas_item_stop_animation (ellipse1);
  goo_canvas_item_stop_animation (ellipse2);
  goo_canvas_item_stop_animation (rect1);
  goo_canvas_item_stop_animation (rect2);
  goo_canvas_item_stop_animation (rect3);
  goo_canvas_item_stop_animation (rect4);
}


static void
on_animation_finished (GooCanvasItem *item,
		       gboolean       stopped,
		       gpointer       data)
{
  g_print ("Animation finished stopped: %i\n", stopped);

#if 0
  /* Test starting another animation. */
  goo_canvas_item_animate (ellipse1, 500, 200, 2, 720, TRUE, 2000, 40,
			   GOO_CANVAS_ANIMATE_BOUNCE);
#endif
}


static void
setup_canvas (GtkWidget *canvas)
{
  GooCanvasItem *root;

  root = goo_canvas_get_root_item (GOO_CANVAS (canvas));

  /* Absolute. */
  ellipse1 = goo_canvas_ellipse_new (root, 0, 0, 25, 15,
				     "fill-color", "blue",
				     NULL);
  goo_canvas_item_translate (ellipse1, 100, 100);
  g_signal_connect (ellipse1, "animation_finished",
		    G_CALLBACK (on_animation_finished), NULL);

  rect1 = goo_canvas_rect_new (root, -10, -10, 20, 20,
			       "fill-color", "blue",
			       NULL);
  goo_canvas_item_translate (rect1, 100, 200);

  rect3 = goo_canvas_rect_new (root, -10, -10, 20, 20,
			       "fill-color", "blue",
			       NULL);
  goo_canvas_item_translate (rect3, 200, 200);

  /* Relative. */
  ellipse2 = goo_canvas_ellipse_new (root, 0, 0, 25, 15,
				     "fill-color", "red",
				     NULL);
  goo_canvas_item_translate (ellipse2, 100, 400);

  rect2 = goo_canvas_rect_new (root, -10, -10, 20, 20,
			       "fill-color", "red",
			       NULL);
  goo_canvas_item_translate (rect2, 100, 500);

  rect4 = goo_canvas_rect_new (root, -10, -10, 20, 20,
			       "fill-color", "red",
			       NULL);
  goo_canvas_item_translate (rect4, 200, 500);
}


GtkWidget *
create_animation_page (void)
{
  GtkWidget *vbox, *hbox, *w, *scrolled_win, *canvas;

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
  g_object_set (vbox, "margin", 4, NULL);
  gtk_widget_show (vbox);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4);
  gtk_box_pack_start (GTK_BOX (vbox), hbox);
  gtk_widget_show (hbox);

  w = gtk_button_new_with_label("Start Animation");
  gtk_box_pack_start (GTK_BOX (hbox), w);
  gtk_widget_show (w);
  g_signal_connect (w, "clicked", G_CALLBACK (start_animation_clicked),
		    NULL);

  w = gtk_button_new_with_label("Stop Animation");
  gtk_box_pack_start (GTK_BOX (hbox), w);
  gtk_widget_show (w);
  g_signal_connect (w, "clicked", G_CALLBACK (stop_animation_clicked),
		    NULL);


  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_win),
				       GTK_SHADOW_IN);
  gtk_widget_show (scrolled_win);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_win);
  gtk_widget_set_halign (scrolled_win, GTK_ALIGN_FILL);
  gtk_widget_set_valign (scrolled_win, GTK_ALIGN_FILL);
  gtk_widget_set_hexpand (scrolled_win, TRUE);
  gtk_widget_set_vexpand (scrolled_win, TRUE);

  canvas = goo_canvas_new ();
  gtk_widget_set_size_request (canvas, 600, 450);
  goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 1000, 1000);
  gtk_widget_show (canvas);
  gtk_container_add (GTK_CONTAINER (scrolled_win), canvas);

  setup_canvas (canvas);

  return vbox;
}
