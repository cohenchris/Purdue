#include "sysinfo.h"
#include "procinfo.h"
#include "devices.h"
#include "proc_actions.h"

#include <gtk/gtk.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define G_SOURCE_FUNC(f) ((GSourceFunc) (void (*)(void)) (f))

system_info g_sys_info_struct = { 0 };
process_info *g_stack = NULL;
int g_stack_num = 0;

/* 0 = all processes (default)
 * 1 = active processes
 * 2 = my processes
 */

int g_toggle = 0;

void create_tree_view(GtkWidget *treeview) {
  // create Processes treeview and label
  GtkTreeModel *model;
  GtkTreeIter toplevel, child, iter;
  GtkTreeStore *treestore = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
      G_TYPE_STRING, G_TYPE_STRING);

  process_info *proc_info = get_proc_info();
  for (int i = 0; i < g_num_procs; i++) {
   process_info proc = proc_info[i];

   // filter out active processes
   if (g_toggle == 1) {
     if (strcmp(proc.state, "R (running)") != 0) {
       continue;
     }
   }
   // filter out current user's processes
   else if (g_toggle == 2) {
     char *user = strdup(getenv("USER"));
     if (strcmp(proc.proc_user, user) != 0) {
       continue;
     }
   }
   
   // cast proc_id to string
   char buf[1000] = {0};
   sprintf(buf, "%ld", proc.proc_id);
   char * proc_id = strdup(buf);
   gtk_tree_store_append(treestore, &toplevel, NULL);
   gtk_tree_store_set(treestore, &toplevel,
                    0, proc.proc_name,
                    1, proc.state,
                    2, proc.cpu_time,
                    3, proc_id,
                    4, proc.virtual_mem,
                    -1);

   /*if (g_stack_num == 0) {
     push_parent(proc);
     gtk_tree_store_append(treestore, &toplevel, NULL);
     gtk_tree_store_set(treestore, &toplevel,
                      0, proc.proc_name,
                      1, proc.state,
                      2, proc.cpu_time,
                      3, proc_id,
                      4, proc.virtual_mem,
                      -1);
   }
   else {
     // get parents
     while ((g_stack_num != 0) && (pop_parent() != proc.parent_id)) {
     }
     gtk_tree_store_append(treestore, &child, &toplevel);
     gtk_tree_store_set(treestore, &child,
                       0, proc.proc_name,
                       1, proc.state,
                       2, proc.cpu_time,
                       3, proc.proc_id,
                       4, proc.virtual_mem,
                       -1);
    }*/
  }
  model = GTK_TREE_MODEL(treestore);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
  g_object_unref(model);
}

void toggle_active(GtkWidget *widget, gpointer data) {
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM (widget))) {
    g_toggle = 1;
    create_tree_view(GTK_WIDGET (data));
  }
}

void toggle_all(GtkWidget *widget, gpointer data) {
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM (widget))) {
    g_toggle = 0;
    create_tree_view(GTK_WIDGET (data));
  }
}

void toggle_my(GtkWidget *widget, gpointer data) {
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM (widget))) {
    g_toggle = 2;
    create_tree_view(GTK_WIDGET (data));
  }
}

void toggle_refresh(GtkWidget *widget, gpointer data) {
  create_tree_view(GTK_WIDGET (data));
}

void create_file_sys_tree_view(GtkWidget *treeview) {
  // create Processes treeview and label
  GtkTreeModel *model;
  GtkTreeIter toplevel, child, iter;
  GtkTreeStore *treestore = gtk_tree_store_new(6, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
      G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  // get device info
  device_info *dev_info = get_device_info();
  for (int i = 0; i < g_num_devices; i++) {
   device_info dev = dev_info[i];   
   gtk_tree_store_append(treestore, &toplevel, NULL);
   gtk_tree_store_set(treestore, &toplevel,
                    0, dev.device,
                    1, dev.num_blocks,
                    2, dev.used,
                    3, dev.available,
                    4, dev.use_percent,
                    5, dev.mount_point,
                    -1);
  }
  model = GTK_TREE_MODEL(treestore);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
  g_object_unref(model);
}

void on_changed(GtkWidget *widget, gpointer label) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {
    gtk_tree_model_get(model, &iter, 3, &value,  -1);
  }
}

void stop_process (GtkWidget *menuitem, gpointer userdata) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(userdata), &model, &iter)) {
    gtk_tree_model_get(model, &iter, 3, &value,  -1);

    char *endptr;
    long pid = strtol(value, &endptr, 10); 
    stop_proc(pid);
  }
}

