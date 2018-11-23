#include <Windows.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <string>

using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point
{int x,y,x2,y2;} currentFigure[4], oldFigure[4],nextFigure[4];
int m=0;
int scores=0;

int figures[7][4] =
{
	1,3,5,7, // I	Столбик
	2,4,5,7, // Z	Молния2
	3,5,4,6, // S	Молния
	3,5,4,7, // T	Т - образна
	2,3,5,7, // L	L2 - образна
	3,5,7,6, // J	L - образна
	2,3,4,5, // O	Квадрат
};

bool check()
{
   for ( int i=0; i<4; i++ )
	if ( currentFigure[ i ].x<0 || currentFigure[ i ].x >= N || currentFigure[ i ].y >= M ) 
		return 0;
    else if ( field[currentFigure[i].y ][ currentFigure[i].x ] ) 
		return 0;
   return 1;
};

// Check if user lost
bool stopGame() {
  int j;
  for (j = 0; j < N; j++) {
    if(field[1][j] != 0) {
      return true;
    }
  }
  return false;
}

// Start from begging
void resetGame() {
  int i, j;
  for(i = 0; i < M; i++) {
    for(j = 0; j < N; j++) {
      field[i][j] = 0;
    }
  }
}


void game()
{
	
	srand(time(0));	 

	RenderWindow window(VideoMode(320, 480), "Tetris game");

    Texture figureTexture, gameBck, gameFrame, nextTextured;
    
	figureTexture.loadFromFile("images/tiles.png");
	nextTextured.loadFromFile("images/tiles.png");
	gameBck.loadFromFile("images/background.png");
	gameFrame.loadFromFile("images/frame.png");
	
	Font font;
	font.loadFromFile("fonts/Pacifico.ttf");

	/* Set up Text for losing */
	Text end_message;
	Text restart_message;
	
	end_message.setFont(font);
	restart_message.setFont(font);
	
	end_message.setString("GAME OVER!");
	restart_message.setString("Press r to restart");
	
	
	end_message.setCharacterSize(30);
	restart_message.setCharacterSize(20);
	
	end_message.setColor(Color::Blue);
	restart_message.setColor(Color::Magenta);
	
	end_message.setStyle(Text::Bold);


	Sprite Figure(figureTexture), background(gameBck), frame(gameFrame), nextFigures(nextTextured);

    int dx=0; bool rotate=0; int colorNum=1;
	float timer=0,delay=0.3; 

	Clock clock;

    while (window.isOpen())
    {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer+=time;

        Event gameWindow;
        while (window.pollEvent(gameWindow))
        {
            if (gameWindow.type == Event::Closed)
                window.close();

			if (gameWindow.type == Event::KeyPressed)
			  if (gameWindow.key.code==Keyboard::Up) rotate=true;
			  else if (gameWindow.key.code==Keyboard::Left) dx=-1;
			  else if (gameWindow.key.code==Keyboard::Right) dx=1;
			  else if (gameWindow.key.code==Keyboard::R) resetGame();
			  else if (gameWindow.key.code == Keyboard::Escape) window.close();
		}

	if (Keyboard::isKeyPressed(Keyboard::Down)) delay=0.05;
	if(!stopGame())
	{
		//// <- Move -> ///
		for ( int i=0; i<4; i++ )  
		{
			oldFigure[i]=currentFigure[i];
			currentFigure[i].x+=dx; 
		}
	    if ( !check() ) 
			for (int i=0; i<4; i++) 
				currentFigure[i]=oldFigure[i];
	
		//////Rotate//////
		if (rotate)
		{
			Point p = currentFigure[1]; //center of rotation
			for (int i=0;i<4;i++)
			{
				int x = currentFigure[i].y - p.y;
				int y = currentFigure[i].x - p.x;
				currentFigure[i].x = p.x - x;
				currentFigure[i].y = p.y + y;
			}
			if (!check()) 
			   for (int i=0;i<4;i++) 
					currentFigure[i]=oldFigure[i];
	
		}
	
		///////Tick//////
		if (timer>delay)
		{
		    for (int i=0;i<4;i++) 
			{
				oldFigure[i] = currentFigure[i]; 
				currentFigure[i].y+=1; 
			}
			if (!check())
			{
				for( int i=0; i<4; i++ ) 
					field[ oldFigure[i].y ][ oldFigure[i].x ] = colorNum;
				colorNum = 1 + rand()%7;
				int n = m;
				m = rand()%7;
				for( int i=0; i<4; i++ )
				{
				    nextFigure[i].x2 = figures[m][i] % 2;
					    nextFigure[i].y2 = figures[m][i] / 2;
				}
				for ( int i=0; i<4; i++ )
				{
					currentFigure[i].x = figures[n][i] % 2;
					currentFigure[i].y = figures[n][i] / 2;
				}
			}
			timer=0;
		}
	
		///////check lines//////////
	    int k = M - 1;
		for (int i = M-1 ; i>0; i-- )
		{
			int count=0;
			for (int j=0;j<N;j++)
			{
			    if (field[i][j]) 
					count++;
			    field[k][j]=field[i][j];
			    scores = scores + 100;
			}
			if (count<N) 
				k--;
		}
	
	    dx=0; rotate=0; delay=0.3;
	
	    /////////draw//////////
	    window.clear(Color::White);	
	    window.draw(background);
			  
		for ( int i=0; i<M; i++ )
			for ( int j=0; j<N ;j++ )
			   {
		        	if ( field[i][j] == 0 ) 
						continue;
					Figure.setTextureRect(IntRect(field[i][j]*18,0,18,18));
					Figure.setPosition(j*18,i*18);
					Figure.move(28,31); //offset
					window.draw(Figure);
			   }
	
		for ( int i=0; i<4; i++ )
		{
			Figure.setTextureRect( IntRect( colorNum * 18, 0, 18, 18 ) );
			Figure.setPosition( currentFigure[i].x * 18, currentFigure[i].y * 18 );
			Figure.move( 28, 31 ); //offset
			window.draw( Figure );
		}
	
		for ( int i = 0; i<4; i++ ) // prediction block 
		{
			nextFigures.setTextureRect( IntRect( colorNum * 18, 0, 18, 18 ) );
			nextFigures.setPosition( nextFigure[i].x2 * 18, nextFigure[i].y2 * 18 );
			nextFigures.move( 260, 90 ); //offset
			window.draw( nextFigures ); // draws prediction block the right of the screen 
		}
		
		if (stopGame())
		{
			end_message.setPosition(50,400); 
			restart_message.setPosition(80,440); 
			window.draw(end_message);
			window.draw(restart_message);
		}	
	
		window.draw( frame );
	 	window.display();
	}
	}
}

