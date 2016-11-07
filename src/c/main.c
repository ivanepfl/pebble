
/*---------------------------------------------------------------------------
Template for TP of the course "System Engineering" 2016, EPFL

Authors: Flavien Bardyn & Martin Savary
Version: 1.0
Date: 10.08.2016

Use this "HelloWorld" example as basis to code your own app, which should at least 
count steps precisely based on accelerometer data. 

- Add the accelerometer data acquisition
- Implement your own pedometer using these data
- (Add an estimation of the distance travelled)

- Make an effort on the design of the app, try to do something fun!
- Comment and indent your code properly!
- Try to use your imagination and not google (we already did it, and it's disappointing!)
  to offer us a smart and original solution of pedometer

Don't hesitate to ask us questions.
Good luck and have fun!
---------------------------------------------------------------------------*/


// Include Pebble library
#include <pebble.h>

static int gait_count;
static int gait_speed_per_hour;

static void accel_data_handler(AccelData *data, uint32_t num_samples);

// Declare the main window and two text layers
Window *main_window;
TextLayer *background_layer;
TextLayer *gait_count_layer_title;
TextLayer *gait_count_layer;
TextLayer *gait_speed_layer_title;
TextLayer *gait_speed_layer;
ActionBarLayer *action_bar;

static GBitmap *pause_icon;
static GBitmap *reset_icon;

static void click_config_provider_up(void *context) {
}

static void click_config_provider_down(void *context) {
}

// Init function called when app is launched
static void init_window(void) {
  
    // Create main Window element and assign to pointer
    main_window = window_create();
    Layer *window_layer = window_get_root_layer(main_window);  
  
    // Create background Layer
    background_layer = text_layer_create(GRect( 0, 0, 144, 168));
  
    // Setup background layer color (black)
    text_layer_set_background_color(background_layer, GColorBlack);
  
    // Create text Layer gait count TITLE
    gait_count_layer_title = text_layer_create(GRect( 10, 20, 100, 25));
    text_layer_set_background_color(gait_count_layer_title, GColorClear);
    text_layer_set_text_color(gait_count_layer_title, GColorWhite);	
    text_layer_set_font(gait_count_layer_title, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(gait_count_layer_title, GTextAlignmentLeft);
  
    // Title display
    static char title_1[60]= "Count [gait]";
    text_layer_set_text(gait_count_layer_title, title_1);
  
    // Create text Layer gait count
    gait_count_layer = text_layer_create(GRect( 0, 40, 100, 40));
    text_layer_set_background_color(gait_count_layer, GColorClear);
    text_layer_set_text_color(gait_count_layer, GColorWhite);	
    text_layer_set_font(gait_count_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(gait_count_layer, GTextAlignmentCenter);
  
    // Create text Layer gait speed TITLE
    gait_speed_layer_title = text_layer_create(GRect( 10, 100, 100, 25));
    text_layer_set_background_color(gait_speed_layer_title, GColorClear);
    text_layer_set_text_color(gait_speed_layer_title, GColorWhite);	
    text_layer_set_font(gait_speed_layer_title, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(gait_speed_layer_title, GTextAlignmentLeft);
  
    // Title display
    static char title_2[60]= "Speed [g/h]";
    text_layer_set_text(gait_speed_layer_title, title_2);
  
    // Create text Layer gait speed
    gait_speed_layer = text_layer_create(GRect( 0, 120, 100, 40));
    text_layer_set_background_color(gait_speed_layer, GColorClear);
    text_layer_set_text_color(gait_speed_layer, GColorWhite);	
    text_layer_set_font(gait_speed_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(gait_speed_layer, GTextAlignmentCenter);
  
    action_bar = action_bar_layer_create();
  
    action_bar_layer_set_background_color(action_bar, GColorWhite);
    // action_bar_layer_set_click_config_provider(action_bar, click_config_provider);
  
    // Set the icons:
    pause_icon = gbitmap_create_with_resource(RESOURCE_ID_PAUSE_ICON);
    reset_icon = gbitmap_create_with_resource(RESOURCE_ID_RESET_ICON);
    action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_UP, pause_icon, true);
    action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_DOWN, reset_icon, true);
  
  
    // Add layers as childs layers to the Window's root layer
    layer_add_child(window_layer, text_layer_get_layer(background_layer));
    layer_add_child(window_layer, text_layer_get_layer(gait_count_layer_title));    
    layer_add_child(window_layer, text_layer_get_layer(gait_count_layer));
    layer_add_child(window_layer, text_layer_get_layer(gait_speed_layer_title));
    layer_add_child(window_layer, text_layer_get_layer(gait_speed_layer));
    action_bar_layer_add_to_window(action_bar, main_window);
  
    // Show the window on the watch, with animated = true
    window_stack_push(main_window, true);
  
}

static void init_acc(void) {
  //Define number of samples 25 hz=1sec
  uint32_t num_samples = 25;
  
  //Allow Accelerometer Event
  accel_data_service_subscribe(num_samples, accel_data_handler);
  
  //Define accelerometer sampling rate
  accel_service_set_sampling_rate(ACCEL_SAMPLING_25HZ);
  
}

//deinit function init_acc
static void deinit_acc(void) {
    //stop acc
   accel_data_service_unsubscribe();
}

//Function accel_data_handler
static void accel_data_handler(AccelData *data, uint32_t num_samples)
{
  //Read samples x,y,z
  int16_t x = data[0].x;
  int16_t y = data[0].y;
  int16_t z = data[0].z;
  
  //tab of char to print result on window
  static char results[60];
  
  // Print result Watch
  snprintf(results, 60, "x: %d", x);
  text_layer_set_text(gait_count_layer, results);
  snprintf(results, 60, "y: %d", z);
  text_layer_set_text(gait_speed_layer, results);
}

// deinit function called when the app is closed
static void deinit_window(void) {
  
    // Destroy layers and main window dd 
    text_layer_destroy(background_layer);
    text_layer_destroy(gait_count_layer_title);
  	text_layer_destroy(gait_count_layer);
  	text_layer_destroy(gait_speed_layer_title);
  	text_layer_destroy(gait_speed_layer);
    action_bar_layer_destroy(action_bar);
    window_destroy(main_window);
}

int main(void) {
    init_window();
    init_acc();
    app_event_loop();
    deinit_acc();
    deinit_window();
}
