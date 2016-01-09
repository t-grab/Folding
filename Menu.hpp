//
// Created by Tobias on 09.01.2016.
//

#ifndef FOLDING_CONSOLEMENU_HPP
#define FOLDING_CONSOLEMENU_HPP

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::cin;
#include <utility>
using std::pair;
#include <stdexcept>
using std::runtime_error;
using std::out_of_range;

typedef unsigned int uint;

class Interactive {
public:
    Interactive(const string&);

    virtual string get_title() const;

    virtual void print_options() const = 0;
    virtual void go_to(uint) = 0;
    virtual void go_back() = 0;
private:
    string title;
};

class Menu : public Interactive {
public:
    explicit Menu(const string&, Menu*);

    void print_options() const;
    void go_to(uint);
    void go_back();
    void add_submenu(Interactive*);
private:
    vector<Interactive*> submenus;
    Interactive* active;
    Menu* parent;
};

class MenuLeaf : public Interactive {
public:
    explicit MenuLeaf(const string&, Menu*);

    void print_options() const;
    void go_to(uint);
    void go_back();
    void add_action(const string&, void (*)());
private:
    vector<pair<string, void (*)()>> actions;
    Menu* parent;
};

#endif //FOLDING_CONSOLEMENU_HPP
