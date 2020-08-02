/**
  * Example project using the neat.h library
  * Copywrite (c) Algorithm X, algortithmx51 - 1/08/2020
  *
  * Flappy Bird using NEAT Implementation project (c) Algorithm X
  *
  **/

#include"neat.h"

#define speed -2 // the speed of the birds
#define pipes 4 // the number of the pipes
#define SIZE_OF_PIPE 40 // the size of the pipes
#define POPULATION 15 // the number of the birds on each generation

class Bird; // define the class Bird
class Pipe; // define the class Pipe

void think(network **);
void handle_user_input(char);

int gen = 0; // Start at gen 0
float gravity = 0.3; // the gravity
int drawall = 0; // if to draw all the birds

Pipe *p ; // define a list of pipes
Bird *b ; // define a list of birds
network **brain; // define a list of pointers to brains (NW)


class Pipe // Class Pipe
{

public:

    int length; // the length of the pipe
    int free_space; //  the free space

    int x; // the x coord of the pipes

    void init(int, int, int); // function to initialize the pipes
    void move(void); // function to move the pipes
    void draw(int); // function to draw the pipes



};

void Pipe::init(int _x, int _length, int _free_space) // initialize the pipes
{

    this->x = _x;
    this->length = _length;
    this->free_space = _free_space;

    return;
}

void Pipe::move(void) // move the pipes
{

    this->x += speed;

    return;

}

void Pipe::draw(int color) // draw the pipes with color "color"
{

    setcolor(color);
    setfillstyle(SOLID_FILL, color);

    bar(this->x, getmaxy() - this->length, x + SIZE_OF_PIPE, getmaxy()); // #1 rectangle

    bar(this->x, 0, this->x + SIZE_OF_PIPE, getmaxy() - this->length - this->free_space); // #2 rectangle


    return;

}

class Bird // Class Bird
{


public:

    int x, y; // the bird's x and y
    float vel_y; // it's velocity
    int dead; // if it is dead
    float score; // the score

    void init(int, int); // function to initialize the bird
    void draw(int); // function to draw the bird
    void control(char); // function to control the bird
    void tick(void); // function to handle the physics of the bird
    int checkhitbox(Pipe *); // function to check the hitbox the bird

};

void Bird::init(int _x, int _y) // initialize the bird
{

    this->x = _x;
    this->y = _y;
    this->vel_y = 0;
    this->dead = 0;
    this->score = 0;

    return;
}

void Bird::draw(int color)// draw the bird with color "color"
{

    setcolor(color);
    circle(this->x, this->y, 10);

    setfillstyle(SOLID_FILL, color);
    floodfill(this->x, this->y, color);
    putpixel(this->x+6, this->y-3, BLACK);
    putpixel(this->x+5, this->y-3, BLACK);
    putpixel(this->x+5, this->y-4, BLACK);
    putpixel(this->x+6, this->y-4, BLACK);

    setcolor(RGB(255, 165, 0));
    line(this->x+9, this->y-3, this->x+16, this->y);
    line(this->x+16, this->y, this->x+9, this->y+3);
    line( this->x+9, this->y+3, this->x+9, this->y-3);
    setfillstyle(SOLID_FILL, RGB(255, 165, 0));
    floodfill(this->x+10, this->y, RGB(255, 165, 0));


    return;
}

void Bird::control(char ch) // handle the controls of the bird - OLD used by the single player game version
{

    switch(ch)
    {

    case ' ':
        this->vel_y = 5;
        break;
    default:
        break;
    }

    return;
}

void Bird::tick(void) // handle the physics of the bird
{

    int index;
    int min = getmaxx();

    this->y -= this->vel_y;
    this->vel_y -= gravity;
    this->score += 0.01;

    for(int i = 0; i < pipes; i++) // find the closest pipe
    {
        if(p[i].x < min && p[i].x >= 100-SIZE_OF_PIPE-SIZE_OF_PIPE/4)
        {
            index = i;
            min = p[i].x;
        }
    }

    if(this->checkhitbox(&p[index]) || this->y > getmaxy() || this->y < 0) // check the hitbox
    {
        this->dead = 1;
    }

    return;

}

