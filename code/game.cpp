/*Dragonlake test task (Catalin Balan) - DoodleJump with options:
1. 5 lives - game resets with life taken (with player spawning on the lowest platform) and ends with 0 lives.
2. Moving platforms.
3+4. Auto-shoot enemy ability, activates on right click when player gathers 20 coins.
*/
#include "Framework.h"
#include <iostream>
#include <cmath>
#include <string>

int globalWidth = 0;
int globalHeight = 0;

//moving object at constant speed in screen bounds
void move(float& x,float speed, int screenWidth, int objWidth, float& dir1) {
	
	x += speed * dir1;
	if (x < 0 || x > screenWidth - objWidth) {
		dir1 *= -1;
	}
}

struct coords {
	float x;
	float y;
};


class MyFramework : public Framework {

private:
	int playerLives;
	int score=0;
	int topScore=0;
	Sprite *background;
	Sprite *loadscreen;
	Sprite *coin;
	Sprite *monster;
	Sprite *bomb;
	Sprite *bullet;
	Sprite *ability;
	Sprite *heart;
	Sprite *plus;
	Sprite *numbers[10];
	Sprite *platform;
	Sprite *playerU;
	Sprite *playerR;
	Sprite *playerL;
	coords platforms[11];
	coords coincc[2];
	coords monstercc[2];
	coords projectilecc;
	coords smallProjectile;
	int gameW, gameH;
	unsigned int savedTime;
	float dir1 = 1;
	float dir2 = 1;
	
