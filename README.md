# COMPE 571 - CPU Scheduling Algorithms  
**Professor:** Baris  
**Authors:** Kohki Kita, Ethan Quach  
**Date:** October 26, 2025  

---

## 🧠 Overview  
This project implements and analyzes several classic **CPU scheduling algorithms** as part of **COMPE 571**.  
Each algorithm is compiled into its own executable, and the program measures the execution time of each process under the given scheduling policy.  

---

## ⚙️ Implemented Scheduling Algorithms  

### **1. FCFS (First Come, First Serve)**  
- Processes are executed in the order they arrive.  
- Non-preemptive scheduling method.  
- Simple to implement but can lead to the *convoy effect* if long jobs block shorter ones.  

### **2. SJF (Shortest Job First)**  
- Selects the process with the shortest expected CPU burst time next.  
- Minimizes average waiting time but requires prior knowledge of burst times.  
- Implemented here as a non-preemptive scheduler.  

### **3. RR (Round Robin)**  
- Each process is assigned a fixed time quantum and scheduled cyclically.  
- Preemptive scheduling algorithm ensuring fairness and responsiveness.  
- Ideal for time-sharing systems.  

### **4. MLFQ (Multi-Level Feedback Queue)**  
- Dynamic scheduling method using multiple queues with varying priorities.  
- Processes can move between queues based on behavior and CPU usage.  
- Balances fairness, responsiveness, and throughput.  

---

## 🖥️ Files 
After compiling, execute the corresponding binary for the desired scheduling algorithm:

```bash
./fcfs   # First Come First Serve  
./sjf    # Shortest Job First  
./rr     # Round Robin  
./mlfq   # Multi-Level Feedback Queue  
