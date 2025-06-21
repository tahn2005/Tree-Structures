import React, { useRef, useLayoutEffect, type RefObject } from 'react';
import { Node } from './tree-lib/bst';
import './tree.css';

interface TreeNodeProps {
  node: Node<number, number> | null;
  highlightedNode: number | null;
  onPositionUpdate: (key: number, pos: { x: number; y: number }) => void;
  containerRef: RefObject<HTMLDivElement | null>;
}

const TreeNode: React.FC<TreeNodeProps> = ({ node, highlightedNode, onPositionUpdate, containerRef }) => {
  const nodeRef = useRef<HTMLDivElement>(null);

  useLayoutEffect(() => {
    if (node && nodeRef.current && containerRef.current) {
      const nodeRect = nodeRef.current.getBoundingClientRect();
      const containerRect = containerRef.current.getBoundingClientRect();
      
      onPositionUpdate(node.key, {
        x: nodeRect.left + nodeRect.width / 2 - containerRect.left,
        y: nodeRect.top + nodeRect.height / 2 - containerRect.top,
      });
    }
  }, [node, onPositionUpdate, containerRef, highlightedNode]);

  if (!node) {
    return <div className="node-placeholder" />;
  }
  
  const isHighlighted = node.key === highlightedNode;

  return (
    <div className="node-container">
      <div className={`node-value ${isHighlighted ? 'highlighted' : ''}`} ref={nodeRef}>
        {node.key}
      </div>
      <div className="children">
        <TreeNode node={node.left} highlightedNode={highlightedNode} onPositionUpdate={onPositionUpdate} containerRef={containerRef} />
        <TreeNode node={node.right} highlightedNode={highlightedNode} onPositionUpdate={onPositionUpdate} containerRef={containerRef} />
      </div>
    </div>
  );
};

export default TreeNode; 