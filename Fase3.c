#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define MAX_TIROS 150

typedef struct BBULLET
{
    Vector2 pos;
    bool active;
    float raio;
    float xspd;
    float yspd;
    float rad;
    float def;
}BBULLET;

typedef struct BOSS
{
    Vector2 pos;
    Vector2 npos;
    Vector2 spd;
    float hp;
    float hitbox;
    float ang;
    int phase;
}BOSS;

typedef struct PBULLET
{
    Vector2 pos;
    float spd;
    float rad;
    float raio;
    Color cor;
    bool active;
}PBULLET;

typedef struct Player
{
    Vector2 pos;
    float spd;
    float dmg;
    Color color;
    float hitbox;
    float firerate;
    PBULLET bullet[MAX_TIROS];
    int tiro;
    bool invincible;
}Player;

static const int Largura_Tela = 720;
static const int Altura_Tela = 800;
static int iFrame = 0;
static int lives=3;
static bool gameOver = false;
static float backgroundScroll = 0;
static Texture2D Nave;
static Texture2D fundo;
static BOSS Pericles;
static BBULLET bbullet[MAX_TIROS];
static int iFrame;
static Player player;


static void InitFase3(void);
static void UpdateFase3(void);
static void DrawFase3(void);
static void Movimento(void);
static void Atirar(void);
static void Pattern1(void);
static void Pattern2(void);
static void Pattern3(void);

int main(void)
{
    InitWindow(Largura_Tela, Altura_Tela, "Fase 3");
    SetTargetFPS(60);
    
    InitFase3();
    while(!WindowShouldClose() && !gameOver)
    {
        UpdateFase3();
        DrawFase3();
    }
}

void Pattern1(void)
{
    static int n_bullet=0;
    int chance = GetRandomValue(1, 100);
    if(chance <=10)
    {
        for(int i=0; i<MAX_TIROS; i++)
        {
            if(bbullet[i].active == false)
            {
                bbullet[i].active = true;
                bbullet[i].rad = Pericles.hitbox*1.5;
                bbullet[i].def = GetRandomValue(-20, 20)/10.0f;
                n_bullet++;
                break;
            }
        }
    }
    
    for(int i=0; i<MAX_TIROS; i++)
    {
        if(bbullet[i].active)
        {
            bbullet[i].pos.x = Pericles.pos.x + bbullet[i].rad * cos(Pericles.ang + bbullet[i].def);
            bbullet[i].pos.y = Pericles.pos.y + bbullet[i].rad * sin(Pericles.ang + bbullet[i].def);
            bbullet[i].rad += 2;
            if(bbullet[i].pos.y >= Altura_Tela)
            {
                bbullet[i].active = false;
            }
        }
        if(bbullet[i].active && !player.invincible && CheckCollisionCircles(bbullet[i].pos, bbullet[i].raio, player.pos, player.hitbox))
        {
            lives--;
            if(lives <=0)
            {
                gameOver = true;
            }
            player.invincible = true;
        }
    }
}

void Pattern2(void)
{
    static float increase = 6;
    float spacing = 2*PI/6;
    for(int i=0; i<6; i++)
    {
        for(int j=0; j<MAX_TIROS; j++)
        {
            if(bbullet[j].active == false)
            {
                bbullet[j].active = true;
                bbullet[j].def = spacing*j;
                bbullet[j].rad = Pericles.hitbox*1.8;
                bbullet[j].pos.x = Pericles.pos.x + bbullet[j].rad*cos(Pericles.ang + bbullet[j].def);
                bbullet[j].pos.y = Pericles.pos.y + bbullet[j].rad*sin(Pericles.ang + bbullet[j].def);
                break;
            }
        }
    }
    spacing = 2*PI/12;
    for(int i=0; i<12; i++)
    {
        for(int j=0; j<MAX_TIROS; j++)
        {
            if(bbullet[j].active == false)
            {
                bbullet[j].active = true;
                bbullet[j].def = spacing*j + 10;
                bbullet[j].rad = Pericles.hitbox*2.8;
                bbullet[j].pos.x = Pericles.pos.x + bbullet[j].rad*cos(Pericles.ang + bbullet[j].def);
                bbullet[j].pos.y = Pericles.pos.y + bbullet[j].rad*sin(Pericles.ang + bbullet[j].def);
                break;
            }
        }
    }
    spacing = 2*PI/20;
    for(int i=0; i<20; i++)
    {
        for(int j=0; j<MAX_TIROS; j++)
        {
            if(bbullet[j].active == false)
            {
                bbullet[j].active = true;
                bbullet[j].def = spacing*j + 30;
                bbullet[j].rad = Pericles.hitbox*3.8;
                bbullet[j].pos.x = Pericles.pos.x + bbullet[j].rad*cos(Pericles.ang + bbullet[j].def);
                bbullet[j].pos.y = Pericles.pos.y + bbullet[j].rad*sin(Pericles.ang + bbullet[j].def);
                break;
            }
        }
    }
    for(int i=0; i<MAX_TIROS; i++)
    {
        if(bbullet[i].active)
        {
            bbullet[i].pos.x = Pericles.pos.x + bbullet[i].rad * cos(Pericles.ang + bbullet[i].def);
            bbullet[i].pos.y = Pericles.pos.y + bbullet[i].rad * sin(Pericles.ang + bbullet[i].def);
            bbullet[i].rad += increase;
            if(bbullet[i].rad >= Altura_Tela || bbullet[i].rad <= Pericles.hitbox)
            {
                increase *= -1;
            }
        }
        if(bbullet[i].active && !player.invincible && CheckCollisionCircles(bbullet[i].pos, bbullet[i].raio, player.pos, player.hitbox))
        {
            lives--;
            if(lives <= 0)
            {
                gameOver = true;
            }
            player.invincible = true;
        }
    }
}

