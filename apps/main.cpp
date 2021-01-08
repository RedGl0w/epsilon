#include "apps_container.h"
#include "global_preferences.h"
#include <poincare/init.h>
#include <poincare/exception_checkpoint.h>
#include <malloc.h>
#include "shared/poincare_helpers.h"

#define MAIN 0 // 0 for dummy main, 1 for classical main, 2 for poincare REPL
#if MAIN==2

void ion_main(int argc, const char * const argv[]) {
  // Initialize the backlight
  Ion::Backlight::init();
  while (1) {
    Ion::Display::pushRectUniform(KDRect(0,0,10,10), KDColorRed);
    Ion::Timing::msleep(100);
    Ion::Display::pushRectUniform(KDRect(0,0,10,10), KDColorBlue);
    Ion::Timing::msleep(100);
  }
}

#elif MAIN==1


void ion_main(int argc, const char * const argv[]) {
  // Initialize Poincare::TreePool::sharedPool
  Poincare::Init();

#if EPSILON_GETOPT
  for (int i=1; i<argc; i++) {
    if (argv[i][0] != '-' || argv[i][1] != '-') {
      continue;
    }
    /* Option should be given at run-time:
     * $ ./epsilon.elf --language fr
     */
    if (strcmp(argv[i], "--language") == 0 && argc > i+1) {
      const char * requestedLanguageId = argv[i+1];
      if (strcmp(requestedLanguageId, "none") == 0) {
        continue;
      }
      for (int j = 0; j < I18n::NumberOfLanguages; j++) {
        if (strcmp(requestedLanguageId, I18n::translate(I18n::LanguageISO6391Names[j])) == 0) {
          GlobalPreferences::sharedGlobalPreferences()->setLanguage((I18n::Language)j);
          break;
        }
      }
      continue;
    }
    /* Option should be given at run-time:
     * $ ./epsilon.elf --[app_name]-[option] [arguments]
     * For example:
     * $ make -j8 PLATFORM=emscripten EPSILON_APPS=code
     * $ ./epsilon.elf --code-script hello_world.py:print("hello") --code-lock-on-console
     */
    const char * appNames[] = {"home", EPSILON_APPS_NAMES};
    for (int j = 0; j < AppsContainer::sharedAppsContainer()->numberOfApps(); j++) {
      App::Snapshot * snapshot = AppsContainer::sharedAppsContainer()->appSnapshotAtIndex(j);
      int cmp = strcmp(argv[i]+2, appNames[j]);
      if (cmp == '-') {
        snapshot->setOpt(argv[i]+2+strlen(appNames[j])+1, argv[i+1]);
        break;
      }
    }
  }
#endif
  AppsContainer::sharedAppsContainer()->run();
}

#else

using namespace Poincare;

void ion_main(int argc, const char * const argv[]) {
  Ion::Console::writeLine("Simple poincare REPL");
  Poincare::Init();
  Poincare::ExceptionCheckpoint ecp;
  char * string = (char*) malloc(2048); // Max input length
  char * buffer = (char*) malloc(2048);
  if (ExceptionRun(ecp)) {
    do {
      Ion::Console::writeLine("> ", false);
      Ion::Console::readLine(string, 2048);
      Expression outputs[] = {Expression(), Expression()};
      Context * sharedCtxt = AppsContainer::sharedAppsContainer()->globalContext();
      Expression e = Expression::Parse(string, sharedCtxt);
      if (e.isUninitialized()) {
        Ion::Console::writeLine("Parsing error");
        continue;
      }
      Shared::PoincareHelpers::ParseAndSimplifyAndApproximate(string, &(outputs[0]), &(outputs[1]), sharedCtxt, Poincare::ExpressionNode::SymbolicComputation::ReplaceAllSymbolsWithDefinitionsOrUndefined);
      outputs[0].serialize(buffer, 2048);
      Ion::Console::writeLine("Simplify : ", false);
      Ion::Console::writeLine(buffer);
      outputs[1].serialize(buffer, 2048);
      Ion::Console::writeLine("Approximate : ", false);
      Ion::Console::writeLine(buffer);
    } while(strcmp(string, "exit"));
  } else {
    Poincare::Tidy();
    Ion::Console::writeLine("Poincare exception. Exiting...");
  }
  free(string);
  free(buffer);
}

#endif
