#include "cache.h"

namespace Ion {
namespace Device {
namespace Cache {

using namespace Regs;

void invalidateDCache() {
  CM4.CSSELR()->set(0);
  dsb();

  // Associativity = 6

  uint32_t sets = CM4.CCSIDR()->getNUMSETS();
  do {
    uint32_t ways = CM4.CCSIDR()->getASSOCIATIVITY();
    do {
      class CM4::DCISW dcisw;
      dcisw.setSET(sets);
      dcisw.setWAY(ways);
      CM4.DCISW()->set(dcisw);
    } while (ways-- != 0);
  } while (sets-- != 0);

  dsb();
}

void enableDCache() {
  invalidateDCache();
  CM4.CCR()->setDC(true);
  dsb();
  isb();
}

void invalidateICache() {
  dsb();
  isb();
  CM4.ICIALLU()->set(0);
  dsb();
  isb();
}

void enableICache() {
  invalidateICache();
  CM4.CCR()->setIC(true);
  dsb();
  isb();
}

}
}
}
