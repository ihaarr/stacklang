#ifndef RGR2_POLYNOMINAL_H
#define RGR2_POLYNOMINAL_H

#include <iostream>

using uint = unsigned int;
using Predicate = double (*)(double, double);

class Polynominal
{
public:
    struct Monominal
    {
    public:
        Monominal(const uint degree_, const double coeff_) : degree(degree_), coeff(coeff_) {}
        friend std::ostream& operator<<(std::ostream& os, const Monominal& obj);
    public:
        uint degree;
        double coeff;
        Monominal* prev;
        Monominal* next;
    };
    struct Iterator
    {
        Iterator (Monominal* ptr_ = nullptr) : ptr(ptr_) {}
        Monominal& operator*() { return *ptr; }
        Monominal* operator->() { return ptr; }
        Monominal* operator++()
        {
            ptr = ptr->next;
            return ptr;
        }
        Monominal* operator++(int)
        {
            Monominal* old = ptr;
            ptr = ptr->next;
            return old;
        }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return !(operator==(other)); }
    private:
        Monominal* ptr;
    };
public:
    Polynominal();
    Polynominal(const Polynominal& other);
    ~Polynominal();
    void Push(const uint degree_, const double coeff_);
    uint MaxDegree() const;
    double GetValue(const double x);
    Polynominal Derivative() const;
    Iterator begin();
    const Iterator begin() const;
    Iterator end();
    const Iterator end() const;
    static Polynominal CalculateTwo(const Polynominal& s1, const Polynominal& s2, Predicate f);

    bool operator==(const Polynominal& other) const;
    bool operator!=(const Polynominal& other) const;
    friend std::ostream& operator<<(std::ostream& os_, const Polynominal& obj_);
    friend std::istream& operator>>(std::istream& in, Polynominal& obj);
    Polynominal& operator=(const Polynominal& other);
    Polynominal operator+(const Polynominal& other) const;
    Polynominal operator-(const Polynominal& other) const;
    Polynominal operator*(const Polynominal& other) const;
    Polynominal operator/(const Polynominal& other) const;
    Polynominal operator%(const Polynominal& other) const;
    double& operator[](uint degree_);
    const double& operator[](uint degree_) const;
private:
    Iterator find(const double degree_) const;
    void release();
    void from(const Polynominal& other);
private:
    size_t size;
    Monominal* tail;
    Monominal* head;
};


#endif //RGR2_POLYNOMINAL_H
