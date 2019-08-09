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
      Device(std::string name, Cost bac, Cost eac, double cap,
        unsigned int veclen)
        : ID(next_id++), NAME(name), BAC(bac), EAC(eac), CAPACITY(cap),
        VECTOR_LENGTH(veclen) {};

      // copy constructor, copy has unique ID
      Device(const Device& source)
      : ID(next_id++), NAME(source.NAME), BAC(source.BAC), EAC(source.EAC),
      CAPACITY(source.CAPACITY), VECTOR_LENGTH(source.VECTOR_LENGTH) {};

      DevID getID() { return ID; }
      std::string getName() { return NAME; }
      Cost getBasicAccessCost(const unsigned int N) { return N * BAC; }
      Cost getExpensiveAccessCost(const unsigned int N) { return N * EAC; }
      double getCapacity() { return CAPACITY; }
      unsigned int getVectorLength() { return VECTOR_LENGTH; }
  };
}

#endif