#include <dlfcn.h>

#include <cstdio>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  // Argument check
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <problem-number>" << std::endl;
    return 1;
  }

  // Process problem number
  int number;
  try {
    number = std::stoi(argv[1]);
  } catch (const std::exception&) {
    std::cerr << "Error: '" << argv[1] << "' is not a valid number" << std::endl;
    return 1;
  }

  // Zero-pad to 4 digits so `1` and `0001` both map to plugins/0001.so.
  // This is just for creating id to have the full id, for searching the
  // plugin/module/lc-question
  char id[5];
  std::snprintf(id, sizeof(id), "%04d", number);

  // Here we create the full file path. This is hardcoded and it currently
  // depends on how Makefile is building shared objects.
  std::string path = "build/plugins/" + std::string(id) + ".so";

  // dlopen loads the shared library at runtime. RTLD_NOW resolves all
  // symbols up front, so a broken plugin fails here instead of mid-call.
  // A null return means the .so doesn't exist (or failed to load).
  void* handle = dlopen(path.c_str(), RTLD_NOW);
  if (!handle) {
    std::cerr << "Error: no plugin for problem " << id << std::endl;
    return 1;
  }

  // dlsym looks up a symbol by name inside the loaded library. Here we ask
  // for `run_solution` — the uniform entry point every plugin exports.
  // dlerror() is called first to clear any prior error, because dlsym can
  // legitimately return null (for a symbol whose value is 0), so the only
  // reliable way to detect failure is to check dlerror() afterwards.
  // The reinterpret_cast turns the void* dlsym returns into a function
  // pointer we can actually call.
  dlerror();
  // function pointers yuh
  void (*run_solution)() = reinterpret_cast<void (*)()>(dlsym(handle, "run_solution"));
  const char* sym_err = dlerror();
  if (sym_err) {
    std::cerr << "Error: " << sym_err << std::endl;
    dlclose(handle);
    return 1;
  }

  // Invoke the plugin's entry point — this runs the Solution code for
  // problem NNNN, which lives entirely inside the .so.
  run_solution();

  // Release the library. Not strictly required at program exit, but good
  // hygiene and matters if main ever loads multiple plugins in one run.
  dlclose(handle);
  return 0;
}
