#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <ctime>

/* Post Search Optimization into O(n) inspite of O(n^2)
struct State {
    int x, y;
    double g;
    double h;
    State(int x, int y, double g, double h) : x(x), y(y), g(g), h(h) {}
    bool operator<(const State& other) const {
        return (g + h) > (other.g + other.h);
    }
};

double heuristic(int x, int y, int goal_x, int goal_y) {
    return std::abs(goal_x - x) + std::abs(goal_y - y);
}
*/

enum Direction
        {
            NORTH,
            EAST,
            SOUTH,
            WEST
        };

std::vector<std::string> node_x =
        {
                "NORTH",
                "EAST",
                "SOUTH",
                "WEST"
        };

std::vector<std::string> node_2x =
        {
                "NORTH",
                "EAST",
                "SOUTH",
                "WEST"
        };

std::map<std::string, int> horizontal =
        {
                { "NORTH", 0 },
                { "EAST", 1 },
                { "SOUTH", 0 },
                { "WEST", -1 }
        };

std::map<std::string, int> vertical =
        {
                { "NORTH", -1 },
                { "EAST", 0 },
                { "SOUTH", 1 },
                { "WEST", 0 }
        };

int maximum_depth = 0;
int count = 0;

std::vector<std::vector<int>> board(7, std::vector<int>(7, 1));
std::vector<std::pair<int, int>> obstacles =
        {
                {0, 0},
                {0, 1},
                {1, 0},
                {1, 1},
                {5, 0},
                {5, 1},
                {6, 0},
                {6, 1},
                {0, 5},
                {0, 6},
                {1, 5},
                {1, 6},
                {5, 5},
                {5, 6},
                {6, 5},
                {6, 6}
        };

int value = board[3][3];

bool move(std::vector<std::vector<int>>& board, int x, int y, std::string d)
{
    return (x + horizontal[d] >= 0 &&
            x + horizontal[d] < 7 &&
            y + vertical[d] >= 0 &&
            y + vertical[d] < 7 &&
            board[x + horizontal[d]][y + vertical[d]] != -1);
}

bool jump(std::vector<std::vector<int>>& board, int x, int y, std::string d)
{
    return (x + 2 * horizontal[d] >= 0 &&
            x + 2 * horizontal[d] < 7 &&
            y + 2 * vertical[d] >= 0 &&
            y + 2 * vertical[d] < 7 &&
            board[x + 2 * horizontal[d]][y + 2 * vertical[d]] != -1);
}

void check_board(const std::vector<std::vector<int>>& board) {
    std::cout << "\n";
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if (board[i][j] == -1) {
                std::cout << "  ";
            }
            else {
                std::cout << board[i][j] << " ";
            }
        }
        std::cout << "\n";
    }
}

bool dfs(std::vector<std::vector<int>>& board, int pegs, int row, int col, std::vector<std::pair<int, int>>& st) {
    check_board(board);
    time_t start_time = std::time(nullptr);
    time_t current_time = std::time(nullptr);
    int limit_time;
    if (current_time - start_time >= limit_time) {
        std::cout << "\nNo solution due to time limit" << "\n";
        return true;
    }
    if (pegs < 1) {
        std::cout << "\nNo solution found" << "\n";
        return false;
    }
    if (pegs == 1 && board[3][3] == 1) {
        std::cout << "\nSolution found" << "\n";
        return true;
    }
    if (st.size() > maximum_depth) {
        maximum_depth = st.size();
    }
    ++count;
    while (!st.empty()) {
        current_time = std::time(nullptr);
        if (current_time - start_time >= limit_time) {
            std::cout << "\nNo solution due to time limit" << "\n";
            return true;
        }
        std::pair<int, int> grid = st.back();
        st.pop_back();
        int x = grid.first;
        int y = grid.second;
        if (board[x][y] == 1) {
            for (std::string direction : node_x) {
                if (move(board, x, y, direction) &&
                    board[x + horizontal[direction]][y + vertical[direction]] == 1 &&
                    std::find(
                            st.begin(),
                            st.end(),
                            std::make_pair(x + horizontal[direction],y + vertical[direction])) == st.end()) {
                    st.push_back(std::make_pair(x + horizontal[direction], y + vertical[direction]));
                }
                if (jump(board, x, y, direction) &&
                    board[x + 2 * horizontal[direction]][y + 2 * vertical[direction]] == 1 &&
                    std::find(st.begin(),
                              st.end(),
                              std::make_pair(x + 2 * horizontal[direction], y + 2 * vertical[direction])) == st.end()) {
                    st.push_back(std::make_pair(x + 2 * horizontal[direction], y + 2 * vertical[direction]));
                }
            }

            for (std::string direction : node_x) {
                if (move(board, x, y, direction) &&
                    jump(board, x, y, direction) &&
                    board[x + horizontal[direction]][y + vertical[direction]] == 1 &&
                    board[x + 2 * horizontal[direction]][y + 2 * vertical[direction]] == 0) {
                    board[x][y] = 0;
                    board[x + horizontal[direction]][y + vertical[direction]] = 0;
                    board[x + 2 * horizontal[direction]][y + 2 * vertical[direction]] = 1;
                    if (dfs(board, pegs - 1, x + 2 * horizontal[direction], y + 2 * vertical[direction], st)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::time_t start_time = std::time(nullptr);
    std::cout << "Select the time limit value as minutes: ";
    int limit_time;
    std::cin >> limit_time;
    limit_time *= 60;
    maximum_depth = 0;
    count = 0;
    std::cout << "\nDepth-First Search\nTime Limit: " << limit_time / 60 << " Minutes" << "\n";
    std::vector<std::pair<int, int>> store;
    store.push_back(std::make_pair(3, 1));
    dfs(board, 32, 3, 1, store);
    double current_time = std::time(nullptr);
    std::cout << "\nTime Spent: " << (current_time - start_time) / 60 << " Minutes\nNumber count: " << count << "\nStored number in memory: " << maximum_depth << "\n";
    return 0;
}
