#ifndef CPU_C_H
#define CPU_C_H

class CPU {
private:
    static void PrintTaskAndMemoryInfo(void *pvParameters);
    // static void CPU_task(void *pvParameters); // Uncomment if you want to use this function
public:
    CPU();
};

#endif
