#include "utilities.hpp"

namespace cool {

void InheritanceGraph::addEdge(const Token& a, const Token& b) {
    if (graph.count(a) != 0) return;
    graph[a] = b;
}

bool InheritanceGraph::isDGA() {
    for(auto it = graph.begin(); it != graph.end(); it++) {
        Token current = it->first;
        Token next = it->second;
        std::set<Token> visited;
        visited.insert(current);
        while (current && current != Object) {
            if(visited.count(next) != 0)
                return false;
            visited.insert(next);
            current = next;
            next = graph[next];
        }
    }
    return true;
}

bool InheritanceGraph::conform(Token a, Token b) {

    if (a == b) 
        return true;
    while (a != Object) {
        if(a == b)
            return true;
        a = graph[a]; 
    }
    return a == b;
}

Token InheritanceGraph::lca(Token a, Token b) {

    if (a == b)
        return a;
    
    int high_a = 0, high_b = 0;
    Token temp_a = a, temp_b = b;

    while(temp_a != Object) {
        temp_a = graph[temp_a];
        high_a++;
    }

    while (temp_b != Object) {
        temp_b = graph[temp_b];
        high_b++;
    }

    temp_a = a, temp_b = b;

    if (high_a > high_b) {
        for (int i = high_a - high_b; i>0; i--) {
            temp_a = graph[temp_a];
        }
    } else {
        for (int i = high_b - high_a; i>0; i--) {
            temp_b = graph[temp_b];
        }
    }

    while (temp_a != temp_b) {
        temp_a = graph[temp_a];
        temp_b = graph[temp_b];
    }
    return temp_a;
}

bool is_base_class(Class* class_) {
    return class_->name == Object || class_->name == Bool || class_->name == Int ||
            class_->name == Str || class_->name == IO;
}


}