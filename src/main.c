#include <pebble.h>
#include "dithered_rects.h"
	
// Settings
#define KEY_VIBRATEONBT 0
#define KEY_SHOWDATE 1
#define KEY_BGCOLOR 2
	
static Window *s_main_window;
static TextLayer *s_hour_1_layer;
static TextLayer *s_hour_2_layer;
static TextLayer *s_serparator_layer;
static TextLayer *s_minute_1_layer;
static TextLayer *s_minute_2_layer;
static TextLayer *s_minute_3_layer;
static TextLayer *s_minute_4_layer;

static TextLayer *s_message_layer;
AppTimer *message_timer;

#ifdef PBL_PLATFORM_APLITE
	Layer *s_aplite_bg_layer;
#endif

char hour_1_buffer[] = "00";
char hour_2_buffer[] = "00";
char minute_1_buffer[] = "00";
char minute_2_buffer[] = "00";
char minute_3_buffer[] = "00";
char minute_4_buffer[] = "00";

// Color will be set in "init()" depending on the device.
static GColor s_background_color;
static GColor s_primary_color;
static GColor s_secondary_color;


// General Interface elements
static void main_window_load(Window *window) {
	// Create hour TextLayers
	s_hour_1_layer = text_layer_create(GRect(0, 53, 65, 62));
	text_layer_set_background_color(s_hour_1_layer, GColorClear);
	text_layer_set_text_color(s_hour_1_layer, s_primary_color);
	text_layer_set_font(s_hour_1_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_REGULAR_48)));
	text_layer_set_text(s_hour_1_layer, "19");
	text_layer_set_text_alignment(s_hour_1_layer, GTextAlignmentRight);
	s_hour_2_layer = text_layer_create(GRect(0, 159, 65, 62));
	text_layer_set_background_color(s_hour_2_layer, GColorClear);
	text_layer_set_text_color(s_hour_2_layer, s_primary_color);
	text_layer_set_font(s_hour_2_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_REGULAR_48)));
	text_layer_set_text(s_hour_2_layer, "20");
	text_layer_set_text_alignment(s_hour_2_layer, GTextAlignmentRight);
	
	// Create serparator TextLayers
	s_serparator_layer = text_layer_create(GRect(65, 50, 14, 65));
	text_layer_set_background_color(s_serparator_layer, GColorClear);
	text_layer_set_text_color(s_serparator_layer, s_primary_color);
	text_layer_set_font(s_serparator_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_REGULAR_48)));
	text_layer_set_text(s_serparator_layer, ":");
	text_layer_set_text_alignment(s_serparator_layer, GTextAlignmentCenter);
	
	// Create minute TextLayers	
	s_minute_1_layer = text_layer_create(GRect(79, 0, 65, 62));
	text_layer_set_background_color(s_minute_1_layer, GColorClear);
	text_layer_set_text_color(s_minute_1_layer, s_secondary_color);
	text_layer_set_font(s_minute_1_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_REGULAR_48)));
	text_layer_set_text(s_minute_1_layer, "18");
	text_layer_set_text_alignment(s_minute_1_layer, GTextAlignmentLeft);
	s_minute_2_layer = text_layer_create(GRect(79, 53, 65, 62));
	text_layer_set_background_color(s_minute_2_layer, GColorClear);
	text_layer_set_text_color(s_minute_2_layer, s_secondary_color);
	text_layer_set_font(s_minute_2_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_REGULAR_48)));
	text_layer_set_text(s_minute_2_layer, "19");
	text_layer_set_text_alignment(s_minute_2_layer, GTextAlignmentLeft);
	s_minute_3_layer = text_layer_create(GRect(79, 106, 65, 62));
	text_layer_set_background_color(s_minute_3_layer, GColorClear);
	text_layer_set_text_color(s_minute_3_layer, s_secondary_color);
	text_layer_set_font(s_minute_3_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_REGULAR_48)));
	text_layer_set_text(s_minute_3_layer, "20");
	text_layer_set_text_alignment(s_minute_3_layer, GTextAlignmentLeft);
	s_minute_4_layer = text_layer_create(GRect(79, 159, 65, 62));
	text_layer_set_background_color(s_minute_4_layer, GColorClear);
	text_layer_set_text_color(s_minute_4_layer, s_secondary_color);
	text_layer_set_font(s_minute_4_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_REGULAR_48)));
	text_layer_set_text(s_minute_4_layer, "21");
	text_layer_set_text_alignment(s_minute_4_layer, GTextAlignmentLeft);
	
	// Create message TextLayer
	s_message_layer = text_layer_create(GRect(0, -50, 144, 50));
	text_layer_set_background_color(s_message_layer, GColorClear);
	text_layer_set_text_color(s_message_layer, s_primary_color);
	text_layer_set_font(s_message_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SEGOEUI_24)));
	text_layer_set_text_alignment(s_message_layer, GTextAlignmentCenter);
	
	// Add it as child layer to the Window's root layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_1_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_2_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_serparator_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minute_1_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minute_2_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minute_3_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_minute_4_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_message_layer));
}