void continue_process (GtkWidget *menuitem, gpointer userdata) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(userdata), &model, &iter)) {
    gtk_tree_model_get(model, &iter, 3, &value,  -1);

    char *endptr;
    long pid = strtol(value, &endptr, 10); 
    continue_proc(pid);
  }
}

void kill_process (GtkWidget *menuitem, gpointer userdata) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(userdata), &model, &iter)) {
    gtk_tree_model_get(model, &iter, 3, &value,  -1);

    char *endptr;
    long pid = strtol(value, &endptr, 10);
    kill_proc(pid);
  }
}

void memory_maps (GtkWidget *menuitem, gpointer userdata) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkWidget *window;
  GtkWidget *label;

  mmap_info *map = NULL;
  char *text = malloc(1000);

  gchar *value;
  gchar *name;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(userdata), &model, &iter)) {
    gtk_tree_model_get(model, &iter, 3, &value,  -1);
    gtk_tree_model_get(model, &iter, 0, &name, -1);

    char *endptr;
    long pid = strtol(value, &endptr, 10);
    map = list_mm(pid);

    sprintf(text, "Memory maps for process \"%s\" (PID %s):", name, value);
    printf("%s\n", text);
    //label = gtk_label_new(text);
    //gtk_container_add(GTK_CONTAINER (window), label);
  }

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (window), "Memory Maps");
  gtk_window_set_default_size(GTK_WINDOW (window), 600, 600);

  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkWidget *treeview;

  // create treeview for memory maps
  treeview = gtk_tree_view_new();

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Address");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 0);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Permissions");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 1);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Offset");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 2);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Dev");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 3);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Inode");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
   
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 4);
 
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Pathname");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 5);

  GtkTreeIter toplevel, child;
  GtkTreeStore *treestore = gtk_tree_store_new(6, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
      G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  for (int i = 0; i < g_num_maps; i++) {
   mmap_info curr_map_info = map[i];   
   gtk_tree_store_append(treestore, &toplevel, NULL);
   gtk_tree_store_set(treestore, &toplevel,
                    0, curr_map_info.addr,
                    1, curr_map_info.perms,
                    2, curr_map_info.offset,
                    3, curr_map_info.dev,
                    4, curr_map_info.inode,
                    5, curr_map_info.pathname,
                    -1);
  }
  model = GTK_TREE_MODEL(treestore);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
  g_object_unref(model);

  GtkWidget *scroll_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER (scroll_window), treeview);

  gtk_container_add(GTK_CONTAINER (window), scroll_window);
  gtk_widget_show_all(window);
}

void open_file (GtkWidget *menuitem, gpointer userdata) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkWidget *window;

  gchar *value;
  gchar *name;

  open_files *files = NULL;
  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(userdata), &model, &iter)) {
    gtk_tree_model_get(model, &iter, 3, &value,  -1);
    gtk_tree_model_get(model, &iter, 0, &name,  -1);
    
    char *endptr;
    long pid = strtol(value, &endptr, 10);
    files = list_open_files(pid);

    char *text = malloc(1000);
    sprintf(text, "Files opened by process \"%s\" (PID %s):", name, value);
    printf("%s\n", text);
  }

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (window), "Open Files");
  gtk_window_set_default_size(GTK_WINDOW (window), 600, 600);

  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkWidget *treeview;

  // create treeview for files
  treeview = gtk_tree_view_new();

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "FD");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 0);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Object");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 1);

  GtkTreeIter toplevel, child;
  GtkTreeStore *treestore = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

  for (int i = 0; i < g_num_fds; i++) {
   open_files f_info = files[i];   
   gtk_tree_store_append(treestore, &toplevel, NULL);
   gtk_tree_store_set(treestore, &toplevel,
                    0, f_info.fd,
                    1, f_info.object,
                    -1);
  }
  model = GTK_TREE_MODEL(treestore);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
  g_object_unref(model);

  GtkWidget *scroll_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER (scroll_window), treeview);

  gtk_container_add(GTK_CONTAINER (window), scroll_window);
  gtk_widget_show_all(window);

}

