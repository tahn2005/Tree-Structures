# Tree Structures - Educational Data Structure Visualizer

A comprehensive project featuring C++ implementations of fundamental tree data structures and an interactive web-based visualization tool for educational purposes.

## Overview

This project was inspired a data structures class, going beyond the scope of the class to create both robust C++ implementations and an educational GUI that demonstrates how these tree structures work in real-time.

## Features

### Tree Data Structures
- **Binary Search Tree (BST)** - Standard binary search tree implementation
- **AVL Tree** - Self-balancing binary search tree with height balancing
- **Splay Tree** - Self-adjusting binary search tree with amortized performance

### C++ Implementations
Located in the `c++/` directory, these are production-ready implementations that can be used by other developers for their own projects:

- `bst.h` - Binary Search Tree implementation
- `avl.h` - AVL Tree implementation  
- `splay.h` - Splay Tree implementation

### 🖥️ Interactive Web GUI
[Link](https://tree-structures.vercel.app/)
A React-based visualization tool that provides:

- **Real-time tree visualization** with SVG rendering
- **Step-by-step animations** showing tree operations
- **Interactive controls** for insert, delete, and find operations
- **Educational insights** with highlighted nodes during operations
- **Multiple tree types** with seamless switching between BST, AVL, and Splay trees

## Getting Started

### Prerequisites
- Node.js (v16 or higher)
- npm or yarn

### Installation

1. Clone the repository:
```bash
git clone <repository-url>
cd tree-structures
```

2. Install dependencies:
```bash
npm install
```

3. Start the development server:
```bash
npm run dev
```

4. Open your browser and navigate to `http://localhost:5173`

## Usage

### Web Interface
1. **Select Tree Type**: Choose between Binary Search Tree, AVL Tree, or Splay Tree from the dropdown
2. **Insert Nodes**: Enter a number and click "Insert" to add it to the tree
3. **Delete Nodes**: Enter a number and click "Delete" to remove it from the tree
4. **Find Nodes**: Enter a number and click "Find" to search for it in the tree
5. **Restart**: Click "Restart" to clear the current tree and start fresh

### C++ Implementations
The C++ header files in the `c++/` directory can be included in your own projects:

```cpp
#include "c++/bst.h"
#include "c++/avl.h"
#include "c++/splay.h"

// Example usage
BinarySearchTree<int, int> bst;
bst.insert(10, 10);
bst.insert(5, 5);
bst.insert(15, 15);
```

## Educational Value

This project serves as an excellent learning resource for:

- **Data Structure Concepts**: Understanding how different tree structures work
- **Algorithm Visualization**: Seeing step-by-step how operations like insertion, deletion, and balancing occur
- **Performance Comparison**: Observing the differences between BST, AVL, and Splay tree behaviors
- **Animation Learning**: Understanding complex algorithms through visual representation

## Technical Details

### Frontend Stack
- **React 18** with TypeScript
- **Vite** for fast development and building
- **SVG** for tree visualization
- **CSS3** for styling and animations

### Client-side Algorithmic Logic
- **TypeScript** implementations of all tree structures
- **Async operations** for step-by-step animation
- **Deep cloning** for state management during animations

### C++ Features
- **Template-based** implementations for type flexibility
- **Memory efficient** with proper cleanup
- **Standard library** integration
- **Header-only** design for easy inclusion

## Contributing

Contributions are welcome! Whether you want to:
- Add new tree data structures
- Improve the visualization
- Enhance the C++ implementations
- Add new features to the GUI

Please feel free to submit pull requests or open issues.

## License

This project is available for personal and educational use. The C++ implementations are provided as-is for developers to use in their own projects.

## Acknowledgments

- Inspired by the need for better educational tools in computer science
- Built with modern web technologies for accessibility and ease of use

---

**Note**: This project demonstrates the power of combining theoretical knowledge with practical implementation and visual learning tools. The C++ implementations provide robust, reusable code while the web interface makes complex algorithms accessible and understandable.
