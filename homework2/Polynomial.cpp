#include "Polynomial.h"
#include <fstream>
#include <cmath>


#define MIN(a, b) ((a<b) ? (a) : (b))
#define MAX(a, b) ((a>b) ? (a) : (b))

using namespace std;

Polynomial::Polynomial()
  : min_d_(0),
    max_d_(0),
    n_(0),
    coefficients_(nullptr) {}

Polynomial::Polynomial(int min_d, int max_d, const int* coefficients)
  : min_d_(min_d),
    max_d_(max_d),
    n_(max_d - min_d + 1) {
  coefficients_ = new int [n_];
  for (int i = 0; i < n_; i++) {
    coefficients_[i] = coefficients[i];
  }
}

Polynomial::Polynomial(const Polynomial &other)
  : min_d_(other.min_d_),
    max_d_(other.max_d_),
    n_(other.n_) {
  coefficients_ = new int [n_];
  for (int i = 0; i < n_; i++) {
    coefficients_[i] = other.coefficients_[i];
  }
}

Polynomial &Polynomial::operator=(const Polynomial &other) {
  if (this == &other) {
    return *this;
  }
  delete []coefficients_;
  min_d_ = other.min_d_;
  max_d_ = other.max_d_;
  n_ = other.n_;
  coefficients_ = new int [n_];
  for (int i = 0; i < n_; i++) {
    coefficients_[i] = other.coefficients_[i];
  }
  return *this;
}

Polynomial::~Polynomial() {
  delete[] coefficients_;
}

Polynomial Polynomial::operator-() const {
  Polynomial res = *this;
  for (int i = 0; i < res.n_; i++) {
    res.coefficients_[i] *= -1;
  }
  return res;
}

bool operator==(const Polynomial &lhs, const Polynomial &rhs) {
  if (lhs.n_ == 0 && rhs.n_ == 0)
    return true;
  else if (!(lhs.n_ && rhs.n_))
    return false;
  for (int i = MIN(lhs.min_d_, rhs.min_d_);
           i < MAX(lhs.max_d_,rhs.max_d_); i++)
    if (lhs[i] != rhs[i])
      return false;
  return true;
}

bool operator!=(const Polynomial &lhs, const Polynomial &rhs) {
  return !(lhs == rhs);
}


Polynomial operator+(const Polynomial &lhs, const Polynomial &rhs) {
  if (lhs == Polynomial()) {
    return rhs;
  }
  if (rhs == Polynomial()) {
    return lhs;
  }

  return Polynomial(lhs) += rhs;
}


Polynomial operator-(const Polynomial &lhs, const Polynomial &rhs) {
  if (lhs == Polynomial()) {
    return rhs;
  }
  if (rhs == Polynomial()) {
    return lhs;
  }

  return Polynomial(lhs) -= rhs;
}

Polynomial operator*(const Polynomial &lhs, const Polynomial &rhs) {
  return Polynomial(lhs) *= rhs;
}

Polynomial operator*(const Polynomial& p, int value) {
  if (value == 1)
    return p;
  return Polynomial(p) *= value;
}

Polynomial operator*(int value, const Polynomial& p) {
  if (value == 1)
    return p;
  return Polynomial(p) *= value;
}

Polynomial Polynomial::operator/(int value) const {
  if (value == 1)
    return *this;
  return Polynomial(*this) /= value;
}

//fixed function
Polynomial& Polynomial::getValueOperationResult(const int& value,
                                                char operation) {
  int *coefficients = new int[this->n_];
  for (int i = 0; i < this->n_; i++) {
    if (operation == '*')
      coefficients[i] = this->coefficients_[i] * value;
    else
      coefficients[i] = this->coefficients_[i] / value;
  }
  for (int i = 0; i < this->n_; i++) {
    if (coefficients[i] == 0)
      this->min_d_++;
    else
      break;
  }
  for (int i = this->n_ - 1; i >= 0; i--) {
    if (coefficients[i] == 0)
      this->max_d_--;
    else
      break;
  }
  this->n_ = this->max_d_ - this->min_d_ + 1;
  int *new_coefficients = new int[this->n_];
  for (int i = this->min_d_; i <= this->max_d_; i++)  {
    new_coefficients[i - this->min_d_] = coefficients[i - this->min_d_];
  }
  this->coefficients_ = new_coefficients;
  return *this;
}

Polynomial& Polynomial::operator*=(int value) {
  return getValueOperationResult(value, '*');
}

Polynomial& Polynomial::operator/=(int value) {
  if (value == 0)
    throw invalid_argument("Error : division by zero");
  return getValueOperationResult(value, '/');
}

