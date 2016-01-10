//
// Created by Tobias on 09.01.2016.
//

#include "Menu.hpp"

// INTERACTIVE IMPLEMENTATION

Interactive::Interactive(const string& t) : title(t) {}

Interactive::~Interactive() {}

string Interactive::get_title() const {
    return title;
}

// MENU IMPLEMENTATION

Menu::Menu(const string& t, Menu* p) : Interactive(t), submenus(), active(0), parent(p) {}

Menu::~Menu() {
    for (auto sub : submenus)
        delete sub;
    submenus.clear();
}

void Menu::enter() {
    active = 0;

    for (uint i = 0U; i < submenus.size(); ++i)
        print_option(i + 1, submenus.at(i)->get_title());
}

void Menu::execute(uint idx) {
    if (!is_active()) {
        active->execute(idx);
        return;
    }

    if (idx == 0 || idx > submenus.size())
        throw out_of_range("Menu::go_to(): no menu entry found for " + idx);

    active = submenus.at(idx - 1);
    active->enter();
}

void Menu::move_up() {
    if (active != 0)
        active->move_up();
    else
        parent->enter();
}

void Menu::add_submenu(Interactive* sub) {
    submenus.push_back(sub);
}

const vector<Interactive*>& Menu::get_submenus() const {
    return submenus;
}

bool Menu::is_active() const {
    return active == 0;
}

void Menu::print_option(uint idx, const string& str) const {
    cout << "[" << idx << "] " << str << std::endl;
}

// BIDIRECTIONALMENU IMPLEMENTATION

BidirectionalMenu::BidirectionalMenu(const string& t, Menu* m) : Menu(t, m) {}

void BidirectionalMenu::enter() {
    Menu::enter();
    cout << std::endl;
    print_option(get_submenus().size() + 1, "Back");
}

void BidirectionalMenu::execute(uint idx) {
    if (is_active() && idx == get_submenus().size() + 1)
        move_up();
    else
        Menu::execute(idx);
}
