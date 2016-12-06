#ifndef SHIPFACTORY_H_INCLUDED
#define SHIPFACTORY_H_INCLUDED

class ShipFactory
{
public:
    ~ShipFactory()
    {
    }

    static ShipFactory *getInstance()
    {
        if (!instance)
          instance = new ShipFactory;
        return instance;
    }

private:
    static ShipFactory *instance;

    ShipFactory()
    {
    }
};

ShipFactory* ShipFactory::instance;

#endif // SHIPFACTORY_H_INCLUDED
