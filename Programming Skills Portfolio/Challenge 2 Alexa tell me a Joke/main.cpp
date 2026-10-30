#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

struct Joke {
    std::string setup;
    std::string punchline;
};

int main() {
    std::ifstream jokeFile("randomJokes.txt");
    if (!jokeFile.is_open()) {
        std::cerr << "Error: Could not open randomJokes.txt\n";
        return 1;
    }

    std::vector<Joke> jokes;
    std::string line;
    while (std::getline(jokeFile, line)) {
        if (line.empty()) continue;
        size_t qmarkPos = line.find('?');
        if (qmarkPos != std::string::npos) {
            jokes.push_back({
                line.substr(0, qmarkPos + 1),
                line.substr(qmarkPos + 1)
            });
        }
    }
    jokeFile.close();

    if (jokes.empty()) {
        std::cerr << "Error: No jokes found in file.\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, jokes.size() - 1);

    std::string userInput;
    while (true) {
        std::cout << "\nType \"Alexa, tell me a joke\" (or 'quit' to exit): ";
        std::getline(std::cin, userInput);

        if (userInput == "quit" || userInput == "Quit") {
            std::cout << "Goodbye!\n";
            break;
        }

        if (userInput == "Alexa, tell me a joke") {
            const Joke& selectedJoke = jokes[dist(gen)];
            std::cout << "\n" << selectedJoke.setup << "\n";
            std::cout << "[Press Enter to see punchline...]";
            std::string enterWait;
            std::getline(std::cin, enterWait);
            std::cout << selectedJoke.punchline << "\n";
        } else {
            std::cout << "Invalid command. Please type exactly: Alexa, tell me a joke\n";
        }
    }

    return 0;
}