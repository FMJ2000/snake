#include "../include/main.hpp"

int main() {
	srand(time(0));

	// window initialization
	sf::RenderWindow window(sf::VideoMode(WINDOW[0], WINDOW[1]), TITLE, sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(FRAME_RATE);
	sf::Clock clock;

	Game * game = new Game();

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
					if (game->state != STOP) {
						if (event.key.code == sf::Keyboard::R) restart(&game);
						if (event.key.code == sf::Keyboard::Space) game->flipState();
						if (event.key.code == sf::Keyboard::Left) game->snake->turn(-1);
						if (event.key.code == sf::Keyboard::Right) game->snake->turn(1); 
						if (event.key.code == sf::Keyboard::Up) game->snake->setSpeed(1);
						if (event.key.code == sf::Keyboard::Down) game->snake->setSpeed(-1);
					}
					if (event.key.code == sf::Keyboard::Escape) window.close();
					break;
				}

				case sf::Event::KeyReleased: {
					if (game->state != STOP) game->snake->turn(0);
					break;
				}

				case sf::Event::EventType::TextEntered: {
					if (game->state == STOP && event.text.unicode < 128) game->typeUsername(static_cast<char>(event.text.unicode));
					break;
				}
			}
		}

		window.setKeyRepeatEnabled(false);

		// game update and draw
		game->update(dt);
		window.clear(sf::Color(BACKGROUND));
		window.draw(*game);
		window.display();
	}

	delete game;
	return 0;
}

void restart(Game ** game) {
	delete *game;
	*game = new Game();
	(*game)->username = "";
}