void menu( RenderWindow &window )
{
	//Texture
	Texture button_start, button_top, button_about,button_exit, about_texture, menu_background, top_texture;
	//---------Button's---------
	button_start.loadFromFile("images/ButtonStart.png");
	button_top.loadFromFile("images/top.png");
//	button_about.loadFromFile("images/ButtonAbout.png");
	button_exit.loadFromFile("images/ButtonExit.png");
	//---Textures/BackGround----
	top_texture.loadFromFile("images/topTexture.png");
//	about_texture.loadFromFile("images/about.png");
	menu_background.loadFromFile("images/backgroundMenu.png");
	//---Initializate Textures---
	Sprite Start(button_start),Top(button_top),/*About(button_about),*/Exit(button_exit),
		topTexture(top_texture),/*aboutTexture(about_texture),*/menuBckg(menu_background);
	///Music
	Music menuMusic;
	menuMusic.openFromFile("musicMenu.ogg");
	menuMusic.play();
	menuMusic.setLoop(true);
	//---------------------------
	
	bool isMenu = 1;
	int menuNum = 0;
	//---Setup---Position for Botton and Background---
	Start.setPosition(70,120);
	Top.setPosition(70,170);
	//About.setPosition(70,220);
	Exit.setPosition(70,270);
	menuBckg.setPosition(0,0);
	//---------------------------
	
	while(isMenu)
	{
		Start.setColor(Color::White);
		Top.setColor(Color::White);
		//About.setColor(Color::White);
		Exit.setColor(Color::White);
		menuNum=0;
		window.clear(Color(129,181,221));
		
		//---Initializate press to button----		
		if(IntRect(70,120,176,50).contains(Mouse::getPosition(window)))
		{
			Start.setColor(Color::Green);
			menuNum=1;
		}
		if(IntRect( 100,170,130,50 ).contains( Mouse::getPosition( window ) ) )
		{
			Top.setColor(Color::Green);
			menuNum=2;
		}
		/*if( IntRect( 115,220,95,50 ).contains( Mouse::getPosition( window ) ) )
		{
			About.setColor(Color::Green);
			menuNum=3;
		}*/
		if( IntRect( 130,270,65,50).contains( Mouse::getPosition( window ) ) )
		{
			Exit.setColor(Color::Green);
			menuNum=3;
		}
	//-------------------------------------
		
		if( Mouse::isButtonPressed( Mouse::Left ) )
		{
			if( menuNum == 1 )
			{
				isMenu = false;
				menuMusic.stop();
				window.close();
				//MusicForGame
				Music gameMusic;
				gameMusic.openFromFile("GameMusic.ogg");
				gameMusic.play();
				gameMusic.setLoop(true);
				/////////////////////////
				game();
			}
			if( menuNum == 2 )
			{
				window.draw( topTexture );
				window.display();
				while( !Keyboard::isKeyPressed ( Keyboard::Escape ) );
			}
		/*	if( menuNum == 3 )
			{
				window.draw( aboutTexture );
				window.display();
				while( !Keyboard::isKeyPressed ( Keyboard::Escape ) );
			}*/
			if( menuNum == 3 )
			{
				window.close();
				isMenu = false;
			}
		}
		
		window.draw(menuBckg);
		window.draw(Start);
		window.draw(Top);
		//window.draw(About);
		window.draw(Exit);
	
		window.display();
	}
	////////////////////////////////////////////////////
}

int main()
{
	RenderWindow window(VideoMode(320,480),"Tetris menu");
	menu(window);
    return 0;
}

