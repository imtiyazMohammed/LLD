#include <bits/stdc++.h>
using namespace std;

enum Size {SMALL, MEDIUM, LARGE};
enum Base {THIN_CRUST, CHEESE_BURST};
enum Toppings {OLIVES, CHEESE, JALAPENOS};

class PriceCatalog {
    unordered_map<int, int> sizePriceMap;
    unordered_map<int, int> basePriceMap;
    unordered_map<int, int> toppingsPriceMap;
    
    public:
    PriceCatalog() {
        sizePriceMap[SMALL] = 100;
        sizePriceMap[MEDIUM] = 150;
        sizePriceMap[LARGE] = 200;
        
        basePriceMap[THIN_CRUST] = 20;
        basePriceMap[CHEESE_BURST] = 40;

        toppingsPriceMap[OLIVES] = 10;
        toppingsPriceMap[CHEESE] = 15;
        toppingsPriceMap[JALAPENOS] = 12;
    }
    
    int getSizePrice(Size size) {
        return sizePriceMap[size];
    }
    
    int getBasePrice(Base base) {
        return basePriceMap[base];
    }
    
    int getToppingsPrice(Toppings topping) {
        return toppingsPriceMap[topping];
    }
};

class Pizza {
    int price;
    public:
    Pizza(Size size, Base base, unordered_map<Toppings, int>& toppings, PriceCatalog& prices) {
        this->price = 0;
        price += prices.getSizePrice(size);
        price += prices.getBasePrice(base);
        for(auto topping: toppings) {
            price += prices.getToppingsPrice(topping.first)*topping.second;
        }
    }
    
    int getPrice() {
        return this->price;
    }
};

class Order {
    vector<pair<Pizza, int>> pizzas;
    public:
    Order(vector<pair<Pizza, int>>& pizzas) {
        this->pizzas = pizzas;
    }
    
    void getOrderSummary() {
        int totalPizzas = 0;
        int totalPrice = 0;
        for(auto pizza: pizzas) {
            totalPizzas += pizza.second;
            totalPrice += pizza.first.getPrice()*pizza.second;
        }
        cout<<"Order Summary:"<<endl;
        cout<<"Total Pizzas: "<<totalPizzas<<endl;
        cout<<"Total Amount: "<<totalPrice<<endl;
    }
};

int main() {
    PriceCatalog prices;

    // First pizza: Medium Thin Crust with 2x OLIVES, 3x JALAPENOS
    unordered_map<Toppings, int> toppings1;
    toppings1[OLIVES] = 2;
    toppings1[JALAPENOS] = 3;
    Pizza pizza1(MEDIUM, THIN_CRUST, toppings1, prices);

    // Second pizza: Large Cheese Burst with 1x CHEESE
    unordered_map<Toppings, int> toppings2;
    toppings2[CHEESE] = 1;
    Pizza pizza2(LARGE, CHEESE_BURST, toppings2, prices);

    // Build the order with quantities
    vector<pair<Pizza, int>> pizzaList;
    pizzaList.push_back({pizza1, 2});  // 2 of pizza1
    pizzaList.push_back({pizza2, 1});  // 1 of pizza2

    // Create the order and print summary
    Order order(pizzaList);
    order.getOrderSummary();

    return 0;
}
