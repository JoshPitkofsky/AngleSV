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
  char title[20];
} article;
 //char *josh = "Josh";
int speedX; 

article list[10];

char *negFiveData;

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
   printf("zero");
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
   printf("one");
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
    
      //menu_cell_basic_draw(ctx, cell_layer, list[cell_index->row].title, *list[cell_index->row].content, NULL);
    
      menu_cell_basic_draw(ctx, cell_layer, pointer, NULL,  NULL);
      //char *pointer;
      //pointer = list[cell_index->row].title[0];


      break;
    case 1:
      switch (cell_index->row) {
        case 0:
          // There is title draw for something more simple than a basic menu item
          menu_cell_title_draw(ctx, cell_layer, "Secrets");
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
   printf("two");
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_int(iter, key, &value, sizeof(int), true);
  app_message_outbox_send();
}

static void str_cut(char *str, int begin, int len)
{ printf("three");
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

}

// returns index of first coming punctuation or space
static int getIndexOfPunctuation(char *originalString) {
  int i = 0;
   printf("four");
  while(originalString[i] != ' ' && originalString[i] != '.' && originalString[i] != ',' && originalString[i] != '!') {
    i++;
  } 

  return i;
}

//gets index of commas 
static int getIndexOfComma(char *originalString) {
  int length = strlen(originalString);
  int i = 0;
  int keepGoing = 1;
  while(originalString[i] != ',' && keepGoing) {
    i++;
    if (i >= length){
      keepGoing = 0;
      i--;
    }
  }
  printf("In index of comma %i", i);

  return i;
}

//GEts a substring
static char *subString(char *originalString) {
  int j = getIndexOfPunctuation(originalString);
   printf("six");
  char *newString="";
  newString = strncpy(newString, originalString, j);
  return newString;
}
static char *subTi(char *originalString) {
  int j = getIndexOfComma(originalString);
   printf("seven");
  char *newString="";
  //newString = strncpy(newString, originalString, j);
  
  return newString;
}

// Finds the position of commas in passed data
static int findArray(char *originalString, int *commaPositions){
  
  printf("OriginalString in findArray %s", originalString);
  
  
  int length = strlen(originalString);
  int j = 0;
  for (int i=0; i < length; i++){
    if (i==length){
      commaPositions[j]=i;
      j++;
    }
    else if (originalString[i]==','){
      commaPositions[j]=i;
      j++;
    }
  }
  return j;
}

