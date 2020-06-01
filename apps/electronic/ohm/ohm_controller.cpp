#include "ohm_controller.h"
#include <assert.h>
#include <kandinsky/color.h>


namespace Electronic {

ohmLawController::ohmLawController(Responder * parentResponder) :
  ViewController(parentResponder)
{
}

View * ohmLawController::view() {
  return &m_ohmLawView;
}


ohmLawController::ohmLawView::ohmLawView() :
  View()
{}

void ohmLawController::ohmLawView::drawRect(KDContext * ctx, KDRect rect) const {
  ctx->fillRect(KDRect(0, 0, bounds().width(), bounds().height()), KDColorBlack);
}

void ohmLawController::ohmLawView::reload() {
  markRectAsDirty(bounds());
}

int ohmLawController::ohmLawView::numberOfSubviews() const {
  return 0;
}

View * ohmLawController::ohmLawView::subviewAtIndex(int index) {
  assert(false);
}

void ohmLawController::ohmLawView::layoutSubviews(bool force) {
}

void ohmLawController::didBecomeFirstResponder() {
}

bool ohmLawController::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::Back) {
    StackViewController * stack = (StackViewController *)(parentResponder());
    stack->pop();
    return true;
  }
  return false;
}

}
