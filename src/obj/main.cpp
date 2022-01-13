#include "../include/main.hpp"

int main() {
	srand(time(0));

	// window initialization
	sf::RenderWindow window(sf::VideoMode(WINDOW[0], WINDOW[1]), TITLE, sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(FRAME_RATE);
	sf::Clock clock;

	sf::Font font;
	font.loadFromFile(FONT_FILE);
	Menu * menu = new Menu(font);
	Game * game = NULL;
	sf::SoundBuffer buffer;
	buffer.loadFromFile(SOUNDS[SI_SELECT]);
	sf::Sound selectSound;
	selectSound.setBuffer(buffer);

	// game loop
	while (window.isOpen()) {
		float dt = clock.restart().asSeconds();
		
		// events
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed: {
					window.close();
					break;
				}

				case sf::Event::KeyPressed: {
					if (game) {
						if (event.key.code == sf::Keyboard::R && game->state > ST_INIT) restart(&game, font);
						else if (event.key.code == sf::Keyboard::Escape) {
							delete game;
							game = NULL;
						} else {
							game->handleInput(event.key.code, 1);
							break;
						}
					} else if (event.key.code == sf::Keyboard::Escape) window.close();
					else if (event.key.code == sf::Keyboard::Num1) game = new NormalGame(font);
					else if (event.key.code == sf::Keyboard::Num2) game = new TimedGame(font);
					selectSound.play();
					break;
				}

				case sf::Event::KeyReleased: {
					if (game) game->handleInput(event.key.code, 0);
					break;
				}		

				case sf::Event::EventType::TextEntered: {
					if (game) game->typeUsername(static_cast<char>(event.text.unicode));
					break;
				}

			}
		}

		window.setKeyRepeatEnabled(false);

		// game update and draw
		window.clear(sf::Color(BACKGROUND));
		if (game) {
			game->update(dt);
			window.draw(*game);
		} else window.draw(*menu);
		window.display();
	}

	delete game;
	return 0;
}

void restart(Game ** game, sf::Font &font) {
	GameType gameType = (*game)->gameType;
	delete (*game);
	*game = NULL;
	if (gameType == GT_NORMAL) *game = new NormalGame(font);
	else if (gameType == GT_TIMED) *game = new TimedGame(font);
}