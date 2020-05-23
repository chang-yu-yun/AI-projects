#include <cstdio>
#include <cstdlib>
#include <climits>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <ctime>

enum class PLAYER { NONE, USER, AI };
static const std::map<int, int> FOUR = {{2, 1}, {3, 1}, {5, 1}, {8, 1}, {13, 1}};
static const std::map<int, int> SIX = {{2, 2}, {3, 2}, {5, 2}, {8, 3}, {13, 2}};
static const int THRESHOLD = 15;
static const int TIMELIMIT = 30;
struct Info {
    int x;
    int y;
    int value;
    int score;
    Info(): x(-1), y(-1), value(-1), score(-1) {}
};

void printBoard(const std::vector<std::vector<int>>& board, const std::vector<std::vector<PLAYER>>& player) {
    char removal = 'X';
    printf("\n");

    for (int i = 0; i < player.size(); ++i) {
        for (int j = 0; j < player[i].size(); ++j) {
            if (player[i][j] == PLAYER::USER) {
                printf("\033[0;32m%4d\033[0m", board[i][j]);
            } else if (player[i][j] == PLAYER::AI) {
                printf("\033[0;33m%4d\033[0m", board[i][j]);
            } else if (board[i][j] == -1) {
                printf("\033[0;31m%4c\033[0m", removal);
            } else {
                printf("%4d", board[i][j]);
            }
        }
        printf("\n");
    }

    printf("\n");
}

void printCards(const std::map<int, int>& user, const std::map<int, int>& ai) {
    printf("\n");

    int count = 0;
    // print cards that user has
    printf("\033[0;42m[User chess pieces]\033[0m: [");
    for (auto iter = user.cbegin(); iter != user.cend(); ++iter) {
        for (int i = 0; i < iter->second; ++i) {
            if (count > 0) {
                printf(", ");
            }
            printf("%d", iter->first);
            ++count;
        }
    }
    printf("]\n");

    count = 0;
    // print cards that AI has
    printf("\033[0;43m[  AI chess pieces]\033[0m: [");
    for (auto iter = ai.cbegin(); iter != ai.cend(); ++iter) {
        for (int i = 0; i < iter->second; ++i) {
            if (count > 0) {
                printf(", ");
            }
            printf("%d", iter->first);
            ++count;
        }
    }
    printf("]\n");

    printf("\n");
}

void checkStatus(std::vector<std::vector<int>>& board, std::vector<std::vector<PLAYER>>& player) {
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            if (player[i][j] != PLAYER::NONE) {
                int sum = 0;
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        int currX = i + dx;
                        int currY = j + dy;
                        if (currX >= 0 && currX < board.size() && currY >= 0 && currY < board[currX].size() &&
                            board[currX][currY] > 0) {
                            sum += board[currX][currY];
                        }
                    }
                }
                if (sum > THRESHOLD) {
                    player[i][j] = PLAYER::NONE;
                }
            }
        }
    }

    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            if (board[i][j] > 0 && player[i][j] == PLAYER::NONE) {
                board[i][j] = -1;
            }
        }
    }
}

void getUserInput(std::map<int, int>& user, std::vector<std::vector<int>>& board, std::vector<std::vector<PLAYER>>& player) {
    int row, col, value;
    bool success = false;

    printf("\n");

    while (!success) {
        printf("Input (row, col, weight): ");
        scanf("%d %d %d", &row, &col, &value);
        printf("\033[0;42m[User]\033[0m: (%d, %d, %d)\n", row, col, value);
        if (row < 0 || row >= board.size() || col < 0 || col >= board[row].size() || board[row][col] != 0) {
            printf("\n\033[0;41m[Error]\033[0m Invalid row or column number! Please try again!\n");
            continue;
        }
        if (user.count(value) == 0 || user[value] == 0) {
            printf("\n\033[0;41m[Error]\033[0m Invalid weight! Please try again!\n");
            continue;
        }
        board[row][col] = value;
        player[row][col] = PLAYER::USER;
        user[value]--;
        success = true;
    }
}

