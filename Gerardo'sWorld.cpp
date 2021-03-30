#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>
using namespace std;

wstring block[8];
int nFieldWidth = 120;
int nFieldHeight = 50;

int nScreenWidth = 160;
int nScreenHeight = 120;

int nOffSet = 2;

unsigned char* pField = nullptr;

wstring character[1];

int Orientation(int px, int py)
{
    return py * 4 + px;
}

bool DoesPieceFit(int nBlock, int nPosX, int nPosY)
{
    for (int px = 0; px < 4; px++)
        for (int py = 0; py < 4; py++)
        {
            // Get index into piece
            int pi = Orientation(px, py);

            // Get index into field
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

            if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
            {
                if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                {
                    if (block[nBlock][pi] == L'X' && pField[fi] != 0)
                        return false;
                }
            }
        }
    return true;
}




///////////////////////////////
// CHARACTER POSITION CHECKS //
bool DoesCharFit(int nPosX, int nPosY)
{
    for (int px = 0; px < 4; px++)
        for (int py = 0; py < 4; py++)
        {
            // Get index into piece
            int pi = Orientation(px, py);

            // Get index into field
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

            if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
            {
                if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                {
                    if (character[0][pi] == L'X' && pField[fi] != 0)
                        return false;
                }
            }
        }
    return true;
}
bool CanCharJump(int nPosX, int nPosY)
{
    if (pField[(nPosY + 1 + nOffSet) * nFieldWidth + nPosX + nOffSet] != 0)
    {
        return true;
    }
    return false;
}
/////////////////////////////


