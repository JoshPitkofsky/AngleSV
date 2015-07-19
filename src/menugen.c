/*#include <pebble.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NUM_MENU_SECTIONS 2
#define NUM_MENU_ICONS 3
#define NUM_FIRST_MENU_ITEMS 2
#define NUM_SECOND_MENU_ITEMS 1

static Window *s_main_window;
static MenuLayer *s_menu_layer;
static GBitmap *s_menu_icons[NUM_MENU_ICONS];
static GBitmap *s_background_bitmap;

typedef struct {
  char *title;
  char *content;
} article;

article list[2];

//static int s_current_icon = 0;

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    case 1:
      return NUM_SECOND_MENU_ITEMS;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Reading List");
      break;
    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "Settings");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
    
      //menu_cell_basic_draw(ctx, cell_layer, list[cell_index->row].title, *list[cell_index->row].content, NULL);
      menu_cell_basic_draw(ctx, cell_layer, list[cell_index->row].title, list[cell_index->row].content, NULL);
      break;
    case 1:
      switch (cell_index->row) {
        case 0:
          // There is title draw for something more simple than a basic menu item
          menu_cell_title_draw(ctx, cell_layer, "Final Item");
          break;
      }
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  printf("FIRST");
  // Use the row to specify which item will receive the select action
  if(cell_index->section == 0) {
    Window *window = window_create();
	  TextLayer *text_layer = text_layer_create(GRect(0, 0, 144, 154));
    printf("SECOND");
    
    
	  // Set the text, font, and text alignment
    text_layer_set_text(text_layer, list[cell_index->row].content);
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);

    printf("THIRD");
    // Add the text layer to the window
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  

      
    // Push the window
   window_stack_push(window, true);
    
    //psleep(2000);
    //text_layer_set_text(text_layer, "For Real");
    for(int i=0; i<=5;i++){
      text_layer_set_text(text_layer, list[0].content[i]);
    psleep(200);
    }
    
    
    }

}
*/
/*
static void main_window_load(Window *window) {
  // Here we load the bitmap assets

  // Now we prepare to initialize the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(s_menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void main_window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(s_menu_layer);

  // Cleanup the menu icons
  for (int i = 0; i < NUM_MENU_ICONS; i++) {
    gbitmap_destroy(s_menu_icons[i]);
  }

  gbitmap_destroy(s_background_bitmap);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
  
  printf("Before Article One");
  
  article one;
  one.title = "Geisel";
  one.content= "Janis";
  printf("After Article One");
  
  article two;
  two.title = "Radical";
  two.content = "ChiCityMayne";
  
  printf("After Part Two");
  
  list[0] = one;
  list[1] = two;
  printf("After list assignment");
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}*/