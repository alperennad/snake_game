#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

//harita olustur ++
//yılan spawnla ++
//klavye kontrolu ekle ++
//yılanı hareket ettir ++
//yem spawnla ++
//yem yedikce yılanı buyut ++
//duvar kontrolu ekle ++
#define ROWS 30
#define COLS 60

int isGameOver = 0, isEaten = 0;
int foodX,foodY;
int snakeX = (ROWS / 2) - 1, snakeY = (COLS / 2) - 1;
int tailX[ROWS * COLS], tailY[ROWS * COLS];

int    draw_snake(int i, int j)
{
    //kelle
    if (snakeX == i && snakeY == j)
    {
        printf("@");
        return 1;
    }
    //kuyruk
    for (int k = 0; k < isEaten; k++)
    {
        if (i == tailX[k] && j == tailY[k])
        {
            printf("o");
            return 1;
        }
    }
    return 0;  
}

void generate_map()
{
    system("clear");

    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLS; j++)
        {
            if (!draw_snake(i,j))
            {
            if (i == 0 ||i == ROWS - 1)
                printf("#");
            else if(j == 0 || j == COLS - 1)
                printf("H");
            else if (i == foodX && j == foodY)
                printf("F");
            else
                printf(" ");
            }
            
        }
        printf("\n");
    }
}


void    generate_food()
{   //rastgele konuma yemek spawnlıyor
    isGameOver = 0;
        if (foodX <= 0)
            foodX = rand() % ROWS - 2;
        if(foodY <= 0)
            foodY = rand() % COLS - 2;
    
}

void    move_check()
{       //yemek yendi mi??
     if (snakeX == foodX && snakeY == foodY)
    {
        foodX = 0;
        foodY = 0;
        isEaten++;
    }
}

void    snake_move()
{
    //klavyeden girdi kontrolu termios.h
    //yon tusundan sonra enterlamamamı sagliyor
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int move = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    for (int i = isEaten; i > 0; i--)
    {
        tailX[i] = tailX[i - 1];
        tailY[i] = tailY[i - 1];
    }

    tailX[0] = snakeX;
    tailY[0] = snakeY;
    //hareket kontrolu
    if (move == 'w')
        snakeX--;
    else if (move == 'a')
        snakeY--;
    else if (move == 's')
        snakeX++;
    else if (move == 'd')
        snakeY++;
    if (move == 'q')
        isGameOver = 1;
    
    //kafa kontrolu:
    if (snakeY >= COLS)
        snakeY = 1;
    else if (snakeY <= 0)
        snakeY = COLS - 1;
    if (snakeX >= ROWS)
        snakeX = 1;
    else if (snakeX <= 0)
        snakeX = ROWS - 1;
    //kuyruk kontrolu
    int a = 0, b = 0;
    while (tailX[a] >= ROWS * 2)
    {
        tailX[a] = 1;
        a++;
    }
     while (tailX[a] <= 0)
    {
        tailY[b] = ROWS - 1;
        a++;
    }
    while (tailY[b] >= COLS * 2)
    {
        tailY[b] = 1;
        b++;
    }
     while (tailY[b] <= 0)
    {
        tailY[b] = COLS - 1;
        b++;
    }



}
int main()
{
    while (!isGameOver)
    {
        generate_food();
        move_check();
        generate_map();
        snake_move();

        usleep(500);
    }
    while (isGameOver)
        return 0;

}
