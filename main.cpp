#include "MiniGitSystem.h"
#include <iostream>

int main() {
    MiniGitSystem system;
    std::string command;

    while (true) {
        std::cout << "\nMiniGit > ";
        std::cin >> command;

        if (command == "init") {
            system.init();
        }
        else if (command == "add") {
            std::string filename;
            std::cin >> filename;
            system.addFile(filename);
        }
        else if (command == "commit") {
            std::cin.ignore();
            std::string message;
            std::cout << "Enter commit message: ";
            std::getline(std::cin, message);
            system.commit(message);
        }
        else if (command == "log") {
            system.log();
        }
        else if (command == "branch") {
            std::string name;
            std::cin >> name;
            system.branch(name);
        }
        else if (command == "checkout") {
            std::string name;
            std::cin >> name;
            system.checkout(name);
        }
        else if (command == "diff") {
            std::string c1, c2;
            std::cin >> c1 >> c2;
            system.diff(c1, c2);
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    return 0;
}
