import { useState, useEffect } from 'react'
import "./App.css";
import { BinarySearchTree } from "./tree-lib/bst";
import { AVLTree } from "./tree-lib/avl";
import { SplayTree } from "./tree-lib/splay";
import Tree from "./tree";

function App() {
  const [tree, setTree] = useState<BinarySearchTree<number, number> | null>(null);
  const [inputValue, setInputValue] = useState("");
  const [treeType, setTreeType] = useState<"bst" | "avl" | "splay">("bst");
  const [isAnimating, setIsAnimating] = useState(false);
  const [highlightedNode, setHighlightedNode] = useState<number | null>(null);
  const [message, setMessage] = useState("");

  // Auto-create tree when type changes
  useEffect(() => {
    if (treeType === "bst") {
      setTree(new BinarySearchTree());
    } else if (treeType === "avl") {
      setTree(new AVLTree());
    } else {
      setTree(new SplayTree());
    }
    setMessage("");
  }, [treeType]);

  const animationStep = (step: any) => {
    return new Promise<void>((resolve) => {
      console.log(step);
      let highlightKey = null;
      if (step.type === 'visit' || step.type === 'found' || step.type === 'splay' || step.type.startsWith('rotate')) {
        highlightKey = step.key;
      }
      setHighlightedNode(highlightKey);

      if(step.type === 'info' || step.type === 'not-found') {
        setMessage(step.message || `Node with key ${step.key} not found!`);
      } else if (step.type !== 'visit' && step.type !== 'found') {
        setMessage("");
      }
      
      setTree(t => t ? Object.create(t) : null);

      setTimeout(() => {
        if(step.type !== 'found') {
            setHighlightedNode(null);
        }
        resolve();
      }, 500);
    });
  };

  const handleRestart = () => {
    if (isAnimating) return;
    if (treeType === "bst") {
      setTree(new BinarySearchTree());
    } else if (treeType === "avl") {
      setTree(new AVLTree());
    } else {
      setTree(new SplayTree());
    }
    setMessage("");
  };

  const handleInsert = async () => {
    if (tree && inputValue && !isAnimating) {
      setIsAnimating(true);
      setMessage(`Inserting ${inputValue}...`);
      const value = parseInt(inputValue, 10);
      if (!isNaN(value)) {
        await tree.insert(value, value, animationStep);
        setInputValue("");
      }
      const newTree = await tree.clone();
      setTree(newTree);
      setIsAnimating(false);
      setMessage(`Inserted ${value}`);
    }
  };

  const handleDelete = async () => {
    if (tree && inputValue && !isAnimating) {
      setIsAnimating(true);
      setMessage(`Removing ${inputValue}...`);
      const value = parseInt(inputValue, 10);
      if (!isNaN(value)) {
        await tree.remove(value, animationStep);
        setInputValue("");
      }
      const newTree = await tree.clone();
      setTree(newTree);
      setIsAnimating(false);
      
      if (newTree.root === null) {
        setMessage(`Removed ${value}. Tree is now empty.`);
        setTimeout(() => {
          if (treeType === "bst") {
            setTree(new BinarySearchTree());
          } else if (treeType === "avl") {
            setTree(new AVLTree());
          } else {
            setTree(new SplayTree());
          }
          setMessage("");
        }, 1000);
      } else {
        setMessage(`Removed ${value}`);
      }
    }
  };

  const handleFind = async () => {
    if (tree && inputValue && !isAnimating) {
      setIsAnimating(true);
      setMessage(`Finding ${inputValue}...`);
      const value = parseInt(inputValue, 10);
      if (!isNaN(value)) {
        const node = await tree.find(value, animationStep);
        if (node) {
          setMessage(`Node with key ${value} found!`);
        } else {
           setMessage(`Node with key ${value} not found!`);
        }
        setInputValue("");
      }
      const newTree = await tree.clone();
      setTree(newTree);
      setIsAnimating(false);
    }
  };

  return (
    <div className="App">
      <div className="controls">
        <select
          value={treeType}
          onChange={(e) => setTreeType(e.target.value as "bst" | "avl" | "splay")}
          disabled={isAnimating}
        >
          <option value="bst">Binary Search Tree</option>
          <option value="avl">AVL Tree</option>
          <option value="splay">Splay Tree</option>
        </select>
        <button onClick={handleRestart} disabled={isAnimating}>Restart</button>
        <input
          type="text"
          value={inputValue}
          onChange={(e) => setInputValue(e.target.value)}
          disabled={isAnimating}
        />
        <button onClick={handleInsert} disabled={isAnimating || !inputValue}>Insert</button>
        <button onClick={handleDelete} disabled={isAnimating || !inputValue}>Delete</button>
        <button onClick={handleFind} disabled={isAnimating || !inputValue}>Find</button>
      </div>
      <div className="message-box">
        {message}
      </div>
      <div className="tree-container">
        {tree && <Tree tree={tree} highlightedNode={highlightedNode} />}
      </div>
    </div>
  );
}

export default App;
