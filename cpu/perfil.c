#ifdef _WIN32
  #include <windows.h>
  #include <psapi.h>
  #include "perfil.h"

long leer_minflt(void) {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        return (long)pmc.PageFaultCount;
    return 0;
}

long leer_majflt(void) {
    return 0;
}

#else
  #include <sys/resource.h>
  #include "perfil.h"

long leer_minflt(void) {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_minflt;
}

long leer_majflt(void) {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_majflt;
}
#endif