void view_popup_menu (GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
  GtkWidget *menu, *menuitem;
  GtkWidget *separator;

  menu = gtk_menu_new();

  // Create menu
  menuitem = gtk_menu_item_new_with_label("Stop Process");
  g_signal_connect(menuitem, "activate",
                   (GCallback) stop_process, userdata);
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), menuitem);

  menuitem = gtk_menu_item_new_with_label("Continue Process");
  g_signal_connect(menuitem, "activate",
                   (GCallback) continue_process, userdata);
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), menuitem);

  // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), separator);
 
  menuitem = gtk_menu_item_new_with_label("End Process");
  //g_signal_connect(menuitem, "activate",
  //                 (GCallback) stop_process, treeview);
  gtk_widget_set_sensitive(menuitem, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), menuitem);
  
  menuitem = gtk_menu_item_new_with_label("Kill Process");
  g_signal_connect(menuitem, "activate",
                   (GCallback) kill_process, userdata);
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), menuitem);

  // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), separator);

  menuitem = gtk_menu_item_new_with_label("Change priority");
  //g_signal_connect(menuitem, "activate",
  //                 (GCallback) stop_process, treeview);
  gtk_widget_set_sensitive(menuitem, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), menuitem);

  // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), separator);
  
  menuitem = gtk_menu_item_new_with_label("Memory Maps");
  g_signal_connect(menuitem, "activate",
                   (GCallback) memory_maps, userdata);
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), menuitem);

  menuitem = gtk_menu_item_new_with_label("Open Files");
  g_signal_connect(menuitem, "activate",
                   (GCallback) open_file, userdata);
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), menuitem);

  // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), separator);

  menuitem = gtk_menu_item_new_with_label("Properties");
  //g_signal_connect(menuitem, "activate",
  //                 (GCallback) stop_process, treeview);
  gtk_widget_set_sensitive(menuitem, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL (menu), menuitem);

  gtk_widget_show_all(menu);

  gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent*) event);
}

void view_onButtonPressed (GtkWidget *treeview, GdkEventButton *event, gpointer userdata) {
  view_popup_menu(treeview, event, userdata);
}

