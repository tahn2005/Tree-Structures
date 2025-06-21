import React, { useState, useRef, useCallback, useLayoutEffect } from 'react';
import { BinarySearchTree, Node } from './tree-lib/bst';
import TreeNode from './TreeNode';
import './tree.css';

interface TreeProps {
  tree: BinarySearchTree<number, number>;
  highlightedNode: number | null;
}

interface Position {
  x: number;
  y: number;
}

const Tree: React.FC<TreeProps> = ({ tree, highlightedNode }) => {
  const [positions, setPositions] = useState<Map<number, Position>>(new Map());
  const containerRef = useRef<HTMLDivElement>(null);

  useLayoutEffect(() => {
    const activeNodeKeys = new Set<number>();
    const collectKeys = (node: Node<number, number> | null) => {
        if (!node) return;
        activeNodeKeys.add(node.key);
        collectKeys(node.left);
        collectKeys(node.right);
    };
    collectKeys(tree.root);

    setPositions(prevPositions => {
        const nextPositions = new Map(prevPositions);
        let changed = false;
        for (const key of prevPositions.keys()) {
            if (!activeNodeKeys.has(key)) {
                nextPositions.delete(key);
                changed = true;
            }
        }
        return changed ? nextPositions : prevPositions;
    });
  }, [tree]);

  const handlePositionUpdate = useCallback((key: number, pos: Position) => {
    setPositions(prev => {
        const oldPos = prev.get(key);
        if (oldPos && oldPos.x === pos.x && oldPos.y === pos.y) {
            return prev;
        }
        const newPositions = new Map(prev);
        newPositions.set(key, pos);
        return newPositions;
    });
  }, []);

  const getEdges = (node: Node<number, number> | null): { from: number; to: number }[] => {
    if (!node) return [];
    
    let edges: { from: number; to: number }[] = [];
    if (node.left) {
      edges.push({ from: node.key, to: node.left.key });
      edges = edges.concat(getEdges(node.left));
    }
    if (node.right) {
      edges.push({ from: node.key, to: node.right.key });
      edges = edges.concat(getEdges(node.right));
    }
    return edges;
  };

  const edges = getEdges(tree.root);
  
  return (
    <div className="tree" ref={containerRef}>
      <svg className="edge-svg">
        {edges.map(({ from, to }) => {
          const fromPos = positions.get(from);
          const toPos = positions.get(to);
          if (fromPos && toPos) {
            return (
              <line
                key={`${from}-${to}`}
                x1={fromPos.x}
                y1={fromPos.y}
                x2={toPos.x}
                y2={toPos.y}
                stroke="black"
                strokeWidth="2"
              />
            );
          }
          return null;
        })}
      </svg>
      {tree.root && (
        <TreeNode node={tree.root} highlightedNode={highlightedNode} onPositionUpdate={handlePositionUpdate} containerRef={containerRef} />
      )}
    </div>
  );
};

export default Tree; 