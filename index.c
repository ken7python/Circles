//cc index.c  libraylib.a -framework IOKit -framework Cocoa && ./a.out
#include<raylib.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

const int screenWidth = 1000;
const int screenHeight = 600;

typedef struct circle
{
    int x;
    int y;
    int xv;
    int yv;

    int speed;

    int radius;
    Color color;
} circle;

void circle_init(circle* c, int x, int y, int xv, int yv,int radius, Color color){
    c->x = x;
    c->y = y;
    c->xv = xv;
    c->yv = yv;
    c->radius = radius;
    c->color = color;

    printf("circle x: %d y: %d xv: %d yv: %d radius: %d\n", c->x, c->y, c->xv, c->yv, c->radius);
}
void circle_draw(circle* c){
    DrawCircle(c->x, c->y, c->radius, c->color);
}
void circle_move(circle* c){
    c->x += c->xv;
    c->y += c->yv;

    if (c->x < 0 || c->x > screenWidth){
        c->xv *= -1;
    }
    if (c->y < 0 || c->y > screenHeight){
        c->yv *= -1;
    }

}
void circle_mouse(circle* c){
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();

    c->x += (mouseX - c->x) * 0.1;
    c->y += (mouseY - c->y) * 0.1;
}

bool circle_col(circle* c1, circle* c2){
    int dx = c1->x - c2->x;
    int dy = c1->y - c2->y;
    int distance = sqrt(dx * dx + dy * dy);

    if (distance < c1->radius + c2->radius){
        return true;
    }
    return false;
}

circle player;
void player_init(){
    circle_init(&player, screenWidth / 2, screenHeight / 2, 0, 0, 12, RED);
}

const int MAX_ENEMIES = 14;
circle enemies[MAX_ENEMIES];
Color colors[5] = {BLUE, GREEN,PURPLE, DARKBLUE, VIOLET};

int rs[MAX_ENEMIES];

void init_enemies() {
    {
        int i = MAX_ENEMIES + 1;
        while(--i >= MAX_ENEMIES / 3){
            rs[i] = GetRandomValue(7, 15);
        }
        while(--i >= 0){
            rs[i] = GetRandomValue(15, 30);
        }
    }

    int i = -1;
    while(++i < MAX_ENEMIES){
        int p = GetRandomValue(0, 3);
        int x,y;
        int xv = 0;
        int yv = 0;

        if(p == 0){
            x = GetRandomValue(0, screenWidth);
            y = GetRandomValue(0, 1) * screenHeight;
        }else
        if (p == 1){
            x = GetRandomValue(0, 1) * screenWidth;
            y = GetRandomValue(0, screenHeight);
        }else
        if (p == 2){
            x = GetRandomValue(0, screenWidth);
            y = 0;
        }else
        if (p == 3){
            x = screenWidth;
            y = GetRandomValue(0, screenHeight);
        }

        while (xv == 0 && yv == 0){
            xv = GetRandomValue(-5, 5);
            yv = GetRandomValue(-5, 5);
        }
        int r = GetRandomValue(3, 30);
        circle_init(&enemies[i], x, y, xv,yv, rs[i], colors[GetRandomValue(0, 4)]);
    }
}

void circle_powerup(circle* c, circle* p){
    c->radius += p->radius / 4;
}

void circle_disappear(circle* c){
    c->x = -100;
    c->y = -100;
    c->xv = 0;
    c->yv = 0;
}

int main(){
    bool start = false;
    bool gameclear = false;

    int heratime = 0;
    int cleartime = 0;

    InitWindow(screenWidth, screenHeight, "Circle Game");
    srand(time(NULL));
    player_init();
    
    init_enemies();
    
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        if (start){
            circle_mouse(&player);
        }
        if (!start && IsKeyPressed(KEY_SPACE)){
            init_enemies();
            start = true;
            heratime = GetTime();
        }
        if (gameclear && IsKeyPressed(KEY_SPACE)){
            gameclear = false;
            player_init();
            init_enemies();
            start = true;
            heratime = GetTime();
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);

            circle_draw(&player);
            if (gameclear){
                DrawText("Game Clear", 250, 200, 50, RED);
                DrawText("Congratulations!", 250, 250, 50, BLUE);
                DrawText(TextFormat("Clear Time: %d seconds", cleartime), 250, 300, 50, GREEN);
                DrawText("Press SPACE to restart", 250, 350, 50, BLACK);
            }else
            if (start){
                int i = -1;
                int count = 0;
                while(++i < MAX_ENEMIES){
                    circle_draw(&enemies[i]);
                    circle_move(&enemies[i]);

                    if (circle_col(&player, &enemies[i])){
                        if (player.radius < enemies[i].radius){
                            start = false;
                            player_init();
                        }else{
                            circle_disappear(&enemies[i]);
                            circle_powerup(&player, &enemies[i]);
                        }
                    }

                    if (enemies[i].x < 0 || enemies[i].x > screenWidth || enemies[i].y < 0 || enemies[i].y > screenHeight){
                        count++;
                    }
                }
                if (count == MAX_ENEMIES){
                    start = false;
                    gameclear = true;
                    cleartime = GetTime() - heratime;
                }
            }else{
                DrawText("Press SPACE to start", screenWidth / 2 - MeasureText("Press SPACE to start", 50) / 2, screenHeight / 2, 50, BLACK);
            }
        EndDrawing();
    }
    return 0;
}