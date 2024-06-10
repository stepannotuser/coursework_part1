#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>

struct Transition {
    int from_state;
    int to_state;
    std::set<std::string> transition_names; 

    void addTransitionName(const std::string& name) {
        transition_names.insert(name);
    }
};

class FiniteAutomaton {
private:
    std::map<int, std::string> state_map; 
    std::vector<Transition> transitions;
    int final_state; 
public:
    FiniteAutomaton() {
        addState("q0"); 
    }

    void addState(const std::string& state) {
        int state_index = state_map.size();
        state_map[state_index] = state;
    }

    void addTransition(int from_state, int to_state, const std::string& transition_name) {
        for (auto& transition : transitions) {
            if (transition.from_state == from_state && transition.to_state == to_state) {
                transition.addTransitionName(transition_name);
                return;
            }
        }

        Transition new_transition = {from_state, to_state};
        new_transition.addTransitionName(transition_name);
        transitions.push_back(new_transition);
    }

    void printStates() const {
        std::cout << "Состояния автомата:\n";
        for (const auto& state_pair : state_map) {
            std::cout << "Состояние " << state_pair.first << ": " << state_pair.second << std::endl;
        }
    }

    void printTransitions() const {
        std::cout << "Переходы автомата:\n";
        for (const auto& transition : transitions) {
            std::cout << "Из состояния " << transition.from_state << " в состояние " << transition.to_state << " по переходам: ";
            for (const auto& name : transition.transition_names) {
                std::cout << "'" << name << "' ";
            }
            std::cout << std::endl;
        }
    }

    bool checkTransitionFromState(int current_state, const std::string& transition_name) const {
        for (const auto& transition : transitions) {
            if (transition.from_state == current_state && transition.transition_names.count(transition_name) > 0) {
                return true;
            }
        }
        return false; 
    }

    void readTextFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::cout << std::endl << "Прочитана строка из файла: " << line << std::endl;
            int current_state = 0;

            for (size_t i = 0; i < line.size(); ++i) {
                std::string transition_name(1, line[i]);
                bool transition_found = false;

                for (const auto& transition : transitions) {
                    if (transition.from_state == current_state && transition.transition_names.count(transition_name)) {
                        current_state = transition.to_state;
                        std::cout << "Переход из состояния " << transition.from_state 
                                  << " в состояние " << transition.to_state << " по символу '" 
                                  << transition_name << "'\n";
                        transition_found = true;
                        break;
                    }
                }

                if (!transition_found && i < line.size() - 1) {
                    std::cout << "из состояния " << current_state 
                              << " нет перехода по символу '" << transition_name << "'\n";
                    break;
                }
            }

            if (current_state == final_state) {
                std::cout << "Последнее состояние " << state_map[current_state] << " является финальным." << std::endl;
            } else {
                std::cout << "Последнее состояние " << state_map[current_state] << " не является финальным." << std::endl;
            }
        }

        file.close();
    }

    void setFinalState(int state) {
        final_state = state;
    }

    bool isFinalState(int state) const {
        return state == final_state;
    }

    void printFinalState() const {
        std::cout << "Финальное состояние: " << final_state << std::endl;
    }
};

void readTransitionsFromFile(const std::string& filename, FiniteAutomaton& automaton) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка открытия файла " << filename << std::endl;
        return;
    }

    int finalStateId;
    if (!(inputFile >> finalStateId)) {
        std::cerr << "He удалось прочитать идентификатор финального состояния." << std::endl;
        inputFile.close();
        return;
    }

    automaton.setFinalState(finalStateId);

    std::cout << "Установлено финальное состояние: " << finalStateId << std::endl;

    int from_state, to_state;
    std::string transition_name;

    while (inputFile >> from_state >> to_state >> transition_name) {
        automaton.addTransition(from_state, to_state, transition_name);
    }

    inputFile.close();
}

int main() {
    FiniteAutomaton automaton;

    readTransitionsFromFile("automaton_notextended.txt", automaton);
    automaton.printTransitions();  

    std::string text = "text_notextended.txt"; 
    automaton.readTextFromFile(text);

    return 0;
}
