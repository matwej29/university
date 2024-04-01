#include <iostream>
#include <vector>
#include <set>
#include "queue"

struct attender{
    u_int id;
    std::set<attender*> friends = {};
};


int main() {
    // number of attenders(almost invited)
    u_int n;
    std::cin >> n;

    std::vector<attender *> attenders;
    attenders.reserve(n);
    for (int i = 0; i < n; i++){
        attenders.push_back(new attender(i));
    }

    u_int m; // number of pairs
    std::cin >> m;

    u_int x, y;
    for(int i = 0; i < m; i++){
        std::cin >> x >> y;
        attenders[x]->friends.emplace(attenders[y]);
        attenders[y]->friends.emplace(attenders[x]);
    }

    std::queue<attender *> Queue;
    Queue.push(attenders[0]);
    while(!Queue.empty()){
        // проверяем есть ли у предполагаемого гостя 5 знакомых
        // Если нет, то у каждого соседа удаляем себя из списка и добавляем его в очередь
        attender *current = Queue.front();
        Queue.pop();

        if (current->friends.size() < 5){
            for (auto familiar: current->friends){
                familiar->friends.erase(familiar->friends.find(current));
                Queue.push(familiar);
            }
        }
    }
}