void Pattern3(void)
{
    int chance = GetRandomValue(1, 100);
    if(chance <=6)
    {
        for(int i=0; i<MAX_TIROS; i++)
        {
            if(bbullet[i].active == false)
            {
                bbullet[i].active = true;
                bbullet[i].pos.x = Pericles.pos.x;
                bbullet[i].pos.y = Pericles.pos.y;
                bbullet[i].xspd = GetRandomValue(-6, 6);
                bbullet[i].yspd = 6;
                break;
            }
        }
    }
    for(int i=0; i<MAX_TIROS; i++)
    {
        if(bbullet[i].active)
        {
            float delta = player.pos.x - bbullet[i].pos.x;
            bbullet[i].pos.y += bbullet[i].yspd;
            if(delta*bbullet[i].xspd <0)
            {
                bbullet[i].xspd = -(bbullet[i].xspd + 0.5);
            }
            bbullet[i].pos.x += bbullet[i].xspd;
            if(bbullet[i].pos.y >= Altura_Tela)
            {
                bbullet[i].active = false;
            }
        }
        if(bbullet[i].active && !player.invincible && CheckCollisionCircles(player.pos, player.hitbox, bbullet[i].pos, bbullet[i].raio))
        {
            lives--;
            if(lives <= 0)
            {
                gameOver = true;
            }
            player.invincible = true;
        }
    }
}

void UpdateFase3(void)
{
    backgroundScroll += 3.0;
    if(backgroundScroll >= Altura_Tela) backgroundScroll = 0;
    
    if(player.invincible)
    {
        iFrame++;
        if(iFrame >= 90)
        {
            iFrame = 0;
            player.invincible = false;
        }
    }
    
    Movimento();
    Atirar();
    BossMov();
    
    Pericles.ang += 0.03;
    
    if(Pericles.hp>200)
    {
        Pattern1();
    } else 
    if(Pericles.hp>100)
    {
        Pattern2();
    } else 
    if(Pericles.hp>0)
    {
        Pattern3();
    }
    
}

void DrawFase3(void)
{
    
    BeginDrawing();
    DrawTexture(fundo, 0, backgroundScroll, RAYWHITE);
    DrawTexture(fundo, 0, backgroundScroll - fundo.height, RAYWHITE);
    
    if(!player.invincible)
    {
        DrawTexture(Nave, player.pos.x - Nave.width/2, player.pos.y - Nave.height/2, RAYWHITE);
    } else 
    {
        if(iFrame%3 == 0 || iFrame%4 == 0)
            {
                DrawTexture(Nave, player.pos.x - Nave.width/2, player.pos.y - Nave.height/2, RAYWHITE);
            }
    }
    DrawCircleV(player.pos, player.hitbox, PINK);
    DrawCircleV(Pericles.pos, Pericles.hitbox, PURPLE);
    
    for(int i=0; i<MAX_TIROS; i++)
    {
        if(player.bullet[i].active)
        {
            DrawCircleV(player.bullet[i].pos, player.bullet[i].raio, BLUE);
        }
        if(bbullet[i].active)
        {
            DrawCircleV(bbullet[i].pos, bbullet[i].raio, RED);
        }
    }
    
    if(Pericles.hp>200)
    {
        DrawRectangle(20, 20, Largura_Tela-40, 20, GREEN);
        DrawRectangle(20, 20, (Largura_Tela*(Pericles.hp - 200)/100) - 40, 20, BLUE);
    } else 
    if(Pericles.hp>100)
    {
        DrawRectangle(20, 20, Largura_Tela-40, 20, RED);
        DrawRectangle(20, 20, (Largura_Tela*(Pericles.hp - 100)/100) - 40, 20, GREEN);
    } else 
    {
        DrawRectangle(20, 20, (Largura_Tela*Pericles.hp/100) - 40, 20, RED);
    }
    
    EndDrawing();
}

