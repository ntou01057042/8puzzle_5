#include <ios>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#define GOAL "012345678"

using std::ios_base;

using std::cin;
using std::cout;

using std::priority_queue;

using std::string;

using std::vector;

struct DataNode {

    string state, action;
    int g{}, h{}, f{};
    DataNode *parent{};
    int insertOrder{};

    friend bool operator<(const DataNode &lhs, const DataNode &rhs) {
        if (lhs.f != rhs.f) {
            return lhs.f > rhs.f;
        }
        return lhs.insertOrder > rhs.insertOrder;
    }
};

priority_queue<DataNode> pqueue;

bool isSolvable(const string &state) {
    int disorderPairs = 0;
    for (int i = 0; i < 9; ++i) {
        for (int j = i + 1; j < 9; ++j) {
            if (state[i] != '0' && state[j] != '0' && state[i] > state[j]) {
                ++disorderPairs;
            }
        }
    }
    if (disorderPairs % 2 == 0) {
        return true;
    }
    return false;
}

int heuristic(const string &state) {
    int distance = 0;
    for (int i = 0; i < 9; ++i) {
        int y = i / 3, x = i % 3;
        switch (state[i]) {
            case '0':
                break;
            case '1':
                distance += abs(y - 0) + abs(x - 1);
                break;
            case '2':
                distance += abs(y - 0) + abs(x - 2);
                break;
            case '3':
                distance += abs(y - 1) + abs(x - 0);
                break;
            case '4':
                distance += abs(y - 1) + abs(x - 1);
                break;
            case '5':
                distance += abs(y - 1) + abs(x - 2);
                break;
            case '6':
                distance += abs(y - 2) + abs(x - 0);
                break;
            case '7':
                distance += abs(y - 2) + abs(x - 1);
                break;
            case '8':
                distance += abs(y - 2) + abs(x - 2);
                break;
        }
    }
    return distance;
}

int findSuccessors(const string &state, vector<string> &nextStates, vector<string> &nextActions) {
    int x0 = 0, y0 = 0;
    for (int i = 0; i < 9; ++i) {
        // Find the coordination of digit 0 as (x0, y0).
        if (state[i] == '0') {
            x0 = i % 3;
            y0 = i / 3;
            break;
        }
    }
    // if not first row, then go up
    if (y0 > 0) {
        nextActions.emplace_back("up");
        string successor = string(state);
        std::swap(successor[3 * y0 + x0], successor[3 * (y0 - 1) + x0]);
        nextStates.push_back(successor);
    }
    // if not last row, then go down
    if (y0 < 2) {
        nextActions.emplace_back("down");
        string successor = string(state);
        std::swap(successor[3 * y0 + x0], successor[3 * (y0 + 1) + x0]);
        nextStates.push_back(successor);
    }
    // if not first column, then go left
    if (x0 > 0) {
        nextActions.emplace_back("left");
        string successor = string(state);
        std::swap(successor[3 * y0 + x0], successor[3 * y0 + (x0 - 1)]);
        nextStates.push_back(successor);
    }
    // if not last column, then go right
    if (x0 < 2) {
        nextActions.emplace_back("right");
        string successor = string(state);
        std::swap(successor[3 * y0 + x0], successor[3 * y0 + (x0 + 1)]);
        nextStates.push_back(successor);
    }
    return nextActions.size();
}

int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr);
    auto *first = new DataNode();
    cin >> first->state;
    if (!isSolvable(first->state)) {
        cout << "No solution!!" << "\n";
        return 0;
    }
    if (first->state == GOAL) {
        cout << "It is the goal state." << "\n";
        return 0;
    }
    int order = 0;
    DataNode *final = nullptr;
    first->g = 0;
    first->h = heuristic(first->state);
    first->f = first->g + first->h;
    first->parent = nullptr;
    first->insertOrder = ++order;
    pqueue.push(*first);   // enqueue
    while (!pqueue.empty()) {
        auto *myNode = new DataNode(pqueue.top());
        pqueue.pop();
        if (myNode->state == GOAL) {
            final = myNode;
            break;
        }
        vector<string> nextStates, nextActions;
        findSuccessors(myNode->state, nextStates, nextActions);
        for (int i = 0; i < nextActions.size(); ++i) {
            auto *newNode = new DataNode();
            newNode->state = nextStates[i];
            newNode->action = nextActions[i];
            newNode->g = myNode->g + 1;
            newNode->h = heuristic(newNode->state);
            newNode->f = newNode->g + newNode->h;
            newNode->parent = myNode;
            newNode->insertOrder = ++order;
            pqueue.push(*newNode);
        }
    }
    if (final == nullptr) {
        cout << "No solution!!" << "\n";
    } else {
        // Reverse the linked list from final to the root (who has no parent).
        DataNode *prev = nullptr, *curr = final, *next = nullptr;
        while (curr != nullptr) {
            next = curr->parent;
            curr->parent = prev;
            prev = curr;
            curr = next;
        }
        // Traverse the linked list and print out the actions.
//        int steps = 0;
        prev = prev->parent;
        while (prev != nullptr) {
//            ++steps;
            cout << "move 0 to " << (*prev).action << "\n";
            prev = prev->parent;
        }
//        cout << "Total steps: " << steps << "\n";
    }
    return 0;
}