void view_onPopupMenu (GtkWidget *treeview, gpointer userdata) {
  view_popup_menu(treeview, NULL, userdata);
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;

  GtkWidget *button;
  GtkWidget *notebook;
  GtkWidget *frame;
  GtkWidget *menuBar;
  GtkWidget *subMenu;
  GtkWidget *menuItem;
  GtkWidget *subItem;
  GtkWidget *label;
  GtkWidget *vbox;
  GtkWidget *grid;
  GtkWidget *text;

  GtkWidget *radio_group;
  GtkWidget *radio_button;

  GtkWidget *tog_stat_active;
  GtkWidget *tog_stat_all;
  GtkWidget *tog_stat_my;
  GtkWidget *refresh;
  GtkWidget *separator;

  GtkWidget *scroll_window;

  // create new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW (window), "System Monitor");
  gtk_window_set_default_size(GTK_WINDOW (window), 600, 600);

  // create box
  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER (window), vbox);

  // create new menu
  menuBar = gtk_menu_bar_new();

  menuItem = gtk_menu_item_new_with_label("Monitor");
  gtk_menu_shell_append(GTK_MENU_SHELL (menuBar), menuItem);

  menuItem = gtk_menu_item_new_with_label("Edit");
  gtk_menu_shell_append(GTK_MENU_SHELL (menuBar), menuItem);
  
  menuItem = gtk_menu_item_new_with_label("View");
  gtk_menu_shell_append(GTK_MENU_SHELL (menuBar), menuItem); 
  
  // create submenu for View
  subMenu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), subMenu);

  // create a radio button group
  radio_group = gtk_radio_menu_item_new(NULL);

  // add submenu items
  tog_stat_active = gtk_radio_menu_item_new_with_label_from_widget(
      GTK_RADIO_MENU_ITEM (radio_group), "Active Processes");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM (tog_stat_active), FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), tog_stat_active);

  tog_stat_all = gtk_radio_menu_item_new_with_label_from_widget(
      GTK_RADIO_MENU_ITEM (radio_group), "All Processes"); 
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM (tog_stat_all), FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), tog_stat_all);

  tog_stat_my = gtk_radio_menu_item_new_with_label_from_widget(
      GTK_RADIO_MENU_ITEM (radio_group), "My Processes");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM (tog_stat_my), FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), tog_stat_my);
 
  // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), separator);

  // Add remaining submenu items
  subItem = gtk_check_menu_item_new_with_label("Dependencies");
  gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), subItem);
  
  // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), separator);
 
  subItem = gtk_menu_item_new_with_label("Memory Maps");
  gtk_widget_set_sensitive(subItem, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), subItem);
  
  subItem = gtk_menu_item_new_with_label("Open Files");
  gtk_widget_set_sensitive(subItem, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), subItem);
  
   // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), separator);
  
  subItem = gtk_menu_item_new_with_label("Properties");
  gtk_widget_set_sensitive(subItem, FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), subItem);

  // Add separator
  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL (subMenu), separator);
  
  refresh = gtk_menu_item_new_with_label("Refresh");
  gtk_menu_shell_append(GTK_MENU_SHELL(subMenu), refresh);

  // Refresh signal handler
  /*g_signal_connect(G_OBJECT (refresh), "activate", G_CALLBACK(toggle_refresh),
      NULL);*/

  // Add remaining menu items
  menuItem = gtk_menu_item_new_with_label("Help");
  gtk_menu_shell_append(GTK_MENU_SHELL (menuBar), menuItem);

  // add menubar to box
  gtk_box_pack_start(GTK_BOX (vbox), menuBar, FALSE, FALSE, 0);

  // create a new notebook
  notebook = gtk_notebook_new();
  
  gtk_box_pack_start(GTK_BOX (vbox), notebook, FALSE, FALSE, 0);

  // set notebook margins
  gtk_widget_set_margin_top(notebook, 20);
  gtk_widget_set_margin_start(notebook, 10);
  gtk_widget_set_margin_end(notebook, 10);
  gtk_widget_set_margin_bottom(notebook, 40);

  // set tab position
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK (notebook), GTK_POS_TOP);

  // create grid
  grid = gtk_grid_new();

  // add system info
  char sys_info[200000];
  sprintf(sys_info, "%s\n\t%s\n\t%s\n\nHardware\n\tMemory: %sKB \
      \n\tProcessor: %s\nSystem Status\n\tAvailable disk space: %s",
      g_sys_info_struct.release_name, g_sys_info_struct.release_version,
      g_sys_info_struct.kernel_version, g_sys_info_struct.ram_size,
      g_sys_info_struct.cpu_info, g_sys_info_struct.disk_space); 

  text = gtk_label_new(sys_info);
  gtk_grid_attach(GTK_GRID (grid), text, 0, 0, 1, 1);

  // create System label
  label = gtk_label_new("System");

  // add System info page
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), grid, label);
  
  // create treeview for processes page
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkWidget *treeview;

  treeview = gtk_tree_view_new();

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Process Name");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 0);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Status");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 1);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "% CPU");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 2);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Id");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 3);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Memory");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 4);

  // Pass treeview to function, create treestore, add to
  // treeview
  create_tree_view(treeview);

 // Active Processes signal handler
  g_signal_connect(G_OBJECT (tog_stat_active), "activate",
      G_CALLBACK(toggle_active), treeview);

  
  // All Processes signal handler
  g_signal_connect(G_OBJECT (tog_stat_all), "activate",
      G_CALLBACK(toggle_all), treeview);

 
  // My Processes signal handler
  g_signal_connect(G_OBJECT (tog_stat_my), "activate",
      G_CALLBACK(toggle_my), treeview);

  // Refresh signal handler
  g_signal_connect(G_OBJECT (refresh), "activate", G_CALLBACK(toggle_refresh),
      treeview);

  // Set selector
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
  
  /*GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;
  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(selection), &model, &iter)) {
    gtk_tree_model_get(model, &iter, 3, &value,  -1);
    printf("value: %s\n", value);
  }*/

  // Connect changed signal to on_changed signal handler
  //g_signal_connect(selection, "changed", G_CALLBACK(on_changed), NULL);
  g_signal_connect(treeview, "button-press-event", (GCallback)
      view_onButtonPressed, selection);
  g_signal_connect(treeview, "popup-menu", (GCallback) view_onPopupMenu, NULL);

  // create scrollable window
  scroll_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER (scroll_window), treeview);

  label = gtk_label_new("Processes");

  // add page
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), scroll_window, label);
  
  //g_timeout_add_full(G_PRIORITY_DEFAULT, 100000,  G_SOURCE_FUNC(create_tree_view), treeview, NULL); 
  
  // create Resources frame and label
  frame = gtk_frame_new(NULL);
  label = gtk_label_new("Resources");

  // add page
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), frame, label);
 
  // create treeview for file systems page
  treeview = gtk_tree_view_new();

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Device");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 0);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Total");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 1);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Used");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 2);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Available");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 3);

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Usage %");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
   
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 4);
 
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Mount Point");
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer,
      "text", 5);

  // Pass treeview to function, create treestore, add to
  // treeview
  create_file_sys_tree_view(treeview);

  // Set selector
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

  // Connect changed signal to on_changed signal handler
  g_signal_connect(selection, "changed",
      G_CALLBACK(on_changed), NULL);

  // create scrollable window
  scroll_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER (scroll_window), treeview);

  // create File Systems label
  label = gtk_label_new("File Systems");

  // add pages
  gtk_notebook_append_page(GTK_NOTEBOOK (notebook), scroll_window, label);
  
  gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  g_sys_info_struct = get_system_info();

  app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return status;
}
