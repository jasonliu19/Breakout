 /*

COPY AND PASTE CODE INTO NEW FREEGLUT PROJECT

Brick breaker game

TODO: Add new power ups, make power ups drop from brick when destroyed, add levels

*/


#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

struct color{
    float R, G, B;
};

struct line{
    GLfloat x, y, length;
    double speedx, speedy;
    color lColor;
};

struct brick{
    GLfloat x1, y1, x2, y2;
    bool destroyed;
    color bColor;
    int powerUp;
};

bool started = false;

int powerUpFrequency;

float rowsOfBricks;

vector<brick> brick_list;

vector <line> ball_list;

//initialize a vector of balls
color lightBlue = {0.2f, 0.5f, 1};

line ball_1 = {50000, 50000, 1000, rand() % 300 - 200.0, -700, lightBlue};
line ball_2 = {50000, 80000, 1000, rand() % 300 - 200.0, -700, lightBlue};
line player = {42500, 10000, 15000, 0, 0, lightBlue};

int playerSpeed = 700;

bool powerUpDisplay = false;
string powerUpMessage;

//-----------------------------------------------------------------------//
void resetGame();

void drawLine(line toDraw, bool horizontal);

void displayText(string message, float xpos, float ypos, int charPos);

void init2D(float r, float g, float b)
{
	glClearColor(r,g,b,0.0);
	glMatrixMode (GL_PROJECTION);
	gluOrtho2D (0.0, 100000.0, 0.0, 100000.0);
}

line initBall()
{
    line ball = {player.x + player.length/2, player.y + 40000, 1000, rand() % 300 - 200.0, -700, lightBlue};
    return ball;
}

void activatePowerups(int powerUp)
{
    if (powerUp == 1)
    {
        for (int i = 0; i < 5; i++)
        {
            int temp = rand() % (brick_list.size() - 1);

            if (brick_list[temp].destroyed != true)
                activatePowerups(brick_list[temp].powerUp);

            brick_list[temp].destroyed = true;
        }
        powerUpMessage = "Destroying bricks";
        powerUpDisplay = true;
    }
    else if (powerUp == 2)
    {
        cout << "make ball";
        ball_list.push_back(initBall());
        powerUpMessage = "Extra ball";
        powerUpDisplay = true;
    }
    else if (powerUp == 3)
    {
        playerSpeed *= 1.2;
        powerUpMessage = "Paddle speed up";
        powerUpDisplay = true;
    }
    else if (powerUp == 4)
    {
        playerSpeed /= 1.2;
        powerUpMessage = "Paddle speed down";
        powerUpDisplay = true;
    }
    else if (powerUp == 5)
    {
        player.length *= 1.2;
        powerUpMessage = "Paddle size up";
        powerUpDisplay = true;
    }
    else if (powerUp == 6)
    {
        player.length /= 1.2;
        powerUpMessage = "Paddle size down";
        powerUpDisplay = true;
    }
}
void brickCollision(int brickNumber)
{
    if(brickNumber < 0)
        return;
    else
    {
        brick temp = brick_list[brickNumber];
        for (int i = 0; i < ball_list.size(); i++)
        {
            line* ball = &ball_list[i];
            if(ball->x > temp.x1 -200 && ball->x < temp.x2 + 200 && ball->y > temp.y2 - 700 && ball->y < temp.y1 && !temp.destroyed && ball->y != -1337)
            {
                cout << "Destroying brick\n";
                ball->speedy = -ball->speedy;
                brick_list[brickNumber].destroyed = true;
                if (ball->x > temp.x1 -200 && ball->x < temp.x1 + 2400 && ball->y > temp.y2 && ball->y < temp.y1 + 800)
                {
                    ball->speedx = -ball->speedx;
                }
                else if(ball->x > temp.x2 -2400 && ball->x < temp.x2 + 200 && ball->y > temp.y2 && ball->y < temp.y1 + 800)
                {
                    ball->speedx = -ball->speedx;
                }
                activatePowerups(brick_list[brickNumber].powerUp);
            }
        }
        brickCollision(brickNumber - 1);
    }
}

void initBricks(float startx, float starty, float width, float length)
{
    if (rowsOfBricks < 70000)
        rowsOfBricks = 70000;
    if (startx + length > 100000)
    {
        startx = 100;
        initBricks(startx, starty - width - 200, width, length);
        return;
    }
    else if (starty < rowsOfBricks)
        return;
    else
    {
        color brickColor;
        brickColor.R = rand() % 10 + 1;
        brickColor.G = rand() % 10 + 1;
        brickColor.B = rand() % 10 + 1;

        int powerUpChance = rand() % (6*powerUpFrequency) + 1;
        brick b1 = {startx, starty, startx + length, starty - width, false, brickColor, powerUpChance};
        brick_list.push_back(b1);
        initBricks(startx + length + 200, starty, width, length);
    }
}