bool checkTerminal(const std::map<int, int>& user, const std::map<int, int>& ai) {
    for (auto iter = user.cbegin(); iter != user.cend(); ++iter) {
        if (iter->second > 0) {
            return false;
        }
    }
    for (auto iter = ai.cbegin(); iter != ai.cend(); ++iter) {
        if (iter->second > 0) {
            return false;
        }
    }
    return true;
}

std::pair<PLAYER, int> getWinner(const std::vector<std::vector<int>>& board, const std::vector<std::vector<PLAYER>>& player) {
    int userScore = 0;
    int userMax = INT_MIN;
    int aiScore = 0;
    int aiMax = INT_MIN;

    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            if (player[i][j] == PLAYER::USER) {
                userScore += board[i][j];
                userMax = std::max(userMax, board[i][j]);
            } else if (player[i][j] == PLAYER::AI) {
                aiScore += board[i][j];
                aiMax = std::max(aiMax, board[i][j]);
            }
        }
    }

    std::pair<PLAYER, int> result = std::make_pair(PLAYER::NONE, 0);
    if (userScore > aiScore) {
        result.first = PLAYER::USER;
        result.second = userScore;
    } else if (userScore < aiScore) {
        result.first = PLAYER::AI;
        result.second = aiScore;
    } else if (userMax > aiMax) {
        result.first = PLAYER::USER;
        result.second = userMax;
    } else if (userMax < aiMax) {
        result.first = PLAYER::AI;
        result.second = aiMax;
    } else {
        result.first = PLAYER::NONE;
        result.second = aiMax;
    }

    return result;
}

int predictScore(const std::vector<std::vector<int>>& board, const std::vector<std::vector<PLAYER>>& player,
                 const std::map<int, int>& user, const std::map<int, int>& ai) {
    int result = 0;
    int userScore = 0, aiScore = 0;
    int userMax = 0, aiMax = 0;
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            if (player[i][j] == PLAYER::USER) {
                userScore += board[i][j];
                userMax = std::max(userMax, board[i][j]);
            } else if (player[i][j] == PLAYER::AI) {
                aiScore += board[i][j];
                aiMax = std::max(aiMax, board[i][j]);
            }
        }
    }
    
    for (auto iter = user.cbegin(); iter != user.cend(); ++iter) {
        if (iter->second > 0) {
            userScore += (iter->first) * (iter->second);
            userMax = std::max(userMax, iter->first);
        }
    }

    for (auto iter = ai.cbegin(); iter != ai.cend(); ++iter) {
        if (iter->second > 0) {
            aiScore += (iter->first) * (iter->second);
            aiMax = std::max(aiMax, iter->first);
        }
    }

    return userScore != aiScore ? userScore - aiScore : userMax - aiMax;
} 

