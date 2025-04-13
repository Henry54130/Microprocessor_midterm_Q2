# Microprocessor_midterm_Q2

# Basic Function Enhancement and Expansion Design

## ✅ I. Basic Function Enhancement: Replace Idle Delay with Timer Interrupt (10%)

**Core Objective:**  
Enhance efficiency and real-time responsiveness by avoiding the use of idle delays like `delay`.

**Method Description:**  
- Remove all unnecessary idle delays.  
- Use **timer interrupts** to generate the scanning clock signal for output.

**Application Features:**  
- Control the **speed of the stepper motor** (e.g., marquee light speed).  
- Control **motor direction switching**.  
- Control **motor positioning**.

**Additional Notes:**  
Button debounce handling can still use external interrupts and is **not included** in this improvement.

---

## ✅ II. Expansion Function: Step Sound Control (10%)

**Combining LED with Sound:**  
- Use 4 LEDs to indicate stepping status (`LED x4`).  
- Each LED corresponds to a musical note.

**Scale Design:**  
- **Segment 1:** Middle C to F (4 notes)  
- **Segment 2:** Middle G to High C (4 notes)  
- The two segments play alternately.

**Termination Condition:**  
- When **either Button 1 or Button 2** is pressed, stop the LED display and tone playback (stepping).