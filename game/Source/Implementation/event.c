#include <event.h>
#include <raylib.h>
#include <stats.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int8_t clamp_i8(int value, int min, int max) {
    if (value < min)
        return (int8_t)min;
    if (value > max)
        return (int8_t)max;
    return (int8_t)value;
}

void apply_consecuence(struct consecuence consecuence) {
    switch (consecuence.stat) {
    case STAT_HEALTH:
        player_stats.health =
            clamp_i8(player_stats.health + consecuence.mod_num, 0, 100);
        break;

    case STAT_SLEEP:
        player_stats.sleep =
            clamp_i8(player_stats.sleep + consecuence.mod_num, 0, 100);
        break;

    case STAT_RAD_SHIELD:
        station_stats.radiation_shield = clamp_i8(
            station_stats.radiation_shield + consecuence.mod_num, 0, 100);
        break;

    case STAT_POWER_STORAGE:
        station_stats.power_stored =
            clamp_i8(station_stats.power_stored + consecuence.mod_num, 0, 100);
        break;

    case STAT_FOOD_STORAGE:
        station_stats.food_stored =
            clamp_i8(station_stats.food_stored + consecuence.mod_num, 0, 100);
        break;

    case STAT_POWER_PRODUCTION:
        production_stats.power_prod += consecuence.mod_num;
        break;

    case STAT_FOOD_PRODUCTION:
        production_stats.food_prod += consecuence.mod_num;
        break;

    case STAT_RAD_SHIELD_CONSUMTION:
        consumed_stats.radiation_shielded_per_day += consecuence.mod_num;
        break;

    case STAT_POWER_CONSUMTION:
        consumed_stats.power_consumed_per_day += consecuence.mod_num;
        break;

    case STAT_FOOD_CONSUMTION:
        consumed_stats.food_consumed_per_day += consecuence.mod_num;
        break;

    default:
        break;
    }
}

const char *problem_names[] = {
    "Water Leak",  "Collapsed Shelf",
    "Power Surge", "Contaminated Air Filter",
    "Crop Pests",  "Sleep Deprivation Event",
};

const char *incident_names[] = {
    "Wiring Fault", "Panel Crack",       "Mold Outbreak", "Pump Jam",
    "Seal Leak",    "Material Shortage", "Arc Shock",     "Overdraw",
    "Fatigue",      "Morale Drop",       "Tool Slip",     "Repair Fails",
    "Grid Flicker", "Fuse Blow",         "Bad Dream",     "Oversleep",
};

const char *creation_names[] = {
    "Solar Panel Array",
    "Hydroponic Tray",
    "Radiation Bunker Upgrade",
};

const char *action_names[] = {
    "Supercharge Shield", "Ration Shift", "Emergency Maintenance",
    "Power Reroute",      "Rest Cycle",
};

