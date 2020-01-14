//
//  landmark.cpp
//  hw3p1
//
//  Created by Sadhana Vadrevu on 2/11/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

class Landmark {
public:
    Landmark(string nm) {
        m_name = nm;
    }
    
    string name() const { //name is not virtual beacause it is never redefined
        return m_name;
    }
    
    virtual string color() const { //color is virtual because it is redefined in at least one base class
        return "yellow";
    }
    
    virtual string icon() const = 0; //icon is pure virtual because it has no function in the Landmark class
    
    virtual ~Landmark() {} //destructor is always virtual in the base class
    
private:
    string m_name; //member variable of all Landmarks
};

class Hotel: public Landmark { //Hotel is a type of landmark
public:
    Hotel(string nm): Landmark(nm) {}
    
    string icon() const {
        return "bed";
    }
    
    ~Hotel() {
        cout << "Destroying the hotel " << name() << "." << endl;
    }
};

class Restaurant: public Landmark { //Restaurant is a type of Landmark
public:
    Restaurant(string nm, int size): Landmark(nm) {
        if (size >= 40)
            m_icon = "large knife/fork";
        else
            m_icon = "small knife/fork";
    }
    
    string icon() const {
        return m_icon;
    }
    
    ~Restaurant() {
        cout << "Destroying the restaurant " << name() << "." << endl;
    }
    
private:
    string m_icon; //icon is not the same for all restaurants
};

class Hospital: public Landmark { //Hospital is a type of landmark
public:
    Hospital(string nm): Landmark(nm) {}
    
    string color() const {
        return "blue";
    }
    
    string icon() const {
        return "H";
    }
    
    ~Hospital() {
        cout << "Destroying the hospital " << name() << "." << endl;
    }
};
