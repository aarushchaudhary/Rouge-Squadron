#include <iostream>
#include <conio.h>
#include <dos.h>
#include <windows.h> #include <time.h> #define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define Enemy_DIF 45

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyX[3];
int enemyY[3];
int enemyFlag[3];
char bird[3][5] = { ' ',' ','±',' ',' ',
                    '|','±','±','±','|',
                    '±','±','±','±','±'};
int birdsPos = WIN_WIDTH/2;
int score = 0;
int bullets[20][4];
int bulletsLife[20];
int bIndex = 0;

void gotoxy(int x, int y){
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}
void setcursor(bool visible, DWORD size){
        if (size == 0)
            size = 20;

        CONSOLE_CURSOR_INFO lpCursor;
        lpCursor.bVisible = visible;
        lpCursor.dwSize = size;
        SetConsoleCursorInfo(console,&lpCursor);
}
void drawBorder(){

    for(int i=0; i<90; i++){
        gotoxy(i,SCREEN_HEIGHT); cout << "±";
    }

    for(int i=0; i<SCREEN_HEIGHT; i++){
        gotoxy(0,i); cout << "±";
        gotoxy(90,i); cout << "±";
    }

    for(int i=0; i<SCREEN_HEIGHT; i++){
        gotoxy(WIN_WIDTH,i); cout << "±";
    }
}
void genEnemy(int ind){
    enemyX[ind] = 3 + rand()%(WIN_WIDTH-10);
}
void drawEnemy(int ind){
    if (enemyFlag[ind] == true){
        gotoxy(enemyX[ind], enemyY[ind]);   cout << ".**.";
        gotoxy(enemyX[ind], enemyY[ind]+1); cout << "****";
        gotoxy(enemyX[ind], enemyY[ind]+2); cout << "****";
        gotoxy(enemyX[ind], enemyY[ind]+3); cout << ".**.";
    }
}
void eraseEnemy(int ind){
    if(enemyFlag[ind] == true){
        gotoxy(enemyX[ind], enemyY[ind]);   cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind]+1); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind]+2); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind]+3); cout << "    ";
    }
}
void resetEnemy(int ind){
    eraseEnemy(ind);
    enemyY[ind] = 4;
    genEnemy(ind);
}
void genBullet(){
    bullets[bIndex][0] = 22;
    bullets[bIndex][1] = birdsPos;
    bullets[bIndex][2] = 22;
    bullets[bIndex][3] = birdsPos+4;
    bIndex++;
    if(bIndex == 20)
        bIndex = 0;
}
void moveBullet(){
    for(int i=0; i<20; i++){
        if( bullets[i][0] > 2 )
            bullets[i][0]--;
        else
            bullets[i][0] = 0;

        if( bullets[i][2] > 2 )
            bullets[i][2]--;
        else
            bullets[i][2] = 0;
    }
}
void drawBullets(){
    for(int i=0; i<20; i++){
        if( bullets[i][0] > 1 ){
            gotoxy(bullets[i][1],bullets[i][0]); cout << ".";
            gotoxy(bullets[i][3],bullets[i][2]); cout << ".";
        }
    }
}
void eraseBullets(){
    for(int i=0; i<20; i++){
        if( bullets[i][0] >=1 ){
            gotoxy(bullets[i][1],bullets[i][0]); cout << " ";
            gotoxy(bullets[i][3],bullets[i][2]); cout << " ";
        }
    }
}
void eraseBullet(int i){
    gotoxy(bullets[i][1],bullets[i][0]); cout << " ";
    gotoxy(bullets[i][3],bullets[i][2]); cout << " ";
}
void drawBird(){
    for(int i=0; i<3; i++){
        for(int j=0; j<5; j++){
            gotoxy(j+birdsPos, i+22); cout << bird[i][j];
        }
    }
}
void eraseBird(){
    for(int i=0; i<3; i++){
        for(int j=0; j<5; j++){
            gotoxy(j+birdsPos, i+22); cout << " ";
        }
    }
}