// Attempts to return the title, but doesn't...
static void returnTitle(char *originalTitles){
  printf("Original Text %s", originalTitles);
  
  int length = strlen(originalTitles);
  printf("length %i", length);
  int commaPositions[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  
  
  int numberOfCommas = findArray(originalTitles, commaPositions);
  
  int commaArray[numberOfCommas];
  for (int i = 0; i<numberOfCommas; i++) {
    commaArray[i] = commaPositions[i];
  }
  
  for (int i = 0; i<numberOfCommas; i++){
    printf("Comma Array in Return Title %i", commaArray[i]);
  }
   
  int k = 0;
  int j = 0;
  printf("Right before the while loop");
  int startIndex = 0;
  
  for (int i = 0; i < numberOfCommas; i++) {
    int endIndex = commaArray[i];
    int sublength = endIndex-startIndex;
    char substr[sublength];
    
    strncpy(substr,&originalTitles[startIndex],sublength);
    printf("substr = %s",substr );
    
    memcpy(list[i].title,subString, sublength);
    startIndex = endIndex+1;
    printf("%s", originalTitles);
  }    
  /*
  while (){
    printf("While loop %i", j);
    if(j==0){
      char* substr="";
      strncpy(substr, originalTitles, commaPositions[j+1]-commaPositions[j]-1);
      
      article newArticle;
      newArticle.title = substr;
      printf("TITLE %s", newArticle.title);
      titleArray[k] = newArticle;
      k++;
    } else if (commaPositions[j+1] == -1 && commaPositions[j]!=-1){
      //last title
      char *substr="";
      strncpy(substr, &originalTitles[commaPositions[j]+2], length-commaPositions[j]);
      
      article newArticle;
      newArticle.title = substr;
      printf("TITLE %s", newArticle.title);
      titleArray[k]=newArticle;
      k++;
    } else {
      //middle title
      char *substr="";
      strncpy(substr, &originalTitles[commaPositions[j]+2], commaPositions[j+1]-commaPositions[j]-1);
      
      article newArticle;
      newArticle.title = substr;
      printf("TITLE %s", newArticle.title);
      titleArray[k]=newArticle;
      k++;
    }
    char* buff = "";
    strncpy(buff, originalTitles+commaPositions[j], length);
    originalTitles = buff;
    j++;
  }*/
  
  /*
  
  char *substring;
  int i = 0;
  //while(strcmp(originalTitles, NULL) != 0){
  while(strlen(originalTitles) > 0 ){
    int endIndex = getIndexOfComma(originalTitles);
    char* substr="";
    strncpy(substr, originalTitles, endIndex+1);
    
    printf("nine");
    printf("before cut %s",originalTitles);
    substring = subTi(originalTitles);
    printf("ten");
    //strncpy(originalTitles, &originalTitles[strlen(substring)], sizeof(&originalTitles));
    str_cut(originalTitles, 0, strlen(substring));
    printf("eleven");
    printf("this is after cuting %s",originalTitles);
    article newArticle;
    
    newArticle.title = substring;
    
    article newArticle;
    newArticle.title = substr;
    titleArray[i] = newArticle;
    char* buff = "";
    strncpy(buff, originalTitles+endIndex+1, strlen(originalTitles));
    originalTitles = buff;
    printf("I is about to be incremented and is now,%i", i);
    i++;
  }*/
}

// loops words, breaks them from an overall string
static void displayWords(char *originalString){
  printf("congratulations you have made it to the display words function");
  char *substring;
  while(originalString != NULL){
    substring = subString(originalString);
    printf("%s",substring);
    //add sleep
    str_cut(originalString, 0, strlen(substring)); 
  }
}


// Sends data relating to the chosen story, attempts to load word by word from the phone application
// but in reality we couldn't figure out how to do this. Word for word because we focused on
// speed reading techniques involving fixation points and saccades
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  
  //SEND INDEX BACk
  
  printf("FIRST");
  // Use the row to specify which item will receive the select action
  if(cell_index->section == 0) {
    Window *window = window_create();
	  static TextLayer  *text_layer, *s_header_layer, *s_body_layer, *s_label_layer;
    printf("SECOND");
    
    send_int(-1,cell_index->row);
    int g=0;
   
    // Add the text layer to the window
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
    text_layer_set_text(text_layer, "negFiveData");
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    // Push the window
    window_stack_push(window, true);
    while(g<30){
          static char s_body_text[18];
          snprintf(s_body_text, sizeof(s_body_text), "%s", negFiveData);
          text_layer_set_text(s_body_layer, s_body_text);
          printf("THIRD");
          psleep(200);
          g++;
    }
    
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

// Unloads the menu
static void main_window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(s_menu_layer);

  // Cleanup the menu icons
  for (int i = 0; i < NUM_MENU_ICONS; i++) {
    gbitmap_destroy(s_menu_icons[i]);
  }

  gbitmap_destroy(s_background_bitmap);
}


// Handles the reception of data from the application
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Get the first pair
  Tuple *t = dict_read_first(iterator);

  // Process all pairs present
  while (t != NULL) {
    // Long lived buffer
    static char s_buffer[64];
      switch(t->key){
        case -4:
          //code to assign title
         // Copy value
        snprintf(s_buffer, sizeof(s_buffer), "Received '%s'", t->value->cstring);
        article one;

        //printf("one title %s", one.title);
        printf("t value %s", t->value->cstring);
        memcpy(one.title, t->value->cstring, 10);
        printf("one title two %s", one.title);
        printf("t value %s", t->value->cstring);
        //one.title = t->value->cstring;
        returnTitle(one.title);
        
        for (int i = 0; i < 5; i++) {
          printf("list %s", list[i].title);
        }
        
        break;
        case -5:
          //code to display content
        //call content functions here display words
        
        snprintf(s_buffer, sizeof(s_buffer), "Received '%s'", t->value->cstring);
        negFiveData = t->value->cstring;
        break;
        case -6:
          //index for where you left off in the reading
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
// Groups different angles into different speeds
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

}

// initialize
static void init() {
  // Register callbacks
    printf("callbacks start");
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  

  // Open AppMessage
    printf("congratulations you have made it to open appmessage");
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

// Deinitialize 
static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
}

// main
int main(void) {
  init();
  app_event_loop();
  deinit();
}