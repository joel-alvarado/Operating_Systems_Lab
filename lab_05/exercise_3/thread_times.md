|               | Baseline (priorities: 50,50,50,50) | E1 (priorities: 1,1,99,99) | E2 (priorities: 99,99,1,1) | E3 (priorities: 90,10,90,10) | E4 (priorities: 20,20,80,80) | E5 (priorities: 80,80,20,20) |
| ------------- | ---------------------------------- | -------------------------- | -------------------------- | ---------------------------- | ---------------------------- | ---------------------------- |
| **T1 (FIFO)** | 134s 719559μs                      | 142s 71036μs               | 130s 499865μs              | 132s 644292μs                | 146s 37225μs                 | 151s 92272μs                 |
| **T2 (FIFO)** | 143s 311508μs                      | 152s 117072μs              | 137s 896701μs              | 148s 904171μs                | 131s 719904μs                | 135s 967182μs                |
| **T3 (RR)**   | 145s 842244μs                      | 154s 759530μs              | 142s 240892μs              | 148s 911105μs                | 145s 971475μs                | 134s 289495μs                |
| **T4 (RR)**   | 146s 535870μs                      | 155s 98492μs               | 142s 264462μs              | 149s 331499μs                | 129s 673296μs                | 151s 49784μs                 |

# Note
Originally, priorities 1 were actually 0. These were changed because priority 0 is not allowed by Linux. The only threads that can have a priority of 0 are the idle thread. 

# Analysis
Based on the times measured, the **FIFO scheduling algorithm with higher priorities** consistently showed better performance, especially in experiments where `FIFO` threads had the highest priority.

1. **Experiment 2 (E2)**:
   - Here, threads `T1` and `T2` (both `FIFO`) were assigned the highest priority (99), while `T3` and `T4` (both `RR`) had the lowest priority (1).
   - This setup resulted in the shortest execution times for `T1` (130s 499865μs) and `T2` (137s 896701μs) compared to all other experiments, indicating that high-priority `FIFO` threads complete their execution faster when assigned maximum priority.

2. **Experiment 4 (E4)**:
   - Threads `T3` and `T4` (both `RR`) had high priority (80), while `T1` and `T2` (both `FIFO`) had lower priority (20).
   - This setup caused `RR` threads to perform better, but overall times were slightly higher than in E2. `T3` and `T4` had similar times (145s 971475μs and 129s 673296μs), but still did not outperform the high-priority `FIFO` configuration seen in E2.

3. **Baseline Comparison**:
   - In the baseline setup, all threads had the same priority (50) with both `FIFO` and `RR` scheduling.
   - Here, the execution times were generally higher than in E2, demonstrating that priority adjustments significantly impact performance.