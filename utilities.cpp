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

std::vector<Token> InheritanceGraph::get_adjacents(Token& class_) const {
    std::vector<Token> adjacents{};
    for (auto& elt: graph) {
        if (elt.second == class_) {
            adjacents.push_back(elt.first);
        }
    }
    return adjacents;
}

bool InheritanceGraph::is_leaf_class(Token& token) const {
    return get_adjacents(token).size() == 0;
}


std::vector<Token> InheritanceGraph::DFS(Token& root) const  {
    std::vector<Token> classes_dfs;
    std::stack<Token> to_visits{};

    to_visits.push(root);
    while (!to_visits.empty()) {
        classes_dfs.push_back(to_visits.top());
        auto v = get_adjacents(to_visits.top());
        to_visits.pop();
        for (auto elt: v) {
            to_visits.push(elt);
        }
    }
    return classes_dfs; 
}

bool is_base_class(Class* class_) {
    return class_->name == Object || class_->name == Bool || class_->name == Int ||
            class_->name == Str || class_->name == IO;
}

bool is_base_function(Token& func_name) {
    return func_name == concat || func_name == cool_abort || func_name == copy ||
        func_name == in_string || func_name == length || func_name == out_string ||
        func_name == in_int || func_name == out_int || func_name == substr || 
        func_name == type_name; 
}

}