void InitFase3(void)
{
    Image tempnave = LoadImage("/raylib/StarlightDrift/texture/nave.png");
    Image tempfundo = LoadImage("/raylib/StarlightDrift/texture/space.png");
    
    ImageResize(&tempnave, 50, 60);
    ImageResize(&tempfundo, Largura_Tela, Altura_Tela);
    
    Nave = LoadTextureFromImage(tempnave);
    fundo = LoadTextureFromImage(tempfundo);
    
    UnloadImage(tempnave);
    UnloadImage(tempfundo);
    
    player.pos.x = Largura_Tela/2;
    player.pos.y = Largura_Tela*0.8;
    player.spd = 7;
    player.dmg = 2;
    player.hitbox = 6;
    player.firerate = 5;
    player.invincible = false;
    for(int i=0; i<MAX_TIROS; i++)
    {
        player.bullet[i].active = false;
        player.bullet[i].spd = 10;
        player.bullet[i].raio = 6;
        bbullet[i].active = false;
        bbullet[i].rad = 7;
        bbullet[i].raio = 7;
    }
    
    Pericles.pos.x = Largura_Tela/2;
    Pericles.pos.y = Altura_Tela*0.2;
    Pericles.hitbox = 50;
    Pericles.hp = 300;
    Pericles.ang = 0;
    Pericles.phase = 1;
}

void Atirar(void)
{
    if(IsKeyDown(KEY_SPACE))
    {
        player.firerate++;
        if(player.firerate >= 5)
        {
            player.firerate = 0;
            for(int i=0; i<MAX_TIROS; i++)
            {
                if(player.bullet[i].active == false)
                {
                    player.bullet[i].active = true;
                    player.bullet[i].pos.x = player.pos.x;
                    player.bullet[i].pos.y = player.pos.y - Nave.height/2;
                    break;
                }
            }
        }
    }
    for(int i=0; i<MAX_TIROS; i++)
    {
        if(player.bullet[i].active)
        {
            player.bullet[i].pos.y -= player.bullet[i].spd;
            
            if(player.bullet[i].pos.y <=0)
            {
                player.bullet[i].active = false;
            }
        }
    }
    
    for(int i=0; i<MAX_TIROS; i++)
    {
        if(player.bullet[i].active && CheckCollisionCircles(player.bullet[i].pos, player.bullet[i].raio, Pericles.pos, Pericles.hitbox))
        {
            player.bullet[i].active = false;
            Pericles.hp -= player.dmg;
            if((Pericles.hp <= 200 && Pericles.phase ==1) || (Pericles.hp <= 100 && Pericles.phase ==2))
            {
                Pericles.phase++;
                for(int i=0; i<MAX_TIROS; i++)
                {
                    bbullet[i].active = false;
                }
            }
        
        }
    }
}

void BossMov(void)
{
    static int BossMovCounter = 0;
    static float x;
    static float y;
    if(BossMovCounter == 0)
    {
        Pericles.npos.x = GetRandomValue(Pericles.hitbox, Largura_Tela - Pericles.hitbox);
        Pericles.npos.y = GetRandomValue(Pericles.hitbox, Altura_Tela/3.0f);
        
        x = Pericles.npos.x - Pericles.pos.x;
        y = Pericles.npos.y - Pericles.pos.y;
        
        float norma = sqrt((x*x) + (y*y));
        
        Pericles.spd.x = x/norma;
        Pericles.spd.y = y/norma;
    } else 
    if(BossMovCounter <=60)
    {
        Pericles.pos.x += Pericles.spd.x;
        Pericles.pos.y += Pericles.spd.y;
    }
    BossMovCounter++;
    if(BossMovCounter>90) BossMovCounter = 0;
}

void Movimento(void)
{
    if(IsKeyDown(KEY_W))
    {
        player.pos.y -= player.spd;
    }
    if(IsKeyDown(KEY_S))
    {
        player.pos.y += player.spd;
    }
    if(IsKeyDown(KEY_A))
    {
        player.pos.x -= player.spd;
    }
    if(IsKeyDown(KEY_D))
    {
        player.pos.x += player.spd;
    }
    
    if(player.pos.x - Nave.width/2 < 0)
    {
        player.pos.x = Nave.width/2;
    }
    if(player.pos.x + Nave.width/2 >= Largura_Tela)
    {
        player.pos.x = Largura_Tela - Nave.width/2;
    }
    if(player.pos.y - Nave.height/2 < 0)
    {
        player.pos.y = Nave.height/2;
    }
    if(player.pos.y + Nave.height/2 >= Altura_Tela)
    {
        player.pos.y = Altura_Tela - Nave.height/2;
    }
}