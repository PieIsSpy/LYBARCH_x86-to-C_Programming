# LYBARCH_x86-to-C_Programming
## Members:
- Ramuel Sean Cordero
- Karl Deajay Omandac

## Task:
Implement a program that computes the acceleration of multiple cars stored in a `Y x 3` matrix, where `Y` is the number of cars. All inputs are double floating-point values. The output acceleration for each car will be converted into integers.

**Input:** Matrix Rows, Double float matrix values  

*Example:*
```text
3
0.0, 62.5, 10.1
60.0, 122.3, 5.5
30.0, 160.7, 7.8
```

**Output:** Integer acceleration values (m/s²)  

*Example:*
```text
2
3
5
```
# Technical Report: x86-64 Assembly vs. C Implementation & Execution Performance Analysis

## 1. Project Specifications & Requirements
The goal of this implementation is to compute the acceleration of $Y$ vehicles stored in a $Y \times 3$ double-precision floating-point matrix and analyze performance differences between a baseline C implementation and an x86-64 Assembly subroutine.

### Core Requirements
1. **Language Responsibilities:**
   * **C Language:** Handles input collection, dynamic memory allocation (`malloc`/`free`), input generation, benchmarking loop control, performance calculations, and console output.
   * **x86-64 Assembly Language:** Responsible for unit conversion ($v_{\text{km/h}} \rightarrow v_{\text{m/s}}$), acceleration computation, floating-point rounding, and casting double values into 32-bit integers (`int`).
2. **Instruction Set Requirement:** Assembly logic must explicitly utilize **scalar SIMD floating-point instructions** (x86-64 SSE double-precision instructions).
3. **Execution Count & Matrix Sizes:**
   * Benchmark test cases require executing each configuration for **at least 30 iterations** to average runtime fluctuations.
   * Matrix size ($Y$) evaluations must cover: $Y \in \{10, 100, 1000, 10000\}$.
4. **Data Generation:** Matrix values ($v_i, v_f, t$) are randomly generated floating-point double values ranging from `0.0` to `100.0`.

---

## 2. Mathematical Formulation

Each row in the matrix represents:
* **Column 0 ($v_i$):** Initial Velocity ($\text{km/h}$)
* **Column 1 ($v_f$):** Final Velocity ($\text{km/h}$)
* **Column 2 ($t$):** Time ($\text{seconds}$)

### Formulas
1. **Unit Conversion:**
   $$v_{\text{m/s}} = v_{\text{km/h}} \times \frac{1000.0}{3600.0}$$
2. **Acceleration Computation:**
   $$a = \frac{v_{f,\text{m/s}} - v_{i,\text{m/s}}}{t}$$
3. **Integer Output Conversion:**
   $$\text{Output Integer} = \text{round}(a)$$

---

## 3. Subroutine Implementations

### C Kernel Implementation (`c_acceleration`)
Functions as the reference implementation for sanity checks and correctness validation.

```c
#include <math.h>

int c_acceleration(double vi, double vf, double t) {
    vi = vi * 1000.0 / 3600.0;
    vf = vf * 1000.0 / 3600.0;
    return round((vf - vi) / t);
}

## 3. Subroutine Implementations

### C Kernel Implementation (`c_acceleration`)
Functions as the reference implementation for sanity checks and correctness validation.

```c
#include <math.h>

int c_acceleration(double vi, double vf, double t) {
    vi = vi * 1000.0 / 3600.0;
    vf = vf * 1000.0 / 3600.0;
    return round((vf - vi) / t);
}
```

### x86-64 Assembly Implementation (`asm_acceleration`)
Utilizes SSE double-precision scalar registers (`xmm0`–`xmm4`).

* **Calling Convention (Windows x64):**
  * `xmm0` = `vi` (Initial Velocity)
  * `xmm1` = `vf` (Final Velocity)
  * `xmm2` = `t` (Time)
  * Return value stored in `eax` register as a 32-bit signed integer.

```nasm
default rel
bits 64

