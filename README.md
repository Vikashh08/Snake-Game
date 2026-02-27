# Building the Classic Snake Game 

So, you know the classic Snake game everyone played on their old Nokia phones? I decided to build one, but instead of making it run in a boring terminal, we are doing it properly with a cool Web UI — HTML, CSS, and JavaScript. The craziest part? Underneath the UI, it actually uses a **Singly Linked List** to control the snake!

##  How to Play (Web Version)

The Web UI is fully cross-platform (Windows, Mac, and Linux). Playing it is super simple:
1. Open the `SnakeWebUI/index.html` file in any web browser (Chrome, Safari, Edge, etc.).
2. Boom! You're in.
3. Click **Start Game**.
4. **Controls**:
   - Move **Up**: `W` or `Up Arrow`
   - Move **Left**: `A` or `Left Arrow`
   - Move **Down**: `S` or `Down  Arrow`
   - Move **Right**: `D` or `Right Arrow`
   - **Pause Game**: Click the active **Pause** button on the screen and hit **Resume** when ready.
5. Eat the red apples, don't hit the walls, and *definitely* don't eat your own tail!

---

## How to Run the C++ Version (Terminal)

If you still want to run the raw, terminal-based C++ version, here is how you compile and run it step-by-step:

###  On Mac (or Linux):
1. Open your **Terminal**.
2. Navigate to the project folder where `snake.cpp` is located.
3. Compile the code using `g++`:
   ```bash
   g++ snake.cpp -o snake
   ```
4. Run the game:
   ```bash
   ./snake
   ```

###  On Windows:
The `snake.cpp` file uses UNIX-specific libraries (`<unistd.h>`, `<termios.h>`) for real-time keyboard input. Because of this, it will **not** compile automatically with a standard Windows compiler (like Visual Studio or MinGW without MSYS2).

**The Easiest Way to Run on Windows (WSL):**
1. Install **Windows Subsystem for Linux (WSL)**. Open Command Prompt or PowerShell as Administrator and run:
   ```cmd
   wsl --install
   ```
2. Restart your computer if prompted.
3. Open the **WSL Terminal** (such as Ubuntu) from the Start menu.
4. Navigate to the folder with `snake.cpp`. *(In WSL, your C: drive is under `/mnt/c/`)*.
5. Compile using `g++`:
   ```bash
   g++ snake.cpp -o snake
   ```
6. Run the game:
   ```bash
   ./snake
   ```
*(Alternatively, just double-click the `index.html` Web Version—it runs perfectly anywhere!)*

---

## The Brains Behind the Game: How It Actually Works

I didn't just want to use a standard array to store the snake's body like every other tutorial online. Instead, I wired it up using a **Singly Linked List**. Here's why that's super cool, and how the whole thing ticks.

### 1. The Linked List Snake (The Cool Part)

What exactly *is* a linked list, and why use it for a snake? Imagine a train where each car holds onto the car behind it. A Singly Linked List is exactly the same concept.

```javascript
class Node {
    constructor(x, y) {
        this.x = x;          // Current position
        this.y = y;
        this.next = null;    // Points to the next body part!
    }
}
```

- Each piece of the snake (we call it a `Node`) knows its own grid coordinates `(x, y)` and only knows about the *next* piece behind it.
- The **Head** is the very first node. It leads the charge!
- Because it's a linked list, the snake can keep growing forever without us having to predefined how big the snake might eventually get.

### 2. How the Heck Does It Move?

If you think about it, making a whole snake slide forward pixel by pixel sounds like an absolute nightmare to code. But using our linked list, movement is literally just two steps:

* **Step 1: Grow a New Head!** 
  Based on which arrow key you pressed, we calculate where the snake *wants* to go. We create a completely new Node at that spot and drop it right in front, making it the new Head. It now points backward to everything else.

* **Step 2: Snip off the Tail!**
  If we just kept adding heads, the snake would just stretch endlessly! So, to simulate movement, every time we add a head to the front, we delete the very last node at the back (the tail).
  
```javascript
// A quick peek at the snippet that chops the tail:
let temp = this.head;
while (temp.next && temp.next.next) {
    temp = temp.next;
}
temp.next = null; 
```

### 3. Oops, I Crashed!

Before adding that shiny new head, the game stops and asks two questions:
- **Did I hit a wall?** It checks if the new `(x, y)` coordinates are completely off the canvas.
- **Did I bite myself?** It traverses the linked list from head to tail. If *any* body part shares the same coordinates as the new head we're trying to add, we've had a crash. Game over!

### 4. Nom Nom Nom (Eating Food)

What happens when the new head lands *exactly* on the food? 
Simple! We add the new head as usual, but **we skip the step where we snip off the tail**. Because we skipped the tail removal, the snake permanently becomes one block longer! Then, my code just tosses a new fruit onto the board randomly (making sure it doesn't accidentally spawn *inside* the snake's body).

---

And that's pretty much it! It's super fun to take a classic Data Structure concept like a Linked List and use it practically to build something visual and interactive. Let me know if you can beat my high score!
