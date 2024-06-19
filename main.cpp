#include <SFML/Graphics.hpp>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;

void forceForegroundWindow(HWND hwnd) {
    DWORD foregroundThreadId = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    DWORD thisThreadId = GetCurrentThreadId();

    AttachThreadInput(thisThreadId, foregroundThreadId, TRUE);

    SetForegroundWindow(hwnd);
    BringWindowToTop(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    AttachThreadInput(thisThreadId, foregroundThreadId, FALSE);
}

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

float psize=50;
float px=100, py=screenHeight/2-psize/2;
float pv=0;
int pipeSize = 80;
int pipeHole = 120;
int tps=60;

void checkEvents(Event event, RenderWindow& window) {
    if(event.type == Event::KeyPressed) {
        if(event.key.code == Keyboard::W) {
            pv -= 5;
            if(pv<-12) pv = -12;
        }
        if(event.key.code == Keyboard::Up){
            pv -= 5;
            if(pv<-12) pv = -12;
        }
    }
    if(event.type == Event::Resized) window.setSize(Vector2u(psize, psize));
    if(event.type == Event::LostFocus) forceForegroundWindow(window.getSystemHandle());
}



int main() {
    srand(time(0));
    rand();

    int pipesX = screenWidth, pipesY = 100 + (rand() % (screenHeight - 200));

    RenderWindow bird(VideoMode(psize, psize), "Bird", Style::None);
    RenderWindow *pipe1 = new RenderWindow(VideoMode(pipeSize, pipesY), "Pipe 1", Style::None);
    RenderWindow *pipe2 = new RenderWindow(VideoMode(pipeSize, screenHeight-pipesY-pipeHole), "Pipe 2", Style::None);

    bird.setPosition(Vector2i(px, py));
    pipe1->setPosition(Vector2i(pipesX, 0));
    pipe2->setPosition(Vector2i(pipesX, screenHeight - pipesY - pipeHole));

    Clock clock;

    while(bird.isOpen()) {


        Event event;
        while (bird.pollEvent(event)) {
            checkEvents(event, bird);
        }
        while(pipe1->pollEvent(event)) {
            checkEvents(event, *pipe1);
        }
        while(pipe2->pollEvent(event)) {
            checkEvents(event, *pipe2);
        }

        if(1.0/tps <= clock.getElapsedTime().asSeconds()) {
            py+=pv;
            bird.setPosition(Vector2i(px, py));
            pv+=-pv/100.0+0.3;

            pipesX-=4;
            pipe1->setPosition(Vector2i (pipesX, 0));
            pipe2->setPosition(Vector2i(pipesX, pipesY + pipeHole));

            if(pipesX+pipeSize < 0) {
                pipesX = screenWidth;
                pipesY = 100 + (rand() % (screenHeight - 200));
                delete pipe1;
                pipe1 = new RenderWindow(VideoMode(pipeSize, pipesY), "Pipe 1", Style::None);;
                delete pipe2;
                pipe2 = new RenderWindow(VideoMode(pipeSize, screenHeight-pipesY-pipeHole), "Pipe 2", Style::None);
            }

            if(px+psize >= pipesX && px+psize <= pipesX+pipeSize)
                if(py <= pipesY || py >= pipesY+pipeHole)
                    bird.close();


            clock.restart();
        }


        bird.clear(Color::Yellow);
        bird.display();
        pipe1->clear(Color::Green);
        pipe1->display();
        pipe2->clear(Color::Green);
        pipe2->display();

        if(py<0) {
            py = 0;
            pv = 0;
        }
        // 40 cause of windows bottom panel
        if(py+psize+40 > screenHeight) {
            py = screenHeight-psize-40;
            pv = 0;
        }

    }

    return 0;
}
