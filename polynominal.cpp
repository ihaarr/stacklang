#include "polynominal.h"

static uint max(uint a, uint b) { return a > b ? a : b;}
static size_t max(size_t a, size_t b) { return a > b ? a : b;}
static double degree(double value, size_t degree)
{
    double ret = 1;
    for(int i = 0; i < degree; i++)
    {
        ret *= value;
    }
    return ret;
}

Polynominal::Polynominal(const Polynominal& other)
{
    from(other);
}
Polynominal::Polynominal() : size(0), tail(nullptr), head(nullptr)
{

}
Polynominal::~Polynominal()
{
    release();
}
const Polynominal::Iterator Polynominal::begin() const
{
    return Iterator(tail);
}
const Polynominal::Iterator Polynominal::end() const
{
    return Iterator(nullptr);
}
Polynominal::Iterator Polynominal::begin()
{
    return Iterator(tail);
}
Polynominal::Iterator Polynominal::end()
{
    return Iterator(nullptr);
}
Polynominal::Iterator Polynominal::find(const double degree_) const
{
    for(auto& monimal : *this)
    {
        if(monimal.degree == degree_)
            return Iterator(&monimal);
    }
    return Iterator(nullptr);
}
void Polynominal::Push(const uint degree_, const double coeff_)
{
    Iterator it = find(degree_);
    if(it != end())
    {
        it->coeff += coeff_;
        return;
    }
    Monominal* node = new Monominal(degree_, coeff_);
    if(size == 0)
    {
        tail = node;
        head = node;
        node->next = nullptr;
        node->prev = nullptr;
        size++;
        return;
    }
    node->prev = head;
    head->next = node;
    head = node;
    size++;
}
Polynominal Polynominal::Derivative() const
{
    if(size == 0)
        return Polynominal();

    Polynominal ret;
    Monominal* current = tail;
    for(int i = 0; i < size; ++i)
    {
        ret.Push(current->coeff - 1, current->coeff * current->degree);
        current = current->next;
    }
    return ret;
}
std::ostream& operator<<(std::ostream& os_, const Polynominal::Monominal& obj_)
{
    if(obj_.coeff >= 0)
        os_ << '+';

    os_ << obj_.coeff << "x^" << obj_.degree;
    return os_;
}
std::ostream& operator<<(std::ostream& os_, const Polynominal& obj_)
{
    if(obj_.size == 0)
    {
        os_ << 0;
    }
    else
    {
        os_ << '[';
        for(uint i = 0; i <= obj_.MaxDegree(); ++i)
        {
            Polynominal::Iterator it = obj_.find(i);
            if(it != obj_.end())
            {
                std::cout << *it;
            }
        }
        os_ << ']';
    }
    return os_;
}
void Polynominal::release()
{
    Monominal* current = tail;
    Monominal* prev = current;
    while(current)
    {
        prev = current;
        prev->next = nullptr;
        prev->prev = nullptr;
        current = current->next;
        delete prev;
    }
}
void Polynominal::from(const Polynominal &other)
{
    size = 0;
    tail = head = nullptr;
    if(other.size == 0)
        return;

    Monominal* other_current = other.tail;
    while(other_current)
    {
        Push(other_current->degree, other_current->coeff);
        other_current = other_current->next;
    }
}
double& Polynominal::operator[](uint degree_)
{
    for(auto& monominal : *this)
    {
        if(monominal.degree == degree_)
            return monominal.coeff;
    }
    throw 1;
}
const double &Polynominal::operator[](uint degree_) const
{
    for(const auto& monominal : *this)
    {
        if(monominal.degree == degree_)
            return monominal.coeff;
    }
    throw 1;
}
bool Polynominal::operator==(const Polynominal& other_) const
{
    if(size != other_.size)
        return false;

    for(const auto& monominal : *this)
    {
        Iterator it = other_.find(monominal.degree);
        if(it == other_.end())
            return false;
        if(monominal.coeff != it->coeff)
            return false;
    }
    return true;
}
uint Polynominal::MaxDegree() const
{
    uint maxDegree = 0;
    Monominal* current = tail;
    for(size_t i = 0; i < size; ++i)
    {
        maxDegree = max(maxDegree, current->degree);
        current = current->next;
    }
    return maxDegree;
}
Polynominal Polynominal::CalculateTwo(const Polynominal &s1, const Polynominal &s2, Predicate f)
{
    if(s2.size == 0 && s1.size == 0)
        return Polynominal();

    if(s2.size == 0)
        return Polynominal(s1);

    Polynominal ret;
    const Polynominal* big;
    const Polynominal* small;
    if(s1.MaxDegree() >= s2.MaxDegree())
    {
        big = &s1;
        small = &s2;
    }
    else
    {
        big = &s2;
        small = &s1;
    }
    for(const auto& monominal : *big)
    {
        Iterator it = small->find(monominal.degree);
        double coeff = monominal.coeff;
        if(it != small->end())
            coeff = f(coeff, it->coeff);
        ret.Push(monominal.degree, coeff);
    }
    return ret;
}
Polynominal Polynominal::operator-(const Polynominal &other) const
{
    return CalculateTwo(*this, other, [](double a, double b) -> double {
        return a - b;
    });
}
Polynominal Polynominal::operator+(const Polynominal& other) const
{
    return CalculateTwo(*this, other, [](double a, double b) -> double {
        return a + b;
    });
}
double Polynominal::GetValue(const double x)
{
    double result = 0;
    Monominal* current = tail;
    for(size_t i = 0; i < size; ++i)
    {
        result += current->coeff * degree(x, current->degree);
        current = current->next;
    }
    return result;
}
bool Polynominal::operator!=(const Polynominal &other) const
{
    return !operator==(other);
}
Polynominal Polynominal::operator*(const Polynominal& other) const
{
    Polynominal ret;
    for(const auto& monominal : *this)
    {
        for(const auto& monominalOther : other)
        {
            ret.Push(monominal.degree + monominalOther.degree, monominal.coeff * monominalOther.coeff);
        }
    }
    return ret;
}
Polynominal Polynominal::operator/(const Polynominal& other) const
{
    if(size < other.size || MaxDegree() < other.MaxDegree())
        return Polynominal();

    Polynominal ret, temp(*this), m, m2;
    uint maxDegreeTemp = temp.MaxDegree();
    uint maxDegreeOther = other.MaxDegree();
    for(uint i = 0; i < temp.MaxDegree(); ++i)
    {
        if(temp.find(i) == temp.end())
        {
            temp.Push(i, 0);
        }
    }

    m.Push(maxDegreeTemp, temp[maxDegreeTemp]);
    m.Push(maxDegreeTemp - 1, temp[maxDegreeTemp - 1]);
    ret.Push(maxDegreeTemp - maxDegreeOther, m[maxDegreeTemp] / other[maxDegreeOther]);
    m2.Push(maxDegreeTemp - maxDegreeOther, m[maxDegreeTemp] / other[maxDegreeOther]);
    --maxDegreeTemp;
    auto k = m2 * other;
    m = m - k;
    ret.Push(maxDegreeTemp - maxDegreeOther, m[maxDegreeTemp] / other[maxDegreeOther]);
    m2 = Polynominal();
    m2.Push(maxDegreeTemp - maxDegreeOther, m[maxDegreeTemp] / other[maxDegreeOther]);
    m.Push(maxDegreeTemp - 1, temp[maxDegreeTemp - 1]);
    --maxDegreeTemp;
    auto s = m2 * other;
    m = m - s;
    return ret;
}
Polynominal &Polynominal::operator=(const Polynominal &other)
{
    if(this == &other)
        return *this;

    release();
    from(other);
    return *this;
}
Polynominal Polynominal::operator%(const Polynominal &other) const
{
    return Polynominal();
}
std::istream &operator>>(std::istream &in, Polynominal &obj)
{
    return in;
}