int collision(){
    if( enemyY[0]+4 >= 23 ){
        if( enemyX[0] + 4 - birdsPos <= 0 && enemyX[0] + 4 - birdsPos < 8 ){
            return 1;
        }
    }
    return 0;
}
int bulletHit(){
    for(int i=0; i<20; i++){
        for(int j=0; j<4; j+=2){
            if( bullets[i][j] != 0){
                if( bullets[i][j] >= enemyY[0] && bullets[i][j] <= enemyY[0]+4 ){
                    if( bullets[i][j+1] >= enemyY[0] && bullets[i][j+1] <= enemyY[0]+4 ){
                        eraseBullet(i);
                        bullets[i][j] = 0;
                        resetEnemy(0);
                        return 1;
                }
            }
            if( bullets[i][j] >= enemyY[0] && bullets[i][j] <= enemyY[0]+4 ){
                    if( bullets[i][j+1] >= enemyY[0] && bullets[i][j+1] <= enemyY[0]+4 ){
                        eraseBullet(i);
                        resetEnemy(1);
                        bullets[i][j] = 0;
                        return 1;
                    }
            }
        }
    }
}
    return 0;
}
void gameover(){
    system("cls");
    cout << endl;
    cout << "\t\t------------------------------------------" << endl;
    cout << "\t\t---YOU HAVE FAILED ME FOR THE LAST TIME---" << endl;
    cout << "\t\t------------------------------------------" << endl << endl;
    cout << "\t\tPress any key to go back to main menu.";
    getch();
}
void updateScore(){
    gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score << endl;
}

void instructions(){

    system("cls");
    cout << "Instructions";
    cout << "\n----------------";
    cout << "\n Press spacebar to make you X-Wing fly";
    cout << "\n Use A for left move and D for right move";
    cout << "\n Press any key to go back to main menu";
    getch();
}

void play(){

    birdsPos = -1 + WIN_WIDTH/2;
    score = 0;
    enemyFlag[0] = 1;
    enemyFlag[1] = 1;
    enemyY[0] = enemyY[1] = 4;

    for(int i=0; i<20; i++){
        bullets[i][0] = bullets[i][1] = 0;
    }

    system("cls");
    drawBorder();
    genEnemy(0);
    genEnemy(1);
    updateScore();

    gotoxy(WIN_WIDTH + 5, 2); cout << "Rouge Squadron";
    gotoxy(WIN_WIDTH + 6, 4); cout << "--------------";
    gotoxy(WIN_WIDTH + 6, 6); cout << "--------------";
    gotoxy(WIN_WIDTH + 7, 12); cout << "Controls";
    gotoxy(WIN_WIDTH + 7, 13); cout << "--------";
    gotoxy(WIN_WIDTH + 2, 14); cout << "A key - Left";
    gotoxy(WIN_WIDTH + 2, 15); cout << "D key - Right";
    gotoxy(WIN_WIDTH + 2, 16); cout << "Spacebar = Blast";

    gotoxy(10, 5); cout << "Press any key to start";
    getch();
    gotoxy(10, 5); cout <<"                         ";

    while(1){
        if(kbhit()){
            char ch = getch();
            if( ch=='a' || ch=='A' ){
                if( birdsPos > 2 )
                    birdsPos -= 2;
            }
            if( ch=='d' || ch=='D' ){
                if( birdsPos < WIN_WIDTH-7 )
                    birdsPos += 2;
            }
            if(ch==32){
                genBullet();
            }
            if(ch==27){
                break;
            }
        }

        drawBird();
        drawEnemy(0);
        drawEnemy(1);
        drawBullets();
        if( collision() == 1 ){
            gameover();
            return;
        }
        if( bulletHit() == 1){
            score++;
            updateScore();
        }
        Sleep(200);
        eraseBird();
        eraseEnemy(0);
        eraseEnemy(1);
        eraseBullets();
        moveBullet();

        if( enemyFlag[0] == 1 )
            enemyY[0] += 1;

        if( enemyFlag[1] == 1 )
            enemyY[1] += 1;

        if( enemyY[0] > SCREEN_HEIGHT-5 ){
            resetEnemy(0);
        }
        if( enemyY[1] > SCREEN_HEIGHT-5 ){
            resetEnemy(1);
        }
    }
}

int main()
{
    setcursor(0,0);

    do{
        system("cls");
        gotoxy(10,5); cout << "-*-*LASTSECRET987's*-*-";
        gotoxy(10,6); cout << "|    Rouge  Squadron  |";
        gotoxy(10,7); cout << "-*-Version Alpha 0.1-*-";
        gotoxy(10,9); cout << "1. Start Game";
        gotoxy(10,10); cout << "2. Instructions";
        gotoxy(10,11); cout << "3. Quit";
        gotoxy(10,13); cout << "Select an option: ";
        char op = getche();

        if( op=='1' ) play();
        else if( op=='2' ) instructions();
        else if( op=='3' ) exit(0);

    }while(1);

    return 0;
}
