/*
 * main.c
 * Sets up Window, AppMessage and a TextLayer to show the message received.
 */

#include <pebble.h>
#define NUM_MENU_SECTIONS 2
#define NUM_MENU_ICONS 3
#define NUM_FIRST_MENU_ITEMS 10
#define NUM_SECOND_MENU_ITEMS 1

static Window *s_main_window;
static MenuLayer *s_menu_layer;
static GBitmap *s_menu_icons[NUM_MENU_ICONS];
static GBitmap *s_background_bitmap;

typedef struct {
  char *title;
  char *content;
} article;

int speedX; 

article list[10];

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

// Called when an item is selected
// Must 
// 1) Send the index of item back to the app
// 2) Start moving through received text
// -1 row
// -2 speed


static void send_int(int key, int value) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_int(iter, key, &value, sizeof(int), true);
  app_message_outbox_send();
}

int str_cut(char *str, int begin, int len)
{
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return len;
}

// returns index of first coming punctuation or space
static int getIndexOfPunctuation(char *originalString) {
  int i = 0;
  
  while(originalString[i] != ' ' && originalString[i] != '.' && originalString[i] != ',' && originalString[i] != '!') {
    i++;
  } 

  return i;
}

static char *subString(char *originalString) {
  int j = getIndexOfPunctuation(originalString);
  
  char *newString="";
  newString = strncpy(newString, originalString, j);
  return newString;
}

static void displayWords(char *originalString){
  
  char *substring;
  while(originalString != NULL){
    substring = subString(originalString);

    //add sleepB
    str_cut(originalString, 0, strlen(substring));
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  
  //SEND INDEX BACK
  
  
  
  printf("FIRST");
  // Use the row to specify which item will receive the select action
  if(cell_index->section == 0) {
    Window *window = window_create();
	  TextLayer *text_layer = text_layer_create(GRect(0, 0, 144, 154));
    printf("SECOND");
    
    send_int(-1,cell_index->row);
    

    
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
    /*for(int i=0; i<=5;i++){
      text_layer_set_text(text_layer, list[0].content[i]);
    psleep(200);
    }*/
    
    
    }

}


static void main_window_load(Window *window) {
  // Here we load the bitmap assets
  

  // And also load the background

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

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Get the first pair
  Tuple *t = dict_read_first(iterator);

  // Process all pairs present
  int i = 0;
  while (t != NULL) {
    // Long lived buffer
    static char s_buffer[64];
      switch(t->key){
        case -4:
          //code to assign title
         // Copy value
        snprintf(s_buffer, sizeof(s_buffer), "Received '%s'", t->value->cstring);
        article one;
        one.title = t->value->cstring;
        list[i] = one;
        i++;
        
        break;
        case -5:
          //code to display content
        //call content functions here display words
        break;
        case -6:
          //index #probs wont do
        break;
        
      }

    // Get next pair, if any
    t = dict_read_next(iterator);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

// Handles data from the accelerometer
static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];

  float avgData = (data[0].x + data[1].x + data[2].x)/3;
  
  if (avgData > -100 && avgData < 100) {
    
  } else if (avgData <= -100 && avgData > -200) {
    speedX = -5;
  } else if (avgData <= -200){
    speedX = -10;
  } else if (avgData >= 100 && avgData <200) {
    speedX = 5;
  } else if (avgData >= 200) {
    speedX = 10;
  }
  
  snprintf(s_buffer, sizeof(s_buffer),
    "N %d", speedX
  );

  //Show the data
  printf("speedX %i", speedX);
  send_int(-2,speedX);
}


static void init() {
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  
  // Accelerometer initialization
  speedX = 0;
  
  int num_samples = 3;
  accel_data_service_subscribe(num_samples, data_handler);

  // Choose update rate
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
  
  
  
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);

}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}