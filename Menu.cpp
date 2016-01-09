//
// Created by Tobias on 09.01.2016.
//

#include "Menu.hpp"

// INTERACTIVE IMPLEMENTATION

Interactive::Interactive(const string& t) : title(t) {}

string Interactive::get_title() const {
    return title;
}

// MENU IMPLEMENTATION

Menu::Menu(const string& t, Menu* p) : Interactive(t), submenus(), active(0), parent(p) {}

void Menu::print_options() const {
    for (uint i = 0U; i < submenus.size(); ++i)
        cout << "[" << i + 1 << "] " << submenus.at(i)->get_title() << std::endl;
}

void Menu::go_to(uint idx) {
    if (active != 0)
        active->go_to(idx);

    if (idx > submenus.size())
        throw out_of_range("Menu::go_to(): index greater than available menu entries");
}

void Menu::go_back() {
    if (active != 0)
        active = 0;
    else
        parent->go_back();
}

void Menu::add_submenu(Interactive* sub) {
    submenus.push_back(sub);
}

// MENULEAF IMPLEMENTATION

MenuLeaf::MenuLeaf(const string& t, Menu* p) : Interactive(t), actions(), parent(p) {}

void MenuLeaf::print_options() const {
    for (uint i = 0U; i < actions.size(); ++i)
        cout << "[" << i + 1 << "] " << actions.at(i).first << std::endl;
}

void MenuLeaf::go_to(uint idx) {
    if (idx > actions.size())
        throw out_of_range("MenuLeaf::go_to(): index greater than available actions");

    actions.at(idx).second();
}

void MenuLeaf::go_back() {
    parent->go_back();
}

void MenuLeaf::add_action(const string& name, void (*action)()) {
    actions.push_back(std::make_pair(name, action));
}