	float x = 300; float y = 560;
	float mx = 0; float my = 0;
	float savedmx = 0; float savedmy = 0;
	float deltax = 0.5; float deltay = 0;
	int ones[3]; int tens[3]; int hundreds[3]; int thousands[3]; int tthousands[3];
	double bAngle = 0;
	double abAngle1 = 0;
	double abAngle2 = 0;
	int coins = 0;
	int counter = 0;
	int bCounter = 0;
	bool movingR = false;
	bool movingL = false;
	bool lookingR = false;
	bool lookingL = true;
	bool shooting = false;
	bool smallShooting = false;
	bool bOnScreen = false;
	bool monsterActive = true;

public:

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		//restrict some resolutions
		if (globalWidth > 640 && globalHeight > 800 && globalWidth < 1601 && globalHeight < 1081) {
			width = globalWidth;
			height = globalHeight;
			fullscreen = false;
		}
		else {
			width = 640;
			height = 800;
			fullscreen = false;
		}
	}

	virtual bool Init() {
		playerLives = 0;
		getScreenSize(gameW,gameH);
		background = createSprite("../images/bck.png");
		loadscreen = createSprite("../images/loadscreen.png");
		platform = createSprite("../images/plat.png");
		playerU = createSprite("../images/ice-top.png");
		playerR = createSprite("../images/ice-right.png");
		playerL = createSprite("../images/ice-left.png");
		coin = createSprite("../images/coin.png");
		ability = createSprite("../images/ability.png");
		monster = createSprite("../images/monster.png");
		bomb = createSprite("../images/projectile.png");
		bullet = createSprite("../images/projectile.png");
		heart = createSprite("../images/heart.png");
		plus = createSprite("../images/plus.png");
		numbers[0]= createSprite("../images/zero.png");
		numbers[1] = createSprite("../images/one.png");
		numbers[2] = createSprite("../images/two.png");
		numbers[3] = createSprite("../images/three.png");
		numbers[4] = createSprite("../images/four.png");
		numbers[5] = createSprite("../images/five.png");
		numbers[6] = createSprite("../images/six.png");
		numbers[7] = createSprite("../images/seven.png");
		numbers[8] = createSprite("../images/eight.png");
		numbers[9] = createSprite("../images/nine.png");
		setSpriteSize(bullet,20,20);
		setSpriteSize(background, gameW, gameH);
		
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
		if(playerLives==0){
			drawSprite(background, 0, 0);
			drawSprite(loadscreen, 0, 0);
			//topscore display
			if (topScore > 99999) {
				drawSprite(numbers[9], 240, 550);
				drawSprite(numbers[9], 270, 550);
				drawSprite(numbers[9], 300, 550);
				drawSprite(numbers[9], 330, 550);
				drawSprite(numbers[9], 360, 550);
				drawSprite(plus, 390, 550);

			}
			else {
				ones[1] = topScore % 10;
				tens[1] = (topScore / 10) % 10;
				hundreds[1] = (topScore / 100) % 10;
				thousands[1] = (topScore / 1000) % 10;
				tthousands[1] = (topScore / 10000) % 10;
				drawSprite(numbers[tthousands[1]], 240, 550);
				drawSprite(numbers[thousands[1]], 270, 550);
				drawSprite(numbers[hundreds[1]], 300, 550);
				drawSprite(numbers[tens[1]], 330, 550);
				drawSprite(numbers[ones[1]], 360, 550);

			}
		
		}
		else {
			drawSprite(background, 0, 0);
			drawSprite(coin, 0, 0);
			drawSprite(heart, gameW-50, 0);
			drawSprite(numbers[playerLives], gameW-40, 10);
			drawSprite(coin, coincc[0].x, coincc[0].y);
			drawSprite(coin, coincc[1].x, coincc[1].y);
			
			
			for (int i = 0; i < 10; i++) {
				drawSprite(platform, platforms[i].x, platforms[i].y);
			}

			//moving platforms
			move(platforms[9].x, deltax, gameW, 115, dir1);
			move(platforms[5].x, deltax, gameW, 115, dir2);

			//coin display
			if (coins >= 0) {
				if (coins > 999) {

					drawSprite(numbers[9], 50, 10);
					drawSprite(numbers[9], 80, 10);
					drawSprite(numbers[9], 110, 10);
					drawSprite(plus, 140, 10);

				}
				else {
					ones[0] = coins % 10;
					tens[0] = (coins / 10) % 10;
					hundreds[0] = (coins / 100) % 10;
					drawSprite(numbers[hundreds[0]], 50, 10);
					drawSprite(numbers[tens[0]], 80, 10);
					drawSprite(numbers[ones[0]], 110, 10);

				}
			}

			//score display
			if (score > 99999) {
				drawSprite(numbers[9], (gameW / 2 - 160), 10);
				drawSprite(numbers[9], (gameW / 2 - 130), 10);
				drawSprite(numbers[9], (gameW / 2 - 100), 10);
				drawSprite(numbers[9], (gameW / 2 - 70), 10);
				drawSprite(numbers[9], (gameW/2-40), 10);
				drawSprite(plus, (gameW / 2 - 10), 10);

			}
			else {
				ones[2] = score % 10;
				tens[2] = (score / 10) % 10;
				hundreds[2] = (score / 100) % 10;
				thousands[2] = (score / 1000) % 10;
				tthousands[2] = (score / 10000) % 10;
				drawSprite(numbers[tthousands[2]], (gameW / 2 - 160), 10);
				drawSprite(numbers[thousands[2]], (gameW / 2 - 130), 10);
				drawSprite(numbers[hundreds[2]], (gameW / 2 - 100), 10);
				drawSprite(numbers[tens[2]], (gameW / 2 - 70), 10);
				drawSprite(numbers[ones[2]], (gameW / 2 - 40), 10);

			}
			
				
			

			//movement
			deltay += 0.015;
			y += deltay;
			if (x > gameW-60)x = -20;
			if (x < -21)x = gameW-60;
			if (movingR )x += 2.0f;
			else if (movingL )x -= 2.0f;

			//playersprite orientation
			if (shooting || smallShooting) drawSprite(playerU, x, y);
			else if (lookingR) drawSprite(playerR, x, y);
			else if (lookingL) drawSprite(playerL, x, y);



			//endless scrolling
			if (y < (gameH / 2) - 40) {
				
				for (int i = 0; i < 10; i++) {
					y = (gameH / 2) - 40;
					platforms[i].y = platforms[i].y - deltay;
					if (platforms[i].y > 790) {
						if (i == 8)monsterActive = true;
						counter++; score++;
						if (counter > 9) { counter = 0; score += 20; }
						platforms[i].x = rand() % (gameW-115);
						platforms[i].y = rand() % 1 - 50;

					}
				}


			}


			if (y < (gameH / 2) - 39) {
				projectilecc.y = projectilecc.y - deltay;
				smallProjectile.y = smallProjectile.y - deltay;
				for (int i = 0; i < 2; i++) {
					coincc[i].y = coincc[i].y - deltay;
					if (coincc[i].y > 790) {

						coincc[i].y = rand() % 1 - 350;
						coincc[i].x = rand() % gameW;

					}
				}


			}

			//monster handler
			if (monsterActive) {
				monstercc[0].x = platforms[8].x+25;
				monstercc[0].y = platforms[8].y-35;
				drawSprite(monster, monstercc[0].x, monstercc[0].y);
			}
			else{
				monstercc[0].x = -50;
				monstercc[0].y = 20;
			}


			//autoshooting
			if (shooting) {

				if ((getTickCount() - savedTime) > 20000) {
					shooting = 0;
				}
				else {
					drawSprite(ability, (gameW/2)-20, 45);
					drawSprite(bomb, projectilecc.x, projectilecc.y);

					if (monstercc[0].y > 0 && monstercc[0].y < 750 && monsterActive) {
						abAngle1 = atan2(monstercc[0].y - projectilecc.y, monstercc[0].x - projectilecc.x);
						projectilecc.x += 5 * cos(abAngle1);
						projectilecc.y += 5 * sin(abAngle1);
						

					}
					
					else { projectilecc.x = x+50; projectilecc.y = y-25; }
				}
			}

			//normal shooting
			if (smallShooting) {
				drawSprite(bullet, smallProjectile.x, smallProjectile.y);
				if (smallProjectile.x > gameW - 60) { smallProjectile.x = -20; bCounter++; }
				if (smallProjectile.x < -21) { smallProjectile.x = gameW - 60; bCounter++; }
				//removes bullet if player shoots it in the exact middle of the screen causing it to shift from the sides forever or abuse this mechanic to have a permanent 'laser' above removing enemies :)
				if (bCounter > 5)smallProjectile.y = -50;

				if (smallProjectile.y > 0 && smallProjectile.y < 800) {
					bOnScreen = true;
					smallProjectile.x += 4 * cos(bAngle);
					smallProjectile.y += 4 * sin(bAngle);
				}
				else { smallShooting = false; bOnScreen = false; }

			}

			//platform collision
			for (int i = 0; i < 10; i++) {
				if (deltay > 0 && x + 75 > platforms[i].x && x < platforms[i].x + 75 && y + 100 > platforms[i].y && y + 100 < platforms[i].y +15 ) deltay = -3;
			}
			//coin collision
			for (int i = 0; i < 2; i++) {
				if (x + 124 > coincc[i].x && x < coincc[i].x + 50 && y + 124 > coincc[i].y && y < coincc[i].y + 50) { 
					coins++;
					coincc[i].y = rand() % 1 - 350;
					coincc[i].x = rand() % gameW ;
				}
			}

			//jumping on top of monster = elimination 
			
			
				if (deltay > 0 && x + 124 > monstercc[0].x && x < monstercc[0].x + 50 && y + 124 > monstercc[0].y && y  < monstercc[0].y -50) {
					monsterActive = false;
					deltay = -3;
				}
			
			
			//monster collision
			
				if (x + 114 > monstercc[0].x && x < monstercc[0].x + 30 && y + 114 > monstercc[0].y && y  < monstercc[0].y +20) {
					monsterActive = false;
					deltay = 0.015;
					x = platforms[counter].x;
					y = platforms[counter].y - 90;
					playerLives--;
				}
				if (projectilecc.x + 30 > monstercc[0].x && projectilecc.x < monstercc[0].x + 50 && projectilecc.y + 30 > monstercc[0].y && projectilecc.y < monstercc[0].y + 50) {

					monsterActive = false;
				}
				if (smallProjectile.x + 20 > monstercc[0].x && smallProjectile.x < monstercc[0].x + 50 && smallProjectile.y + 20 > monstercc[0].y && smallProjectile.y < monstercc[0].y + 50) {

					monsterActive = false;
				}
				
			
			//fall detection
			if (y > 800){
				monsterActive = false;
				playerLives--;
				deltay = 0.015;
				x = platforms[counter].x;
				y = platforms[counter].y-90;
			}
			}

			if (score > topScore)topScore = score;
		

		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		mx = x;
		my = y;
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {


		if (button == FRMouseButton::LEFT && playerLives == 0) {
			playerLives = 5;
			coins = 0;
			score = 0;
			counter = 0;
			shooting = false;
			bOnScreen = false;
			monsterActive = true;
			srand(1);
			platforms[0].x = 300;
			platforms[0].y = 780;
			coincc[0].x = 200;
			coincc[0].y = 50;
			coincc[1].x = 450;
			coincc[1].y = -275;
			monstercc[0].x = -50;
			monstercc[0].y = 20;
			projectilecc.x = -300;
			projectilecc.y = 400;
			smallProjectile.x = -400;
			smallProjectile.y = 400;
			srand(1);
			for (int i = 1; i < 10; i++) {
				platforms[i].x = rand() % (gameW-115);
				platforms[i].y = 750 - (i * (rand() % (80 - 75 + 1) + 75));
			}
			x = 300;
			y = 560;
		}

		if (button == FRMouseButton::RIGHT && coins >= 20 && !shooting) {
			shooting = true;
			coins -= 20;
			savedTime = getTickCount();
			projectilecc.x = x + 50; projectilecc.y = y + 12;
			
		}

		if (button == FRMouseButton::LEFT && playerLives > 0 && !shooting && isReleased && !bOnScreen) {
			bCounter = 0;
			smallProjectile.x = x + 50; smallProjectile.y = y + 20;
			savedmx = mx;
			savedmy = my;
			bAngle = atan2(savedmy - smallProjectile.y, savedmx - smallProjectile.x);
			smallShooting = true;
		}
		
			
	}

	virtual void onKeyPressed(FRKey k) {
		if (k == FRKey::RIGHT)
		{
			movingR = true;
			lookingR = true;
			lookingL = false;
		}
		else if (k == FRKey::LEFT)
		{
			movingL = true;
			lookingR = false;
			lookingL = true;
		}
	}

	virtual void onKeyReleased(FRKey k) {
		if (k == FRKey::RIGHT) {
		movingR = false;
		
	}

		else if (k == FRKey::LEFT) {
			movingL = false;
			
		}
	
	}
	
	virtual const char* GetTitle() override
	{
		return "DoodleJump - Dragonlake - Balan";
	}



};

int main(int argc, char *argv[])
{
	
	std::string first="";
	std::string second="";
	std::string character;
	std::string resolution;
	unsigned int position = 0;
	//find if a resolution is given in command arguments
	for (int i = 1; i < argc; i++) {
		std::string s=argv[i];
		if (s.find("x") != std::string::npos) { resolution = s; 
		position = resolution.find("x");
		if (position != std::string::npos) {
			character = resolution.substr(0, position);
			first += character;
			unsigned int position2 = resolution.find("x", position + 1);
			if (position2 != std::string::npos) {
				character = resolution.substr(position + 1, position2 - position - 1);
				second += " " + character;
			}
		}
		}
	}
	if (!first.empty() && !second.empty()) {
		globalWidth = stoi(first);
		globalHeight = stoi(second);
	}
	return run(new MyFramework);
}