static void main_window_unload(Window *window) {
	// Destroy TextLayers
	text_layer_destroy(s_hour_1_layer);
	text_layer_destroy(s_hour_2_layer);
	text_layer_destroy(s_serparator_layer);
	text_layer_destroy(s_minute_1_layer);
	text_layer_destroy(s_minute_2_layer);
	text_layer_destroy(s_minute_3_layer);
	text_layer_destroy(s_minute_4_layer);
	text_layer_destroy(s_message_layer);
}


// HANDLE ANIMATIONS
void on_animation_stopped(Animation *anim, bool finished, void *context) {
    //Free the memory used by the Animation
    property_animation_destroy((PropertyAnimation*) anim);
}
 
void animate_layer(Layer *layer, GRect *start, GRect *finish, int duration, int delay) {
    //Declare animation
    PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);
 
    //Set characteristics
    animation_set_duration((Animation*) anim, duration);
    animation_set_delay((Animation*) anim, delay);
 
	
	#ifdef PBL_PLATFORM_APLITE
    	//Set stopped handler to free memory
    	AnimationHandlers handlers = {
     		//The reference to the stopped handler is the only one in the array
    	    .stopped = (AnimationStoppedHandler) on_animation_stopped
    	};
		animation_set_handlers((Animation*) anim, handlers, NULL);
	#endif
	
    //Start animation!
    animation_schedule((Animation*) anim);
}


// HANDLE TIME
void move_minutes()	{
	GRect start;
	GRect finish;
	
	start = GRect(79, 0, 65, 62);
    finish = GRect(79, -53, 65, 62);
    animate_layer(text_layer_get_layer(s_minute_1_layer), &start, &finish, 700, 500);
	start = GRect(79, 53, 65, 62);
    finish = GRect(79, 0, 65, 62);
	animate_layer(text_layer_get_layer(s_minute_2_layer), &start, &finish, 700, 500);
	start = GRect(79, 106, 65, 62);
    finish = GRect(79, 53, 65, 62);
	animate_layer(text_layer_get_layer(s_minute_3_layer), &start, &finish, 700, 500);
	start = GRect(79, 159, 65, 62);
    finish = GRect(79, 106, 65, 62);
	animate_layer(text_layer_get_layer(s_minute_4_layer), &start, &finish, 700, 500);
}

void move_hours() {
	GRect start;
	GRect finish;
	
	start = GRect(0, 53, 65, 62);
    finish = GRect(0, -53, 65, 62);
    animate_layer(text_layer_get_layer(s_hour_1_layer), &start, &finish, 700, 500);
	start = GRect(0, 159, 65, 62);
    finish = GRect(0, 53, 65, 62);
	animate_layer(text_layer_get_layer(s_hour_2_layer), &start, &finish, 700, 500);
}

