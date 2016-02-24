#include "X3D.h"

#include <SDL/SDL.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdk.h>

void init() {
  X3D_InitSettings init = {
    .screen_w = 640,
    .screen_h = 480,
    .screen_scale = 1,
    .fullscreen = X3D_FALSE,
    .fov = ANG_60,
    .flags = X3D_INIT_VIRTUAL_SCREEN
  };
  
  x3d_init(&init);
}

void init_scene() {
  // Create an octogonal prism
  X3D_Prism3D* prism = alloca(1000);
  
  x3d_prism3d_construct(prism, 8, 500, 200, (X3D_Vex3D_angle256) { 0, 0, 0 });
  uint16 id = x3d_segmentbuilder_add_uncompressed_segment(prism)->base.id;
  
  x3d_segmentbuilder_add_extruded_segment(x3d_segfaceid_create(id, 3), 200);
  
  // Setup the camera
  // Setup the camera
  x3d_camera_init();
  X3D_CameraObject* cam = x3d_playermanager_get()->player[0].cam;

  cam->base.base.pos = (X3D_Vex3D_fp16x8) { 0, 0, 0 };
  cam->base.angle = (X3D_Vex3D_angle256) { 0, 0, 0 };
  x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
}

GtkWidget* drawing_area;

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
  X3D_CameraObject* cam = x3d_playermanager_get()->player[0].cam;
  
  switch (event->keyval)
  {
    case GDK_p:
      cam->base.angle.y -= 2;
      x3d_mat3x3_construct(&cam->base.mat, &cam->base.angle);
      gtk_widget_queue_draw(drawing_area);
      break;
  }
  
  return FALSE;
}


void sdlsurface_to_drawing_area(SDL_Surface* s, GdkDrawable* drawing_area) {
  GdkGC* ignored_gc = gdk_gc_new(drawing_area);
  
  int width = -1, height = -1;
  gdk_drawable_get_size(drawing_area, &width, &height);
  /* gdk_draw_rgb_image uses hardcoded 24bit RGB (in that order),
    which is usually the reverse order than my x86 display */
  Uint32 rmask, gmask, bmask, amask;
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  #else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  #endif
  amask = 0x00000000;
  SDL_Surface *screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,
                                            24, rmask, gmask, bmask, amask);

  SDL_BlitSurface(s, NULL, screen, NULL);
  
  gdk_draw_rgb_image(drawing_area, ignored_gc,
                   0, 0, screen->w, screen->h,
                   GDK_RGB_DITHER_NONE,
                   (guchar*)screen->pixels, screen->pitch);
  
  SDL_FreeSurface(screen);

  g_object_unref(ignored_gc);
}

gboolean
draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
 
  x3d_render(x3d_playermanager_get()->player[0].cam);
  sdlsurface_to_drawing_area(x3d_screen_get_internal(), GDK_DRAWABLE(widget->window));
  
 return FALSE;
}

int init_gtk(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *vbox;

  GtkWidget *menubar;
  GtkWidget *fileMenu;
  GtkWidget *fileMi;
  GtkWidget *newMi;
  GtkWidget *openMi;
  GtkWidget *quitMi;

  GtkWidget *sep;

  GtkAccelGroup *accel_group = NULL;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
  gtk_window_set_title(GTK_WINDOW(window), "Images");

  vbox = gtk_vbox_new(0, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  menubar = gtk_menu_bar_new();
  fileMenu = gtk_menu_new();

  accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

  fileMi = gtk_menu_item_new_with_mnemonic("_File");
  newMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
  openMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
  sep = gtk_separator_menu_item_new();
  quitMi = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), newMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), sep);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(quitMi), "activate",
      G_CALLBACK(gtk_main_quit), NULL);
  
  drawing_area = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawing_area, 640, 480);
  g_signal_connect (G_OBJECT (drawing_area), "expose_event",
                    G_CALLBACK (draw_callback), NULL);
  
  gtk_container_add (GTK_CONTAINER (vbox), drawing_area);
  gtk_widget_show(drawing_area);

  g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (on_key_press), NULL);
  
  gtk_widget_show_all(window);
  
  gtk_main();

  return 0;
}


int main(int argc, char *argv[]) {
  init();
  init_scene();
  
  x3d_screen_clear(0);
  x3d_render(x3d_playermanager_get()->player[0].cam);
  
  init_gtk(argc, argv);

  
  x3d_cleanup();
  
}