Polynomial& Polynomial::getResultOfAddOrSubOperation(const Polynomial &other,
                                                     int operation) {
  for (int i = MIN(min_d_, other.min_d_); i <= MAX(max_d_, other.max_d_);
                                             i++)
    if (i <= other.max_d_ && i >= other.min_d_)
      (*this)[i] += operation * other[i];
  return *this;
}

Polynomial& Polynomial::operator+=(const Polynomial &other) {
  return getResultOfAddOrSubOperation(other, 1);
}

Polynomial& Polynomial::operator-=(const Polynomial &other) {
  return getResultOfAddOrSubOperation(other, -1);
}

Polynomial &Polynomial::operator*=(const Polynomial &other) {
  if (*this == Polynomial() || other == Polynomial()) {
    return * new Polynomial();
  }
  auto copy = *this;
  for (int i = this->min_d_; i <= this->max_d_; i++)
      (*this)[i] = 0;

  for (int i = copy.min_d_; i <= copy.max_d_; i++)
    for (int j = other.min_d_; j <= other.max_d_; j++)
      (*this)[i + j] += copy[i] * other[j];

  return *this;
}

int& Polynomial::operator[](int i) {
  if (i < min_d_) {
    int *new_coeff = new int [max_d_ - i + 1];
    for (int j = 0; j < max_d_ - i + 1; j++) {
      new_coeff[j] = 0;
    }
    for (int j = 0; j < n_; j++) {
      new_coeff[j + max_d_ - i + 1 - n_] = coefficients_[j];
    }
    min_d_ = i;
    n_ = max_d_ - min_d_ + 1;
    coefficients_ = new_coeff;
  } else if (i > max_d_) {
    int *new_coeff = new int [i - min_d_ + 1];
    for (int j = 0; j < i - min_d_ + 1; j++) {
      new_coeff[j] = 0;
    }
    for (int j = 0; j < n_; j++) {
      new_coeff[j] = coefficients_[j];
    }
    max_d_ = i;
    n_ = max_d_ - min_d_ + 1;
    coefficients_ = new_coeff;
  }
  return this->coefficients_[i - min_d_];
}

int Polynomial::operator[](int i) const {
  if (i > max_d_ || i < min_d_) {
    return 0;
  }
  return this->coefficients_[i - min_d_];
}

std::ostream &operator<<(std::ostream &out, const Polynomial &polynomial) {
  if (polynomial.n_ == 0) {
    out << 0;
    return out;
  }
  int i = polynomial.n_ - 1; // index of coefficients NB: polnomial.coefficiends_[i] != 0
  int j = polynomial.max_d_; // current power

  if (j != 0) {
    if (polynomial.coefficients_[i] != 1 && polynomial.coefficients_[i] != -1) {
      out << polynomial.coefficients_[i];
    } else if (polynomial.coefficients_[i] == -1){
      out << "-";
    }

    if (j == 1) {
      out << "x";
    } else {
      out << "x^" << j;
    }
  } else {
    if (polynomial.coefficients_[i] > 0)
      out << "+" << polynomial.coefficients_[i];
    else
      out << polynomial.coefficients_[i];
  }

  for (i--, j--; i >= 0; i--, j--) {
    if (polynomial.coefficients_[i]) {
      if (j != 0) {
        if (polynomial.coefficients_[i] != 1 && polynomial.coefficients_[i] != -1) {
          if (polynomial.coefficients_[i] > 0)
            out << "+" << polynomial.coefficients_[i];
          else
            out << polynomial.coefficients_[i];
        } else if (polynomial.coefficients_[i] == 1){
          out << "+";
        } else {
          out << "-";
        }
        if (j == 1) {
          out << "x";
        } else {
          out << "x^" << j;
        }
      } else {
        if (polynomial.coefficients_[i] > 0)
          out << "+" << polynomial.coefficients_[i];
        else
          out << polynomial.coefficients_[i];
      }
    }
  }
  return out;
}

std::istream &operator>>(std::istream &in, Polynomial &polynomial) {
  int min_d, max_d;
  in >> min_d >> max_d;
  int n = max_d - min_d + 1;
  int *coeff = new int[n];
  for (int i = 0; i < n; i++) {
    in >> coeff[i];
  }
  polynomial = Polynomial(min_d, max_d, coeff);
  return in;
}

double Polynomial::get(int value) {
  double res = 0;
  for (int i = n_ - 1, power = max_d_; i >= 0; i--, power--) {
    res += coefficients_[i] * pow(value + 0.0, power + 0.0);
  }
  return res;
}
