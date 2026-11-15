#pragma once

#include <stats.h>
#include <stddef.h>
#include <stdint.h>
struct consecuence {
    int8_t mod_num;
    enum stat_kinds stat;
};

extern const char *problem_names[];
extern const char *incident_names[];
extern const char *creation_names[];
extern const char *action_names[];

struct event_incident {
	size_t incident_name_index;
	uint8_t probability;
	struct consecuence consecuences[3];
};

struct event_problem {
    size_t problem_name_index;
    struct consecuence addressing_consecuences[3];
    struct consecuence ignoring_consecuences[3];
};

struct event_action {
	size_t action_name_index;
	struct consecuence consecuences[3];
	struct event_incident incidents[3];
};

struct event_creation {
	size_t creation_name_index;
	const char *optA;
	const char *optB;
	struct consecuence consecuences_of_option_A[3];
	struct consecuence consecuences_of_option_B[3];
	struct event_incident incidents[3];
};

struct event {
	enum {
		EVENT_NONE,
		EVENT_CREATION,
		EVENT_ACTION,
		EVENT_PRROBLEM,
		EVENT_INCIDENT,
	} type;
	union {
		struct event_creation creation;
		struct event_action action;
		struct event_problem problem;
		struct event_incident incident;
	} as;
	const char *card_path;
};

void apply_consecuence(struct consecuence);
Texture *draw_event_card(size_t idx);

// Returns true if we should switch to the next card
bool input_loop_event_card(size_t idx, Vector2 pos);

#define EVENT_LIST_COUNT 14
extern struct event event_list[EVENT_LIST_COUNT];
