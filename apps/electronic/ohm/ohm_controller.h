#ifndef ELECTRONIC_OHM_LAW_CONTROLLER_H
#define ELECTRONIC_OHM_LAW_CONTROLLER_H

#include <escher.h>
#include "../electronic_law.h"

namespace Electronic {

class ohmLawController : public electronicLaw, public ViewController {
  public:
    ohmLawController(Responder * parentResponder);
    View * view() override;
    bool handleEvent(Ion::Events::Event event) override;
    void didBecomeFirstResponder() override;
    const I18n::Message lawName() override {return I18n::Message::OhmLaw; }

  class ohmLawView : public View {
    public:
      ohmLawView();
      void drawRect(KDContext * ctx, KDRect rect) const override;
      void reload();
      int numberOfSubviews() const override;
      View * subviewAtIndex(int index) override;
    private:
      void layoutSubviews(bool force = false) override;
  };
  private:
      ohmLawView m_ohmLawView;
    };
}

#endif