void update_minutes(struct tm *tick_time) {
	struct tm time_copy = *tick_time;
	
	// Update TextLayer 1
	if(tick_time->tm_min == 0) {
		time_copy.tm_min = 59;
	}
	else {
		time_copy.tm_min = time_copy.tm_min - 1;
	}
	strftime(minute_1_buffer, sizeof("00"), "%M", &time_copy);
	text_layer_set_text(s_minute_1_layer, minute_1_buffer);
	
	// Update TextLayer 2
	strftime(minute_2_buffer, sizeof("00"), "%M", tick_time);
	text_layer_set_text(s_minute_2_layer, minute_2_buffer);
	
	// Update TextLayer 2
	if(tick_time->tm_min == 59) {
		time_copy.tm_min = 0;
	}
	else if(tick_time->tm_min == 0) {
		time_copy.tm_min = 1;
	}
	else {
		time_copy.tm_min = time_copy.tm_min + 2;
	}
	strftime(minute_3_buffer, sizeof("00"), "%M", &time_copy);
	text_layer_set_text(s_minute_3_layer, minute_3_buffer);
	
	// Update TextLayer 2
	if(tick_time->tm_min == 58) {
		time_copy.tm_min = 0;
	}
	else if(tick_time->tm_min == 59) {
		time_copy.tm_min = 1;
	}
	else if(tick_time->tm_min == 60) {
		time_copy.tm_min = 2;
	}
	else {
		time_copy.tm_min = time_copy.tm_min + 1;
	}
	strftime(minute_4_buffer, sizeof("00"), "%M", &time_copy);
	text_layer_set_text(s_minute_4_layer, minute_4_buffer);
	
	// Set position of the TextLayers
	GRect finish;
	
    finish = GRect(79, 0, 65, 62);
	layer_set_frame(text_layer_get_layer(s_minute_1_layer), finish);
    finish = GRect(79, 53, 65, 62);
	layer_set_frame(text_layer_get_layer(s_minute_2_layer), finish);
    finish = GRect(79, 106, 65, 62);
	layer_set_frame(text_layer_get_layer(s_minute_3_layer), finish);
    finish = GRect(79, 159, 65, 62);
	layer_set_frame(text_layer_get_layer(s_minute_4_layer), finish);
}

void update_hours(struct tm *tick_time) {
	struct tm time_copy = *tick_time;
	
	// Update TextLayer 1
	if (clock_is_24h_style()) {
		strftime(hour_1_buffer, sizeof("00"), "%H", tick_time);
	} else {
		strftime(hour_1_buffer, sizeof("00"), "%I", tick_time);
	}
	text_layer_set_text(s_hour_1_layer, hour_1_buffer);
	
	// Update TextLayer 2
	if(tick_time->tm_hour == 23) {
		time_copy.tm_hour = 0;
	}
	else {
		time_copy.tm_hour = time_copy.tm_hour + 1;
	}
	if (clock_is_24h_style()) {
		strftime(hour_2_buffer, sizeof("00"), "%H", &time_copy);
	} else {	
		strftime(hour_2_buffer, sizeof("00"), "%I", &time_copy);
	}
	text_layer_set_text(s_hour_2_layer, hour_2_buffer);
	
	// Set position of the TextLayers
	GRect finish;
	
    finish = GRect(0, 53, 65, 62);
	layer_set_frame(text_layer_get_layer(s_hour_1_layer), finish);
    finish = GRect(0, 159, 65, 62);
	layer_set_frame(text_layer_get_layer(s_hour_2_layer), finish);
}

void update_time() {
	time_t temp = time(NULL); 
	struct tm *tick_time = localtime(&temp);
	
	update_minutes(tick_time);
	update_hours(tick_time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	int second = tick_time->tm_sec;
	int minute = tick_time->tm_min;
	
	if(second == 59 && minute != 59) {
		move_minutes();
	}
	else if(second == 59 && minute == 59) {
		move_hours();
		move_minutes();
	}
	else if(second == 29) {
		update_time();
	}	
}


// SHOW APLITE SOME LOVE
void draw_aplite_background(Layer *cell_layer, GContext *ctx) {
	draw_dithered_rect(ctx, GRect(0,0,144,168), GColorBlack, GColorWhite, DITHER_50_PERCENT);
}


// SHOW / HIDE MESSAGES
void hide_message(void *data) {
	animate_layer(text_layer_get_layer(s_message_layer), &GRect(0, 0, 144, 50), &GRect(0, -50, 144, 50), 500, 0);
}

void show_message(GColor color, int time, char message[20]) {
	text_layer_set_text(s_message_layer, message);
	text_layer_set_background_color(s_message_layer, color);
	
    animate_layer(text_layer_get_layer(s_message_layer), &GRect(0, -50, 144, 50), &GRect(0, 0, 144, 50), 500, 0);
	
	message_timer = app_timer_register(time, hide_message, NULL);
}

// HANDLE BLUETOOTH CONNECTION
void bt_handler(bool connected) {
	if (!connected) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Phone is not connected!");
		
		if (persist_read_bool(KEY_VIBRATEONBT)) {
			vibes_short_pulse();
		}
		
		show_message(COLOR_FALLBACK(GColorBlueMoon, GColorWhite), 5000, "BT lost");
	}
}


