extern int const WIDTH;
extern int const HEIGHT;

void script3(int &x, int &y)
{
    x = rand() % 200;
    y = rand() % 200;
    bool choice1 = rand() % 2;
    bool choice2 = rand() % 2;
    if(choice1 && choice2)
        x += WIDTH, y += HEIGHT;
    if(!choice1 && choice2)
        x *= -1, y += HEIGHT;
    if(choice1 && !choice2)
        x += WIDTH, y *= -1;
    if(!choice1 && !choice2)
        x *= -1, y *= -1;
}

void script1(int &x, int &y)
{
    int choice = rand() % 4;
    if(choice == 0)
        x = WIDTH + rand() % 300, y = HEIGHT/2 - 16;
    if(choice == 1)
        x = - rand() % 300, y = HEIGHT/2 - 16;
    if(choice == 2)
        x = WIDTH/2 - 16, y = HEIGHT + rand() % 300;
    if(choice == 3)
        x = WIDTH/2 - 16, y = - rand() % 300;
}

void script2(int &x, int &y)
{
    int choice = rand() % 4;
    short temp = rand() % 200;
    if(choice == 0)
    {
        x = -temp;
        y = -temp;
    }
    if(choice == 1)
    {
        x = WIDTH + temp;
        y = -temp;
    }
    if(choice == 2)
    {
        x = WIDTH + temp;
        y = HEIGHT + temp;
    }
    if(choice == 3)
    {
        x = -temp;
        y = HEIGHT + temp;
    }
}

bool check_collision(SDL_Rect A, SDL_Rect B, double aAngle, double bAngle)
{

    int leftB, leftA, rightA, rightB, topA, topB, bottomA, bottomB;

    leftA = A.x;///* cos(aAngle);
    rightA = (A.x + A.w);/// * cos(aAngle);
    topA = A.y;/// * sin(aAngle);
    bottomA = (A.y + A.h);///*sin(aAngle);

    leftB = B.x;/// * cos(bAngle);
    rightB = (B.x + B.w);///*cos(bAngle);
    topB = B.y;/// * sin(bAngle);
    bottomB = (B.y + B.h);/// * sin(bAngle);

    if(bottomA <= topB)
        return false;
    if(topA >= bottomB)
        return false;
    if(rightA <= leftB)
        return false;
    if(leftA >= rightB )
        return false;
    return true;

}

void send(SDL_Rect &object, int pulse, double angle)
{
    object.x += pulse * cos(angle);
    object.y += pulse * sin(angle);
}

void toValue(double &x, double value)
{
    if(x > value)
        x--;
    else if(x < value)
        x++;
}