int main()
{
 
    character[0].append(L"....");
    character[0].append(L"....");
    character[0].append(L"..X.");
    character[0].append(L"....");

     
    block[0].append(L"....");
    block[0].append(L"....");
    block[0].append(L"..X.");
    block[0].append(L"....");
   

    block[1].append(L"....");
    block[1].append(L"..X.");
    block[1].append(L".X..");
    block[1].append(L"....");
    

    block[2].append(L"....");
    block[2].append(L"....");
    block[2].append(L"..X.");
    block[2].append(L"...X");
    

    block[3].append(L"....");
    block[3].append(L"....");
    block[3].append(L"....");
    block[3].append(L"XXXX");
    

    block[4].append(L"....");
    block[4].append(L"....");
    block[4].append(L".X..");
    block[4].append(L"XX..");
    

    block[5].append(L"....");
    block[5].append(L"....");
    block[5].append(L".XX.");
    block[5].append(L"XXXX");
    

    block[6].append(L"....");
    block[6].append(L"....");
    block[6].append(L"..X.");
    block[6].append(L".XXX");

    block[7].append(L"....");
    block[7].append(L"....");
    block[7].append(L".X..");
    block[7].append(L".XX.");

     

    pField = new unsigned char[nFieldWidth * nFieldHeight];
    for (int x = 0; x < nFieldWidth; x++)
        for (int y = 0; y < nFieldHeight; y++)
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;


    ///////////////////////////////////////////////////////
    // Game Logic Stuff////////////////////////////////////
    // Piece Login    /////////////////////////////////////
    int nCurrentPiece = rand() % 5 + 3;
    int nCurrentX = (rand() % nFieldWidth) * .95 + 1;
    int nCurrentY = 1;

    int nCurrentPiece1 = rand() % 4;
    int nCurrentX1 = (rand() % nFieldWidth) * .95 + 1;
    int nCurrentY1 = 1;
    ///////////////////////////////////////////////////////
    // Character Logic    /////////////////////////////////
    int nCurrentCharacter = 0;
    int nCurrentCharX = nFieldWidth / 2;
    int nCurrentCharY = 5;
    ///////////////////////////////////////////////////////
    bool bKey[4];


    ///////////////////////////////////////////////////////
    // Block Mechanisms////////////////////////////////////
    int nSpeed = 2;              // Block 1
    int nSpeedCounter = 0;
    bool bForceDown = false;

    int nSpeed1 = 1;             // Block 2
    int nSpeedCounter1 = 0;
    bool bForceDown1 = false;

    ///////////////////////////////////////////////////////
    // Player Mechanisms
    bool bForcePlayerDown = false;
    int nFallSpeed = 2;
    int nFallSpeedCounter = 0;
    int nJumpHeight = 3;
    ///////////////////////////////////////////////////////



    bool bGameOver = false;
    bool bKeyHeld = false;

    while (!bGameOver)
    {   
        // GAME TIMING
        this_thread::sleep_for(60ms);
        nSpeedCounter++;
        nSpeedCounter1++;
        bForceDown = (nSpeedCounter == nSpeed);
        bForceDown1 = (nSpeedCounter1 == nSpeed1);

        ///////Player Game Timing
        nFallSpeedCounter++;
        bForcePlayerDown = (nFallSpeedCounter >= nFallSpeed);
        // INPUT
        for (int k = 0; k < 4; k++)
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;


        // GAME LOGIC



        if (bKey[1])
        {
            if (DoesCharFit(nCurrentCharX - 1, nCurrentCharY))
            {
                nCurrentCharX = nCurrentCharX - 1;
            }
        }

        if (bKey[0])
        {
            if (DoesCharFit(nCurrentCharX + 1 , nCurrentCharY))
            {
                nCurrentCharX = nCurrentCharX + 1;
            }
        }

        if (bKey[3])
        {   
                if (!bKeyHeld && CanCharJump(nCurrentCharX, nCurrentCharY) == true)
                {
                    nCurrentCharY = nCurrentCharY - 1;
                    nCurrentCharY = nCurrentCharY - 1;
                    bKeyHeld = true;
                }
        }
        else
            bKeyHeld = false;


        if (bForceDown)
        {
            if (DoesPieceFit(nCurrentPiece, nCurrentX, nCurrentY + 1))
                nCurrentY++;
            else
            {
                // Lock Current Piece
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if (block[nCurrentPiece][Orientation(px, py)] == L'X')
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;
                // Choose next Piece
                nCurrentX = (rand() % nFieldWidth) * .95 + 1;
                nCurrentY = 1;
                nCurrentPiece = rand() % 5 + 3;

            }
            nSpeedCounter = 0;
        }

        if (bForceDown1)
        {
            if (DoesPieceFit(nCurrentPiece1, nCurrentX1, nCurrentY1 + 1))
                nCurrentY1++;
            else
            {
                // Lock Current Piece
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if (block[nCurrentPiece1][Orientation(px, py)] == L'X')
                            pField[(nCurrentY1 + py) * nFieldWidth + (nCurrentX1 + px)] = nCurrentPiece1 + 1;
                // Choose next Piece
                nCurrentX1 = (rand() % nFieldWidth) * .95 + 1;
                nCurrentY1 = 1;
                nCurrentPiece1 = rand() % 3;

            }
            nSpeedCounter1 = 0;
        }

        if (bForcePlayerDown)
        {
            if (DoesCharFit(nCurrentCharX, nCurrentCharY + 1))
                nCurrentCharY++;
            nFallSpeedCounter = 0;
        }


        // RENDER OUTPUT

        // Draw Field
        for (int x = 0; x < nFieldWidth; x++)
            for (int y = 0; y < nFieldHeight; y++)
                screen[(y + nOffSet) * nScreenWidth + (x + nOffSet)] = L" #########"[pField[y * nFieldWidth + x]];

        // Draw Current Piece
        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                if (block[nCurrentPiece][Orientation(px, py)] == L'X')
                    screen[(nCurrentY + py + nOffSet) * (nScreenWidth) + (nCurrentX + px + nOffSet)] = 88;

        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                if (block[nCurrentPiece1][Orientation(px, py)] == L'X')
                    screen[(nCurrentY1 + py + nOffSet) * (nScreenWidth) + (nCurrentX1 + px + nOffSet)] = 88;

        // Draw Character
    
        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                if (character[0][Orientation(px, py)] == L'X')
                    screen[(nCurrentCharY + py + nOffSet) * nScreenWidth + (nCurrentCharX + px + nOffSet)] = 65;

        // Display Frame
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
    return 0;
}