// HANDLE SETTINGS
void update_settings() {
	// Here goes color and date or something
}

static void inbox_received_handler(DictionaryIterator *iterator, void *context) {
	// Get tuple
	Tuple *t = dict_read_first(iterator);
	
	while (t != NULL) {
		switch (t->key) {
			case KEY_VIBRATEONBT:
				if (strcmp(t->value->cstring, "on") == 0) {
					APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_VIBRATEONBT = true");
					
					persist_write_bool(KEY_VIBRATEONBT, true);
				} else if (strcmp(t->value->cstring, "off") == 0) {
					 APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_VIBRATEONBT = false");
					
					persist_write_bool(KEY_VIBRATEONBT, false);
				}
				break;
			case KEY_SHOWDATE:
				if (strcmp(t->value->cstring, "on") == 0) {
					APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_SHOWDATE = true");
					
					persist_write_bool(KEY_SHOWDATE, true);
				} else if (strcmp(t->value->cstring, "off") == 0) {
					APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_SHOWDATE = false");
					
					persist_write_bool(KEY_SHOWDATE, false);
				}
				break;
			case KEY_BGCOLOR:
				APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_BGCOLOR = %s", t->value->cstring);
				break;
		}
		
		// Get next pair, if any
    	t = dict_read_next(iterator);
	}
	
	update_settings();
}


static void init() {
	// Initialyze settings
	app_message_register_inbox_received(inbox_received_handler);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	// Set default settings if needed
	if (persist_exists(KEY_VIBRATEONBT) == false) {
		persist_write_bool(KEY_VIBRATEONBT, true);
	}
	if (persist_exists(KEY_SHOWDATE) == false) {
		persist_write_bool(KEY_SHOWDATE, false);
	}
	
	// Set colors for Aplite and Basalt. Aplite background is Clear for the dithering layer
	s_background_color = COLOR_FALLBACK(GColorDarkGray, GColorClear);
	s_primary_color = COLOR_FALLBACK(GColorBlack, GColorBlack);
	s_secondary_color = COLOR_FALLBACK(GColorWhite, GColorWhite);
	
	// Create main Window element and assign to pointer
	s_main_window = window_create();
	window_set_background_color(s_main_window, s_background_color);

	// Set handlers to manage the elements inside the Window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	// Create and fill dithering layer on Aplite
	#ifdef PBL_PLATFORM_APLITE
		s_aplite_bg_layer = layer_create(GRect(0,0,144,168));
		layer_set_update_proc(s_aplite_bg_layer, draw_aplite_background);
	
		layer_add_child(window_get_root_layer(s_main_window), s_aplite_bg_layer);
	#endif
	
	// Register with TickTimerService
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

	// Register with BluetoothConnectionService
	bluetooth_connection_service_subscribe(bt_handler);
		
	// Show the Window on the watch, with animated=true
	window_stack_push(s_main_window, true);
	
	// Initially show the correct time
	update_time();
}

static void deinit() {
	// Unregister from TickTimerService
	tick_timer_service_unsubscribe();
	
	// Destroy dithering layer on Aplite
	#ifdef PBL_PLATFORM_APLITE
		layer_destroy(s_aplite_bg_layer);
	#endif
	
	// Destroy Window
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}