int drawBricks(int brickNumber, int destroyedBricks)
{
    if (brickNumber < 0)
    {
        return destroyedBricks;
    }
    else if (destroyedBricks == brick_list.size() - 1)
    {
        rowsOfBricks -= 3000;
        resetGame();
    }
    else
    {
        if(brick_list[brickNumber].destroyed == false)
        {
            GLfloat x1 = brick_list[brickNumber].x1;
            GLfloat x2 = brick_list[brickNumber].x2;
            GLfloat y1 = brick_list[brickNumber].y1;
            GLfloat y2 = brick_list[brickNumber].y2;

            glColor3f(1/brick_list[brickNumber].bColor.R, 1/brick_list[brickNumber].bColor.G, 1/brick_list[brickNumber].bColor.B);
            glBegin(GL_POLYGON);
                glVertex2f(x1, y1);
                glVertex2f(x2, y1);
                glVertex2f(x2, y2);
                glVertex2f(x1, y2);
            glEnd();

            if (brick_list[brickNumber].powerUp == 1)
            {
                color* Red = new color{1, 0, 0};
                line* powerUpRed = new line{(x1 + x2)/2, y2 + 1500, 1000, 0, 0, *Red};
                glColor3f(1, 0, 0);
                drawLine(*powerUpRed, false);
                delete powerUpRed;
                delete Red;
            }
            else if (brick_list[brickNumber].powerUp == 2)
            {
                line* powerUpBlue = new line{(x1 + x2)/2, y2 + 1500, 1000, 0, 0, lightBlue};
                drawLine(*powerUpBlue, false);
                delete powerUpBlue;
            }
            else if (brick_list[brickNumber].powerUp == 3)
            {
                color* Green = new color{0, 1, 0};
                line* powerUpGreen = new line{(x1 + x2)/2, y2 + 1500, 1000, 0, 0, *Green};
                drawLine(*powerUpGreen, false);
                delete powerUpGreen;
                delete Green;
            }
            else if (brick_list[brickNumber].powerUp == 4)
            {
                color* Purple = new color{1, 0, 1};
                line* powerUpPurple = new line{(x1 + x2)/2, y2 + 1500, 1000, 0, 0, *Purple};
                drawLine(*powerUpPurple, false);
                delete powerUpPurple;
                delete Purple;
            }
            else if (brick_list[brickNumber].powerUp == 5)
            {
                color* Yellow = new color{1, 1, 0};
                line* powerUpYellow = new line{(x1 + x2)/2, y2 + 1500, 1000, 0, 0, *Yellow};
                drawLine(*powerUpYellow, false);
                delete powerUpYellow;
                delete Yellow;
            }
            else if (brick_list[brickNumber].powerUp == 6)
            {
                color* Black = new color{0, 0, 0};
                line* powerUpBlack = new line{(x1 + x2)/2, y2 + 1500, 1000, 0, 0, *Black};
                drawLine(*powerUpBlack, false);
                delete powerUpBlack;
                delete Black;
            }
        }
        else
            destroyedBricks++;

        drawBricks(brickNumber - 1, destroyedBricks);
    }
}

void drawLine(line toDraw, bool horizontal)
{
    glColor3f(toDraw.lColor.R, toDraw.lColor.G, toDraw.lColor.B);

    if (horizontal)
    {
        glBegin(GL_LINES);

            glVertex2f(toDraw.x, toDraw.y);
            glVertex2f(toDraw.x + toDraw.length, toDraw.y);

        glEnd();
    }
    else
    {
        glBegin(GL_LINES);

            glVertex2f(toDraw.x, toDraw.y);
            glVertex2f(toDraw.x, toDraw.y + toDraw.length);

        glEnd();
    }
}

void displayText(string message, float xpos, float ypos, int charPos)
{
    glColor3f(0.2, 0.5, 01);
    if (charPos < 0)
        return;
    else
    {
        glRasterPos2f(xpos, ypos);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message[charPos]);

        displayText(message, xpos - 1400, ypos, charPos - 1);
    }

}

void displayScore(int value)
{
    //Bricks remaining
    string message = "Bricks: ";
    displayText(message, 11000, 5000, message.length()- 1);

    char remainingBricksChar[3];
    itoa(value, remainingBricksChar, 10);
    string remainingBricksString = remainingBricksChar;

    displayText(remainingBricksString, 13000, 5000, remainingBricksString.length()- 1);

    //Level indicator
    message = "Level: ";
    itoa((100000 - rowsOfBricks)/3000, remainingBricksChar, 10);
    remainingBricksString = remainingBricksChar;
    displayText(message, 11000, 12000, message.length()- 1);
    displayText(remainingBricksString, 12000, 12000, remainingBricksString.length()- 1);
}


