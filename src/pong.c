// Copyright 2025 Farruh4eg

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define FIELD_WIDTH 80
#define FIELD_HEIGHT 25
#define TARGET_FPS 15
#define MICROSECONDS_PER_FRAME (1000000 / TARGET_FPS)
#define WINNING_SCORE 21

typedef struct Player {
    int pos;
    int score;
} Player;

typedef struct Ball {
    int x;
    int y;
    int dx;  // direction by x: -1 or 1
    int dy;  // direction by y: -1 or 1
} Ball;

void draw_field(Ball, Player, Player);
void clear();
void get_input(Player *player_one, Player *player_two, int *to_exit);
void wait_for_frame();
int to_deflect(Ball, Player);
void update_ball(Ball *ball, Player player_one, Player player_two);
void check_goal(Ball *ball, Player *player_one, Player *player_two);

int main() {
    srand(time(NULL));

    int to_exit = 0;

    Player player_one = {1, 0};

    Player player_two = {12, 0};

    Ball ball = {FIELD_WIDTH / 2, FIELD_HEIGHT / 2, 1, 1};

    while (!to_exit) {
        clear();

        draw_field(ball, player_one, player_two);

        printf("СЧЕТ: %d - %d\n", player_one.score, player_two.score);
        if (player_one.score >= WINNING_SCORE) {
            printf("PLAYER 1 WINS!\n");
            break;
        }
        if (player_two.score >= WINNING_SCORE) {
            printf("PLAYER 2 WINS!\n");
            break;
        }

        get_input(&player_one, &player_two, &to_exit);
        update_ball(&ball, player_one, player_two);
        check_goal(&ball, &player_one, &player_two);

        // can remove the next line. No fps in turn-based game, right?
        wait_for_frame();
    }
}

void draw_field(Ball ball, Player player_one, Player player_two) {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (y == 0 || y == FIELD_HEIGHT - 1) {
                printf("=");
            } else if (x == ball.x && y == ball.y) {
                printf("0");
            } else if (y >= player_one.pos && y <= player_one.pos + 2 &&
                       x == 1) {
                printf("|");
            } else if (y >= player_two.pos && y <= player_two.pos + 2 &&
                       x == FIELD_WIDTH - 2) {
                printf("|");
            } else {
                printf(" ");
            }
        }

        printf("\n");
    }
}

void clear() {
    printf("\033[2J\033[H");
    fflush(stdout);  // NOLINT
}

void wait_for_frame() { usleep(MICROSECONDS_PER_FRAME); }

void get_input(Player *player_one, Player *player_two, int *to_exit) {
    int input_int = getchar();

    if (input_int == EOF) {
        return;
    }

    char input = input_int;

    if (input == 'a') {
        if (player_one->pos > 1) player_one->pos--;
    } else if (input == 'z') {
        if (player_one->pos < FIELD_HEIGHT - 4) player_one->pos++;
    } else if (input == 'k') {
        if (player_two->pos > 1) player_two->pos--;
    } else if (input == 'm') {
        if (player_two->pos <= FIELD_HEIGHT - 4) player_two->pos++;
    } else if (input == ' ') {
        // skip turn
    } else if (input == 'q') {
        *to_exit = 1;
    }

    // buffer clear
    if (input != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
}

void update_ball(Ball *ball, Player player_one, Player player_two) {
    if (ball->x == 2 && ball->dx == -1 &&
        (ball->y >= player_one.pos && ball->y <= player_one.pos + 2)) {
        ball->dx *= -1;
    }
    if (ball->x == FIELD_WIDTH - 2 && ball->dx == 1 &&
        (ball->y >= player_two.pos && ball->y <= player_two.pos + 2)) {
        ball->dx *= -1;
    }

    if ((ball->y <= 1 && ball->dy == -1) ||
        (ball->y >= FIELD_HEIGHT - 2 && ball->dy == 1)) {
        ball->dy *= -1;
    }

    ball->x += ball->dx;
    ball->y += ball->dy;
}

void check_goal(Ball *ball, Player *player_one, Player *player_two) {
    if (ball->x < 0) {
        player_two->score++;
        ball->x = FIELD_WIDTH / 2;
        ball->y = FIELD_HEIGHT / 2;
        ball->dx = 1;
    } else if (ball->x > 79) {
        player_one->score++;
        ball->x = FIELD_WIDTH / 2;
        ball->y = FIELD_HEIGHT / 2;
        ball->dx = -1;
    }
}