int Bird::checkhitbox(Pipe * P) // check the hitbox of the bird
{

    // printf("Status : x1 %d x2 %d y1 %d y2 %d | x1 %d x2 %d y1 %d y2 %d\n", this->x1, this->x2, this->y1, this->y2, box2.x1, box2.x2, box2.y1, box2.y2);

    int x1, x2, y1, y2; // Bird
    int x3, x4, y3, y4, x5, x6, y5, y6; // Pipes

    x1 = this->x - 10; // Bird Hitbox
    x2 = this->x + 10;
    y1 = this->y - 10;
    y2 = this->y + 10;

    x3 = P->x; // Pipe #1 Hitbox
    x4 = P->x + SIZE_OF_PIPE;
    y3 = 0;
    y4 = getmaxy() - P->length - P->free_space;

    x5 = P->x; // Pipe #2 Hitbox
    x6 = P->x + SIZE_OF_PIPE;
    y5 = getmaxy() - (P->length);
    y6 = getmaxy();


//   setcolor(RED);
//   rectangle(x1, y1, x2, y2);
//   rectangle(x3, y3, x4, y4);
//   rectangle(x5, y5, x6, y6);

    int colliding = 0;

    if(y2 < y3 || y4 < y1)
    {
        colliding = 0;
    }
    else if(x2 < x3 || x4 < x1)
    {
        colliding = 0;
    }
    else
        colliding = 1;

    if(colliding)
        return colliding;

    if(y2 < y5 || y6 < y1)
    {
        colliding = 0;
    }
    else if(x2 < x5 || x6 < x1)
    {
        colliding = 0;
    }
    else
        colliding = 1;

    if(colliding)
        return colliding;

    return colliding;

}

void think(network **BRAIN) // function to decide the movement of each bird
{

    int index = 0;
    int min = getmaxx();

    for(int i = 0; i < pipes; i++) // find the closest pipe
    {
        if(p[i].x < min && p[i].x >= 100-SIZE_OF_PIPE-SIZE_OF_PIPE/4)
        {
            index = i;
            min = p[i].x;
        }
    }

    float INP[3] = {0, 0, 0}; // a tmp list of floats

    for(int i = 0; i < POPULATION; i++) // loop through the population
    {
        if(b[i].dead == 0) // if that bird is not dead
        {

            INP[0] = (float) min / 1000; // load the variables
            INP[1] = b[i].y - (getmaxy() - p[index].length - p[index].free_space);       /// Brain structure :
            INP[1] /= 1000;                                                              /// Input layer : 1. distance from the closest pipe , 2. distance on the y from the upper part of the closest pipe and 3. distance on the y from the lower part of the closest pipe
            INP[2] = b[i].y - (getmaxy() - p[index].length);                             /// Hidden layers : None
            INP[2] /= 1000;                                                              /// Output layer : 1. Jump and 2. Do not jump

            setNet(BRAIN[i], INP); // set the input
            feedforward(BRAIN[i]); // feed the net

            if(BRAIN[i]->l[BRAIN[i]->layers-1].n[0].output > BRAIN[i]->l[BRAIN[i]->layers-1].n[1].output) // process the output
                b[i].control(' ');

            b[i].tick();
        }
    }

}

void handle_user_input(char ch) // function to handle user input
{
    int index, max = 0;
    FILE *file;

    switch(ch)
    {
    case 'q':
        exit(0);
        break;

    case 'p': // pause
        while(!kbhit()) {;}
        break;

    case 's': // save the state of the neural network

        for(int i = 0; i < POPULATION; i++) // find the best player
        {
            if(max < b[i].score)
            {
                max = b[i].score;
                index = i;
            }
        }

        file = fopen("BESTNW.bin", "wb"); // open "BESTNW.bin" to write , or best-neural-network
        exportNet(brain[index], file); // export it
        fclose(file);
        exit(0);
        break;

    case 'a':
        drawall = 1 - drawall;
        break;

    default:
        break;
    }

}

