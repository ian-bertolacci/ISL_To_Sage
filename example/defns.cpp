#include <string>
#include <iostream>

template<typename T>
class AbstractClass {
  public:
    AbstractClass();
    virtual T will_implement( T thing);
    virtual T wont_implement_1( T thing ) = 0;
    /* 98 more declarations */
    virtual T wont_implement_100( T thing ) = 0;
};

class ConcreteClass : public AbstractClass<std::string> {
  public:
    ConcreteClass();
    std::string wont_implement_1( std::string thing );
    /* 98 more declarations */
    std::string wont_implement_100( std::string thing );
};

// Implement the things we want across all subclasses of AbstractClass
template<typename T>
AbstractClass<T>::AbstractClass(){
  std::cout << "AbstractClass::AbstractClass" << std::endl;
}

template<typename T>
T AbstractClass<T>::will_implement( T thing ){
  std::cout << "AbstractClass::will_implement" << std::endl;
  return thing;
}

// Implement methods for ConcreteClass
ConcreteClass::ConcreteClass(){
  std::cout << "ConcreteClass::ConcreteClass" << std::endl;
}

// Implement the unimplemented AbstractClass methods for ConcreteClass
std::string ConcreteClass::wont_implement_1( std::string thing ){
  std::cout << "ConcreteClass::wont_implement_1" << std::endl;
  return this->will_implement( thing );
}

/* 98 more definitions */

// Implement the unimplemented AbstractClass methods for ConcreteClass
std::string ConcreteClass::wont_implement_100( std::string thing ){
  std::cout << "ConcreteClass::wont_implement_100" << std::endl;
  return this->will_implement( thing );
}

int main(){

  ConcreteClass cc;
  std::cout << "+ " << cc.will_implement( std::string("cc.will_implement") ) << std::endl;
  std::cout << "+ " << cc.wont_implement_1( std::string("cc.wont_implement_1") ) << std::endl;
  std::cout << "+ " << cc.wont_implement_100( std::string("cc.wont_implement_100") ) << std::endl;

  return 0;
}
