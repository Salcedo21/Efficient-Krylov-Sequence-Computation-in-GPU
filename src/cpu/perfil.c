#ifdef _WIN32
  #include <windows.h>
  #include <psapi.h>
  #include "perfil.h"

/* Retorna el número acumulado de fallos de página del proceso usando la API de Windows. */
long leer_minflt(void) {
    // En Windows, PageFaultCount agrupa fallos menores y mayores
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        return (long)pmc.PageFaultCount;
    return 0;
}

/* Retorna 0 en Windows: la API no distingue fallos de página mayores de los menores. */
long leer_majflt(void) {
    // Windows no expone fallos mayores por separado; se reporta cero
    return 0;
}

/* Retorna el tiempo de CPU consumido por el proceso (usuario + kernel) en microsegundos. */
long long leer_cpu_us_proceso(void) {
    FILETIME creation, exit, kernel, user;
    if (!GetProcessTimes(GetCurrentProcess(), &creation, &exit, &kernel, &user))
        return 0;

    // Convertir los FILETIME (unidades de 100 ns) a un entero de 64 bits
    ULARGE_INTEGER u, k;
    u.LowPart = user.dwLowDateTime;   u.HighPart = user.dwHighDateTime;
    k.LowPart = kernel.dwLowDateTime; k.HighPart = kernel.dwHighDateTime;

    // Dividir por 10 para pasar de unidades de 100 ns a microsegundos
    return (long long)((u.QuadPart + k.QuadPart) / 10);
}

#else
  #include <stdio.h>
  #include <unistd.h>
  #include <sys/resource.h>
  #include "perfil.h"

/* Retorna el número acumulado de fallos de página menores del proceso. */
long leer_minflt(void) {
    // Leer estadísticas de uso de recursos del proceso actual
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_minflt;
}

/* Retorna el número acumulado de fallos de página mayores del proceso. */
long leer_majflt(void) {
    // Leer estadísticas de uso de recursos del proceso actual
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_majflt;
}

/* Retorna el tiempo de CPU consumido por el proceso (usuario + kernel) en microsegundos. */
long long leer_cpu_us_proceso(void) {
    // Leer /proc/self/stat para obtener utime y stime en ticks de reloj
    FILE *f = fopen("/proc/self/stat", "r");
    if (!f) return 0;

    unsigned long utime = 0, stime = 0;
    int pid; char comm[256]; char state;
    int ppid, pgrp, session, tty_nr, tpgid;
    unsigned int flags;
    unsigned long minflt, cminflt, majflt, cmajflt;
    fscanf(f, "%d %255s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu",
           &pid, comm, &state, &ppid, &pgrp, &session, &tty_nr, &tpgid,
           &flags, &minflt, &cminflt, &majflt, &cmajflt, &utime, &stime);
    fclose(f);

    // Convertir ticks a microsegundos usando la frecuencia del reloj del sistema
    long clk_tck = sysconf(_SC_CLK_TCK);
    if (clk_tck <= 0) return 0;
    return (long long)((utime + stime) * 1000000LL / clk_tck);
}
#endif