int destroyCounter = 0;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(6);


    for (int i = 0; i < ball_list.size(); i++)
    {
        if(ball_list[i].speedy != 0)
            drawLine(ball_list[i], false);
    }

    drawLine(player, true);

    brickCollision(brick_list.size() - 1);

    displayScore(brick_list.size() - drawBricks(brick_list.size() - 1, 0));

    if (powerUpDisplay)
    {
        displayText(powerUpMessage, 60000, 60000, powerUpMessage.length()- 1);
        destroyCounter += 1; //delta timer

        if (destroyCounter > 100)
        {
            powerUpDisplay = false;
            destroyCounter = 0;
        }
    }

    if (!started)
    {
        string startMessage = "Press space to start";
        displayText(startMessage, 62000, 60000, startMessage.length()- 1);
    }

	glFlush();
}

void resetGame()
{
    vector<brick> emptyVector;
    brick_list = emptyVector;
    vector<line> emptyBallVector;
    initBricks(100, 100000, 3000, 4800);
    ball_list = emptyBallVector;
    line ball_temp = {50000, 50000, 1000, rand() % 300 - 300.0, -700, lightBlue};
    ball_list.push_back(ball_temp);
    player = {42500, 10000, 15000, 0, 0, lightBlue};
    playerSpeed = 700;
    started = false;
}

void playerMovement()
{
    if (player.x + player.speedx < -500)
        player.x = -500;
    else if (player.x + player.length + player.speedx > 100500)
        player.x = 100500 - player.length;
    player.x += player.speedx;
}

void ballMovement()
{
    //make it run through a vector
    int numberofBalls = 0;
    for (int i = 0; i < ball_list.size(); i++)
    {
        line* ball = &ball_list[i];

        if (ball->y != -1337)
        {
            if (ball->y + ball->speedy > 0 && ball->y + ball->speedy < 100000)
                ball->y += ball->speedy;
            else if (ball->y + ball->speedy > 100000)
                ball->speedy = -700;

            if (ball->x + ball->speedx > 0 && ball->x + ball->speedx < 100000)
                ball->x += ball->speedx;
            else
                ball->speedx = -ball->speedx;

            if (ball->y < player.y + 200 && ball->y > player.y - 200 && ball->x < player.x + player.length && ball->x > player.x)
            {
                ball->speedy = 700;

                if(ball->x < player.x + 2*player.length/5)
                    ball->speedx -= abs((ball->x + 1000)/player.x * 100);
                else if (ball->x > player.x + 3*player.length/5)
                    ball->speedx += abs((ball->x + 3*player.length/2 + 1000)/player.x * 100);
            }
        }

        if (ball->speedx > 400)
            ball->speedx = 400;
        else if(ball->speedx < -400)
            ball->speedx = -400;

        if (ball_list[i].y + ball_list[i].speedy < 0)
        {
            ball_list[i].y = -1337;
            numberofBalls++;
        }
    }
    if (numberofBalls == ball_list.size())
        resetGame();
    numberofBalls = 0;
}


int updatePos()
{
    playerMovement();
    ballMovement();
    glutPostRedisplay();
    return 0;
}

void update(int value)
{
    if(started)
    {
        updatePos();
        glutTimerFunc(10, update, 0);
    }
}

void keypress(unsigned char key, int x1, int y1)
{
        switch(key)
        {
            case ' ':
                if (!started)
                {
                    cout << "starting" << endl;
                    started = true;
                    update(0);
                }
                break;
            case 'd':
                player.speedx = playerSpeed;
                break;
            case 'a':
                player.speedx = -playerSpeed;
                break;
        }

    glutPostRedisplay();
}

void keyUp(unsigned char key, int x1, int y1)
{
    switch(key)
    {
        case 'd':
            if(player.speedx == -playerSpeed)
                break;
            else
                player.speedx = 0;
            break;
        case 'a':
            if(player.speedx == playerSpeed)
                break;
            else
                player.speedx = 0;
            break;
    }

    glutPostRedisplay();
}

//void createScore(int selection)
//{
//    glutAddMenuEntry("Hello", 0);
//};

int main(int argc,char *argv[])
{
    int tempRows;
    cout << "Rows of bricks (rec. below 10): ";
    cin >> tempRows;
    cout << "Power up frequency (1 is every block, 2 is half the blocks, etc.):";
    cin >> powerUpFrequency;
    rowsOfBricks = 100000 - tempRows*3000;
    resetGame();

//    int resx, resy;
//    cout << "Enter in x resolution: ";
//    cin >> resx;
//    cout << "Enter in y resolution: ";
//    cin >>resy;

	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (1366, 780); //best: 1366, 780
	glutInitWindowPosition (250, 75);
	glutCreateWindow ("Final Assignment - Jason");
	init2D(0.94,0.9,0.55);
//    glutSetMenu(glutCreateMenu(createScore));
	glutKeyboardUpFunc(keyUp);
	glutKeyboardFunc(keypress);
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}