int main(void) /// MAIN
{

    int mut;

    printf("Welcome to Flappy Bird AI v2\n");
    printf("Enter the mutation rate of your birds (0 -> 100) : ");
    scanf("%d", &mut); // read the mutation rate
    printf("Controls : q -> quit the program , p -> pause, s -> save neural network, a -> switch display-all state \n");

    system("pause");

    srand((unsigned) time((time_t*)NULL)); // initialize the random number generator

    p = (Pipe *) malloc ( sizeof (Pipe) * pipes ); // allocate enough bytes for "pipes" number of pipes
    b = (Bird *) malloc ( sizeof (Bird) * POPULATION ); // allocate enough bytes for "POPULATION" birds
    brain  = (network **) malloc ( sizeof (network *) * POPULATION); // allocate enough bytes to store "POPULATION" number of pointers to neural networks

    int w1, w2, nmap[] = {3,2}; // w1, w2 : integers to store the ids of the windows that will be created, nmap : network map -> how many neurons on each layer

    for(int i = 0; i < POPULATION; i++) // create "POPULATION" neural networks to work with
        brain[i] = confNet(2, nmap);

    FILE *loadNW = fopen("BESTNW.bin", "rb");

    if(loadNW != NULL)
    {
        char ch;
        scanf("%c", &ch); // through the \n away

        printf("Save file found, do you want to load it ? (Y/N) : ");
        scanf("%c", &ch);

        if(ch == 'y' || ch == 'Y')
            for(int i = 0; i < POPULATION; i++)
            {
                brain[i] = importNet(loadNW);
                if(i != 0)
                    mutateNet(brain[i], 1, mut);
            }

        fclose(loadNW);
    }

    printf("Staring Now\n");
    delay(500); // delay of 500 ms

    system("cls"); // clear the screen

    w2 = initwindow(600, 400, "Neural Network of the Best Player", 800, 0); // create a window to display the "brain" of the best player
    w1 = initwindow(800, 600, "Flappy Bird AI", 0, 0); // create a window to display the game

    setcurrentwindow(w1); // draw on the "game" window

    for(int i = 0; i < POPULATION; i++) // initialize all the player's classes
        b[i].init(100, 100);

    int page = 0; // current drawing page
    int dead = 0; // if our birds are dead or not
    int index = 0; // an index variable that will be used as a reference to the best player
    float BEST = 0; // the BEST score
    char ch; // a char variable to handle user input

    unsigned long int current_time, start_time;

    while(1)
    {

        printf("At generation %d\n", gen);


        dead = 0; // we start over, so our bird are alive

        for(int i = 0; i < pipes; i++) // render the pipes
        {
            p[i].init(getmaxx() + i * (800 / pipes), rand() % (getmaxy() - 230), rand() % 100 + 120);
        }

        start_time = clock();

        while(!dead)
        {
            setactivepage(page); // swapbuffers implementation
            setvisualpage(1 - page);

            current_time = clock();

            if(current_time - start_time < 13) // limit at 13 msec.
            {
                delay(current_time - start_time);
                start_time = current_time;
            }
            else
                start_time = current_time;


            BEST = 0; // initialize every round the best score

            if(kbhit()) // if we have user input
            {
                ch = getch(); // read it
                handle_user_input(ch); // and process it
            }

            cleardevice(); // clear the screen

            for(int i = 0; i < pipes; i++) // draw the pipes
            {
                p[i].draw(GREEN);
            }

            for(int i = 0; i < pipes; i++) // check if a pipe is off the screen and move every pipe
            {
                if(p[i].x < -SIZE_OF_PIPE)
                    p[i].init(getmaxx(), rand() % (getmaxy() - 230), rand() % 100 + 120);

                p[i].move();
            }

            think(brain); // THINK

            dead = 1; // say that every bird is dead

            for(int i = 0; i < POPULATION; i++) // scan the POPULATION
            {
                if(!b[i].dead)  // if we find an alive bird
                {
                    dead = 0; // the birds are alive

                    if(drawall == 1)
                        b[i].draw(YELLOW);

                    if(b[i].score > BEST) // find the bird with the best score
                    {
                    BEST = b[i].score;
                    index = i;
                    }
                }
            }

            drawNet(brain[index], w2, 0); // display that network

            if(!drawall)
            {
                b[index].draw(YELLOW); // draw the best player
            }

            if(b[0].dead == 0 && gen > 0)
                b[0].draw(BLUE); // draw the old best

            char buffer[100];
            sprintf(buffer, "Score : %f", b[index].score); // print the score on the screen
            setcolor(YELLOW);
            outtextxy(10, 10, buffer);
            sprintf(buffer, "Average weight of the best network : %f", calcAvgWeight(brain[index]));
            setcolor(RGB(255, 165, 0));
            outtextxy(10, 30, buffer);

            page = 1 - page; // we change the page
        }

        /// Our birds died

     //   printf("Index : %d, b[index].x = %d, b[index].y = %d\n", index, b[index].x, b[index].y);
     //   b[index].draw(BLUE);
     //   getch();

        if(index != 0)
        {
            free(brain[0]); // free the first slot of the list of the "brains"
            brain[0] = copyNet(brain[index]); // copy the best player's net to the first slot
        }

       //  free(b); // free the bird's array

       //  b = (Bird *) malloc ( sizeof (Bird) * POPULATION ); // allocate enough bytes for "POPULATION" birds

        b[0].init(100, 100); // initialize it's bird

        for(int i = 1; i < POPULATION; i++) // copy the best brain to all the slots and mutate the a little bit
        {
            int r = rand() % 2; // decide if we will mutate them slightly or a lot

            if(r == 1) // if we will mutate them a lot
                if(rand() % 100 - 60 > 0) // there is a 60% of this changing to normal mutation
                    r = 0;

            free(brain[i]); // free that slot
            brain[i] = copyNet(brain[0]); // copy the best brain
            mutateNet(brain[i], r, mut); // mutate it

            b[i].init(100, 100); // and initialize it's bird
        }

        gen++; // change generation
        printf("Generation %d ended\n", gen-1);

    }


    getch();
    return 0;
}

