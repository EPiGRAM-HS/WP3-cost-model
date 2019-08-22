#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include "typedefs.h"

namespace CostModel {
  class Device {
    private:
      static DevID next_id;
      const DevID ID;
      const std::string NAME;
      const Cost BAC;
      const Cost EAC;
      const double CAPACITY;
      const unsigned int VECTOR_LENGTH;

    public:
      Device() = delete;
      Device(void *) : ID(0), NAME(std::string("NULL")), BAC(0), EAC(0),
        CAPACITY(0.0), VECTOR_LENGTH(0) {};
      Device(std::string name, Cost bac, Cost eac, double cap,
        unsigned int veclen)
        : ID(next_id++), NAME(name), BAC(bac), EAC(eac), CAPACITY(cap),
        VECTOR_LENGTH(veclen) {};

      // copy constructor, copy has unique ID
      Device(const Device& source)
      : ID(next_id++), NAME(source.NAME), BAC(source.BAC), EAC(source.EAC),
      CAPACITY(source.CAPACITY), VECTOR_LENGTH(source.VECTOR_LENGTH) {};

      bool isNull() const { return ID == 0; }
      DevID getID() const { return ID; }
      std::string getName() const { return NAME; }
      Cost getBasicAccessCost(const unsigned int N) const { return N * BAC; }
      Cost getExpensiveAccessCost(const unsigned int N) const {return N * EAC;}
      double getCapacity() const { return CAPACITY; }
      unsigned int getVectorLength() const { return VECTOR_LENGTH; }
  };
}

#endif