Info minimax(std::vector<std::vector<int>> board, std::vector<std::vector<PLAYER>> player,
             std::map<int, int>& user, std::map<int, int>& ai, int depth, int limit, const time_t& startTime, bool isMaxPlayer) {
    Info result;
    time_t currTime;
    time(&currTime);
    if (checkTerminal(user, ai) || depth == limit || difftime(currTime, startTime) >= 30.0) {
        int prediction = predictScore(board, player, user, ai);
        result.score = prediction;
        return result;
    }
    std::vector<std::vector<int>> currBoard;
    std::vector<std::vector<PLAYER>> currPlayer;
    if (isMaxPlayer) {
        result.score = INT_MIN;
        for (auto iter = user.begin(); iter != user.end(); ++iter) {
            if (iter->second > 0) {
                for (int i = 0; i < board.size(); ++i) {
                    for (int j = 0; j < board[i].size(); ++j) {
                        if (board[i][j] == 0 && player[i][j] == PLAYER::NONE) {
                            currBoard = board;
                            currPlayer = player;
                            currBoard[i][j] = iter->first;
                            currPlayer[i][j] = PLAYER::USER;
                            --(iter->second);
                            checkStatus(currBoard, currPlayer);
                            Info next = minimax(currBoard, currPlayer, user, ai, depth + 1, limit, startTime, false);
                            if (next.score > result.score) {
                                result.score = next.score;
                                result.x = i;
                                result.y = j;
                                result.value = iter->first;
                            }
                            ++(iter->second);
                        }
                    }
                }
            }
        }
    } else {
        result.score = INT_MAX;
        for (auto iter = ai.begin(); iter != ai.end(); ++iter) {
            if (iter->second > 0) {
                for (int i = 0; i < board.size(); ++i) {
                    for (int j = 0; j < board[i].size(); ++j) {
                        if (board[i][j] == 0 && player[i][j] == PLAYER::NONE) {
                            currBoard = board;
                            currPlayer = player;
                            currBoard[i][j] = iter->first;
                            currPlayer[i][j] = PLAYER::AI;
                            --(iter->second);
                            checkStatus(currBoard, currPlayer);
                            Info next = minimax(currBoard, currPlayer, user, ai, depth + 1, limit, startTime, true);
                            if (next.score < result.score) {
                                result.score = next.score;
                                result.x = i;
                                result.y = j;
                                result.value = iter->first;
                            }
                            ++(iter->second);
                        }
                    }
                }
            }
        }
    }
    return result;
}

void init(int& user, int& boardSize) {
    int option;

    printf("User first? (0/1): ");
    while (scanf("%d", &option) != 1 || (option != 0 && option != 1)) {
        printf("\033[0;41m[Error]\033[0m Invalid option! Please try again!\n");
        printf("User first? (0/1): ");
    }
    user = 1 - option;

    printf("Board size? (4 or 6): ");
    while (scanf("%d", &option) != 1 || (option != 4 && option != 6)) {
        printf("\033[0;41m[Error]\033[0m Invalid board size! Please try again!\n");
        printf("Board size? (4 or 6): ");
    }
    boardSize = option;
}

int main() {
    int userTurn, boardSize;
    init(userTurn, boardSize);
    
    std::vector<std::vector<int>> board(boardSize, std::vector<int>(boardSize, 0));
    std::vector<std::vector<PLAYER>> player(boardSize, std::vector<PLAYER>(boardSize, PLAYER::NONE));
    std::map<int, int> user, ai;
    int count = 0, depthLimit = 3;
    time_t startTime, currTime;
    user = boardSize == 4 ? FOUR : SIX;
    ai = boardSize == 4 ? FOUR : SIX;
    printBoard(board, player);
    printCards(user, ai);
    
    double diff;
    while (true) {
        if (count % 2 == userTurn) {
            getUserInput(user, board, player);
        } else {
            time(&startTime);
            Info curr;
            curr.score = INT_MIN;
            for (int i = 1; i <= depthLimit; ++i) {
                Info temp = minimax(board, player, user, ai, 0, i, startTime, false);
                if (temp.score > curr.score) {
                    curr = temp;
                }
                time(&currTime);
                if (difftime(currTime, startTime) >= 30.0) {
                    break;
                }
            }
            printf("\033[0;43m[AI]\033[0m: (%d, %d, %d)\n", curr.x, curr.y, curr.value);
            board[curr.x][curr.y] = curr.value;
            player[curr.x][curr.y] = PLAYER::AI;
            --ai[curr.value];
        }
        checkStatus(board, player);
        printBoard(board, player);
        printCards(user, ai);
        if (checkTerminal(user, ai)) {
            break;
        }
        ++count;
    }

    PLAYER winner = getWinner(board, player).first;
    if (winner == PLAYER::USER) {
        printf("\n\033[0;42mUser Wins!\033[0m\n");
    } else if (winner == PLAYER::AI) {
        printf("\n\033[0;43mAI Wins!\033[0m\n");
    } else {
        printf("\n\033[0;44mTie!\033[0m\n");
    }

    return 0;
}
