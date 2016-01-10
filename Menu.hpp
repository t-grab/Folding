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
#include <stdexcept>
using std::runtime_error;
using std::out_of_range;
#include <sstream>
typedef std::stringstream sstream;
#include <iomanip>

typedef unsigned int uint;

class Interactive {
public:
    explicit Interactive(const string&);
    virtual ~Interactive();

    virtual string get_title() const;

    virtual void enter() = 0;
    virtual void move_down(uint) = 0;
    virtual void move_up() = 0;
private:
    string title;
};

class Menu : public Interactive {
public:
    Menu(const string&, Menu*);
    ~Menu();

    void enter();
    void move_down(uint);
    void move_up();

    void add_submenu(Interactive*);
    const vector<Interactive*>& get_submenus() const;
    bool is_active() const;
protected:
    void print_option(uint, const string&) const;
private:
    vector<Interactive*> submenus;
    Interactive* active;
    Menu* parent;
};

class BidirectionalMenu : public Menu {
public:
    BidirectionalMenu(const string&, Menu*);

    void enter();
    void move_down(uint);
};

template<typename T>
class Action : public Interactive {
public:
    Action(const string& t, Menu* p, T a) : Interactive(t), parent(p), action(a) {}

    void enter() {
        action();
        move_up();
    }

    void move_down(uint) {}

    void move_up() {
        parent->enter();
    }
private:
    T action;
    Menu* parent;
};

template<typename T>
Action<T>* make_action(const string& t, Menu* p, T a) {
    return new Action<T>(t, p, a);
}

template<typename T>
class Parameter : public Interactive {
public:
    Parameter(const string& t, Menu* p, T& val) : Interactive(t), parent(p), value(val) {}

    string get_title() const {
        sstream title;
        title << Interactive::get_title() << "\t\t";
        title << value;

        return title.str();
    }

    void enter() {
        cout << "Enter new value for parameter " << Interactive::get_title() << ": ";
        T val;
        cin >> val;

        if (!cin)
            throw runtime_error("Paramter::enter(): error reading value from stdin");

        value = val;
        move_up();
    }

    void move_down(uint) {}

    void move_up() {
        parent->enter();
    }
private:
    T& value;
    Menu* parent;
};

#endif //FOLDING_CONSOLEMENU_HPP