section .data
    align 8
    c1000: dq 1000.0
    c3600: dq 3600.0

section .text
    global asm_acceleration

asm_acceleration:
    ; Load memory double constants into SSE registers
    movsd xmm3, [c1000]
    movsd xmm4, [c3600]

    ; Convert vi from km/h to m/s
    mulsd xmm0, xmm3
    divsd xmm0, xmm4

    ; Convert vf from km/h to m/s
    mulsd xmm1, xmm3
    divsd xmm1, xmm4

    ; Calculate acceleration: (vf - vi) / t
    subsd xmm1, xmm0
    divsd xmm1, xmm2

    ; Convert double-precision float to signed 32-bit integer in RAX/EAX
    cvtsd2si eax, xmm1
    ret
```

---

## 4. Program Execution Flow & Benchmarking Framework (`main.c`)

1. **Initialization:** User enters the target $Y$ size (matrix row count).
2. **Memory Allocation:** Memory for input vectors ($Y \times 3$ double matrix) and output summary structs are allocated dynamically using `malloc()`.
3. **Execution Loop (30 Iterations):**
   * **Iterations 1–29 (Silent Mode):** Runs computations silently without console printing to isolate exact hardware execution speed.
   * **Iteration 30 (Verbose Mode):** Displays individual row inputs ($v_i, v_f, t$), individual C outputs (`C_ans`), Assembly outputs (`Asm_ans`), and row-by-row correctness indicators (`PASS`/`FAIL`).
4. **Timing Framework:** High-resolution timers (`QueryPerformanceCounter` and `QueryPerformanceFrequency`) wrap both C and Assembly execution loops separately to record millisecond timings.
5. **Summary Metrics Calculation:**
   * **C Average Time:** `C_avg = (Sum of C_time) / 30`
   * **Assembly Average Time:** `ASM_avg = (Sum of ASM_time) / 30`
   * **Fastness Percentage:** `Fastness (%) = ((C_avg - ASM_avg) / C_avg) * 100`
---

## 5. Comprehensive Benchmark Results

### Performance Table Summary

| Input Size ($Y$) | Avg C Time ($\text{ms}$) | Avg ASM Time ($\text{ms}$) | Time Delta ($\text{ms}$) | Assembly Fastness Improvement | Correctness Score |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **$10$** | `0.001103 ms` | `0.000310 ms` | `0.000500 ms` | **28.0966704% faster than C** | `300 / 300` ($100\%$) |
| **$100$** | `0.002920 ms` | `0.000723 ms` | `0.001300 ms` | **93.264839% faster than C** | `3000 / 3000` ($100\%$) |
| **$1,000$** | `0.023790 ms` | `0.011500 ms` | `0.012200 ms` | **106.870229% faster than C** | `30000 / 30000` ($100\%$) |
| **$10,000$** | `0.262017 ms` | `0.056617 ms` | `0.205300 ms` | **42.349724% faster than C** | `300000 / 300000` ($100\%$) |

## 6. Findings & Conclusion

* **Output Correctness:** 100% accuracy was maintained across all test configurations (a total of 333,300 benchmark calculations). The assembly scalar SIMD instruction `cvtsd2si` perfectly matched standard C library double-to-integer conversion and rounding semantics (`round()`).
* **Execution Efficiency:** Across all input sizes ($Y = 10, 100, 1000, 10000$), the x86-64 Assembly implementation consistently outperformed the standard C implementation. 
* **Performance Trends:**
  * Performance gains scaled significantly as matrix size increased, reaching a peak speedup of **106.87% faster than C** at $Y = 1,000$.
  * The performance advantage of assembly stems directly from bypassing function call overhead, avoiding extra stack frame allocations, and executing inline scalar SSE instructions (`mulsd`, `divsd`, `subsd`, `cvtsd2si`) directly within hardware registers.