struct event event_list[EVENT_LIST_COUNT] = {
    // Creations
    {
        .type = EVENT_CREATION,
        .as.creation =
            {
                .creation_name_index = 0,
                .optA = "Install 3 panels",
                .optB = "Install 5 panels",
                .consecuences_of_option_A =
                    {
                        {.mod_num = 4, .stat = STAT_POWER_PRODUCTION},
                        {0, 0},
                        {0, 0},
                    },
                .consecuences_of_option_B =
                    {
                        {.mod_num = 7, .stat = STAT_POWER_PRODUCTION},
                        {.mod_num = -5, .stat = STAT_POWER_STORAGE},
                        {0, 0},
                    },
                .incidents =
                    {
                        {
                            .incident_name_index = 0,
                            .probability = 20,
                            .consecuences =
                                {
                                    {.mod_num = -1,
                                     .stat = STAT_POWER_PRODUCTION},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                        {
                            .incident_name_index = 1,
                            .probability = 15,
                            .consecuences =
                                {
                                    {.mod_num = -2,
                                     .stat = STAT_POWER_PRODUCTION},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                    },
            },
    },
    {
        .type = EVENT_CREATION,
        .as.creation =
            {
                .creation_name_index = 1,
                .optA = "Install 2 trays",
                .optB = "Install 4 trays",
                .consecuences_of_option_A =
                    {
                        {.mod_num = 2, .stat = STAT_FOOD_PRODUCTION},
                        {0, 0},
                        {0, 0},
                    },
                .consecuences_of_option_B =
                    {
                        {.mod_num = 4, .stat = STAT_FOOD_PRODUCTION},
                        {.mod_num = -1, .stat = STAT_POWER_PRODUCTION},
                        {0, 0},
                    },
                .incidents =
                    {
                        {
                            .incident_name_index = 2,
                            .probability = 20,
                            .consecuences =
                                {
                                    {.mod_num = -1,
                                     .stat = STAT_FOOD_PRODUCTION},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                        {
                            .incident_name_index = 3,
                            .probability = 15,
                            .consecuences =
                                {
                                    {.mod_num = 0,
                                     .stat =
                                         STAT_FOOD_PRODUCTION}, // no production
                                                                // until fixed
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                    },
            },
    },
    {
        .type = EVENT_CREATION,
        .as.creation =
            {
                .creation_name_index = 2,
                .optA = "Reinforce shelter",
                .optB = "Full seal upgrade",
                .consecuences_of_option_A =
                    {
                        {.mod_num = 8, .stat = STAT_RAD_SHIELD},
                        {0, 0},
                        {0, 0},
                    },
                .consecuences_of_option_B =
                    {
                        {.mod_num = 12, .stat = STAT_RAD_SHIELD},
                        {.mod_num = 1, .stat = STAT_POWER_CONSUMTION},
                        {0, 0},
                    },
                .incidents =
                    {
                        {
                            .incident_name_index = 4,
                            .probability = 15,
                            .consecuences =
                                {
                                    {.mod_num = -3, .stat = STAT_RAD_SHIELD},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                        {
                            .incident_name_index = 5,
                            .probability = 10,
                            .consecuences =
                                {
                                    {0, 0},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                    },
            },
    },

    // Actions
    {
        .type = EVENT_ACTION,
        .as.action =
            {
                .action_name_index = 0,
                .consecuences =
                    {
                        {.mod_num = 5, .stat = STAT_RAD_SHIELD},
                        {.mod_num = -3, .stat = STAT_POWER_STORAGE},
                        {0, 0},
                    },
                .incidents =
                    {
                        {
                            .incident_name_index = 6,
                            .probability = 20,
                            .consecuences =
                                {
                                    {.mod_num = -5, .stat = STAT_HEALTH},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                        {
                            .incident_name_index = 7,
                            .probability = 15,
                            .consecuences =
                                {
                                    {.mod_num = -2, .stat = STAT_POWER_STORAGE},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                    },
            },
    },
    {
        .type = EVENT_ACTION,
        .as.action =
            {
                .action_name_index = 1,
                .consecuences =
                    {
                        {.mod_num = -1, .stat = STAT_FOOD_CONSUMTION},
                        {.mod_num = -5, .stat = STAT_SLEEP},
                        {0, 0},
                    },
                .incidents =
                    {
                        {
                            .incident_name_index = 8,
                            .probability = 15,
                            .consecuences =
                                {
                                    {.mod_num = -10, .stat = STAT_SLEEP},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                        {
                            .incident_name_index = 9,
                            .probability = 10,
                            .consecuences =
                                {
                                    {.mod_num = -2, .stat = STAT_HEALTH},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                    },
            },
    },
    {
        .type = EVENT_ACTION,
        .as.action =
            {
                .action_name_index = 2,
                .consecuences =
                    {
                        {.mod_num = 1, .stat = STAT_POWER_CONSUMTION},
                        {0, 0},
                        {0, 0},
                    },
                .incidents =
                    {
                        {
                            .incident_name_index = 10,
                            .probability = 15,
                            .consecuences =
                                {
                                    {.mod_num = -4, .stat = STAT_HEALTH},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                        {
                            .incident_name_index = 11,
                            .probability = 10,
                            .consecuences =
                                {
                                    {0, 0},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                    },
            },
    },
    {
        .type = EVENT_ACTION,
        .as.action =
            {
                .action_name_index = 3,
                .consecuences =
                    {
                        {.mod_num = 2, .stat = STAT_POWER_PRODUCTION},
                        {.mod_num = -2, .stat = STAT_RAD_SHIELD},
                        {0, 0},
                    },
                .incidents =
                    {
                        {
                            .incident_name_index = 12,
                            .probability = 20,
                            .consecuences =
                                {
                                    {.mod_num = -1,
                                     .stat = STAT_POWER_PRODUCTION},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                        {
                            .incident_name_index = 13,
                            .probability = 15,
                            .consecuences =
                                {
                                    {.mod_num = -4, .stat = STAT_POWER_STORAGE},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                    },
            },
    },
    {
        .type = EVENT_ACTION,
        .as.action =
            {
                .action_name_index = 4,
                .consecuences =
                    {
                        {.mod_num = 15, .stat = STAT_SLEEP},
                        {.mod_num = -2, .stat = STAT_POWER_STORAGE},
                        {0, 0},
                    },
                .incidents =
                    {
                        {
                            .incident_name_index = 14,
                            .probability = 10,
                            .consecuences =
                                {
                                    {.mod_num = -5, .stat = STAT_SLEEP},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                        {
                            .incident_name_index = 15,
                            .probability = 10,
                            .consecuences =
                                {
                                    {.mod_num = 1,
                                     .stat = STAT_FOOD_CONSUMTION},
                                    {0, 0},
                                    {0, 0},
                                },
                        },
                    },
            },
    },

    // Problems
    {
        .type = EVENT_PRROBLEM,
        .as.problem =
            {
                .problem_name_index = 0,
                .addressing_consecuences =
                    {
                        {.mod_num = -2, .stat = STAT_POWER_STORAGE},
                        {0, 0},
                        {0, 0},
                    },
                .ignoring_consecuences =
                    {
                        {.mod_num = -4, .stat = STAT_HEALTH},
                        {.mod_num = -3, .stat = STAT_SLEEP},
                        {0, 0},
                    },
            },
    },
    {
        .type = EVENT_PRROBLEM,
        .as.problem =
            {
                .problem_name_index = 1,
                .addressing_consecuences =
                    {
                        {.mod_num = -2, .stat = STAT_HEALTH},
                        {0, 0},
                        {0, 0},
                    },
                .ignoring_consecuences =
                    {
                        {.mod_num = -3, .stat = STAT_FOOD_STORAGE},
                        {0, 0},
                        {0, 0},
                    },
            },
    },
    {
        .type = EVENT_PRROBLEM,
        .as.problem =
            {
                .problem_name_index = 2,
                .addressing_consecuences =
                    {
                        {.mod_num = -4, .stat = STAT_POWER_STORAGE},
                        {0, 0},
                        {0, 0},
                    },
                .ignoring_consecuences =
                    {
                        {.mod_num = -2, .stat = STAT_POWER_PRODUCTION},
                        {0, 0},
                        {0, 0},
                    },
            },
    },
    {
        .type = EVENT_PRROBLEM,
        .as.problem =
            {
                .problem_name_index = 3,
                .addressing_consecuences =
                    {
                        {.mod_num = -3, .stat = STAT_POWER_STORAGE},
                        {0, 0},
                        {0, 0},
                    },
                .ignoring_consecuences =
                    {
                        {.mod_num = -6, .stat = STAT_HEALTH},
                        {0, 0},
                        {0, 0},
                    },
            },
    },
    {
        .type = EVENT_PRROBLEM,
        .as.problem =
            {
                .problem_name_index = 4,
                .addressing_consecuences =
                    {
                        {.mod_num = -2, .stat = STAT_FOOD_STORAGE},
                        {0, 0},
                        {0, 0},
                    },
                .ignoring_consecuences =
                    {
                        {.mod_num = -2, .stat = STAT_FOOD_PRODUCTION},
                        {0, 0},
                        {0, 0},
                    },
            },
    },
    {
        .type = EVENT_PRROBLEM,
        .as.problem =
            {
                .problem_name_index = 5,
                .addressing_consecuences =
                    {
                        {.mod_num = 20, .stat = STAT_SLEEP},
                        {.mod_num = -1, .stat = STAT_POWER_PRODUCTION},
                        {0, 0},
                    },
                .ignoring_consecuences =
                    {
                        {.mod_num = -3, .stat = STAT_HEALTH},
                        {.mod_num = -10, .stat = STAT_SLEEP},
                        {0, 0},
                    },
            },
    },
};

#define CARD_W 295
#define CARD_H 390

#define TOP_IMG_X 14
#define TOP_IMG_Y 0
#define TOP_IMG_W 267
#define TOP_IMG_H 140

#define CARD_MARGIN 12
#define TEXT_X 18
#define TEXT_W 259

#define CHOICE_H 34
#define CHOICE_W 250

static void DrawWrappedText(const char *text, int x, int y, int fontSize,
                            int spacing, int maxWidth, Color color) {
    if (!text || !text[0])
        return;

    const char *p = text;
    char line[512] = {0};
    int lineLen = 0;
    int curY = y;

    while (*p) {
        char word[128] = {0};
        int wlen = 0;

        while (*p && *p != ' ' && *p != '\n' && wlen < (int)sizeof(word) - 1)
            word[wlen++] = *p++;

        word[wlen] = '\0';

        char test[512] = {0};

        if (lineLen > 0)
            snprintf(test, sizeof(test), "%s %s", line, word);
        else
            snprintf(test, sizeof(test), "%s", word);

        if (MeasureText(test, fontSize) > maxWidth && lineLen > 0) {
            DrawText(line, x, curY, fontSize, color);
            curY += fontSize + spacing;
            line[0] = '\0';
            lineLen = 0;
        }

        if (lineLen > 0)
            strncat(line, " ", sizeof(line) - strlen(line) - 1);
        strncat(line, word, sizeof(line) - strlen(line) - 1);
        lineLen = (int)strlen(line);

        while (*p == ' ')
            p++;
        if (*p == '\n') {
            DrawText(line, x, curY, fontSize, color);
            curY += fontSize + spacing;
            line[0] = '\0';
            lineLen = 0;
            p++;
            while (*p == ' ')
                p++;
        }
    }

    if (lineLen > 0)
        DrawText(line, x, curY, fontSize, color);
}

static void DrawChoiceBox(Rectangle r, const char *text, bool hover) {
    DrawRectangleRounded(r, 0.15f, 8,
                         hover ? (Color){70, 70, 70, 255}
                               : (Color){40, 40, 40, 255});
    DrawRectangleRoundedLines(r, 0.15f, 2, RAYWHITE);
    DrawWrappedText(text, (int)r.x + 10, (int)r.y + 8, 16, 2, (int)r.width - 20,
                    RAYWHITE);
}

static Texture card_texture;
Texture *draw_event_card(size_t idx) {
    /* Replace with your own base card path */
    const char *basePath = "assets/event_card.png";

    Image base = LoadImage(basePath);
    if (base.data == NULL) {
        /* Fallback blank card */
        base = GenImageColor(CARD_W, CARD_H, RED);
    }

    if (base.width != CARD_W || base.height != CARD_H) {
        ImageResize(&base, CARD_W, CARD_H);
    }

    Texture2D baseTex = LoadTextureFromImage(base);

    RenderTexture2D target = LoadRenderTexture(CARD_W, CARD_H);

    BeginTextureMode(target);
    ClearBackground(BLANK);
    DrawTexture(baseTex, 0, 0, WHITE);

    const char *title;
    const char *c1 = NULL, *c2 = "Ok";
    switch (event_list[idx].type) {
    case EVENT_ACTION:
        title = action_names[event_list[idx].as.action.action_name_index];
        break;
    case EVENT_PRROBLEM:
        title = problem_names[event_list[idx].as.problem.problem_name_index];
        c1 = "Address";
        c2 = "Ignore";
        break;
    case EVENT_INCIDENT:
        title = incident_names[event_list[idx].as.incident.incident_name_index];
        break;
    case EVENT_CREATION:
        title = creation_names[event_list[idx].as.creation.creation_name_index];
        c1 = event_list[idx].as.creation.optA;
        c2 = event_list[idx].as.creation.optB;
        break;
    case EVENT_NONE:
    default:
        title = "Err event";
        break;
    }

    // Don't draw over the top image area (0..140)
    DrawWrappedText(title, TEXT_X, 150, 20, 2, TEXT_W, BLACK);

    Rectangle b1 = {22, 290, CHOICE_W, CHOICE_H};
    Rectangle b2 = {22, 332, CHOICE_W, CHOICE_H};

    if (event_list[idx].type == EVENT_CREATION ||
        event_list[idx].type == EVENT_PRROBLEM) {
        DrawChoiceBox(b1, c1, false);
    }
    DrawChoiceBox(b2, c2, false);

    EndTextureMode();

    UnloadTexture(baseTex);
    UnloadImage(base);

    card_texture = target.texture;
    return &card_texture;
}

bool input_loop_event_card(size_t idx, Vector2 pos) {
    struct event e = event_list[idx];

    Vector2 m = GetMousePosition();
    int type = e.type;

    struct consecuence *consecuences = NULL;

    if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        goto after_input;

    TraceLog(LOG_WARNING, "Click on %d,%d", GetMouseX(), GetMouseY());

    Rectangle b1 = {22 + pos.x, 290 + pos.y, CHOICE_W, CHOICE_H};
    Rectangle b2 = {22 + pos.x, 332 + pos.y, CHOICE_W, CHOICE_H};
    switch (type) {
    case EVENT_CREATION:
        struct event_creation c = e.as.creation;
        if (CheckCollisionPointRec(m, b1))
            consecuences = c.consecuences_of_option_A;
        else if (CheckCollisionPointRec(m, b2))
            consecuences = c.consecuences_of_option_B;
        break;
    case EVENT_PRROBLEM:
        struct event_problem p = e.as.problem;
        if (CheckCollisionPointRec(m, b1))
            consecuences = p.addressing_consecuences;
        else if (CheckCollisionPointRec(m, b2))
            consecuences = p.ignoring_consecuences;
        break;
    case EVENT_INCIDENT:
        if (CheckCollisionPointRec(m, b2))
            consecuences = e.as.incident.consecuences;
        break;
    case EVENT_ACTION:
        if (CheckCollisionPointRec(m, b2))
            consecuences = e.as.action.consecuences;
        break;
    }
after_input:
    if (consecuences == NULL)
        return false;

    for (size_t i = 0; i < 3; i++)
        apply_consecuence(consecuences[i]);
    return true;
}
